/*******************************************************
  * Name    :ModemPlatform.c
  * Author  :LoH
  * Function:Candidate AECS registration, receive, heartbeat and MSD workflow.
  * Version :V1.0.0
  * Data    :2026.11.14
*******************************************************/
#include "ModemPlatform.h"
#include <stdio.h>
#include <string.h>
#include "Hmi.h"
#include "ModemAt_Cfg.h"
#include "ModemPort.h"
#include "ModemIdentity.h"
#include "ModemMsdCodec.h"
#include "cmsis_os.h"

#define MODEM_PLATFORM_PROCESS_PERIOD_MS        100U
#define MODEM_PLATFORM_JSON_LENGTH               64U
#define MODEM_PLATFORM_FRAME_LENGTH              (MODEM_PLATFORM_JSON_LENGTH + 5U)
#define MODEM_PLATFORM_COMMAND_LENGTH            32U
#define MODEM_PLATFORM_RX_MAX_LENGTH             750U
#define MODEM_PLATFORM_REPLY_TYPE_LENGTH          32U
#define MODEM_PLATFORM_REPLY_LOG_LENGTH           56U
#define MODEM_PLATFORM_REPLY_TYPE_CURRENT        0x80U
#define MODEM_PLATFORM_REPLY_TYPE_REGISTER_LEGACY 0x01U
#define MODEM_PLATFORM_REPLY_TYPE_HEARTBEAT_LEGACY 0x02U
#define MODEM_PLATFORM_REPLY_TYPE_MSD_LEGACY     0x11U
#define MODEM_PLATFORM_MSD_PAYLOAD_LENGTH        111U
#define MODEM_PLATFORM_MSD_FRAME_LENGTH          MODEM_MSD_MAX_FRAME_LENGTH

typedef enum
{
    MODEM_PLATFORM_PHASE_IDLE = 0U,
    MODEM_PLATFORM_PHASE_REGISTERING,
    MODEM_PLATFORM_PHASE_ONLINE,
    MODEM_PLATFORM_PHASE_MSD_UPLOADING
} ModemPlatformPhase_t;

typedef enum
{
    MODEM_PLATFORM_REGISTER_STEP_PREPARE = 0U,
    MODEM_PLATFORM_REGISTER_STEP_QUEUE,
    MODEM_PLATFORM_REGISTER_STEP_WAIT_PROMPT,
    MODEM_PLATFORM_REGISTER_STEP_SEND,
    MODEM_PLATFORM_REGISTER_STEP_WAIT_RESULT
} ModemPlatformRegisterStep_t;

typedef enum
{
    MODEM_PLATFORM_RX_STEP_IDLE = 0U,
    MODEM_PLATFORM_RX_STEP_LENGTH_QUEUE,
    MODEM_PLATFORM_RX_STEP_LENGTH_WAIT,
    MODEM_PLATFORM_RX_STEP_DATA_QUEUE,
    MODEM_PLATFORM_RX_STEP_DATA_WAIT
} ModemPlatformReceiveStep_t;

typedef enum
{
    MODEM_PLATFORM_HEARTBEAT_STEP_WAIT = 0U,
    MODEM_PLATFORM_HEARTBEAT_STEP_PREPARE,
    MODEM_PLATFORM_HEARTBEAT_STEP_QUEUE,
    MODEM_PLATFORM_HEARTBEAT_STEP_WAIT_PROMPT,
    MODEM_PLATFORM_HEARTBEAT_STEP_SEND,
    MODEM_PLATFORM_HEARTBEAT_STEP_WAIT_RESULT
} ModemPlatformHeartbeatStep_t;

typedef enum
{
    MODEM_PLATFORM_MSD_STEP_PREPARE = 0U,
    MODEM_PLATFORM_MSD_STEP_QUEUE,
    MODEM_PLATFORM_MSD_STEP_WAIT_PROMPT,
    MODEM_PLATFORM_MSD_STEP_SEND,
    MODEM_PLATFORM_MSD_STEP_WAIT_RESULT
} ModemPlatformMsdStep_t;

typedef struct
{
    ModemPlatformRegisterStep_t enRegisterStep;
    ModemPlatformReceiveStep_t enReceiveStep;
    ModemPlatformHeartbeatStep_t enHeartbeatStep;
    ModemPlatformMsdStep_t enMsdStep;
    uint32_t u32HeartbeatHoldMs;
    char acRegisterCommand[MODEM_PLATFORM_COMMAND_LENGTH];
    char acHeartbeatCommand[MODEM_PLATFORM_COMMAND_LENGTH];
    char acMsdCommand[MODEM_PLATFORM_COMMAND_LENGTH];
    char acReceiveLengthCommand[MODEM_PLATFORM_COMMAND_LENGTH];
    char acReceiveDataCommand[MODEM_PLATFORM_COMMAND_LENGTH];
    uint8_t au8RegisterFrame[MODEM_PLATFORM_FRAME_LENGTH];
    uint8_t au8HeartbeatFrame[MODEM_PLATFORM_FRAME_LENGTH];
    uint8_t au8ReceiveFrame[MODEM_PLATFORM_RX_MAX_LENGTH];
    uint16_t u16RegisterFrameLength;
    uint16_t u16HeartbeatFrameLength;
    uint16_t u16ReceiveLength;
    uint16_t u16ReceiveFrameLength;
    uint8_t u8ReceiveLinkId;
    uint8_t au8StreamFrame[MODEM_MSD_MAX_FRAME_LENGTH];
    uint16_t u16StreamLength;
    uint8_t u8RegisterAckReceived;
    uint8_t u8AwaitingRegisterAck;
    uint32_t u32RegisterAckStartMs;
} ModemPlatformRuntime_t;

typedef struct
{
    ModemPlatformPhase_t enPhase;
    ModemPlatformEvent_t enEvent;
} ModemPlatformStatus_t;

typedef struct
{
    uint8_t u8MsdUploadRequested;
    uint8_t u8MsdUploadComplete;
    uint8_t u8MsdAckReceived;
    uint8_t u8AttemptCount;
    uint8_t u8AwaitingAck;
    uint8_t au8MsdFrame[MODEM_MSD_MAX_FRAME_LENGTH];
    uint16_t u16MsdFrameLength;
    uint32_t u32NextSer;
    uint32_t u32Ser;
    uint32_t u32RequestStartMs;
    uint32_t u32AttemptStartMs;
    uint32_t u32AckStartMs;
    EcallMsd_t stLastMsd;
} ModemPlatformMsdRuntime_t;

static ModemPlatformMsdRuntime_t s_mMsdRun;
/* One additional frozen event may wait without corrupting a CIPSEND in progress. */
static ModemPlatformMsdRuntime_t s_mMsdQueued;
static ModemPlatformRuntime_t s_mPlatRun;
static ModemPlatformStatus_t s_mPlatSts;

static uint8_t ModemPlatform_IsRetryDue(void);

/*******************************************************
  * Name    :ModemPlatform_EnterFailure
  * Author  :LoH
  * Function:Stop platform work with one consumable failure event.
  * Version :V1.0.0
  * Data    :2026.08.28
*******************************************************/
static void ModemPlatform_EnterFailure(void)
{
    s_mPlatSts.enPhase = MODEM_PLATFORM_PHASE_IDLE;
    s_mPlatSts.enEvent = MODEM_PLATFORM_EVENT_FAILED;
}

