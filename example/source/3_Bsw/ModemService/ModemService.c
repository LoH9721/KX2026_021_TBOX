/*******************************************************
  * Name    :ModemService.c
  * Author  :LoH
  * Function:Modem emergency call service implementation.
  * Version :V1.0.0
  * Data    :2026.11.14
*******************************************************/
#include "ModemService.h"
#include <string.h>
#include "Hmi.h"
#include "ModemAudio.h"
#include "ModemAt_Cfg.h"
#include "ModemCall.h"
#include "ModemGnss.h"
#include "ModemNetwork.h"
#include "ModemPlatform.h"
#include "ModemPower.h"
#include "ModemPort.h"
#include "ModemTime.h"
#include "ModemIdentity.h"
#include "cmsis_os.h"
#include "FreeRTOS.h"
#include "task.h"

#define MODEM_NETWORK_PROCESS_PERIOD_MS     100U
#define MODEM_NETWORK_SLEEP_HOLD_MS         300000UL

typedef struct
{
    /* EcallTask writes this mailbox under a critical section; ModemTask consumes it. */
    volatile uint8_t u8NewEmergencyRequested;
    EcallMsd_t stRequestedMsd;
    uint8_t u8NetworkRetryWaiting;
    uint32_t u32NetworkRetryStartMs;
    uint32_t u32EmergencyStartMs;
    uint8_t u8EmergencyRequestPending;
    uint8_t u8NetworkOnlineRequested;
    uint8_t u8NetworkSleepRequested;
    uint32_t u32NetworkSleepHoldMs;
    uint8_t u8TcpClosePending;
    uint8_t u8TcpCloseStarted;
    uint32_t u32TcpCloseStartMs;
    uint32_t u32TcpCloseDelayMs;
    volatile uint8_t u8CallbackStandbyUpdatePending;
    volatile uint8_t u8CallbackStandbyRequested;
} ModemServiceRuntime_t;

typedef struct
{
    ModemCallState_t enCallState;
    ModemServiceState_t enServiceState;
} ModemServiceStatus_t;

static ModemServiceRuntime_t s_mSvcRun;
static ModemServiceStatus_t s_mSvcSts;

static void ModemService_TryStartNetwork(void);

/*******************************************************
  * Name    :ModemService_ScheduleTcpClose
  * Author  :LoH
  * Function:Schedule one link-only close after ACK processing has completed.
  * Version :V1.0.0
  * Data    :2026.11.17
*******************************************************/
static void ModemService_ScheduleTcpClose(uint32_t u32DelayMs)
{
    if ((s_mSvcRun.u8TcpClosePending == 0U) && (s_mSvcRun.u8TcpCloseStarted == 0U))
    {
        s_mSvcRun.u8TcpClosePending = 1U;
        s_mSvcRun.u32TcpCloseStartMs = osKernelSysTick();
        s_mSvcRun.u32TcpCloseDelayMs = u32DelayMs;
        HMI_LOG_MODEM("TCP_CLOSE_SCHEDULED\r\n");
    }
}

/*******************************************************
  * Name    :ModemService_IsVoiceCallInProgress
  * Author  :LoH
  * Function:Return whether TCP recovery must wait to avoid interfering with voice setup or audio.
  * Version :V1.0.0
  * Data    :2026.08.28
*******************************************************/
static uint8_t ModemService_IsVoiceCallInProgress(void)
{
    return ((s_mSvcSts.enCallState == MODEM_CALL_DIALING)
            || (s_mSvcSts.enCallState == MODEM_CALL_ACTIVE)) ? 1U : 0U;
}

/*******************************************************
  * Name    :ModemService_EnterNetworkSleep
  * Author  :LoH
  * Function:Stop network and platform work and report a pending eCall failure.
  * Version :V1.0.0
  * Data    :2026.08.28
*******************************************************/
static void ModemService_EnterNetworkSleep(void)
{
    s_mSvcSts.enServiceState = MODEM_SERVICE_SLEEP;
    ModemNetwork_Reset();
    ModemPlatform_Reset();
    s_mSvcRun.u8NetworkOnlineRequested = 0U;

    if (s_mSvcRun.u8EmergencyRequestPending != 0U)
    {
        s_mSvcSts.enCallState = MODEM_CALL_FAILED;
    }
}

