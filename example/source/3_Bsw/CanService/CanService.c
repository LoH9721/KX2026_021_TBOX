/*******************************************************
  * Name    :CanService.c
  * Author  :LoH
  * Function:CAN2 FD routing, sample eCall input and DBC eCall status output.
  * Version :V1.0.0
  * Data    :2026.09.06
*******************************************************/
#include "CanService.h"
#include "Can.h"
#include "cmsis_os.h"
#include "FreeRTOS.h"
#include "Hmi.h"
#include <string.h>

#define CAN_SERVICE_MOCK_ECALL_ID             0x126U
#define CAN_SERVICE_ECALL_STATUS_ID           0x21CU
#define CAN_SERVICE_FRAME_LENGTH              8U
#define CAN_SERVICE_ECALL_TX_PERIOD_MS        100U
#define CAN_SERVICE_ECALL_DATA_FRESH_MS       2000U
#define CAN_SERVICE_END_STATUS_HOLD_MS        200U
#define CAN_SERVICE_BUS_OFF_RECOVERY_MS       1000U
#define CAN_SERVICE_HOUR_QUARTER_SECONDS      900U
#define CAN_SERVICE_HOUR_RAW_MAX              253U

#define CAN_SERVICE_CRASH_MASK                0x01U
#define CAN_SERVICE_ROLLOVER_MASK             0x02U

typedef struct
{
    CanServiceEcallData_t stEcallData;
    CanServiceEcallData_t stTriggeredEcallData;
    CanServiceEcallStatus_t enEcallStatus;
    uint32_t u32EcallDataUpdateMs;
    uint32_t u32CallStartMs;
    uint32_t u32CallDurationSeconds;
    uint32_t u32EndStatusDeadlineMs;
    uint32_t u32LastEcallTxMs;
    uint32_t u32BusOffDetectedMs;
    uint8_t u8EcallDataValid;
    uint8_t u8CrashArmed;
    uint8_t u8TriggerPending;
    uint8_t u8TriggeredDataPending;
    uint8_t u8BusOffRecoveryPending;
} CanServiceRuntime_t;

typedef struct
{
    uint32_t u32LastRxOverflowCount;
    uint32_t u32LastTxErrorCount;
} CanServiceStatus_t;

static CanServiceRuntime_t s_mCanSvcRun;
static CanServiceStatus_t s_mCanSvcSts;

/*******************************************************
  * Name    :CanService_IsMockEcallFrame
  * Author  :LoH
  * Function:Validate the exact sample-only CAN FD+BRS accident frame contract.
  * Version :V1.0.0
  * Data    :2026.09.06
*******************************************************/
static uint8_t CanService_IsMockEcallFrame(const CanRxFrame_t *pstFrame)
{
    if ((pstFrame == NULL)
        || (pstFrame->u32Id != CAN_SERVICE_MOCK_ECALL_ID)
        || (pstFrame->u8Ide != 0U)
        || (pstFrame->u8Fdf != 1U)
        || (pstFrame->u8Brs != 1U)
        || (pstFrame->u8Length != CAN_SERVICE_FRAME_LENGTH)
        || (pstFrame->au8Data[4] != 0U)
        || (pstFrame->au8Data[5] != 0U)
        || (pstFrame->au8Data[6] != 0U)
        || (pstFrame->au8Data[7] != 0U))
    {
        return 0U;
    }
    return 1U;
}