/*******************************************************
  * Name    :ModemPlatform_IsDataSendComplete
  * Author  :LoH
  * Function:Return whether CIPSEND confirms every requested platform byte.
  * Version :V1.0.0
  * Data    :2026.08.28
*******************************************************/
static uint8_t ModemPlatform_IsDataSendComplete(const char *pcResponse, uint16_t u16ExpectedLength)
{
    unsigned int u32LinkId;
    unsigned int u32RequestedLength;
    unsigned int u32ConfirmedLength;

    if ((pcResponse == 0)
        || (sscanf(pcResponse, "+CIPSEND: %u,%u,%u", &u32LinkId, &u32RequestedLength, &u32ConfirmedLength) != 3))
    {
        return 0U;
    }

    return ((u32LinkId == MODEM_PLATFORM_LINK_ID)
            && (u32RequestedLength == u16ExpectedLength)
            && (u32ConfirmedLength == u16ExpectedLength)) ? 1U : 0U;
}

/*******************************************************
  * Name    :ModemPlatform_PrepareRegisterFrame
  * Author  :LoH
  * Function:Build the candidate AECS registration frame and CIPSEND command.
  * Version :V1.0.0
  * Data    :2026.08.28
*******************************************************/
static uint8_t ModemPlatform_PrepareRegisterFrame(void)
{
    int i32JsonLength;
    int i32CommandLength;
    uint32_t u32JsonLength;

    i32JsonLength = snprintf((char *)&s_mPlatRun.au8RegisterFrame[5], MODEM_PLATFORM_JSON_LENGTH,
                             MODEM_PLATFORM_REGISTER_JSON_FORMAT, MODEM_PLATFORM_TEST_DEVICE_ID);
    if ((i32JsonLength <= 0) || ((uint32_t)i32JsonLength >= MODEM_PLATFORM_JSON_LENGTH))
    {
        return 0U;
    }

    u32JsonLength = (uint32_t)i32JsonLength;
    s_mPlatRun.au8RegisterFrame[0] = MODEM_PLATFORM_MSG_REGISTER;
    s_mPlatRun.au8RegisterFrame[1] = (uint8_t)(u32JsonLength >> 24U);
    s_mPlatRun.au8RegisterFrame[2] = (uint8_t)(u32JsonLength >> 16U);
    s_mPlatRun.au8RegisterFrame[3] = (uint8_t)(u32JsonLength >> 8U);
    s_mPlatRun.au8RegisterFrame[4] = (uint8_t)u32JsonLength;
    s_mPlatRun.u16RegisterFrameLength = (uint16_t)(u32JsonLength + 5U);

    i32CommandLength = snprintf(s_mPlatRun.acRegisterCommand, sizeof(s_mPlatRun.acRegisterCommand),
                                MODEM_AT_CMD_TCP_SEND_FORMAT, MODEM_PLATFORM_LINK_ID,
                                s_mPlatRun.u16RegisterFrameLength);
    return ((i32CommandLength > 0)
            && ((uint32_t)i32CommandLength < sizeof(s_mPlatRun.acRegisterCommand))) ? 1U : 0U;
}

/*******************************************************
  * Name    :ModemPlatform_PrepareHeartbeatFrame
  * Author  :LoH
  * Function:Build the candidate AECS heartbeat frame and CIPSEND command.
  * Version :V1.0.0
  * Data    :2026.08.28
*******************************************************/
static uint8_t ModemPlatform_PrepareHeartbeatFrame(void)
{
    int i32JsonLength;
    int i32CommandLength;
    uint32_t u32JsonLength;

    i32JsonLength = snprintf((char *)&s_mPlatRun.au8HeartbeatFrame[5], MODEM_PLATFORM_JSON_LENGTH,
                             MODEM_PLATFORM_HEARTBEAT_JSON_FORMAT, MODEM_PLATFORM_TEST_DEVICE_ID);
    if ((i32JsonLength <= 0) || ((uint32_t)i32JsonLength >= MODEM_PLATFORM_JSON_LENGTH))
    {
        return 0U;
    }

    u32JsonLength = (uint32_t)i32JsonLength;
    s_mPlatRun.au8HeartbeatFrame[0] = MODEM_PLATFORM_MSG_HEARTBEAT;
    s_mPlatRun.au8HeartbeatFrame[1] = (uint8_t)(u32JsonLength >> 24U);
    s_mPlatRun.au8HeartbeatFrame[2] = (uint8_t)(u32JsonLength >> 16U);
    s_mPlatRun.au8HeartbeatFrame[3] = (uint8_t)(u32JsonLength >> 8U);
    s_mPlatRun.au8HeartbeatFrame[4] = (uint8_t)u32JsonLength;
    s_mPlatRun.u16HeartbeatFrameLength = (uint16_t)(u32JsonLength + 5U);

    i32CommandLength = snprintf(s_mPlatRun.acHeartbeatCommand, sizeof(s_mPlatRun.acHeartbeatCommand),
                                MODEM_AT_CMD_TCP_SEND_FORMAT, MODEM_PLATFORM_LINK_ID,
                                s_mPlatRun.u16HeartbeatFrameLength);
    return ((i32CommandLength > 0)
            && ((uint32_t)i32CommandLength < sizeof(s_mPlatRun.acHeartbeatCommand))) ? 1U : 0U;
}

/*******************************************************
  * Name    :ModemPlatform_PrepareMsdFrame
  * Author  :LoH
  * Function:Prepare CIPSEND for the immutable standard MSD and optional V0.2 extension.
  * Version :V1.0.0
  * Data    :2026.11.14
*******************************************************/
static uint8_t ModemPlatform_PrepareMsdFrame(void)
{
    int i32CommandLength;
    if (s_mMsdRun.u16MsdFrameLength == 0U)
    {
        return 0U;
    }
    i32CommandLength = snprintf(s_mPlatRun.acMsdCommand, sizeof(s_mPlatRun.acMsdCommand),
                                MODEM_AT_CMD_TCP_SEND_FORMAT, MODEM_PLATFORM_LINK_ID, s_mMsdRun.u16MsdFrameLength);
    return ((i32CommandLength > 0) && ((uint32_t)i32CommandLength < sizeof(s_mPlatRun.acMsdCommand))) ? 1U : 0U;
}

/*******************************************************
  * Name    :ModemPlatform_ParseReceiveLength
  * Author  :LoH
  * Function:Read the buffered-data length returned by CIPRXGET mode four.
  * Version :V1.0.0
  * Data    :2026.11.14
*******************************************************/
static uint8_t ModemPlatform_ParseReceiveLength(const char *pcResponse, uint8_t u8LinkId, uint16_t *pu16Length)
{
    unsigned int u32ResponseLinkId;
    unsigned int u32Length;

    if ((pcResponse == 0)
        || (pu16Length == 0)
        || (sscanf(pcResponse, "+CIPRXGET: 4,%u,%u", &u32ResponseLinkId, &u32Length) != 2)
        || (u32ResponseLinkId != u8LinkId)
)
    {
        return 0U;
    }

    *pu16Length = (u32Length > MODEM_PLATFORM_RX_MAX_LENGTH) ? MODEM_PLATFORM_RX_MAX_LENGTH : (uint16_t)u32Length;
    return 1U;
}

