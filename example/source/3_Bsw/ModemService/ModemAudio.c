/*******************************************************
  * Name    :ModemAudio.c
  * Author  :LoH
  * Function:AC7600 I2C control of the AW87559 external-speaker audio path.
  * Version :V1.0.0
  * Data    :2026.09.13
*******************************************************/
#include "ModemAudio.h"
#include <stdio.h>
#include <string.h>
#include "Hmi.h"
#include "ModemAt_Cfg.h"
#include "ModemPort.h"

typedef enum
{
    MODEM_AUDIO_STEP_IDLE = 0U,
    MODEM_AUDIO_STEP_ID_QUEUE,
    MODEM_AUDIO_STEP_ID_WAIT,
    MODEM_AUDIO_STEP_GAIN_QUEUE,
    MODEM_AUDIO_STEP_GAIN_WAIT,
    MODEM_AUDIO_STEP_CONTROL_QUEUE,
    MODEM_AUDIO_STEP_CONTROL_WAIT,
    MODEM_AUDIO_STEP_GAIN_VERIFY_QUEUE,
    MODEM_AUDIO_STEP_GAIN_VERIFY_WAIT,
    MODEM_AUDIO_STEP_CONTROL_VERIFY_QUEUE,
    MODEM_AUDIO_STEP_CONTROL_VERIFY_WAIT,
    MODEM_AUDIO_STEP_OFF_QUEUE,
    MODEM_AUDIO_STEP_OFF_WAIT,
    MODEM_AUDIO_STEP_READY,
    MODEM_AUDIO_STEP_FAILED
} ModemAudioStep_t;

typedef struct
{
    ModemAudioStep_t enStep;
} ModemAudioRuntime_t;

typedef struct
{
    uint8_t u8SpeakerReady;
    uint8_t u8SpeakerOff;
    uint8_t u8HasFailure;
} ModemAudioStatus_t;

static ModemAudioRuntime_t s_mAudioRun;
static ModemAudioStatus_t s_mAudioSts;

#define MODEM_AUDIO_READBACK_ID                  0x005AU
#define MODEM_AUDIO_READBACK_SPEAKER_GAIN         0x080EU
#define MODEM_AUDIO_READBACK_SPEAKER_ENABLED      0x7809U

/*******************************************************
  * Name    :ModemAudio_EnterFailed
  * Author  :LoH
  * Function:Store one terminal U22 external-speaker configuration failure and log it once.
  * Version :V1.0.0
  * Data    :2026.09.13
*******************************************************/
static void ModemAudio_EnterFailed(void)
{
    s_mAudioRun.enStep = MODEM_AUDIO_STEP_FAILED;
    s_mAudioSts.u8SpeakerReady = 0U;
    s_mAudioSts.u8HasFailure = 1U;
    HMI_LOG_MODEM("AUDIO_SPEAKER_FAILED\r\n");
}

/*******************************************************
  * Name    :ModemAudio_QueueCommand
  * Author  :LoH
  * Function:Queue one U22 I2C AT command only after the shared AT port is idle.
  * Version :V1.0.0
  * Data    :2026.09.13
*******************************************************/
static uint8_t ModemAudio_QueueCommand(const char *pcCommand, const char *pcExpected)
{
    if ((ModemPort_IsIdle() == 0U)
        || (ModemPort_QueueCommand(pcCommand, pcExpected, MODEM_AT_CFG_AUDIO_I2C_TIMEOUT_MS) == 0U))
    {
        return 0U;
    }

    return 1U;
}

/*******************************************************
  * Name    :ModemAudio_IsReadbackExpected
  * Author  :LoH
  * Function:Parse one AC7600 I2C hexadecimal readback and compare its numerical value.
  * Version :V1.0.0
  * Data    :2026.09.13
*******************************************************/
static uint8_t ModemAudio_IsReadbackExpected(uint16_t u16ExpectedValue)
{
    unsigned int u32ReadbackValue;

    if (sscanf(ModemPort_GetLastMatchedResponse(), "+CRIIC: 0x%x", &u32ReadbackValue) != 1)
    {
        return 0U;
    }

    return ((u32ReadbackValue <= 0xFFFFU) && ((uint16_t)u32ReadbackValue == u16ExpectedValue)) ? 1U : 0U;
}

