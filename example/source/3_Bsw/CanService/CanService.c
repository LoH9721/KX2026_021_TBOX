/*******************************************************
  * Name    :CanService.c
  * Author  :LoH
  * Function:CAN FD routing, sample eCall input and DBC eCall status output.
  * Version :V1.0.0
  * Data    :2026.08.29
*******************************************************/
#include "CanService.h"
#include "Can.h"
#include "cmsis_os.h"
#include "FreeRTOS.h"
#include "Hmi.h"
#include "TimeService.h"
#include <string.h>

#define CAN_SERVICE_SRS_INFO_ID                0x10CU
#define CAN_SERVICE_ECALL_STATUS_ID           0x21CU
#define CAN_SERVICE_TIME_ID                   0x2A3U
#define CAN_SERVICE_FRAME_LENGTH              8U
#define CAN_SERVICE_ECALL_TX_PERIOD_MS        100U
#define CAN_SERVICE_TIME_TX_PERIOD_MS         100U
#define CAN_SERVICE_TIME_TX_PHASE_MS          50U
#define CAN_SERVICE_LOCAL_UTC_OFFSET_HOURS     8U
#define CAN_SERVICE_ECALL_DATA_FRESH_MS       2000U
#define CAN_SERVICE_END_STATUS_HOLD_MS        200U
#define CAN_SERVICE_BUS_OFF_RECOVERY_MS       1000U
#define CAN_SERVICE_TX_ERROR_REARM_MS          1000U
#define CAN_SERVICE_HOUR_QUARTER_SECONDS      900U
#define CAN_SERVICE_HOUR_RAW_MAX              253U

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
    uint32_t u32LastTimeTxMs;
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
    uint32_t u32LastTxErrorMs;
    uint8_t u8TxErrorReported;
    uint8_t u8EcallTxFailed;
    uint8_t u8TimeTxFailed;
} CanServiceStatus_t;

/*******************************************************
  * Name    :CanService_ReportTxResult
  * Author  :LoH
  * Function:Report one periodic transmit fault once and its later recovery once.
  * Version :V1.0.0
  * Data    :2026.09.13
*******************************************************/
static void CanService_ReportTxResult(uint8_t u8Success, uint8_t *pu8FailureLatched,
                                      const char *pcFailureLog, const char *pcRecoveryLog)
{
    if (u8Success == 0U)
    {
        if (*pu8FailureLatched == 0U)
        {
            *pu8FailureLatched = 1U;
            HMI_LOG_CAN(pcFailureLog);
        }
    }
    else if (*pu8FailureLatched != 0U)
    {
        *pu8FailureLatched = 0U;
        HMI_LOG_CAN(pcRecoveryLog);
    }
}

static CanServiceRuntime_t s_mCanSvcRun;
static CanServiceStatus_t s_mCanSvcSts;

/*******************************************************
  * Name    :CanService_IsSrsInfoFrame
  * Author  :LoH
  * Function:Validate the DBC SRS_Info classic CAN frame format.
  * Version :V1.0.0
  * Data    :2026.08.29
*******************************************************/
static uint8_t CanService_IsSrsInfoFrame(const CanRxFrame_t *pstFrame)
{
    if ((pstFrame == NULL)
        || (pstFrame->u32Id != CAN_SERVICE_SRS_INFO_ID)
        || (pstFrame->u8Ide != 0U)
        || (pstFrame->u8Fdf != 0U)
        || (pstFrame->u8Brs != 0U)
        || (pstFrame->u8Length != CAN_SERVICE_FRAME_LENGTH))
    {
        return 0U;
    }
    return 1U;
}

/*******************************************************
  * Name    :CanService_HandleSrsInfoFrame
  * Author  :LoH
  * Function:Latch one eCall trigger from the DBC SRS crash rising edge.
  * Version :V1.0.0
  * Data    :2026.08.29
*******************************************************/
static void CanService_HandleSrsInfoFrame(const CanRxFrame_t *pstFrame)
{
    CanServiceSrsInfoFrame_t stSrsInfo;
    uint8_t u8Crash;
    uint8_t u8TriggerLatched = 0U;

    if (CanService_IsSrsInfoFrame(pstFrame) == 0U)
    {
        HMI_LOG_CAN("RX_10C_FORMAT_REJECTED\r\n");
        return;
    }

    (void)memcpy(stSrsInfo.U, pstFrame->au8Data, CAN_SERVICE_FRAME_LENGTH);
    u8Crash = (stSrsInfo.B.u8CrashOutputStatus != 0U) ? 1U : 0U;
    portENTER_CRITICAL();
    /* SRS_Info defines only the crash state; MSD impact fields remain unknown. */
    s_mCanSvcRun.u8EcallDataValid = 0U;
    s_mCanSvcRun.u8TriggeredDataPending = 0U;

    if ((u8Crash != 0U) && (s_mCanSvcRun.u8CrashArmed != 0U))
    {
        s_mCanSvcRun.u8CrashArmed = 0U;
        s_mCanSvcRun.u8TriggerPending = 1U;
        u8TriggerLatched = 1U;
    }
    else if (u8Crash == 0U)
    {
        s_mCanSvcRun.u8CrashArmed = 1U;
    }
    portEXIT_CRITICAL();

    if (u8TriggerLatched != 0U)
    {
        HMI_LOG_CAN("RX_10C_ECALL_TRIGGER\r\n");
    }
}