/*******************************************************
  * Name    :ModemPlatform_HexToBytes
  * Author  :LoH
  * Function:Convert one fixed-length hexadecimal modem response to bytes.
  * Version :V1.0.0
  * Data    :2026.08.28
*******************************************************/
static uint8_t ModemPlatform_HexToBytes(const char *pcHex, uint16_t u16Length, uint8_t *pu8Bytes)
{
    uint16_t u16Index;
    uint8_t u8HighNibble;
    uint8_t u8LowNibble;
    char cHigh;
    char cLow;

    if ((pcHex == 0) || (pu8Bytes == 0) || (pcHex[(uint16_t)(u16Length * 2U)] != '\0'))
    {
        return 0U;
    }

    for (u16Index = 0U; u16Index < u16Length; u16Index++)
    {
        cHigh = pcHex[u16Index * 2U];
        cLow = pcHex[(u16Index * 2U) + 1U];
        if ((cHigh >= '0') && (cHigh <= '9'))
        {
            u8HighNibble = (uint8_t)(cHigh - '0');
        }
        else if ((cHigh >= 'A') && (cHigh <= 'F'))
        {
            u8HighNibble = (uint8_t)(cHigh - 'A' + 10U);
        }
        else if ((cHigh >= 'a') && (cHigh <= 'f'))
        {
            u8HighNibble = (uint8_t)(cHigh - 'a' + 10U);
        }
        else
        {
            return 0U;
        }

        if ((cLow >= '0') && (cLow <= '9'))
        {
            u8LowNibble = (uint8_t)(cLow - '0');
        }
        else if ((cLow >= 'A') && (cLow <= 'F'))
        {
            u8LowNibble = (uint8_t)(cLow - 'A' + 10U);
        }
        else if ((cLow >= 'a') && (cLow <= 'f'))
        {
            u8LowNibble = (uint8_t)(cLow - 'a' + 10U);
        }
        else
        {
            return 0U;
        }

        pu8Bytes[u16Index] = (uint8_t)((u8HighNibble << 4U) | u8LowNibble);
    }
    return 1U;
}

/*******************************************************
  * Name    :ModemPlatform_LogReplyType
  * Author  :LoH
  * Function:Report the parsed platform reply type without logging the complete payload.
  * Version :V1.0.0
  * Data    :2026.08.28
*******************************************************/
static void ModemPlatform_LogReplyType(const char *pcType)
{
    char acLog[MODEM_PLATFORM_REPLY_LOG_LENGTH];
    int i32LogLength;

    if (pcType == 0)
    {
        HMI_LOG_MODEM("PLATFORM_RX_TYPE_INVALID\r\n");
        return;
    }

    i32LogLength = snprintf(acLog, sizeof(acLog), "PLATFORM_RX_TYPE:%s\r\n", pcType);
    if ((i32LogLength > 0) && ((uint32_t)i32LogLength < sizeof(acLog)))
    {
        HMI_LOG_MODEM(acLog);
    }
    else
    {
        HMI_LOG_MODEM("PLATFORM_RX_TYPE_INVALID\r\n");
    }
}

/*******************************************************
  * Name    :ModemPlatform_ProcessRegistering
  * Author  :LoH
  * Function:Send registration, verify CIPSEND lengths and then wait for register_ack.
  * Version :V1.0.0
  * Data    :2026.08.28
*******************************************************/
static void ModemPlatform_ProcessRegistering(void)
{
    switch (s_mPlatRun.enRegisterStep)
    {
        case MODEM_PLATFORM_REGISTER_STEP_PREPARE:
            if (ModemPlatform_PrepareRegisterFrame() != 0U)
            {
                s_mPlatRun.enRegisterStep = MODEM_PLATFORM_REGISTER_STEP_QUEUE;
            }
            else
            {
                HMI_LOG_MODEM("REGISTER_FRAME_FAILED\r\n");
                ModemPlatform_EnterFailure();
            }
            break;

        case MODEM_PLATFORM_REGISTER_STEP_QUEUE:
            ModemPort_ClearFailure();
            if (ModemPort_QueueCommand(s_mPlatRun.acRegisterCommand, MODEM_AT_RSP_PROMPT,
                                       MODEM_AT_CFG_SEND_PROMPT_TIMEOUT_MS) != 0U)
            {
                s_mPlatRun.enRegisterStep = MODEM_PLATFORM_REGISTER_STEP_WAIT_PROMPT;
            }
            else
            {
                HMI_LOG_MODEM("REGISTER_QUEUE_FAILED\r\n");
                ModemPlatform_EnterFailure();
            }
            break;

        case MODEM_PLATFORM_REGISTER_STEP_WAIT_PROMPT:
            if (ModemPort_HasFailure() != 0U)
            {
                HMI_LOG_MODEM("REGISTER_PROMPT_FAILED\r\n");
                ModemPlatform_EnterFailure();
            }
            else if (ModemPort_IsIdle() != 0U)
            {
                s_mPlatRun.enRegisterStep = MODEM_PLATFORM_REGISTER_STEP_SEND;
            }
            else
            {
                /* Wait for the CIPSEND prompt. */
            }
            break;

        case MODEM_PLATFORM_REGISTER_STEP_SEND:
            if (ModemPort_SendData(s_mPlatRun.au8RegisterFrame, s_mPlatRun.u16RegisterFrameLength,
                                   MODEM_AT_RSP_SEND, MODEM_AT_CFG_SEND_RESULT_TIMEOUT_MS) != 0U)
            {
                s_mPlatRun.enRegisterStep = MODEM_PLATFORM_REGISTER_STEP_WAIT_RESULT;
            }
            else
            {
                HMI_LOG_MODEM("REGISTER_SEND_FAILED\r\n");
                ModemPlatform_EnterFailure();
            }
            break;

        case MODEM_PLATFORM_REGISTER_STEP_WAIT_RESULT:
            if (ModemPort_HasFailure() != 0U)
            {
                HMI_LOG_MODEM("REGISTER_RESULT_FAILED\r\n");
                ModemPlatform_EnterFailure();
            }
            else if (ModemPort_IsIdle() != 0U)
            {
                if (ModemPlatform_IsDataSendComplete(ModemPort_GetLastMatchedResponse(), s_mPlatRun.u16RegisterFrameLength) != 0U)
                {
                    HMI_LOG_MODEM("PLATFORM_REGISTER_SENT_WAIT_ACK\r\n");
                    s_mPlatSts.enPhase = MODEM_PLATFORM_PHASE_ONLINE;
                    s_mPlatRun.u8RegisterAckReceived = 0U;
                    s_mPlatRun.u8AwaitingRegisterAck = 1U;
                    s_mPlatRun.u32RegisterAckStartMs = osKernelSysTick();
                }
                else
                {
                    HMI_LOG_MODEM("REGISTER_CONFIRM_FAILED\r\n");
                    ModemPlatform_EnterFailure();
                }
            }
            else
            {
                /* Wait for +CIPSEND: <link>,<req>,<cnf>. */
            }
            break;

        default:
            HMI_LOG_MODEM("REGISTER_STEP_UNKNOWN\r\n");
            ModemPlatform_EnterFailure();
            break;
    }
}

