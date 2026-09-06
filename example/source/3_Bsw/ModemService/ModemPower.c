/*******************************************************
  * Name    :ModemPower.c
  * Author  :LoH
  * Function:AC7600 physical power-on and RX-UART sleep control.
  * Version :V1.0.0
  * Data    :2026.09.06
*******************************************************/
#include "ModemPower.h"
#include <string.h>
#include "Hmi.h"
#include "ModemAt_Cfg.h"
#include "ModemPort.h"
#include "OutputService.h"

#define MODEM_POWER_PROCESS_PERIOD_MS        100U
/* Board candidate: verify CAT4_EN rail stabilization time on the target hardware. */
#define MODEM_POWER_RAIL_SETTLE_MS            100U
#define MODEM_POWER_KEY_ASSERT_MS            1000U
#define MODEM_POWER_BOOT_WAIT_MS            25000UL

typedef enum
{
    MODEM_POWER_RAIL_SETTLE = 0U,
    MODEM_POWER_KEY_ASSERT,
    MODEM_POWER_BOOT_WAIT,
    MODEM_POWER_ONLINE,
    MODEM_POWER_UART_SLEEP_WAIT,
    MODEM_POWER_UART_SLEEP,
    MODEM_POWER_UART_WAKE_WAIT
} ModemPowerState_t;

typedef struct
{
    uint8_t u8OnlineRequested;
    uint8_t u8UartSleepRequested;
    uint32_t u32ElapsedMs;
} ModemPowerRuntime_t;

typedef struct
{
    ModemPowerState_t enPowerState;
} ModemPowerStatus_t;

static ModemPowerRuntime_t s_mPowerRun;
static ModemPowerStatus_t s_mPowerSts;

/*******************************************************
  * Name    :ModemPower_EnterState
  * Author  :LoH
  * Function:Enter one physical power-control phase and reset its timer.
  * Version :V1.0.0
  * Data    :2026.09.06
*******************************************************/
static void ModemPower_EnterState(ModemPowerState_t enPowerState)
{
    s_mPowerSts.enPowerState = enPowerState;
    s_mPowerRun.u32ElapsedMs = 0U;
}

/*******************************************************
  * Name    :ModemPower_ProcessStartup
  * Author  :LoH
  * Function:Apply the documented PWRKEY low pulse after CAT4 supply is enabled.
  * Version :V1.0.0
  * Data    :2026.09.06
*******************************************************/
static void ModemPower_ProcessStartup(void)
{
    if (s_mPowerRun.u32ElapsedMs < MODEM_POWER_BOOT_WAIT_MS)
    {
        s_mPowerRun.u32ElapsedMs += MODEM_POWER_PROCESS_PERIOD_MS;
    }

    switch (s_mPowerSts.enPowerState)
    {
        case MODEM_POWER_RAIL_SETTLE:
            if (s_mPowerRun.u32ElapsedMs >= MODEM_POWER_RAIL_SETTLE_MS)
            {
                OutputService_SetState(OUTPUT_SERVICE_SIG_CAT4_PWR_KEY, HIGH);
                HMI_LOG_MODEM("PWRKEY_ASSERT\r\n");
                ModemPower_EnterState(MODEM_POWER_KEY_ASSERT);
            }
            break;

        case MODEM_POWER_KEY_ASSERT:
            if (s_mPowerRun.u32ElapsedMs >= MODEM_POWER_KEY_ASSERT_MS)
            {
                OutputService_SetState(OUTPUT_SERVICE_SIG_CAT4_PWR_KEY, LOW);
                HMI_LOG_MODEM("PWRKEY_RELEASE\r\n");
                ModemPower_EnterState(MODEM_POWER_BOOT_WAIT);
            }
            break;

        case MODEM_POWER_BOOT_WAIT:
            if (s_mPowerRun.u32ElapsedMs >= MODEM_POWER_BOOT_WAIT_MS)
            {
                HMI_LOG_MODEM("POWER_ON_READY\r\n");
                ModemPower_EnterState(MODEM_POWER_ONLINE);
            }
            break;

        default:
            break;
    }
}