/*******************************************************
  * Name    :CanService_HandleMockEcallFrame
  * Author  :LoH
  * Function:Cache accident data and latch one crash rising-edge trigger.
  * Version :V1.0.0
  * Data    :2026.09.06
*******************************************************/
static void CanService_HandleMockEcallFrame(const CanRxFrame_t *pstFrame)
{
    uint8_t u8Crash;
    uint8_t u8TriggerLatched = 0U;

    if (CanService_IsMockEcallFrame(pstFrame) == 0U)
    {
        HMI_LOG_CAN("RX_126_FORMAT_REJECTED\r\n");
        return;
    }

    u8Crash = ((pstFrame->au8Data[0] & CAN_SERVICE_CRASH_MASK) != 0U) ? 1U : 0U;
    portENTER_CRITICAL();
    s_mCanSvcRun.stEcallData.u8Rollover =
        ((pstFrame->au8Data[0] & CAN_SERVICE_ROLLOVER_MASK) != 0U) ? 1U : 0U;
    s_mCanSvcRun.stEcallData.u8DeltaVLongitudinalRaw = pstFrame->au8Data[1];
    s_mCanSvcRun.stEcallData.u8DeltaVLateralRaw = pstFrame->au8Data[2];
    s_mCanSvcRun.stEcallData.u8Occupants = pstFrame->au8Data[3];
    s_mCanSvcRun.u32EcallDataUpdateMs = osKernelSysTick();
    s_mCanSvcRun.u8EcallDataValid = 1U;

    if ((u8Crash != 0U) && (s_mCanSvcRun.u8CrashArmed != 0U))
    {
        s_mCanSvcRun.u8CrashArmed = 0U;
        s_mCanSvcRun.u8TriggerPending = 1U;
        s_mCanSvcRun.stTriggeredEcallData = s_mCanSvcRun.stEcallData;
        s_mCanSvcRun.u8TriggeredDataPending = 1U;
        u8TriggerLatched = 1U;
    }
    else if (u8Crash == 0U)
    {
        s_mCanSvcRun.u8CrashArmed = 1U;
    }
    portEXIT_CRITICAL();

    if (u8TriggerLatched != 0U)
    {
        HMI_LOG_CAN("RX_126_ECALL_TRIGGER\r\n");
    }
}

/*******************************************************
  * Name    :CanService_ProcessRx
  * Author  :LoH
  * Function:Drain and route all CAN frames queued by the ISR.
  * Version :V1.0.0
  * Data    :2026.09.06
*******************************************************/
static void CanService_ProcessRx(void)
{
    CanRxFrame_t stFrame;

    while (Can_TakeRxFrame(&stFrame) != 0U)
    {
        if (stFrame.u32Id == CAN_SERVICE_MOCK_ECALL_ID)
        {
            CanService_HandleMockEcallFrame(&stFrame);
        }
    }
}

/*******************************************************
  * Name    :CanService_GetCallDurationSeconds
  * Author  :LoH
  * Function:Return the active or frozen call duration for 0x21C packing.
  * Version :V1.0.0
  * Data    :2026.08.29
*******************************************************/
static uint32_t CanService_GetCallDurationSeconds(uint32_t u32NowMs)
{
    if (s_mCanSvcRun.enEcallStatus == CAN_SERVICE_ECALL_ON_THE_PHONE)
    {
        return (u32NowMs - s_mCanSvcRun.u32CallStartMs) / 1000U;
    }
    return s_mCanSvcRun.u32CallDurationSeconds;
}

/*******************************************************
  * Name    :CanService_BuildEcallStatusFrame
  * Author  :LoH
  * Function:Pack DBC TBOX_EcallInfo 0x21C without C bitfields.
  * Version :V1.0.0
  * Data    :2026.08.29
*******************************************************/
static void CanService_BuildEcallStatusFrame(uint8_t *pu8Data, uint32_t u32NowMs)
{
    uint32_t u32DurationSeconds;
    uint32_t u32HourRaw;

    (void)memset(pu8Data, 0, CAN_SERVICE_FRAME_LENGTH);
    u32DurationSeconds = CanService_GetCallDurationSeconds(u32NowMs);
    u32HourRaw = u32DurationSeconds / CAN_SERVICE_HOUR_QUARTER_SECONDS;
    if (u32HourRaw > CAN_SERVICE_HOUR_RAW_MAX)
    {
        u32HourRaw = CAN_SERVICE_HOUR_RAW_MAX;
    }

    pu8Data[0] = (uint8_t)s_mCanSvcRun.enEcallStatus & 0x07U;
    pu8Data[1] = (uint8_t)(u32DurationSeconds % 60U);
    pu8Data[2] = (uint8_t)((u32DurationSeconds / 60U) % 60U);
    pu8Data[3] = (uint8_t)u32HourRaw;
}