/*******************************************************
  * Name    :ModemPlatform_ProcessHeartbeat
  * Author  :LoH
  * Function:Send periodic heartbeat without colliding with receive, MSD or sleep work.
  * Version :V1.0.0
  * Data    :2026.11.14
*******************************************************/
static void ModemPlatform_ProcessHeartbeat(uint8_t u8HeartbeatEnabled)
{
    if ((s_mPlatRun.enReceiveStep != MODEM_PLATFORM_RX_STEP_IDLE)
        || ((s_mPlatRun.enHeartbeatStep == MODEM_PLATFORM_HEARTBEAT_STEP_WAIT)
            && ((u8HeartbeatEnabled == 0U)
                || ((s_mMsdRun.u8MsdUploadRequested != 0U)
                    && (s_mMsdRun.u8MsdUploadComplete == 0U)
                    && (ModemPlatform_IsRetryDue() != 0U)))))
    {
        return;
    }

    switch (s_mPlatRun.enHeartbeatStep)
    {
        case MODEM_PLATFORM_HEARTBEAT_STEP_WAIT:
            if (s_mPlatRun.u32HeartbeatHoldMs < MODEM_PLATFORM_HEARTBEAT_PERIOD_MS)
            {
                s_mPlatRun.u32HeartbeatHoldMs += MODEM_PLATFORM_PROCESS_PERIOD_MS;
            }

            if (s_mPlatRun.u32HeartbeatHoldMs >= MODEM_PLATFORM_HEARTBEAT_PERIOD_MS)
            {
                s_mPlatRun.enHeartbeatStep = MODEM_PLATFORM_HEARTBEAT_STEP_PREPARE;
            }
            break;

        case MODEM_PLATFORM_HEARTBEAT_STEP_PREPARE:
            if (ModemPlatform_PrepareHeartbeatFrame() != 0U)
            {
                s_mPlatRun.enHeartbeatStep = MODEM_PLATFORM_HEARTBEAT_STEP_QUEUE;
            }
            else
            {
                HMI_LOG_MODEM("HEARTBEAT_FRAME_FAILED\r\n");
                ModemPlatform_EnterFailure();
            }
            break;

        case MODEM_PLATFORM_HEARTBEAT_STEP_QUEUE:
            ModemPort_ClearFailure();
            if (ModemPort_QueueCommand(s_mPlatRun.acHeartbeatCommand, MODEM_AT_RSP_PROMPT,
                                       MODEM_AT_CFG_SEND_PROMPT_TIMEOUT_MS) != 0U)
            {
                s_mPlatRun.enHeartbeatStep = MODEM_PLATFORM_HEARTBEAT_STEP_WAIT_PROMPT;
            }
            else
            {
                HMI_LOG_MODEM("HEARTBEAT_QUEUE_FAILED\r\n");
                ModemPlatform_EnterFailure();
            }
            break;

        case MODEM_PLATFORM_HEARTBEAT_STEP_WAIT_PROMPT:
            if (ModemPort_HasFailure() != 0U)
            {
                HMI_LOG_MODEM("HEARTBEAT_PROMPT_FAILED\r\n");
                ModemPlatform_EnterFailure();
            }
            else if (ModemPort_IsIdle() != 0U)
            {
                s_mPlatRun.enHeartbeatStep = MODEM_PLATFORM_HEARTBEAT_STEP_SEND;
            }
            else
            {
                /* Wait for the CIPSEND prompt. */
            }
            break;

        case MODEM_PLATFORM_HEARTBEAT_STEP_SEND:
            if (ModemPort_SendData(s_mPlatRun.au8HeartbeatFrame, s_mPlatRun.u16HeartbeatFrameLength,
                                   MODEM_AT_RSP_SEND, MODEM_AT_CFG_SEND_RESULT_TIMEOUT_MS) != 0U)
            {
                s_mPlatRun.enHeartbeatStep = MODEM_PLATFORM_HEARTBEAT_STEP_WAIT_RESULT;
            }
            else
            {
                HMI_LOG_MODEM("HEARTBEAT_SEND_FAILED\r\n");
                ModemPlatform_EnterFailure();
            }
            break;

        case MODEM_PLATFORM_HEARTBEAT_STEP_WAIT_RESULT:
            if (ModemPort_HasFailure() != 0U)
            {
                HMI_LOG_MODEM("HEARTBEAT_RESULT_FAILED\r\n");
                ModemPlatform_EnterFailure();
            }
            else if (ModemPort_IsIdle() != 0U)
            {
                if (ModemPlatform_IsDataSendComplete(ModemPort_GetLastMatchedResponse(), s_mPlatRun.u16HeartbeatFrameLength) != 0U)
                {
                    HMI_LOG_MODEM("HEARTBEAT_SENT\r\n");
                    s_mPlatRun.u32HeartbeatHoldMs = 0U;
                    s_mPlatRun.enHeartbeatStep = MODEM_PLATFORM_HEARTBEAT_STEP_WAIT;
                }
                else
                {
                    HMI_LOG_MODEM("HEARTBEAT_CONFIRM_FAILED\r\n");
                    ModemPlatform_EnterFailure();
                }
            }
            else
            {
                /* Wait for +CIPSEND: <link>,<req>,<cnf>. */
            }
            break;

        default:
            HMI_LOG_MODEM("HEARTBEAT_STEP_UNKNOWN\r\n");
            ModemPlatform_EnterFailure();
            break;
    }
}

/*******************************************************
  * Name    :ModemPlatform_IsRetryDue
  * Author  :LoH
  * Function:Use elapsed wall time and retain one frozen message across all transport retries.
  * Version :V1.0.0
  * Data    :2026.11.14
*******************************************************/
static uint8_t ModemPlatform_IsRetryDue(void)
{
    uint32_t u32Delay = (s_mMsdRun.u8AttemptCount <= 1U) ? MODEM_MSD_FIRST_RETRY_MS : MODEM_MSD_LATER_RETRY_MS;
    return ((s_mMsdRun.u8AttemptCount == 0U)
        || ((uint32_t)(osKernelSysTick() - s_mMsdRun.u32AttemptStartMs) >= u32Delay)) ? 1U : 0U;
}

/*******************************************************
  * Name    :ModemPlatform_IsReplyFrameTypeSupported
  * Author  :LoH
  * Function:Accept the current unified reply type and the deployed legacy reply types.
  * Version :V1.0.0
  * Data    :2026.11.16
*******************************************************/
static uint8_t ModemPlatform_IsReplyFrameTypeSupported(uint8_t u8FrameType)
{
    return ((u8FrameType == MODEM_PLATFORM_REPLY_TYPE_CURRENT)
        || (u8FrameType == MODEM_PLATFORM_REPLY_TYPE_REGISTER_LEGACY)
        || (u8FrameType == MODEM_PLATFORM_REPLY_TYPE_HEARTBEAT_LEGACY)
        || (u8FrameType == MODEM_PLATFORM_REPLY_TYPE_MSD_LEGACY)) ? 1U : 0U;
}