/*******************************************************
  * Name    :ModemAudio_ProcessId
  * Author  :LoH
  * Function:Verify that AC7600 I2C reaches the expected AW87559 device.
  * Version :V1.0.0
  * Data    :2026.09.13
*******************************************************/
static void ModemAudio_ProcessId(void)
{
    if (s_mAudioRun.enStep == MODEM_AUDIO_STEP_ID_QUEUE)
    {
        ModemPort_ClearTransactionFailure();
        if (ModemAudio_QueueCommand(MODEM_AT_CMD_AUDIO_ID_QUERY, MODEM_AT_RSP_AUDIO_READBACK) != 0U)
        {
            s_mAudioRun.enStep = MODEM_AUDIO_STEP_ID_WAIT;
        }
        else
        {
            ModemAudio_EnterFailed();
        }
    }
    else if (ModemPort_HasFailure() != 0U)
    {
        ModemAudio_EnterFailed();
    }
    else if ((ModemPort_IsIdle() != 0U)
             && (ModemAudio_IsReadbackExpected(MODEM_AUDIO_READBACK_ID) != 0U))
    {
        s_mAudioRun.enStep = MODEM_AUDIO_STEP_GAIN_QUEUE;
        HMI_LOG_MODEM("AUDIO_U22_ID_OK\r\n");
    }
    else if (ModemPort_IsIdle() != 0U)
    {
        ModemAudio_EnterFailed();
    }
    else
    {
        /* Wait for the AW87559 ID read response. */
    }
}

/*******************************************************
  * Name    :ModemAudio_ProcessGain
  * Author  :LoH
  * Function:Write the configured external-speaker gain to AW87559 PAGR.
  * Version :V1.0.0
  * Data    :2026.09.13
*******************************************************/
static void ModemAudio_ProcessGain(void)
{
    if (s_mAudioRun.enStep == MODEM_AUDIO_STEP_GAIN_QUEUE)
    {
        if (ModemAudio_QueueCommand(MODEM_AT_CMD_AUDIO_SPEAKER_GAIN, MODEM_AT_RSP_OK) != 0U)
        {
            s_mAudioRun.enStep = MODEM_AUDIO_STEP_GAIN_WAIT;
        }
        else
        {
            ModemAudio_EnterFailed();
        }
    }
    else if (ModemPort_HasFailure() != 0U)
    {
        ModemAudio_EnterFailed();
    }
    else if (ModemPort_IsIdle() != 0U)
    {
        s_mAudioRun.enStep = MODEM_AUDIO_STEP_CONTROL_QUEUE;
    }
    else
    {
        /* Wait for the AW87559 external-speaker PAGR write response. */
    }
}

/*******************************************************
  * Name    :ModemAudio_ProcessControl
  * Author  :LoH
  * Function:Enable AW87559 external-speaker mode, boost, PA and switch.
  * Version :V1.0.0
  * Data    :2026.09.13
*******************************************************/
static void ModemAudio_ProcessControl(void)
{
    if (s_mAudioRun.enStep == MODEM_AUDIO_STEP_CONTROL_QUEUE)
    {
        if (ModemAudio_QueueCommand(MODEM_AT_CMD_AUDIO_SPEAKER_ENABLE, MODEM_AT_RSP_OK) != 0U)
        {
            s_mAudioRun.enStep = MODEM_AUDIO_STEP_CONTROL_WAIT;
        }
        else
        {
            ModemAudio_EnterFailed();
        }
    }
    else if (ModemPort_HasFailure() != 0U)
    {
        ModemAudio_EnterFailed();
    }
    else if (ModemPort_IsIdle() != 0U)
    {
        s_mAudioRun.enStep = MODEM_AUDIO_STEP_GAIN_VERIFY_QUEUE;
    }
    else
    {
        /* Wait for the AW87559 external-speaker SYSCTRL write response. */
    }
}

/*******************************************************
  * Name    :ModemAudio_ProcessGainVerify
  * Author  :LoH
  * Function:Read back AW87559 PAGR and verify external-speaker gain.
  * Version :V1.0.0
  * Data    :2026.09.13
*******************************************************/
static void ModemAudio_ProcessGainVerify(void)
{
    if (s_mAudioRun.enStep == MODEM_AUDIO_STEP_GAIN_VERIFY_QUEUE)
    {
        if (ModemAudio_QueueCommand(MODEM_AT_CMD_AUDIO_GAIN_READ, MODEM_AT_RSP_AUDIO_READBACK) != 0U)
        {
            s_mAudioRun.enStep = MODEM_AUDIO_STEP_GAIN_VERIFY_WAIT;
        }
        else
        {
            ModemAudio_EnterFailed();
        }
    }
    else if (ModemPort_HasFailure() != 0U)
    {
        ModemAudio_EnterFailed();
    }
    else if ((ModemPort_IsIdle() != 0U)
             && (ModemAudio_IsReadbackExpected(MODEM_AUDIO_READBACK_SPEAKER_GAIN) != 0U))
    {
        s_mAudioRun.enStep = MODEM_AUDIO_STEP_CONTROL_VERIFY_QUEUE;
    }
    else if (ModemPort_IsIdle() != 0U)
    {
        ModemAudio_EnterFailed();
    }
    else
    {
        /* Wait for the AW87559 external-speaker PAGR read response. */
    }
}