/*******************************************************
  * Name    :CanService_ProcessEcallStatusTx
  * Author  :LoH
  * Function:Send DBC TBOX_EcallInfo as an 8-byte CAN FD+BRS frame every 100 ms.
  * Version :V1.0.0
  * Data    :2026.08.29
*******************************************************/
static void CanService_ProcessEcallStatusTx(void)
{
    uint8_t au8Data[CAN_SERVICE_FRAME_LENGTH];
    uint32_t u32NowMs;

    u32NowMs = osKernelSysTick();
    portENTER_CRITICAL();
    if ((s_mCanSvcRun.enEcallStatus == CAN_SERVICE_ECALL_END_OF_CALL)
        && ((int32_t)(u32NowMs - s_mCanSvcRun.u32EndStatusDeadlineMs) >= 0))
    {
        s_mCanSvcRun.enEcallStatus = CAN_SERVICE_ECALL_NO_DISPLAY;
    }

    if ((u32NowMs - s_mCanSvcRun.u32LastEcallTxMs) < CAN_SERVICE_ECALL_TX_PERIOD_MS)
    {
        portEXIT_CRITICAL();
        return;
    }
    s_mCanSvcRun.u32LastEcallTxMs = u32NowMs;
    CanService_BuildEcallStatusFrame(au8Data, u32NowMs);
    portEXIT_CRITICAL();

    if (Can_TxMsg(CANFD, CAN_SERVICE_ECALL_STATUS_ID, au8Data, CAN_SERVICE_FRAME_LENGTH) == FALSE)
    {
        HMI_LOG_CAN("TX_21C_FAILED\r\n");
    }
}

/*******************************************************
  * Name    :CanService_ProcessErrors
  * Author  :LoH
  * Function:Log counter changes and recover CAN2 outside interrupt context.
  * Version :V1.0.0
  * Data    :2026.08.29
*******************************************************/
static void CanService_ProcessErrors(void)
{
    uint32_t u32NowMs;
    uint32_t u32RxOverflowCount;
    uint32_t u32TxErrorCount;

    u32NowMs = osKernelSysTick();
    u32RxOverflowCount = Can_GetRxOverflowCount();
    u32TxErrorCount = Can_GetTxErrorCount();

    if (u32RxOverflowCount != s_mCanSvcSts.u32LastRxOverflowCount)
    {
        s_mCanSvcSts.u32LastRxOverflowCount = u32RxOverflowCount;
        HMI_LOG_CAN("RX_OVERFLOW\r\n");
    }
    if (u32TxErrorCount != s_mCanSvcSts.u32LastTxErrorCount)
    {
        s_mCanSvcSts.u32LastTxErrorCount = u32TxErrorCount;
        HMI_LOG_CAN("TX_ERROR\r\n");
    }

    if ((Can_GetBusOffFlag() != 0U) && (s_mCanSvcRun.u8BusOffRecoveryPending == 0U))
    {
        s_mCanSvcRun.u8BusOffRecoveryPending = 1U;
        s_mCanSvcRun.u32BusOffDetectedMs = u32NowMs;
        HMI_LOG_CAN("BUS_OFF\r\n");
    }
    if ((s_mCanSvcRun.u8BusOffRecoveryPending != 0U)
        && ((u32NowMs - s_mCanSvcRun.u32BusOffDetectedMs) >= CAN_SERVICE_BUS_OFF_RECOVERY_MS))
    {
        Net_Can_DeInit();
        Can_Init();
        s_mCanSvcRun.u8BusOffRecoveryPending = 0U;
        s_mCanSvcSts.u32LastRxOverflowCount = 0U;
        s_mCanSvcSts.u32LastTxErrorCount = 0U;
        HMI_LOG_CAN("BUS_OFF_RECOVERY\r\n");
    }
}

/*******************************************************
  * Name    :CanService_Init
  * Author  :LoH
  * Function:Initialize service defaults and start CAN2 hardware.
  * Version :V1.0.0
  * Data    :2026.08.29
*******************************************************/
void CanService_Init(void)
{
    (void)memset(&s_mCanSvcRun, 0, sizeof(s_mCanSvcRun));
    (void)memset(&s_mCanSvcSts, 0, sizeof(s_mCanSvcSts));
    s_mCanSvcRun.stEcallData.u8Occupants = 0xFFU;
    s_mCanSvcRun.u8CrashArmed = 1U;
    s_mCanSvcRun.enEcallStatus = CAN_SERVICE_ECALL_NO_DISPLAY;
    Can_Init();
    HMI_LOG_CAN("CAN2_FD_500K_2M_INIT\r\n");
}