/*******************************************************
  * Name    :ModemPlatform_IsReplyTypeMatchingFrame
  * Author  :LoH
  * Function:Bind each legacy frame type to its JSON reply type while allowing unified replies.
  * Version :V1.0.0
  * Data    :2026.11.16
*******************************************************/
static uint8_t ModemPlatform_IsReplyTypeMatchingFrame(uint8_t u8FrameType, const char *pcType)
{
    if (pcType == 0)
    {
        return 0U;
    }
    if (u8FrameType == MODEM_PLATFORM_REPLY_TYPE_CURRENT)
    {
        return 1U;
    }
    if (u8FrameType == MODEM_PLATFORM_REPLY_TYPE_REGISTER_LEGACY)
    {
        return (strcmp(pcType, "register_ack") == 0) ? 1U : 0U;
    }
    if (u8FrameType == MODEM_PLATFORM_REPLY_TYPE_HEARTBEAT_LEGACY)
    {
        return (strcmp(pcType, "heartbeat_ack") == 0) ? 1U : 0U;
    }
    if (u8FrameType == MODEM_PLATFORM_REPLY_TYPE_MSD_LEGACY)
    {
        return (strcmp(pcType, "msd_ack") == 0) ? 1U : 0U;
    }
    return 0U;
}

/*******************************************************
  * Name    :ModemPlatform_ConsumeStream
  * Author  :LoH
  * Function:Reassemble bounded TCP frames before validating correlated MSD receipts.
  * Version :V1.0.0
  * Data    :2026.11.14
*******************************************************/
static uint8_t ModemPlatform_ConsumeStream(const uint8_t *pu8Bytes, uint16_t u16Length)
{
    uint16_t u16Index;
    uint32_t u32PayloadLength;
    char acType[MODEM_PLATFORM_REPLY_TYPE_LENGTH];
    for (u16Index = 0U; u16Index < u16Length; u16Index++)
    {
        if (s_mPlatRun.u16StreamLength >= sizeof(s_mPlatRun.au8StreamFrame))
        {
            return 0U;
        }
        s_mPlatRun.au8StreamFrame[s_mPlatRun.u16StreamLength++] = pu8Bytes[u16Index];
        if (s_mPlatRun.u16StreamLength < 5U)
        {
            continue;
        }
        u32PayloadLength = ((uint32_t)s_mPlatRun.au8StreamFrame[1] << 24U)
            | ((uint32_t)s_mPlatRun.au8StreamFrame[2] << 16U)
            | ((uint32_t)s_mPlatRun.au8StreamFrame[3] << 8U) | s_mPlatRun.au8StreamFrame[4];
        if ((ModemPlatform_IsReplyFrameTypeSupported(s_mPlatRun.au8StreamFrame[0]) == 0U)
            || (u32PayloadLength > MODEM_MSD_MAX_PAYLOAD_LENGTH))
        {
            return 0U;
        }
        if (s_mPlatRun.u16StreamLength == (u32PayloadLength + 5U))
        {
            if (ModemMsdCodec_ReadReplyType(&s_mPlatRun.au8StreamFrame[5], (uint16_t)u32PayloadLength, acType, sizeof(acType)) != 0U)
            {
                if (ModemPlatform_IsReplyTypeMatchingFrame(s_mPlatRun.au8StreamFrame[0], acType) != 0U)
                {
                    ModemPlatform_LogReplyType(acType);
                    if (strcmp(acType, "register_ack") == 0)
                    {
                        if ((s_mPlatRun.u8AwaitingRegisterAck != 0U)
                            && (ModemMsdCodec_IsMatchingControlAck(&s_mPlatRun.au8StreamFrame[5],
                                (uint16_t)u32PayloadLength, "register_ack") != 0U))
                        {
                            s_mPlatRun.u8AwaitingRegisterAck = 0U;
                            s_mPlatRun.u8RegisterAckReceived = 1U;
                            s_mPlatSts.enEvent = MODEM_PLATFORM_EVENT_REGISTERED;
                            HMI_LOG_MODEM("REGISTER_ACK_CONFIRMED\r\n");
                        }
                        else
                        {
                            HMI_LOG_MODEM("REGISTER_ACK_REJECTED\r\n");
                        }
                    }
                    else if ((strcmp(acType, "msd_ack") == 0) && (s_mMsdRun.u8MsdUploadRequested != 0U)
                        && (s_mMsdRun.u8AttemptCount != 0U))
                    {
                        if (ModemMsdCodec_IsMatchingAck(&s_mPlatRun.au8StreamFrame[5], (uint16_t)u32PayloadLength,
                            s_mMsdRun.u32Ser, 1U, s_mMsdRun.stLastMsd.utc_seconds, MODEM_PLATFORM_MSD_EXTENSION_ENABLED) != 0U)
                        {
                            s_mMsdRun.u8MsdAckReceived = 1U;
                            s_mMsdRun.u8MsdUploadRequested = 0U;
                            s_mMsdRun.u8AwaitingAck = 0U;
                            s_mPlatSts.enEvent = MODEM_PLATFORM_EVENT_MSD_UPLOADED;
                            HMI_LOG_MODEM("MSD_ACK_CONFIRMED\r\n");
                        }
                        else
                        {
                            HMI_LOG_MODEM("MSD_ACK_REJECTED\r\n");
                        }
                    }
                }
                else
                {
                    HMI_LOG_MODEM("PLATFORM_RX_TYPE_MISMATCH\r\n");
                }
            }
            else
            {
                HMI_LOG_MODEM("PLATFORM_RX_TYPE_INVALID\r\n");
            }
            s_mPlatRun.u16StreamLength = 0U;
        }
    }
    return 1U;
}