/*******************************************************
  * Name    :ModemService_StartEmergencyCallFallback
  * Author  :LoH
  * Function:Release failed data-service work and continue one approved emergency voice call.
  * Version :V1.0.0
  * Data    :2026.11.14
*******************************************************/
static void ModemService_StartEmergencyCallFallback(const char *pcLog)
{
    ModemNetwork_Reset();
    ModemPlatform_Reset();
    s_mSvcSts.enServiceState = MODEM_SERVICE_SLEEP;
    s_mSvcRun.u8NetworkRetryWaiting = 1U;
    s_mSvcRun.u32NetworkRetryStartMs = osKernelSysTick();
    if (s_mSvcSts.enCallState == MODEM_CALL_PREPARING)
    {
        ModemCall_Start(&s_mSvcSts.enCallState);
    }
    /* Pending MSD remains queued and the data network retries independently. */
    HMI_LOG_MODEM(pcLog);
}

/*******************************************************
  * Name    :ModemService_TryStartNetwork
  * Author  :LoH
  * Function:Start the network flow only after the physical modem path is ready for AT traffic.
  * Version :V1.0.0
  * Data    :2026.11.14
*******************************************************/
static void ModemService_TryStartNetwork(void)
{
    if ((s_mSvcRun.u8NetworkOnlineRequested != 0U)
        && (s_mSvcSts.enServiceState == MODEM_SERVICE_SLEEP)
        && (ModemPower_IsOnlineReady() != 0U)
        && ((s_mSvcRun.u8NetworkRetryWaiting == 0U)
            || ((uint32_t)(osKernelSysTick() - s_mSvcRun.u32NetworkRetryStartMs) >= 30000UL)))
    {
        if ((ModemPlatform_HasPendingMsd() != 0U)
            && (ModemPlatform_IsMsdAttemptDue() == 0U))
        {
            return;
        }
        s_mSvcRun.u8NetworkRetryWaiting = 0U;
        if (ModemNetwork_IsPdpReady() != 0U)
        {
            if (ModemNetwork_RequestTcpConnect() != 0U)
            {
                s_mSvcSts.enServiceState = MODEM_SERVICE_NETWORK_STARTUP;
                HMI_LOG_MODEM("TCP_RECONNECT_REQUESTED\r\n");
            }
        }
        else
        {
            ModemNetwork_Start();
            s_mSvcSts.enServiceState = MODEM_SERVICE_NETWORK_STARTUP;
        }
    }
}

/*******************************************************
  * Name    :ModemService_ProcessNetworkStartup
  * Author  :LoH
  * Function:Drive the private network module until TCP is ready or fails.
  * Version :V1.0.0
  * Data    :2026.11.14
*******************************************************/
static void ModemService_ProcessNetworkStartup(void)
{
    if (ModemNetwork_TakeFailure() != 0U)
    {
        if ((s_mSvcRun.u8EmergencyRequestPending != 0U) || (ModemPlatform_HasPendingMsd() != 0U))
        {
            ModemService_StartEmergencyCallFallback("NETWORK_FAILED_CALL_FALLBACK\r\n");
        }
        else
        {
            ModemService_EnterNetworkSleep();
        }
    }
    else if (ModemNetwork_TakeTcpConnected() != 0U)
    {
        ModemPlatform_Start();
        s_mSvcSts.enServiceState = MODEM_SERVICE_PLATFORM_ACTIVE;
    }
}