/*******************************************************
  * Name    :ModemPower_ProcessUartSleep
  * Author  :LoH
  * Function:Queue and supervise the AC7600 RX-UART sleep command while supply stays enabled.
  * Version :V1.0.0
  * Data    :2026.09.06
*******************************************************/
static void ModemPower_ProcessUartSleep(void)
{
    if (s_mPowerSts.enPowerState == MODEM_POWER_ONLINE)
    {
        if ((s_mPowerRun.u8UartSleepRequested != 0U) && (ModemPort_IsIdle() != 0U))
        {
            if (ModemPort_QueueCommand(MODEM_AT_CMD_UART_SLEEP_RX, MODEM_AT_RSP_OK,
                                       MODEM_AT_CFG_UART_SLEEP_TIMEOUT_MS) != 0U)
            {
                ModemPower_EnterState(MODEM_POWER_UART_SLEEP_WAIT);
            }
            else
            {
                HMI_LOG_MODEM("UART_SLEEP_QUEUE_FAILED\r\n");
                s_mPowerRun.u8UartSleepRequested = 0U;
            }
        }
    }
    else if (s_mPowerSts.enPowerState == MODEM_POWER_UART_SLEEP_WAIT)
    {
        if (ModemPort_HasFailure() != 0U)
        {
            HMI_LOG_MODEM("UART_SLEEP_FAILED\r\n");
            ModemPort_ClearTransactionFailure();
            s_mPowerRun.u8UartSleepRequested = 0U;
            ModemPower_EnterState(MODEM_POWER_ONLINE);
        }
        else if (ModemPort_IsIdle() != 0U)
        {
            HMI_LOG_MODEM("UART_SLEEP_ARMED\r\n");
            ModemPower_EnterState(MODEM_POWER_UART_SLEEP);
        }
    }
    else if (s_mPowerSts.enPowerState == MODEM_POWER_UART_SLEEP)
    {
        if (s_mPowerRun.u8OnlineRequested != 0U)
        {
            if (ModemPort_QueueCommand(MODEM_AT_CMD_BASIC, MODEM_AT_RSP_OK,
                                       MODEM_AT_CFG_SYNC_TIMEOUT_MS) != 0U)
            {
                ModemPower_EnterState(MODEM_POWER_UART_WAKE_WAIT);
            }
            else
            {
                HMI_LOG_MODEM("UART_WAKE_QUEUE_FAILED\r\n");
            }
        }
    }
    else if (s_mPowerSts.enPowerState == MODEM_POWER_UART_WAKE_WAIT)
    {
        if (ModemPort_HasFailure() != 0U)
        {
            HMI_LOG_MODEM("UART_WAKE_FAILED\r\n");
            ModemPort_ClearTransactionFailure();
            s_mPowerRun.u8OnlineRequested = 0U;
            ModemPower_EnterState(MODEM_POWER_UART_SLEEP);
        }
        else if (ModemPort_IsIdle() != 0U)
        {
            HMI_LOG_MODEM("UART_WAKE\r\n");
            ModemPower_EnterState(MODEM_POWER_ONLINE);
        }
    }
    else
    {
        /* Startup phases are handled separately. */
    }
}

/*******************************************************
  * Name    :ModemPower_Init
  * Author  :LoH
  * Function:Enable CAT4 supply and release low-active PWRKEY and RESET before startup timing.
  * Version :V1.0.0
  * Data    :2026.09.06
*******************************************************/
void ModemPower_Init(void)
{
    (void)memset(&s_mPowerRun, 0, sizeof(s_mPowerRun));
    (void)memset(&s_mPowerSts, 0, sizeof(s_mPowerSts));
    OutputService_SetState(OUTPUT_SERVICE_SIG_CAT4_EN, HIGH);
    OutputService_SetState(OUTPUT_SERVICE_SIG_CAT4_PWR_KEY, LOW);
    OutputService_SetState(OUTPUT_SERVICE_SIG_CAT4_RST, LOW);
    ModemPower_EnterState(MODEM_POWER_RAIL_SETTLE);
}

/*******************************************************
  * Name    :ModemPower_RequestOnline
  * Author  :LoH
  * Function:Request normal AT access after startup or RX-UART sleep.
  * Version :V1.0.0
  * Data    :2026.09.06
*******************************************************/
void ModemPower_RequestOnline(void)
{
    s_mPowerRun.u8OnlineRequested = 1U;
    s_mPowerRun.u8UartSleepRequested = 0U;
}

/*******************************************************
  * Name    :ModemPower_RequestUartSleep
  * Author  :LoH
  * Function:Request RX-UART sleep after normal network work has stopped.
  * Version :V1.0.0
  * Data    :2026.09.06
*******************************************************/
void ModemPower_RequestUartSleep(void)
{
    s_mPowerRun.u8OnlineRequested = 0U;
    s_mPowerRun.u8UartSleepRequested = 1U;
}

/*******************************************************
  * Name    :ModemPower_Process
  * Author  :LoH
  * Function:Dispatch physical startup, UART sleep and UART wake phases.
  * Version :V1.0.0
  * Data    :2026.09.06
*******************************************************/
void ModemPower_Process(void)
{
    if ((s_mPowerSts.enPowerState == MODEM_POWER_RAIL_SETTLE)
        || (s_mPowerSts.enPowerState == MODEM_POWER_KEY_ASSERT)
        || (s_mPowerSts.enPowerState == MODEM_POWER_BOOT_WAIT))
    {
        ModemPower_ProcessStartup();
    }
    else
    {
        ModemPower_ProcessUartSleep();
    }
}

/*******************************************************
  * Name    :ModemPower_IsOnlineReady
  * Author  :LoH
  * Function:Return whether ordinary modem AT traffic may be started.
  * Version :V1.0.0
  * Data    :2026.09.06
*******************************************************/
uint8_t ModemPower_IsOnlineReady(void)
{
    return (s_mPowerSts.enPowerState == MODEM_POWER_ONLINE) ? 1U : 0U;
}