/*******************************************************
  * Name    :ModemPlatform_ProcessOnline
  * Author  :LoH
  * Function:Retrieve buffered platform frames and schedule heartbeat or MSD work.
  * Version :V1.0.0
  * Data    :2026.11.14
*******************************************************/
static void ModemPlatform_ProcessOnline(uint8_t u8HeartbeatEnabled)
{
    int i32CommandLength;

    ModemPlatform_ProcessHeartbeat(u8HeartbeatEnabled);

    if (s_mPlatSts.enEvent == MODEM_PLATFORM_EVENT_FAILED)
    {
        return;
    }

    switch (s_mPlatRun.enReceiveStep)
    {
        case MODEM_PLATFORM_RX_STEP_IDLE:
            if ((s_mPlatRun.enHeartbeatStep == MODEM_PLATFORM_HEARTBEAT_STEP_WAIT)
                && (ModemPort_TakeRxDataEvent(&s_mPlatRun.u8ReceiveLinkId) != 0U))
            {
                s_mPlatRun.enReceiveStep = MODEM_PLATFORM_RX_STEP_LENGTH_QUEUE;
            }
            break;

        case MODEM_PLATFORM_RX_STEP_LENGTH_QUEUE:
            i32CommandLength = snprintf(s_mPlatRun.acReceiveLengthCommand,
                                        sizeof(s_mPlatRun.acReceiveLengthCommand),
                                        MODEM_AT_CMD_TCP_RX_LENGTH_FORMAT, s_mPlatRun.u8ReceiveLinkId);
            if ((i32CommandLength > 0)
                && ((uint32_t)i32CommandLength < sizeof(s_mPlatRun.acReceiveLengthCommand))
                && (ModemPort_QueueCommand(s_mPlatRun.acReceiveLengthCommand, MODEM_AT_RSP_TCP_RX_LENGTH,
                                           MODEM_AT_CFG_TCP_CFG_TIMEOUT_MS) != 0U))
            {
                s_mPlatRun.enReceiveStep = MODEM_PLATFORM_RX_STEP_LENGTH_WAIT;
            }
            else
            {
                HMI_LOG_MODEM("PLATFORM_RX_LENGTH_QUEUE_FAILED\r\n");
                ModemPlatform_EnterFailure();
            }
            break;

        case MODEM_PLATFORM_RX_STEP_LENGTH_WAIT:
            if (ModemPort_HasFailure() != 0U)
            {
                HMI_LOG_MODEM("PLATFORM_RX_LENGTH_FAILED\r\n");
                ModemPlatform_EnterFailure();
            }
            else if (ModemPort_IsIdle() != 0U)
            {
                if (ModemPlatform_ParseReceiveLength(ModemPort_GetLastMatchedResponse(), s_mPlatRun.u8ReceiveLinkId,
                                                     &s_mPlatRun.u16ReceiveLength) != 0U)
                {
                    if (s_mPlatRun.u16ReceiveLength != 0U)
                    {
                        s_mPlatRun.enReceiveStep = MODEM_PLATFORM_RX_STEP_DATA_QUEUE;
                    }
                    else
                    {
                        HMI_LOG_MODEM("PLATFORM_RX_EMPTY\r\n");
                        s_mPlatRun.enReceiveStep = MODEM_PLATFORM_RX_STEP_IDLE;
                    }
                }
                else
                {
                    HMI_LOG_MODEM("PLATFORM_RX_LENGTH_INVALID\r\n");
                    ModemPlatform_EnterFailure();
                }
            }
            else
            {
                /* Wait for +CIPRXGET: 4,<link>,<restLength>. */
            }
            break;

        case MODEM_PLATFORM_RX_STEP_DATA_QUEUE:
            i32CommandLength = snprintf(s_mPlatRun.acReceiveDataCommand,
                                        sizeof(s_mPlatRun.acReceiveDataCommand),
                                        MODEM_AT_CMD_TCP_RX_DATA_FORMAT, s_mPlatRun.u8ReceiveLinkId,
                                        s_mPlatRun.u16ReceiveLength);
            if ((i32CommandLength > 0)
                && ((uint32_t)i32CommandLength < sizeof(s_mPlatRun.acReceiveDataCommand))
                && (ModemPort_QueueCommand(s_mPlatRun.acReceiveDataCommand, MODEM_AT_RSP_OK,
                                           MODEM_AT_CFG_TCP_CFG_TIMEOUT_MS) != 0U))
            {
                s_mPlatRun.enReceiveStep = MODEM_PLATFORM_RX_STEP_DATA_WAIT;
            }
            else
            {
                HMI_LOG_MODEM("PLATFORM_RX_DATA_QUEUE_FAILED\r\n");
                ModemPlatform_EnterFailure();
            }
            break;

        case MODEM_PLATFORM_RX_STEP_DATA_WAIT:
            if (ModemPort_HasFailure() != 0U)
            {
                HMI_LOG_MODEM("PLATFORM_RX_DATA_FAILED\r\n");
                ModemPlatform_EnterFailure();
            }
            else if (ModemPort_IsIdle() != 0U)
            {
                if ((ModemPlatform_HexToBytes(ModemPort_GetLastDataLine(), s_mPlatRun.u16ReceiveLength, s_mPlatRun.au8ReceiveFrame) != 0U)
                    && (ModemPlatform_ConsumeStream(s_mPlatRun.au8ReceiveFrame, s_mPlatRun.u16ReceiveLength) != 0U))
                {
                    /* Query the remaining modem buffer even when no new receive URC is emitted. */
                    s_mPlatRun.enReceiveStep = MODEM_PLATFORM_RX_STEP_LENGTH_QUEUE;
                }
                else
                {
                    HMI_LOG_MODEM("PLATFORM_RX_FRAME_INVALID\r\n");
                    ModemPlatform_EnterFailure();
                }
            }
            else
            {
                /* Wait for hex payload and final OK from CIPRXGET mode three. */
            }
            break;

        default:
            HMI_LOG_MODEM("PLATFORM_RX_STEP_UNKNOWN\r\n");
            ModemPlatform_EnterFailure();
            break;
    }

    if (s_mPlatSts.enEvent == MODEM_PLATFORM_EVENT_FAILED)
    {
        return;
    }

    if ((s_mMsdRun.u8MsdUploadRequested != 0U)
        && (s_mMsdRun.u8MsdUploadComplete == 0U)
        && (s_mPlatRun.u8RegisterAckReceived != 0U)
        && (ModemPlatform_IsRetryDue() != 0U)
        && (s_mPlatRun.enHeartbeatStep == MODEM_PLATFORM_HEARTBEAT_STEP_WAIT)
        && (s_mPlatRun.enReceiveStep == MODEM_PLATFORM_RX_STEP_IDLE)
        && (ModemPort_IsIdle() != 0U))
    {
        s_mPlatSts.enPhase = MODEM_PLATFORM_PHASE_MSD_UPLOADING;
        s_mPlatSts.enEvent = MODEM_PLATFORM_EVENT_MSD_STARTED;
    }
}

/*******************************************************
  * Name    :ModemPlatform_ProcessMsdUploading
  * Author  :LoH
  * Function:Upload one reference-compatible MSD frame and verify CIPSEND lengths.
  * Version :V1.0.0
  * Data    :2026.11.14
*******************************************************/
static void ModemPlatform_ProcessMsdUploading(void)
{
    switch (s_mPlatRun.enMsdStep)
    {
        case MODEM_PLATFORM_MSD_STEP_PREPARE:
            if (ModemPlatform_PrepareMsdFrame() != 0U)
            {
                s_mMsdRun.u32AttemptStartMs = osKernelSysTick();
                if (s_mMsdRun.u8AttemptCount < 255U)
                {
                    s_mMsdRun.u8AttemptCount++;
                }
                HMI_LOG_MODEM("MSD_UPLOAD_START\r\n");
                s_mPlatRun.enMsdStep = MODEM_PLATFORM_MSD_STEP_QUEUE;
            }
            else
            {
                HMI_LOG_MODEM("MSD_FRAME_FAILED\r\n");
                ModemPlatform_EnterFailure();
            }
            break;

        case MODEM_PLATFORM_MSD_STEP_QUEUE:
            ModemPort_ClearFailure();
            if (ModemPort_QueueCommand(s_mPlatRun.acMsdCommand, MODEM_AT_RSP_PROMPT,
                                       MODEM_AT_CFG_SEND_PROMPT_TIMEOUT_MS) != 0U)
            {
                s_mPlatRun.enMsdStep = MODEM_PLATFORM_MSD_STEP_WAIT_PROMPT;
            }
            else
            {
                HMI_LOG_MODEM("MSD_QUEUE_FAILED\r\n");
                ModemPlatform_EnterFailure();
            }
            break;

        case MODEM_PLATFORM_MSD_STEP_WAIT_PROMPT:
            if (ModemPort_HasFailure() != 0U)
            {
                HMI_LOG_MODEM("MSD_PROMPT_FAILED\r\n");
                ModemPlatform_EnterFailure();
            }
            else if (ModemPort_IsIdle() != 0U)
            {
                s_mPlatRun.enMsdStep = MODEM_PLATFORM_MSD_STEP_SEND;
            }
            else
            {
                /* Wait for the CIPSEND prompt. */
            }
            break;

        case MODEM_PLATFORM_MSD_STEP_SEND:
            if (ModemPort_SendData(s_mMsdRun.au8MsdFrame, s_mMsdRun.u16MsdFrameLength,
                                   MODEM_AT_RSP_SEND, MODEM_AT_CFG_MSD_SEND_RESULT_TIMEOUT_MS) != 0U)
            {
                s_mPlatRun.enMsdStep = MODEM_PLATFORM_MSD_STEP_WAIT_RESULT;
            }
            else
            {
                HMI_LOG_MODEM("MSD_SEND_FAILED\r\n");
                ModemPlatform_EnterFailure();
            }
            break;

        case MODEM_PLATFORM_MSD_STEP_WAIT_RESULT:
            if (ModemPort_HasFailure() != 0U)
            {
                HMI_LOG_MODEM("MSD_RESULT_FAILED\r\n");
                ModemPlatform_EnterFailure();
            }
            else if (ModemPort_IsIdle() != 0U)
            {
                if (ModemPlatform_IsDataSendComplete(ModemPort_GetLastMatchedResponse(), s_mMsdRun.u16MsdFrameLength) != 0U)
                {
                    s_mMsdRun.u8MsdUploadComplete = 1U;
                    s_mMsdRun.u8AwaitingAck = 1U;
                    s_mMsdRun.u32AckStartMs = osKernelSysTick();
                    s_mPlatSts.enPhase = MODEM_PLATFORM_PHASE_ONLINE;
                    HMI_LOG_MODEM("MSD_UPLOADED_WAIT_ACK\r\n");
                }
                else
                {
                    HMI_LOG_MODEM("MSD_CONFIRM_FAILED\r\n");
                    ModemPlatform_EnterFailure();
                }
            }
            else
            {
                /* Wait for +CIPSEND: <link>,<req>,<cnf>. */
            }
            break;

        default:
            HMI_LOG_MODEM("MSD_STEP_UNKNOWN\r\n");
            ModemPlatform_EnterFailure();
            break;
    }
}