/*******************************************************
  * Name    :ModemService_ProcessPlatform
  * Author  :LoH
  * Function:Drive the private platform workflow and handle its failure event.
  * Version :V1.0.0
  * Data    :2026.11.14
*******************************************************/
static void ModemService_ProcessPlatform(void)
{
    ModemPlatformEvent_t enEvent;

    /* Short TCP sessions do not send application heartbeat frames. */
    ModemPlatform_Process(0U);
    enEvent = ModemPlatform_TakeEvent();

    switch (enEvent)
    {
        case MODEM_PLATFORM_EVENT_NONE:
        case MODEM_PLATFORM_EVENT_MSD_STARTED:
            break;

        case MODEM_PLATFORM_EVENT_REGISTERED:
            if ((ModemPlatform_HasPendingMsd() == 0U)
                || (ModemPlatform_IsMsdAttemptDue() == 0U))
            {
                ModemService_ScheduleTcpClose(MODEM_PLATFORM_TCP_CLOSE_GRACE_MS);
            }
            break;

        case MODEM_PLATFORM_EVENT_MSD_UPLOADED:
            HMI_LOG_MODEM("MSD_DELIVERY_CONFIRMED\r\n");
            ModemService_ScheduleTcpClose(MODEM_PLATFORM_TCP_CLOSE_GRACE_MS);
            break;

        case MODEM_PLATFORM_EVENT_FAILED:
        default:
            ModemService_ScheduleTcpClose(0U);
            break;
    }
}

/*******************************************************
  * Name    :ModemService_ProcessTcpClose
  * Author  :LoH
  * Function:Close the short TCP session and retain PDP for the next event attempt.
  * Version :V1.0.0
  * Data    :2026.11.17
*******************************************************/
static void ModemService_ProcessTcpClose(void)
{
    if (s_mSvcRun.u8TcpCloseStarted != 0U)
    {
        if (ModemNetwork_TakeTcpClosed() != 0U)
        {
            ModemPlatform_Reset();
            s_mSvcRun.u8TcpCloseStarted = 0U;
            s_mSvcRun.u8TcpClosePending = 0U;
            s_mSvcSts.enServiceState = MODEM_SERVICE_SLEEP;
            s_mSvcRun.u8NetworkOnlineRequested = (ModemPlatform_HasPendingMsd() != 0U) ? 1U : 0U;
            HMI_LOG_MODEM("SHORT_SESSION_CLOSED\r\n");
        }
        else if (ModemNetwork_TakeFailure() != 0U)
        {
            s_mSvcRun.u8TcpCloseStarted = 0U;
            s_mSvcRun.u8TcpClosePending = 0U;
            if ((s_mSvcRun.u8EmergencyRequestPending != 0U) || (ModemPlatform_HasPendingMsd() != 0U))
            {
                ModemService_StartEmergencyCallFallback("TCP_CLOSE_FAILED_CALL_FALLBACK\r\n");
            }
            else
            {
                ModemService_EnterNetworkSleep();
            }
        }
    }
    else if ((s_mSvcRun.u8TcpClosePending != 0U)
        && ((uint32_t)(osKernelSysTick() - s_mSvcRun.u32TcpCloseStartMs) >= s_mSvcRun.u32TcpCloseDelayMs)
        && (ModemPlatform_CanYieldPort() != 0U)
        && (ModemNetwork_CanYieldPort() != 0U)
        && (ModemPort_IsIdle() != 0U)
        && (ModemNetwork_RequestTcpClose() != 0U))
    {
        s_mSvcRun.u8TcpClosePending = 0U;
        s_mSvcRun.u8TcpCloseStarted = 1U;
        HMI_LOG_MODEM("TCP_CLOSE_REQUESTED\r\n");
    }
}

/*******************************************************
  * Name    :ModemService_ProcessOnlineSleep
  * Author  :LoH
  * Function:Apply the KL15 delayed sleep policy after platform online work.
  * Version :V1.0.0
  * Data    :2026.11.14
*******************************************************/
static void ModemService_ProcessOnlineSleep(void)
{
    /* CALLBACK_STANDBY is intentionally excluded so the modem remains ready for platform return calls. */
    if ((s_mSvcRun.u8NetworkSleepRequested != 0U)
        && (ModemPlatform_HasPendingMsd() == 0U)
        && ((s_mSvcSts.enCallState == MODEM_CALL_IDLE)
            || (s_mSvcSts.enCallState == MODEM_CALL_ENDED)
            || (s_mSvcSts.enCallState == MODEM_CALL_FAILED)))
    {
        if (s_mSvcRun.u32NetworkSleepHoldMs < MODEM_NETWORK_SLEEP_HOLD_MS)
        {
            s_mSvcRun.u32NetworkSleepHoldMs += MODEM_NETWORK_PROCESS_PERIOD_MS;
        }

        if (s_mSvcRun.u32NetworkSleepHoldMs >= MODEM_NETWORK_SLEEP_HOLD_MS)
        {
            HMI_LOG_MODEM("KL15_OFF_SLEEP\r\n");
            ModemService_EnterNetworkSleep();
            ModemPower_RequestUartSleep();
            s_mSvcRun.u32NetworkSleepHoldMs = 0U;
        }
    }
}

