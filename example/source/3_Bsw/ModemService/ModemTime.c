/*******************************************************
  * Name    :ModemTime.c
  * Author  :LoH
  * Function:AC7600 NITZ/RTC UTC-time query workflow.
  * Version :V1.0.0
  * Data    :2026.09.13
*******************************************************/
#include "ModemTime.h"
#include <stdio.h>
#include <string.h>
#include "cmsis_os.h"
#include "FreeRTOS.h"
#include "ModemAt_Cfg.h"
#include "ModemPort.h"

#define MODEM_TIME_QUERY_PERIOD_MS 10000UL

typedef enum
{
    MODEM_TIME_STEP_CTZU_QUERY_QUEUE = 0U,
    MODEM_TIME_STEP_CTZU_QUERY_WAIT,
    MODEM_TIME_STEP_CTZU_SET_QUEUE,
    MODEM_TIME_STEP_CTZU_SET_WAIT,
    MODEM_TIME_STEP_QUERY_HOLD,
    MODEM_TIME_STEP_CCLK_QUEUE,
    MODEM_TIME_STEP_CCLK_WAIT
} ModemTimeStep_t;

typedef struct
{
    ModemTimeStep_t enStep;
    uint32_t u32LastQueryMs;
    ModemTimeSample_t stSample;
    uint8_t u8SamplePending;
} ModemTimeRuntime_t;

static ModemTimeRuntime_t s_mTimeRun;

/*******************************************************
  * Name    :ModemTime_IsLeapYear
  * Author  :LoH
  * Function:Return whether one Gregorian calendar year is leap.
  * Version :V1.0.0
  * Data    :2026.09.13
*******************************************************/
static uint8_t ModemTime_IsLeapYear(uint16_t u16Year)
{
    return (((u16Year % 4U) == 0U) && (((u16Year % 100U) != 0U) || ((u16Year % 400U) == 0U))) ? 1U : 0U;
}

/*******************************************************
  * Name    :ModemTime_GetMonthDays
  * Author  :LoH
  * Function:Return the day count for one specified calendar month.
  * Version :V1.0.0
  * Data    :2026.09.13
*******************************************************/
static uint8_t ModemTime_GetMonthDays(uint16_t u16Year, uint8_t u8Month)
{
    static const uint8_t au8MonthDays[12] = {31U, 28U, 31U, 30U, 31U, 30U,
                                              31U, 31U, 30U, 31U, 30U, 31U};

    if ((u8Month == 0U) || (u8Month > 12U))
    {
        return 0U;
    }
    return (uint8_t)(au8MonthDays[u8Month - 1U]
                     + (((u8Month == 2U) && (ModemTime_IsLeapYear(u16Year) != 0U)) ? 1U : 0U));
}

/*******************************************************
  * Name    :ModemTime_CalendarToUtcSeconds
  * Author  :LoH
  * Function:Convert one local CCLK value to UTC seconds since 1970.
  * Version :V1.0.0
  * Data    :2026.09.13
*******************************************************/
static uint8_t ModemTime_CalendarToUtcSeconds(const ModemTimeSample_t *pstLocalTime,
                                               int16_t i16TimezoneQuarter,
                                               uint32_t *pu32UtcSeconds)
{
    uint16_t u16Year;
    uint16_t u16LoopYear;
    uint32_t u32Days = 0U;
    uint8_t u8Month;
    int32_t i32UtcSeconds;

    if ((pstLocalTime == 0) || (pu32UtcSeconds == 0)
        || (pstLocalTime->u8Year < 24U) || (pstLocalTime->u8Month == 0U)
        || (pstLocalTime->u8Day == 0U) || (pstLocalTime->u8Hour > 23U)
        || (pstLocalTime->u8Minute > 59U) || (pstLocalTime->u8Second > 59U)
        || (pstLocalTime->u8Day > ModemTime_GetMonthDays((uint16_t)(2000U + pstLocalTime->u8Year), pstLocalTime->u8Month)))
    {
        return 0U;
    }

    u16Year = (uint16_t)(2000U + pstLocalTime->u8Year);
    for (u16LoopYear = 1970U; u16LoopYear < u16Year; u16LoopYear++)
    {
        u32Days += (ModemTime_IsLeapYear(u16LoopYear) != 0U) ? 366U : 365U;
    }
    for (u8Month = 1U; u8Month < pstLocalTime->u8Month; u8Month++)
    {
        u32Days += ModemTime_GetMonthDays(u16Year, u8Month);
    }

    i32UtcSeconds = (int32_t)((u32Days + (uint32_t)(pstLocalTime->u8Day - 1U)) * 86400UL)
                    + ((int32_t)pstLocalTime->u8Hour * 3600)
                    + ((int32_t)pstLocalTime->u8Minute * 60)
                    + pstLocalTime->u8Second
                    - ((int32_t)i16TimezoneQuarter * 900);
    if (i32UtcSeconds < 0)
    {
        return 0U;
    }
    *pu32UtcSeconds = (uint32_t)i32UtcSeconds;
    return 1U;
}