/*******************************************************
  * Name    :ModemPlatform_Init
  * Author  :LoH
  * Function:Initialize private platform runtime and status data.
  * Version :V1.0.0
  * Data    :2026.11.14
*******************************************************/
void ModemPlatform_Init(void)
{
    (void)memset(&s_mPlatRun, 0, sizeof(s_mPlatRun));
    (void)memset(&s_mMsdRun, 0, sizeof(s_mMsdRun));
    (void)memset(&s_mMsdQueued, 0, sizeof(s_mMsdQueued));
    s_mMsdRun.u32NextSer = 1U;
    (void)memset(&s_mPlatSts, 0, sizeof(s_mPlatSts));
    s_mPlatRun.enRegisterStep = MODEM_PLATFORM_REGISTER_STEP_PREPARE;
    s_mPlatRun.enReceiveStep = MODEM_PLATFORM_RX_STEP_IDLE;
    s_mPlatRun.enHeartbeatStep = MODEM_PLATFORM_HEARTBEAT_STEP_WAIT;
    s_mPlatRun.enMsdStep = MODEM_PLATFORM_MSD_STEP_PREPARE;
    s_mPlatSts.enPhase = MODEM_PLATFORM_PHASE_IDLE;
    s_mPlatSts.enEvent = MODEM_PLATFORM_EVENT_NONE;
}

/*******************************************************
  * Name    :ModemPlatform_Reset
  * Author  :LoH
  * Function:Stop active platform work while preserving a pending emergency MSD.
  * Version :V1.0.0
  * Data    :2026.11.14
*******************************************************/
void ModemPlatform_Reset(void)
{
    (void)memset(&s_mPlatRun, 0, sizeof(s_mPlatRun));
    (void)memset(&s_mPlatSts, 0, sizeof(s_mPlatSts));
    s_mPlatRun.enRegisterStep = MODEM_PLATFORM_REGISTER_STEP_PREPARE;
    s_mPlatRun.enMsdStep = MODEM_PLATFORM_MSD_STEP_PREPARE;
    s_mMsdRun.u8MsdUploadComplete = 0U;
    s_mMsdRun.u8AwaitingAck = 0U;
    /* Frozen frame, sequence and retry start survive all transport reconnects. */
}

/*******************************************************
  * Name    :ModemPlatform_Start
  * Author  :LoH
  * Function:Start platform registration while preserving a pending MSD request.
  * Version :V1.0.0
  * Data    :2026.08.28
*******************************************************/
void ModemPlatform_Start(void)
{
    s_mPlatRun.enRegisterStep = MODEM_PLATFORM_REGISTER_STEP_PREPARE;
    s_mPlatRun.enReceiveStep = MODEM_PLATFORM_RX_STEP_IDLE;
    s_mPlatRun.enHeartbeatStep = MODEM_PLATFORM_HEARTBEAT_STEP_WAIT;
    s_mPlatRun.u32HeartbeatHoldMs = 0U;
    s_mPlatRun.u8RegisterAckReceived = 0U;
    s_mPlatRun.u8AwaitingRegisterAck = 0U;
    s_mPlatRun.u32RegisterAckStartMs = 0U;
    s_mPlatSts.enPhase = MODEM_PLATFORM_PHASE_REGISTERING;
    s_mPlatSts.enEvent = MODEM_PLATFORM_EVENT_NONE;
}

/*******************************************************
  * Name    :ModemPlatform_RequestMsdUpload
  * Author  :LoH
  * Function:Latch one frozen MSD for upload after registration or active receive work.
  * Version :V1.0.0
  * Data    :2026.11.14
*******************************************************/
void ModemPlatform_RequestMsdUpload(const EcallMsd_t *pstMsd)
{
    ModemIdentitySnapshot_t stIdentity;
    ModemPlatformMsdRuntime_t *pstTarget;
    uint32_t u32NextSer = s_mMsdRun.u32NextSer;
    if (pstMsd != 0)
    {
        if ((s_mMsdRun.u8MsdUploadRequested != 0U) || (s_mPlatSts.enPhase == MODEM_PLATFORM_PHASE_MSD_UPLOADING))
        {
            pstTarget = &s_mMsdQueued;
            if (s_mMsdQueued.u8MsdUploadRequested != 0U)
            {
                HMI_LOG_MODEM("MSD_WAITING_EVENT_REPLACED\r\n");
            }
        }
        else
        {
            pstTarget = &s_mMsdRun;
            s_mPlatRun.enMsdStep = MODEM_PLATFORM_MSD_STEP_PREPARE;
        }
        (void)memset(pstTarget, 0, sizeof(*pstTarget));
        pstTarget->u32Ser = u32NextSer;
        pstTarget->stLastMsd = *pstMsd;
        ModemIdentity_GetSnapshot(&stIdentity);
        pstTarget->u16MsdFrameLength = ModemMsdCodec_BuildFrame(pstMsd, &stIdentity, u32NextSer,
            1U, MODEM_PLATFORM_MSD_EXTENSION_ENABLED, pstTarget->au8MsdFrame, sizeof(pstTarget->au8MsdFrame));
        pstTarget->u8MsdUploadRequested = (pstTarget->u16MsdFrameLength != 0U) ? 1U : 0U;
        pstTarget->u32RequestStartMs = osKernelSysTick();
        s_mMsdRun.u32NextSer = (u32NextSer == UINT32_MAX) ? 1U : (u32NextSer + 1U);
    }
}