/*******************************************************
  * Name    :ModemAudio_ProcessControlVerify
  * Author  :LoH
  * Function:Read back AW87559 SYSCTRL and publish verified external-speaker readiness.
  * Version :V1.0.0
  * Data    :2026.09.13
*******************************************************/
static void ModemAudio_ProcessControlVerify(void)
{
    if (s_mAudioRun.enStep == MODEM_AUDIO_STEP_CONTROL_VERIFY_QUEUE)
    {
        if (ModemAudio_QueueCommand(MODEM_AT_CMD_AUDIO_CONTROL_READ, MODEM_AT_RSP_AUDIO_READBACK) != 0U)
        {
            s_mAudioRun.enStep = MODEM_AUDIO_STEP_CONTROL_VERIFY_WAIT;
        }
        else
        {
            ModemAudio_EnterFailed();
        }
    }
    else if (ModemPort_HasFailure() != 0U)
    {
        ModemAudio_EnterFailed();
    }
    else if ((ModemPort_IsIdle() != 0U)
             && (ModemAudio_IsReadbackExpected(MODEM_AUDIO_READBACK_SPEAKER_ENABLED) != 0U))
    {
        s_mAudioRun.enStep = MODEM_AUDIO_STEP_READY;
        s_mAudioSts.u8SpeakerReady = 1U;
        HMI_LOG_MODEM("AUDIO_SPEAKER_READY\r\n");
    }
    else if (ModemPort_IsIdle() != 0U)
    {
        ModemAudio_EnterFailed();
    }
    else
    {
        /* Wait for the AW87559 external-speaker SYSCTRL read response. */
    }
}

/*******************************************************
  * Name    :ModemAudio_ProcessOff
  * Author  :LoH
  * Function:Disable the AW87559 external-speaker path after one terminal voice call.
  * Version :V1.0.0
  * Data    :2026.09.13
*******************************************************/
static void ModemAudio_ProcessOff(void)
{
    if (s_mAudioRun.enStep == MODEM_AUDIO_STEP_OFF_QUEUE)
    {
        if (ModemAudio_QueueCommand(MODEM_AT_CMD_AUDIO_SPEAKER_DISABLE, MODEM_AT_RSP_OK) != 0U)
        {
            s_mAudioRun.enStep = MODEM_AUDIO_STEP_OFF_WAIT;
        }
        else
        {
            ModemAudio_EnterFailed();
        }
    }
    else if (ModemPort_HasFailure() != 0U)
    {
        ModemAudio_EnterFailed();
    }
    else if (ModemPort_IsIdle() != 0U)
    {
        s_mAudioRun.enStep = MODEM_AUDIO_STEP_IDLE;
        s_mAudioSts.u8SpeakerReady = 0U;
        s_mAudioSts.u8SpeakerOff = 1U;
        HMI_LOG_MODEM("AUDIO_SPEAKER_OFF\r\n");
    }
    else
    {
        /* Wait for the AW87559 external-speaker SYSCTRL shutdown write response. */
    }
}

/*******************************************************
  * Name    :ModemAudio_Init
  * Author  :LoH
  * Function:Initialize the private U22 receiver configuration runtime and status.
  * Version :V1.0.0
  * Data    :2026.09.13
*******************************************************/
void ModemAudio_Init(void)
{
    (void)memset(&s_mAudioRun, 0, sizeof(s_mAudioRun));
    (void)memset(&s_mAudioSts, 0, sizeof(s_mAudioSts));
    s_mAudioRun.enStep = MODEM_AUDIO_STEP_IDLE;
}

/*******************************************************
  * Name    :ModemAudio_Reset
  * Author  :LoH
  * Function:Clear one completed or failed U22 receiver configuration result.
  * Version :V1.0.0
  * Data    :2026.09.13
*******************************************************/
void ModemAudio_Reset(void)
{
    ModemAudio_Init();
}