/*******************************************************
  * Name    :ModemTime_UtcSecondsToCalendar
  * Author  :LoH
  * Function:Convert UTC seconds since 1970 to the exported two-digit calendar.
  * Version :V1.0.0
  * Data    :2026.09.13
*******************************************************/
static void ModemTime_UtcSecondsToCalendar(uint32_t u32UtcSeconds, ModemTimeSample_t *pstTime)
{
    uint16_t u16Year = 1970U;
    uint32_t u32Days = u32UtcSeconds / 86400UL;
    uint32_t u32SecondsOfDay = u32UtcSeconds % 86400UL;
    uint16_t u16DaysInYear;
    uint8_t u8Month = 1U;
    uint8_t u8DaysInMonth;

    while (u16Year < 2100U)
    {
        u16DaysInYear = (ModemTime_IsLeapYear(u16Year) != 0U) ? 366U : 365U;
        if (u32Days < u16DaysInYear)
        {
            break;
        }
        u32Days -= u16DaysInYear;
        u16Year++;
    }
    while (u8Month <= 12U)
    {
        u8DaysInMonth = ModemTime_GetMonthDays(u16Year, u8Month);
        if (u32Days < u8DaysInMonth)
        {
            break;
        }
        u32Days -= u8DaysInMonth;
        u8Month++;
    }

    pstTime->u8Year = (uint8_t)(u16Year - 2000U);
    pstTime->u8Month = u8Month;
    pstTime->u8Day = (uint8_t)(u32Days + 1U);
    pstTime->u8Hour = (uint8_t)(u32SecondsOfDay / 3600UL);
    pstTime->u8Minute = (uint8_t)((u32SecondsOfDay % 3600UL) / 60UL);
    pstTime->u8Second = (uint8_t)(u32SecondsOfDay % 60UL);
}

/*******************************************************
  * Name    :ModemTime_ParseCclk
  * Author  :LoH
  * Function:Parse one valid CCLK response and normalize it to UTC.
  * Version :V1.0.0
  * Data    :2026.09.13
*******************************************************/
static uint8_t ModemTime_ParseCclk(const char *pcResponse, ModemTimeSample_t *pstUtcTime)
{
    unsigned int uYear;
    unsigned int uMonth;
    unsigned int uDay;
    unsigned int uHour;
    unsigned int uMinute;
    unsigned int uSecond;
    unsigned int uTimezone;
    char cTimezoneSign;
    int16_t i16TimezoneQuarter;
    uint32_t u32UtcSeconds;
    ModemTimeSample_t stLocalTime;

    if ((pcResponse == 0) || (pstUtcTime == 0)
        || (sscanf(pcResponse, "+CCLK: \"%2u/%2u/%2u,%2u:%2u:%2u%c%2u\"",
                   &uYear, &uMonth, &uDay, &uHour, &uMinute, &uSecond, &cTimezoneSign, &uTimezone) != 8))
    {
        return 0U;
    }
    if ((uTimezone > 48U) || ((cTimezoneSign != '+') && (cTimezoneSign != '-')))
    {
        return 0U;
    }

    stLocalTime.u8Year = (uint8_t)uYear;
    stLocalTime.u8Month = (uint8_t)uMonth;
    stLocalTime.u8Day = (uint8_t)uDay;
    stLocalTime.u8Hour = (uint8_t)uHour;
    stLocalTime.u8Minute = (uint8_t)uMinute;
    stLocalTime.u8Second = (uint8_t)uSecond;
    i16TimezoneQuarter = (int16_t)uTimezone;
    if (cTimezoneSign == '-')
    {
        i16TimezoneQuarter = (int16_t)(-i16TimezoneQuarter);
    }
    if (ModemTime_CalendarToUtcSeconds(&stLocalTime, i16TimezoneQuarter, &u32UtcSeconds) == 0U)
    {
        return 0U;
    }
    ModemTime_UtcSecondsToCalendar(u32UtcSeconds, pstUtcTime);
    return 1U;
}

/*******************************************************
  * Name    :ModemTime_Init
  * Author  :LoH
  * Function:Reset the private NITZ configuration and CCLK query workflow.
  * Version :V1.0.0
  * Data    :2026.09.13
*******************************************************/
void ModemTime_Init(void)
{
    (void)memset(&s_mTimeRun, 0, sizeof(s_mTimeRun));
    s_mTimeRun.enStep = MODEM_TIME_STEP_CTZU_QUERY_QUEUE;
}