/*******************************************************
  * Name    :ModemPlatform_Process
  * Author  :LoH
  * Function:Dispatch one private platform phase.
  * Version :V1.0.0
  * Data    :2026.08.28
*******************************************************/
void ModemPlatform_Process(uint8_t u8HeartbeatEnabled)
{
    switch (s_mPlatSts.enPhase)
    {
        case MODEM_PLATFORM_PHASE_IDLE:
            break;

        case MODEM_PLATFORM_PHASE_REGISTERING:
            ModemPlatform_ProcessRegistering();
            break;

        case MODEM_PLATFORM_PHASE_ONLINE:
            ModemPlatform_ProcessOnline(u8HeartbeatEnabled);
            break;

        case MODEM_PLATFORM_PHASE_MSD_UPLOADING:
            ModemPlatform_ProcessMsdUploading();
            break;

        default:
            HMI_LOG_MODEM("PLATFORM_PHASE_UNKNOWN\r\n");
            ModemPlatform_EnterFailure();
            break;
    }
}

/*******************************************************
  * Name    :ModemPlatform_IsOnline
  * Author  :LoH
  * Function:Return whether the private platform workflow is stably online.
  * Version :V1.0.0
  * Data    :2026.08.28
*******************************************************/
uint8_t ModemPlatform_IsOnline(void)
{
    return (s_mPlatSts.enPhase == MODEM_PLATFORM_PHASE_ONLINE) ? 1U : 0U;
}

/*******************************************************
  * Name    :ModemPlatform_IsBackgroundAllowed
  * Author  :LoH
  * Function:Return whether no receive, heartbeat or emergency upload owns the AT port.
  * Version :V1.0.0
  * Data    :2026.11.14
*******************************************************/
uint8_t ModemPlatform_IsBackgroundAllowed(void)
{
    return ((s_mPlatSts.enPhase == MODEM_PLATFORM_PHASE_ONLINE)
            && (s_mPlatRun.enReceiveStep == MODEM_PLATFORM_RX_STEP_IDLE)
            && (s_mPlatRun.enHeartbeatStep == MODEM_PLATFORM_HEARTBEAT_STEP_WAIT)
            && (ModemPlatform_HasPendingMsd() == 0U)) ? 1U : 0U;
}

/*******************************************************
  * Name    :ModemPlatform_TakeEvent
  * Author  :LoH
  * Function:Return and clear one platform result event.
  * Version :V1.0.0
  * Data    :2026.08.28
*******************************************************/
ModemPlatformEvent_t ModemPlatform_TakeEvent(void)
{
    ModemPlatformEvent_t enEvent = s_mPlatSts.enEvent;
    s_mPlatSts.enEvent = MODEM_PLATFORM_EVENT_NONE;
    return enEvent;
}

/*******************************************************
  * Name    :ModemPlatform_Tick
  * Author  :LoH
  * Function:Advance ACK and overall attempt windows even when another workflow owns the port.
  * Version :V1.0.0
  * Data    :2026.11.14
*******************************************************/
void ModemPlatform_Tick(void)
{
    if ((s_mPlatRun.u8AwaitingRegisterAck != 0U)
        && ((uint32_t)(osKernelSysTick() - s_mPlatRun.u32RegisterAckStartMs)
            >= MODEM_PLATFORM_REGISTER_ACK_TIMEOUT_MS))
    {
        s_mPlatRun.u8AwaitingRegisterAck = 0U;
        HMI_LOG_MODEM("REGISTER_ACK_TIMEOUT\r\n");
        ModemPlatform_EnterFailure();
    }

    if ((s_mMsdRun.u8MsdUploadRequested == 0U)
        && (s_mMsdQueued.u8MsdUploadRequested != 0U)
        && (s_mPlatSts.enPhase != MODEM_PLATFORM_PHASE_MSD_UPLOADING))
    {
        uint32_t u32NextSer = s_mMsdRun.u32NextSer;
        s_mMsdRun = s_mMsdQueued;
        s_mMsdRun.u32NextSer = u32NextSer;
        (void)memset(&s_mMsdQueued, 0, sizeof(s_mMsdQueued));
        s_mPlatRun.enMsdStep = MODEM_PLATFORM_MSD_STEP_PREPARE;
    }
    if (s_mMsdRun.u8MsdUploadRequested == 0U)
    {
        return;
    }
    if ((uint32_t)(osKernelSysTick() - s_mMsdRun.u32RequestStartMs) >= MODEM_MSD_RETRY_WINDOW_MS)
    {
        s_mMsdRun.u8MsdUploadRequested = 0U;
        s_mMsdRun.u8AwaitingAck = 0U;
        HMI_LOG_MODEM("MSD_RETRY_WINDOW_EXPIRED\r\n");
    }
    else if ((s_mMsdRun.u8AwaitingAck != 0U)
        && ((uint32_t)(osKernelSysTick() - s_mMsdRun.u32AckStartMs) >= MODEM_MSD_ACK_TIMEOUT_MS))
    {
        s_mMsdRun.u8AwaitingAck = 0U;
        s_mMsdRun.u8MsdUploadComplete = 0U;
        s_mPlatRun.enMsdStep = MODEM_PLATFORM_MSD_STEP_PREPARE;
        HMI_LOG_MODEM("MSD_ACK_TIMEOUT_CLOSE\r\n");
        ModemPlatform_EnterFailure();
    }
}

/*******************************************************
  * Name    :ModemPlatform_HasPendingMsd
  * Author  :LoH
  * Function:Keep data attempts independent of the voice session terminal state.
  * Version :V1.0.0
  * Data    :2026.11.14
*******************************************************/
uint8_t ModemPlatform_HasPendingMsd(void)
{
    return ((s_mMsdRun.u8MsdUploadRequested != 0U) || (s_mMsdQueued.u8MsdUploadRequested != 0U)) ? 1U : 0U;
}

/*******************************************************
  * Name    :ModemPlatform_IsMsdAttemptDue
  * Author  :LoH
  * Function:Return whether a pending frozen MSD is ready for a new TCP session.
  * Version :V1.0.0
  * Data    :2026.11.17
*******************************************************/
uint8_t ModemPlatform_IsMsdAttemptDue(void)
{
    return ((s_mMsdRun.u8MsdUploadRequested != 0U)
        && (ModemPlatform_IsRetryDue() != 0U)) ? 1U : 0U;
}

/*******************************************************
  * Name    :ModemPlatform_CanYieldPort
  * Author  :LoH
  * Function:Protect CIPSEND prompt and payload as one indivisible service transaction.
  * Version :V1.0.0
  * Data    :2026.11.14
*******************************************************/
uint8_t ModemPlatform_CanYieldPort(void)
{
    return ((s_mPlatSts.enPhase == MODEM_PLATFORM_PHASE_IDLE)
        || ((s_mPlatSts.enPhase == MODEM_PLATFORM_PHASE_ONLINE)
            && (s_mPlatRun.enReceiveStep == MODEM_PLATFORM_RX_STEP_IDLE)
            && (s_mPlatRun.enHeartbeatStep == MODEM_PLATFORM_HEARTBEAT_STEP_WAIT))
        || ((s_mPlatSts.enPhase == MODEM_PLATFORM_PHASE_MSD_UPLOADING)
            && ((s_mPlatRun.enMsdStep == MODEM_PLATFORM_MSD_STEP_PREPARE)
                || (s_mPlatRun.enMsdStep == MODEM_PLATFORM_MSD_STEP_QUEUE)))) ? 1U : 0U;
}