/*******************************************************
  * Name    :ModemService_Init
  * Author  :LoH
  * Function:Reset modem network, platform, audio and call states before task scheduling.
  * Version :V1.0.0
  * Data    :2026.11.14
*******************************************************/
void ModemService_Init(void)
{
    (void)memset(&s_mSvcRun, 0, sizeof(s_mSvcRun));
    (void)memset(&s_mSvcSts, 0, sizeof(s_mSvcSts));
    s_mSvcSts.enCallState = MODEM_CALL_IDLE;
    s_mSvcSts.enServiceState = MODEM_SERVICE_SLEEP;
    ModemPort_Init();
    ModemNetwork_Init();
    ModemPlatform_Init();
    ModemAudio_Init();
    ModemCall_Init();
    ModemGnss_Init();
    ModemTime_Init();
    ModemIdentity_Init();
    ModemPower_Init();
}

/*******************************************************
  * Name    :ModemService_RequestEmergencyCall
  * Author  :LoH
  * Function:Latch MSD and request emergency network service.
  * Version :V1.0.0
  * Data    :2026.11.14
*******************************************************/
void ModemService_RequestEmergencyCall(const EcallMsd_t *pstMsd)
{
    if (pstMsd == 0)
    {
        return;
    }
    taskENTER_CRITICAL();
    if ((s_mSvcRun.u8NewEmergencyRequested == 0U)
        && ((s_mSvcSts.enCallState == MODEM_CALL_IDLE)
            || (s_mSvcSts.enCallState == MODEM_CALL_CALLBACK_STANDBY)
            || (s_mSvcSts.enCallState == MODEM_CALL_ENDED)
            || (s_mSvcSts.enCallState == MODEM_CALL_FAILED)))
    {
        s_mSvcRun.stRequestedMsd = *pstMsd;
        s_mSvcRun.u8NewEmergencyRequested = 1U;
        s_mSvcSts.enCallState = MODEM_CALL_PREPARING;
    }
    taskEXIT_CRITICAL();
}

/*******************************************************
  * Name    :ModemService_RequestNetworkOnline
  * Author  :LoH
  * Function:Request normal KL15-controlled network online state.
  * Version :V1.0.0
  * Data    :2026.08.27
*******************************************************/
void ModemService_RequestNetworkOnline(void)
{
    s_mSvcRun.u8NetworkSleepRequested = 0U;
    s_mSvcRun.u32NetworkSleepHoldMs = 0U;
    s_mSvcRun.u8NetworkOnlineRequested = 1U;
    ModemPower_RequestOnline();
}

/*******************************************************
  * Name    :ModemService_RequestNetworkSleep
  * Author  :LoH
  * Function:Request normal network sleep after pending emergency work.
  * Version :V1.0.0
  * Data    :2026.08.27
*******************************************************/
void ModemService_RequestNetworkSleep(void)
{
    if (s_mSvcRun.u8NetworkSleepRequested == 0U)
    {
        s_mSvcRun.u8NetworkSleepRequested = 1U;
        s_mSvcRun.u32NetworkSleepHoldMs = 0U;
        HMI_LOG_MODEM("KL15_OFF_HOLD\r\n");
    }
}

/*******************************************************
  * Name    :ModemService_SetCallbackStandby
  * Author  :LoH
  * Function:Post one callback-standby state update from EcallTask to ModemTask.
  * Version :V1.0.0
  * Data    :2026.11.17
*******************************************************/
void ModemService_SetCallbackStandby(uint8_t u8Active)
{
    taskENTER_CRITICAL();
    s_mSvcRun.u8CallbackStandbyRequested = (u8Active != 0U) ? 1U : 0U;
    s_mSvcRun.u8CallbackStandbyUpdatePending = 1U;
    taskEXIT_CRITICAL();
}