/*******************************************************
  * Name    :ModemAudio_RequestSpeakerEnable
  * Author  :LoH
  * Function:Start one 21 dB AW87559 external-speaker configuration sequence.
  * Version :V1.0.0
  * Data    :2026.09.13
*******************************************************/
uint8_t ModemAudio_RequestSpeakerEnable(void)
{
    if ((s_mAudioRun.enStep != MODEM_AUDIO_STEP_IDLE)
        && (s_mAudioRun.enStep != MODEM_AUDIO_STEP_FAILED)
        && (s_mAudioRun.enStep != MODEM_AUDIO_STEP_READY))
    {
        return 0U;
    }

    s_mAudioSts.u8SpeakerReady = 0U;
    s_mAudioSts.u8SpeakerOff = 0U;
    s_mAudioSts.u8HasFailure = 0U;
    s_mAudioRun.enStep = MODEM_AUDIO_STEP_ID_QUEUE;
    return 1U;
}

/*******************************************************
  * Name    :ModemAudio_RequestSpeakerOff
  * Author  :LoH
  * Function:Start or confirm the U22 external-speaker shutdown sequence.
  * Version :V1.0.0
  * Data    :2026.09.13
*******************************************************/
uint8_t ModemAudio_RequestSpeakerOff(void)
{
    if (s_mAudioSts.u8SpeakerReady == 0U)
    {
        s_mAudioSts.u8SpeakerOff = 1U;
        return 1U;
    }

    if (s_mAudioRun.enStep != MODEM_AUDIO_STEP_READY)
    {
        return 0U;
    }

    s_mAudioSts.u8SpeakerOff = 0U;
    s_mAudioRun.enStep = MODEM_AUDIO_STEP_OFF_QUEUE;
    return 1U;
}

/*******************************************************
  * Name    :ModemAudio_Process
  * Author  :LoH
  * Function:Dispatch the private AW87559 I2C configuration state machine.
  * Version :V1.0.0
  * Data    :2026.09.13
*******************************************************/
void ModemAudio_Process(void)
{
    switch (s_mAudioRun.enStep)
    {
        case MODEM_AUDIO_STEP_ID_QUEUE:
        case MODEM_AUDIO_STEP_ID_WAIT:
            ModemAudio_ProcessId();
            break;

        case MODEM_AUDIO_STEP_GAIN_QUEUE:
        case MODEM_AUDIO_STEP_GAIN_WAIT:
            ModemAudio_ProcessGain();
            break;

        case MODEM_AUDIO_STEP_CONTROL_QUEUE:
        case MODEM_AUDIO_STEP_CONTROL_WAIT:
            ModemAudio_ProcessControl();
            break;

        case MODEM_AUDIO_STEP_GAIN_VERIFY_QUEUE:
        case MODEM_AUDIO_STEP_GAIN_VERIFY_WAIT:
            ModemAudio_ProcessGainVerify();
            break;

        case MODEM_AUDIO_STEP_CONTROL_VERIFY_QUEUE:
        case MODEM_AUDIO_STEP_CONTROL_VERIFY_WAIT:
            ModemAudio_ProcessControlVerify();
            break;

        case MODEM_AUDIO_STEP_OFF_QUEUE:
        case MODEM_AUDIO_STEP_OFF_WAIT:
            ModemAudio_ProcessOff();
            break;

        case MODEM_AUDIO_STEP_IDLE:
        case MODEM_AUDIO_STEP_READY:
        case MODEM_AUDIO_STEP_FAILED:
            break;

        default:
            ModemAudio_EnterFailed();
            break;
    }
}

/*******************************************************
  * Name    :ModemAudio_IsSpeakerReady
  * Author  :LoH
  * Function:Return the verified AW87559 external-speaker readiness conclusion.
  * Version :V1.0.0
  * Data    :2026.09.13
*******************************************************/
uint8_t ModemAudio_IsSpeakerReady(void)
{
    return s_mAudioSts.u8SpeakerReady;
}

/*******************************************************
  * Name    :ModemAudio_IsSpeakerOff
  * Author  :LoH
  * Function:Return the stable U22 external-speaker shutdown conclusion.
  * Version :V1.0.0
  * Data    :2026.09.13
*******************************************************/
uint8_t ModemAudio_IsSpeakerOff(void)
{
    return s_mAudioSts.u8SpeakerOff;
}

/*******************************************************
  * Name    :ModemAudio_HasFailure
  * Author  :LoH
  * Function:Return whether the requested AW87559 configuration failed.
  * Version :V1.0.0
  * Data    :2026.09.13
*******************************************************/
uint8_t ModemAudio_HasFailure(void)
{
    return s_mAudioSts.u8HasFailure;
}