/*******************************************************
  * Name    :ModemTime_Process
  * Author  :LoH
  * Function:Configure NITZ update once and periodically query valid CCLK time.
  * Version :V1.0.0
  * Data    :2026.09.13
*******************************************************/
void ModemTime_Process(uint8_t u8QueryAllowed)
{
    ModemTimeSample_t stSample;

    if ((u8QueryAllowed == 0U) && (ModemTime_IsBusy() == 0U))
    {
        return;
    }

    switch (s_mTimeRun.enStep)
    {
        case MODEM_TIME_STEP_CTZU_QUERY_QUEUE:
            if ((u8QueryAllowed != 0U) && (ModemPort_IsIdle() != 0U)
                && (ModemPort_QueueCommand(MODEM_AT_CMD_TIME_NITZ_QUERY, MODEM_AT_RSP_TIME_NITZ,
                                            MODEM_AT_CFG_TIME_TIMEOUT_MS) != 0U))
            {
                s_mTimeRun.enStep = MODEM_TIME_STEP_CTZU_QUERY_WAIT;
            }
            break;

        case MODEM_TIME_STEP_CTZU_QUERY_WAIT:
            if (ModemPort_HasFailure() != 0U)
            {
                ModemPort_ClearTransactionFailure();
                s_mTimeRun.enStep = MODEM_TIME_STEP_QUERY_HOLD;
            }
            else if (ModemPort_IsIdle() != 0U)
            {
                s_mTimeRun.enStep = (strstr(ModemPort_GetLastMatchedResponse(), "+CTZU: 1") != 0)
                                    ? MODEM_TIME_STEP_QUERY_HOLD : MODEM_TIME_STEP_CTZU_SET_QUEUE;
            }
            break;

        case MODEM_TIME_STEP_CTZU_SET_QUEUE:
            if ((u8QueryAllowed != 0U) && (ModemPort_IsIdle() != 0U)
                && (ModemPort_QueueCommand(MODEM_AT_CMD_TIME_NITZ_ENABLE, MODEM_AT_RSP_OK,
                                            MODEM_AT_CFG_TIME_TIMEOUT_MS) != 0U))
            {
                s_mTimeRun.enStep = MODEM_TIME_STEP_CTZU_SET_WAIT;
            }
            break;

        case MODEM_TIME_STEP_CTZU_SET_WAIT:
            if (ModemPort_HasFailure() != 0U)
            {
                ModemPort_ClearTransactionFailure();
            }
            if (ModemPort_IsIdle() != 0U)
            {
                s_mTimeRun.u32LastQueryMs = osKernelSysTick();
                s_mTimeRun.enStep = MODEM_TIME_STEP_QUERY_HOLD;
            }
            break;

        case MODEM_TIME_STEP_QUERY_HOLD:
            if ((u8QueryAllowed != 0U)
                && ((osKernelSysTick() - s_mTimeRun.u32LastQueryMs) >= MODEM_TIME_QUERY_PERIOD_MS))
            {
                s_mTimeRun.enStep = MODEM_TIME_STEP_CCLK_QUEUE;
            }
            break;

        case MODEM_TIME_STEP_CCLK_QUEUE:
            if ((u8QueryAllowed != 0U) && (ModemPort_IsIdle() != 0U)
                && (ModemPort_QueueCommand(MODEM_AT_CMD_TIME_CLOCK_QUERY, MODEM_AT_RSP_TIME_CLOCK,
                                            MODEM_AT_CFG_TIME_TIMEOUT_MS) != 0U))
            {
                s_mTimeRun.enStep = MODEM_TIME_STEP_CCLK_WAIT;
            }
            break;

        case MODEM_TIME_STEP_CCLK_WAIT:
            if (ModemPort_HasFailure() != 0U)
            {
                ModemPort_ClearTransactionFailure();
            }
            else if ((ModemPort_IsIdle() != 0U)
                     && (ModemTime_ParseCclk(ModemPort_GetLastMatchedResponse(), &stSample) != 0U))
            {
                portENTER_CRITICAL();
                s_mTimeRun.stSample = stSample;
                s_mTimeRun.u8SamplePending = 1U;
                portEXIT_CRITICAL();
            }
            if (ModemPort_IsIdle() != 0U)
            {
                s_mTimeRun.u32LastQueryMs = osKernelSysTick();
                s_mTimeRun.enStep = MODEM_TIME_STEP_QUERY_HOLD;
            }
            break;

        default:
            s_mTimeRun.enStep = MODEM_TIME_STEP_CTZU_QUERY_QUEUE;
            break;
    }
}

/*******************************************************
  * Name    :ModemTime_IsBusy
  * Author  :LoH
  * Function:Return whether the time workflow owns an active AT transaction.
  * Version :V1.0.0
  * Data    :2026.09.13
*******************************************************/
uint8_t ModemTime_IsBusy(void)
{
    return ((s_mTimeRun.enStep == MODEM_TIME_STEP_CTZU_QUERY_WAIT)
            || (s_mTimeRun.enStep == MODEM_TIME_STEP_CTZU_SET_WAIT)
            || (s_mTimeRun.enStep == MODEM_TIME_STEP_CCLK_WAIT)) ? 1U : 0U;
}

/*******************************************************
  * Name    :ModemTime_TakeSample
  * Author  :LoH
  * Function:Return and clear one valid normalized network UTC sample.
  * Version :V1.0.0
  * Data    :2026.09.13
*******************************************************/
uint8_t ModemTime_TakeSample(ModemTimeSample_t *pstSample)
{
    uint8_t u8Available;

    if (pstSample == 0)
    {
        return 0U;
    }
    portENTER_CRITICAL();
    u8Available = s_mTimeRun.u8SamplePending;
    if (u8Available != 0U)
    {
        *pstSample = s_mTimeRun.stSample;
        s_mTimeRun.u8SamplePending = 0U;
    }
    portEXIT_CRITICAL();
    return u8Available;
}