/*******************************************************
  * Name    :ModemService_ConsumeCallbackStandbyUpdate
  * Author  :LoH
  * Function:Apply callback standby only in the task that owns the modem call state.
  * Version :V1.0.0
  * Data    :2026.11.17
*******************************************************/
static void ModemService_ConsumeCallbackStandbyUpdate(void)
{
    uint8_t u8Pending;
    uint8_t u8Requested;

    taskENTER_CRITICAL();
    u8Pending = s_mSvcRun.u8CallbackStandbyUpdatePending;
    u8Requested = s_mSvcRun.u8CallbackStandbyRequested;
    s_mSvcRun.u8CallbackStandbyUpdatePending = 0U;
    taskEXIT_CRITICAL();

    if (u8Pending == 0U)
    {
        return;
    }

    if (u8Requested != 0U)
    {
        if ((s_mSvcSts.enCallState == MODEM_CALL_IDLE)
            || (s_mSvcSts.enCallState == MODEM_CALL_ENDED)
            || (s_mSvcSts.enCallState == MODEM_CALL_FAILED))
        {
            s_mSvcSts.enCallState = MODEM_CALL_CALLBACK_STANDBY;
            HMI_LOG_MODEM("CALLBACK_STANDBY_STARTED\r\n");
        }
    }
    else if (s_mSvcSts.enCallState == MODEM_CALL_CALLBACK_STANDBY)
    {
        ModemCall_Reset();
        (void)ModemPort_TakeVoiceEvent();
        s_mSvcSts.enCallState = MODEM_CALL_IDLE;
        HMI_LOG_MODEM("CALLBACK_STANDBY_STOPPED\r\n");
    }
}

/*******************************************************
  * Name    :ModemService_ConsumeEmergencyRequest
  * Author  :LoH
  * Function:Move the EcallTask mailbox into workflows owned exclusively by ModemTask.
  * Version :V1.0.0
  * Data    :2026.11.14
*******************************************************/
static void ModemService_ConsumeEmergencyRequest(void)
{
    EcallMsd_t stMsd;
    uint8_t u8Requested;
    taskENTER_CRITICAL();
    u8Requested = s_mSvcRun.u8NewEmergencyRequested;
    if (u8Requested != 0U)
    {
        stMsd = s_mSvcRun.stRequestedMsd;
        s_mSvcRun.u8NewEmergencyRequested = 0U;
    }
    taskEXIT_CRITICAL();
    if (u8Requested != 0U)
    {
        ModemCall_Reset();
        ModemPlatform_RequestMsdUpload(&stMsd);
        s_mSvcRun.u8EmergencyRequestPending = 1U;
        s_mSvcRun.u8NetworkOnlineRequested = 1U;
        s_mSvcRun.u8NetworkSleepRequested = 0U;
        s_mSvcRun.u8NetworkRetryWaiting = 0U;
        s_mSvcRun.u32EmergencyStartMs = osKernelSysTick();
        ModemPower_RequestOnline();
    }
}