/*******************************************************
  * Name    :CanService_Process
  * Author  :LoH
  * Function:Execute one bounded CAN service processing cycle.
  * Version :V1.0.0
  * Data    :2026.08.29
*******************************************************/
void CanService_Process(void)
{
    CanService_ProcessRx();
    CanService_ProcessErrors();
    if (s_mCanSvcRun.u8BusOffRecoveryPending == 0U)
    {
        CanService_ProcessEcallStatusTx();
    }
}

/*******************************************************
  * Name    :CanService_TakeEcallTrigger
  * Author  :LoH
  * Function:Consume one latched sample-frame crash trigger.
  * Version :V1.0.0
  * Data    :2026.08.29
*******************************************************/
uint8_t CanService_TakeEcallTrigger(void)
{
    uint8_t u8Trigger;

    portENTER_CRITICAL();
    u8Trigger = s_mCanSvcRun.u8TriggerPending;
    s_mCanSvcRun.u8TriggerPending = 0U;
    portEXIT_CRITICAL();
    return u8Trigger;
}

/*******************************************************
  * Name    :CanService_GetEcallData
  * Author  :LoH
  * Function:Copy fresh accident data for one frozen MSD snapshot.
  * Version :V1.0.0
  * Data    :2026.08.29
*******************************************************/
uint8_t CanService_GetEcallData(CanServiceEcallData_t *pstData)
{
    uint8_t u8Valid;
    uint32_t u32NowMs;

    if (pstData == NULL)
    {
        return 0U;
    }

    u32NowMs = osKernelSysTick();
    portENTER_CRITICAL();
    if (s_mCanSvcRun.u8TriggeredDataPending != 0U)
    {
        *pstData = s_mCanSvcRun.stTriggeredEcallData;
        s_mCanSvcRun.u8TriggeredDataPending = 0U;
        u8Valid = 1U;
    }
    else
    {
        u8Valid = ((s_mCanSvcRun.u8EcallDataValid != 0U)
                   && ((u32NowMs - s_mCanSvcRun.u32EcallDataUpdateMs)
                       <= CAN_SERVICE_ECALL_DATA_FRESH_MS)) ? 1U : 0U;
        if (u8Valid != 0U)
        {
            *pstData = s_mCanSvcRun.stEcallData;
        }
    }
    portEXIT_CRITICAL();
    return u8Valid;
}

/*******************************************************
  * Name    :CanService_SetEcallStatus
  * Author  :LoH
  * Function:Update the 0x21C status and preserve one visible end-of-call interval.
  * Version :V1.0.0
  * Data    :2026.08.29
*******************************************************/
void CanService_SetEcallStatus(CanServiceEcallStatus_t enStatus)
{
    uint32_t u32NowMs;

    if (enStatus > CAN_SERVICE_ECALL_END_OF_CALL)
    {
        return;
    }

    u32NowMs = osKernelSysTick();
    portENTER_CRITICAL();
    if ((enStatus == CAN_SERVICE_ECALL_ON_THE_PHONE)
        && (s_mCanSvcRun.enEcallStatus != CAN_SERVICE_ECALL_ON_THE_PHONE))
    {
        s_mCanSvcRun.u32CallStartMs = u32NowMs;
        s_mCanSvcRun.u32CallDurationSeconds = 0U;
    }
    else if (enStatus == CAN_SERVICE_ECALL_END_OF_CALL)
    {
        s_mCanSvcRun.u32CallDurationSeconds = CanService_GetCallDurationSeconds(u32NowMs);
        s_mCanSvcRun.u32EndStatusDeadlineMs = u32NowMs + CAN_SERVICE_END_STATUS_HOLD_MS;
    }
    else if ((enStatus == CAN_SERVICE_ECALL_NO_DISPLAY)
             && (s_mCanSvcRun.enEcallStatus == CAN_SERVICE_ECALL_END_OF_CALL)
             && ((int32_t)(u32NowMs - s_mCanSvcRun.u32EndStatusDeadlineMs) < 0))
    {
        portEXIT_CRITICAL();
        return;
    }
    else if (enStatus == CAN_SERVICE_ECALL_NO_DISPLAY)
    {
        s_mCanSvcRun.u32CallDurationSeconds = 0U;
    }
    s_mCanSvcRun.enEcallStatus = enStatus;
    portEXIT_CRITICAL();
}