/*******************************************************
  * Name    :CanService_ProcessRx
  * Author  :LoH
  * Function:Drain and route all CAN frames queued by the ISR.
  * Version :V1.0.0
  * Data    :2026.08.29
*******************************************************/
static void CanService_ProcessRx(void)
{
    CanRxFrame_t stFrame;

    while (Can_TakeRxFrame(&stFrame) != 0U)
    {
        if (stFrame.u32Id == CAN_SERVICE_SRS_INFO_ID)
        {
            CanService_HandleSrsInfoFrame(&stFrame);
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
  * Name    :CanService_IsLeapYear
  * Author  :LoH
  * Function:Return whether one Gregorian calendar year is leap.
  * Version :V1.0.0
  * Data    :2026.09.13
*******************************************************/
static uint8_t CanService_IsLeapYear(uint16_t u16Year)
{
    return (((u16Year % 4U) == 0U) && (((u16Year % 100U) != 0U) || ((u16Year % 400U) == 0U))) ? 1U : 0U;
}

/*******************************************************
  * Name    :CanService_GetMonthDays
  * Author  :LoH
  * Function:Return the day count for one specified calendar month.
  * Version :V1.0.0
  * Data    :2026.09.13
*******************************************************/
static uint8_t CanService_GetMonthDays(uint16_t u16Year, uint8_t u8Month)
{
    static const uint8_t au8MonthDays[12] = {31U, 28U, 31U, 30U, 31U, 30U,
                                              31U, 31U, 30U, 31U, 30U, 31U};

    if ((u8Month == 0U) || (u8Month > 12U))
    {
        return 0U;
    }

    return (uint8_t)(au8MonthDays[u8Month - 1U]
                     + (((u8Month == 2U) && (CanService_IsLeapYear(u16Year) != 0U)) ? 1U : 0U));
}

/*******************************************************
  * Name    :CanService_ConvertUtcToLocal
  * Author  :LoH
  * Function:Convert one UTC calendar to the configured local CAN calendar.
  * Version :V1.0.0
  * Data    :2026.09.13
*******************************************************/
static void CanService_ConvertUtcToLocal(TimeServiceTime_t *pstTime)
{
    uint16_t u16Year;

    pstTime->u8Hour = (uint8_t)(pstTime->u8Hour + CAN_SERVICE_LOCAL_UTC_OFFSET_HOURS);
    if (pstTime->u8Hour < 24U)
    {
        return;
    }

    pstTime->u8Hour -= 24U;
    pstTime->u8Day++;
    u16Year = (uint16_t)(2000U + pstTime->u8Year);
    if (pstTime->u8Day <= CanService_GetMonthDays(u16Year, pstTime->u8Month))
    {
        return;
    }

    pstTime->u8Day = 1U;
    pstTime->u8Month++;
    if (pstTime->u8Month <= 12U)
    {
        return;
    }

    pstTime->u8Month = 1U;
    pstTime->u8Year++;
}

/*******************************************************
  * Name    :CanService_BuildEcallStatusFrame
  * Author  :LoH
  * Function:Pack DBC TBOX_EcallInfo 0x21C through the message union view.
  * Version :V1.0.0
  * Data    :2026.08.29
*******************************************************/
static void CanService_BuildEcallStatusFrame(uint8_t *pu8Data, uint32_t u32NowMs)
{
    CanServiceEcallInfoFrame_t stEcallInfo;
    uint32_t u32DurationSeconds;
    uint32_t u32HourRaw;

    (void)memset(&stEcallInfo, 0, sizeof(stEcallInfo));
    u32DurationSeconds = CanService_GetCallDurationSeconds(u32NowMs);
    u32HourRaw = u32DurationSeconds / CAN_SERVICE_HOUR_QUARTER_SECONDS;
    if (u32HourRaw > CAN_SERVICE_HOUR_RAW_MAX)
    {
        u32HourRaw = CAN_SERVICE_HOUR_RAW_MAX;
    }

    stEcallInfo.B.u8EcallStatus = (uint8_t)s_mCanSvcRun.enEcallStatus;
    stEcallInfo.B.u8DurationSecond = (uint8_t)(u32DurationSeconds % 60U);
    stEcallInfo.B.u8DurationMinute = (uint8_t)((u32DurationSeconds / 60U) % 60U);
    stEcallInfo.B.u8DurationHourRaw = (uint8_t)u32HourRaw;
    (void)memcpy(pu8Data, stEcallInfo.U, CAN_SERVICE_FRAME_LENGTH);
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

    CanService_ReportTxResult(Can_TxMsg(CANFD, CAN_SERVICE_ECALL_STATUS_ID, au8Data,
                                        CAN_SERVICE_FRAME_LENGTH), &s_mCanSvcSts.u8EcallTxFailed,
                              "TX_21C_FAILED\r\n", "TX_21C_RECOVERED\r\n");
}

/*******************************************************
  * Name    :CanService_BuildTimeFrame
  * Author  :LoH
  * Function:Pack the DBC TBOX_Time raw values from the shared UTC clock.
  * Version :V1.0.0
  * Data    :2026.09.13
*******************************************************/
static void CanService_BuildTimeFrame(uint8_t *pu8Data)
{
    CanServiceTimeFrame_t stTimeFrame;
    TimeServiceTime_t stUtcTime;
    uint16_t u16Year;

    (void)memset(&stTimeFrame, 0, sizeof(stTimeFrame));
    if (TimeService_GetUtc(&stUtcTime) == TIME_SERVICE_SOURCE_INVALID)
    {
        (void)memcpy(pu8Data, stTimeFrame.U, CAN_SERVICE_FRAME_LENGTH);
        return;
    }

    CanService_ConvertUtcToLocal(&stUtcTime);
    u16Year = (uint16_t)(2000U + stUtcTime.u8Year);
    stTimeFrame.B.u8YearHigh = (uint8_t)(u16Year >> 8U);
    stTimeFrame.B.u8YearLow = (uint8_t)u16Year;
    stTimeFrame.B.u8Month = stUtcTime.u8Month;
    stTimeFrame.B.u8DayRaw = (uint8_t)(stUtcTime.u8Day * 4U);
    stTimeFrame.B.u8Hour = stUtcTime.u8Hour;
    stTimeFrame.B.u8Minute = stUtcTime.u8Minute;
    stTimeFrame.B.u8SecondRaw = (uint8_t)(stUtcTime.u8Second * 4U);
    (void)memcpy(pu8Data, stTimeFrame.U, CAN_SERVICE_FRAME_LENGTH);
}

/*******************************************************
  * Name    :CanService_ProcessTimeTx
  * Author  :LoH
  * Function:Send DBC TBOX_Time as an 8-byte CAN FD+BRS frame every 100 ms.
  * Version :V1.0.0
  * Data    :2026.09.13
*******************************************************/
static void CanService_ProcessTimeTx(void)
{
    uint8_t au8Data[CAN_SERVICE_FRAME_LENGTH];
    uint32_t u32NowMs;

    u32NowMs = osKernelSysTick();
    portENTER_CRITICAL();
    if ((u32NowMs - s_mCanSvcRun.u32LastTimeTxMs) < CAN_SERVICE_TIME_TX_PERIOD_MS)
    {
        portEXIT_CRITICAL();
        return;
    }
    s_mCanSvcRun.u32LastTimeTxMs = u32NowMs;
    portEXIT_CRITICAL();

    CanService_BuildTimeFrame(au8Data);
    CanService_ReportTxResult(Can_TxMsg(CANFD, CAN_SERVICE_TIME_ID, au8Data,
                                        CAN_SERVICE_FRAME_LENGTH), &s_mCanSvcSts.u8TimeTxFailed,
                              "TX_2A3_FAILED\r\n", "TX_2A3_RECOVERED\r\n");
}

/*******************************************************
  * Name    :CanService_ProcessErrors
  * Author  :LoH
  * Function:Log counter changes and recover CAN1 outside interrupt context.
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
        s_mCanSvcSts.u32LastTxErrorMs = u32NowMs;
        if (s_mCanSvcSts.u8TxErrorReported == 0U)
        {
            s_mCanSvcSts.u8TxErrorReported = 1U;
            HMI_LOG_CAN("TX_ERROR\r\n");
        }
    }
    else if ((s_mCanSvcSts.u8TxErrorReported != 0U)
             && ((u32NowMs - s_mCanSvcSts.u32LastTxErrorMs) >= CAN_SERVICE_TX_ERROR_REARM_MS))
    {
        s_mCanSvcSts.u8TxErrorReported = 0U;
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
        s_mCanSvcSts.u8TxErrorReported = 0U;
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
    s_mCanSvcRun.u32LastTimeTxMs = osKernelSysTick()
                                  - (CAN_SERVICE_TIME_TX_PERIOD_MS - CAN_SERVICE_TIME_TX_PHASE_MS);
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
        CanService_ProcessTimeTx();
    }
}

/*******************************************************
  * Name    :CanService_TakeEcallTrigger
  * Author  :LoH
  * Function:Consume one latched SRS_Info crash trigger.
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
  * Function:Return any cached CAN accident data for one frozen MSD snapshot.
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