/*******************************************************
  * Name    :ModemService_Process
  * Author  :LoH
  * Function:Dispatch network, platform, audio and independent call workflows.
  * Version :V1.0.0
  * Data    :2026.11.14
*******************************************************/
void ModemService_Process(void)
{
    ModemService_ConsumeEmergencyRequest();
    ModemService_ConsumeCallbackStandbyUpdate();
    ModemPower_Process();
    ModemPlatform_Tick();

    if (ModemIdentity_IsBusy() != 0U)
    {
        ModemIdentity_Process(0U);
        ModemCall_Process(&s_mSvcSts.enCallState, 0U);
        return;
    }
    if ((s_mSvcRun.u8EmergencyRequestPending != 0U) && (ModemGnss_IsBusy() != 0U))
    {
        ModemGnss_AbortForEmergency();
    }
    if (ModemGnss_IsBusy() != 0U)
    {
        ModemGnss_Process(0U);
        ModemCall_Process(&s_mSvcSts.enCallState, 0U);
        return;
    }
    if (ModemTime_IsBusy() != 0U)
    {
        ModemTime_Process(0U);
        ModemCall_Process(&s_mSvcSts.enCallState, 0U);
        return;
    }

    if ((s_mSvcSts.enCallState == MODEM_CALL_PREPARING)
        && (ModemPower_IsOnlineReady() != 0U)
        && ((ModemNetwork_IsVoiceReady() != 0U)
            || ((uint32_t)(osKernelSysTick() - s_mSvcRun.u32EmergencyStartMs) >= 30000UL)))
    {
        ModemCall_Start(&s_mSvcSts.enCallState);
        HMI_LOG_MODEM("VOICE_REQUEST_WITHOUT_ACK_GATE\r\n");
    }

    /* The owner consumes a completed result before another workflow can enqueue work. */
    if ((ModemCall_NeedsPort() != 0U)
        && (ModemNetwork_CanYieldPort() != 0U)
        && (ModemPlatform_CanYieldPort() != 0U))
    {
        ModemAudio_Process();
        ModemCall_Process(&s_mSvcSts.enCallState, 1U);
        return;
    }

    ModemService_TryStartNetwork();
    if (ModemPlatform_CanYieldPort() != 0U)
    {
        ModemNetwork_Process((ModemService_IsVoiceCallInProgress() == 0U) ? 1U : 0U);
    }
    if (ModemNetwork_TakeTransportRecoveryStarted() != 0U)
    {
        if (s_mSvcSts.enServiceState != MODEM_SERVICE_SLEEP)
        {
            ModemPlatform_Reset();
            s_mSvcSts.enServiceState = MODEM_SERVICE_NETWORK_STARTUP;
        }
    }

    ModemService_ProcessTcpClose();

    switch (s_mSvcSts.enServiceState)
    {
        case MODEM_SERVICE_SLEEP:
            break;
        case MODEM_SERVICE_NETWORK_STARTUP:
            ModemService_ProcessNetworkStartup();
            break;
        case MODEM_SERVICE_PLATFORM_ACTIVE:
            if ((s_mSvcRun.u8TcpCloseStarted == 0U)
                && (ModemNetwork_CanYieldPort() != 0U))
            {
                ModemService_ProcessPlatform();
            }
            break;
        default:
            ModemService_EnterNetworkSleep();
            break;
    }

    ModemService_ProcessOnlineSleep();

    ModemCall_Process(&s_mSvcSts.enCallState, 0U);
    if ((s_mSvcSts.enCallState == MODEM_CALL_ENDED) || (s_mSvcSts.enCallState == MODEM_CALL_FAILED))
    {
        s_mSvcRun.u8EmergencyRequestPending = 0U;
    }

    if ((((s_mSvcSts.enServiceState == MODEM_SERVICE_PLATFORM_ACTIVE)
            && (ModemPlatform_IsBackgroundAllowed() != 0U))
        || ((s_mSvcSts.enServiceState == MODEM_SERVICE_SLEEP)
            && (ModemNetwork_IsPdpReady() != 0U)))
        && (s_mSvcRun.u8EmergencyRequestPending == 0U)
        && (s_mSvcRun.u8NetworkSleepRequested == 0U)
        && (s_mSvcRun.u8TcpClosePending == 0U)
        && (s_mSvcRun.u8TcpCloseStarted == 0U)
        && (ModemNetwork_CanYieldPort() != 0U)
        && (ModemPort_IsIdle() != 0U))
    {
        ModemIdentity_Process(1U);
        if (ModemIdentity_IsBusy() == 0U)
        {
            ModemTime_Process(1U);
            if (ModemTime_IsBusy() == 0U)
            {
                ModemGnss_Process(1U);
            }
        }
    }
}

/*******************************************************
  * Name    :ModemService_GetCallState
  * Author  :LoH
  * Function:Return modem call state for EcallTask notification.
  * Version :V1.0.0
  * Data    :2026.08.27
*******************************************************/
ModemCallState_t ModemService_GetCallState(void)
{
    return s_mSvcSts.enCallState;
}

/*******************************************************
  * Name    :ModemService_GetState
  * Author  :LoH
  * Function:Return current modem service lifecycle state.
  * Version :V1.0.0
  * Data    :2026.08.27
*******************************************************/
ModemServiceState_t ModemService_GetState(void)
{
    return s_mSvcSts.enServiceState;
}
