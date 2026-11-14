/*******************************************************
  * Name    :TimeService.c
  * Author  :LoH
  * Function:GNSS-priority and cellular-fallback UTC-time arbitration.
  * Version :V1.0.0
  * Data    :2026.09.13
*******************************************************/
#include "TimeService.h"
#include <string.h>
#include "cmsis_os.h"
#include "FreeRTOS.h"
#include "Ecall.h"
#include "GnssService.h"
#include "ModemTime.h"

#define TIME_SERVICE_GNSS_FRESHNESS_MS 5000UL
/* A zero freshness period retains the cellular clock until the next power cycle. */
#define TIME_SERVICE_CELLULAR_FRESHNESS_MS 0UL

typedef struct
{
    TimeServiceTime_t stTime;
    uint32_t u32SampleMs;
} TimeServiceCache_t;

typedef struct
{
    TimeServiceCache_t stGnssCache;
    TimeServiceCache_t stCellularCache;
} TimeServiceRuntime_t;

static TimeServiceRuntime_t s_mTimeSvcRun;

/*******************************************************
  * Name    :TimeService_IsLeapYear
  * Author  :LoH
  * Function:Return whether one Gregorian calendar year is leap.
  * Version :V1.0.0
  * Data    :2026.09.13
*******************************************************/
static uint8_t TimeService_IsLeapYear(uint16_t u16Year)
{
    return (((u16Year % 4U) == 0U) && (((u16Year % 100U) != 0U) || ((u16Year % 400U) == 0U))) ? 1U : 0U;
}

/*******************************************************
  * Name    :TimeService_GetMonthDays
  * Author  :LoH
  * Function:Return the day count for one specified calendar month.
  * Version :V1.0.0
  * Data    :2026.09.13
*******************************************************/
static uint8_t TimeService_GetMonthDays(uint16_t u16Year, uint8_t u8Month)
{
    static const uint8_t au8MonthDays[12] = {31U, 28U, 31U, 30U, 31U, 30U,
                                              31U, 31U, 30U, 31U, 30U, 31U};

    if ((u8Month == 0U) || (u8Month > 12U))
    {
        return 0U;
    }
    return (uint8_t)(au8MonthDays[u8Month - 1U]
                     + (((u8Month == 2U) && (TimeService_IsLeapYear(u16Year) != 0U)) ? 1U : 0U));
}

/*******************************************************
  * Name    :TimeService_UtcSecondsToCalendar
  * Author  :LoH
  * Function:Convert Unix UTC seconds to the exported two-digit calendar.
  * Version :V1.0.0
  * Data    :2026.09.13
*******************************************************/
static void TimeService_UtcSecondsToCalendar(uint32_t u32UtcSeconds, TimeServiceTime_t *pstTime)
{
    uint16_t u16Year = 1970U;
    uint16_t u16DaysInYear;
    uint32_t u32Days = u32UtcSeconds / 86400UL;
    uint32_t u32SecondsOfDay = u32UtcSeconds % 86400UL;
    uint8_t u8Month = 1U;
    uint8_t u8DaysInMonth;

    while (u16Year < 2100U)
    {
        u16DaysInYear = (TimeService_IsLeapYear(u16Year) != 0U) ? 366U : 365U;
        if (u32Days < u16DaysInYear)
        {
            break;
        }
        u32Days -= u16DaysInYear;
        u16Year++;
    }
    while (u8Month <= 12U)
    {
        u8DaysInMonth = TimeService_GetMonthDays(u16Year, u8Month);
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
  * Name    :TimeService_AddSeconds
  * Author  :LoH
  * Function:Advance one exported UTC calendar by whole seconds.
  * Version :V1.0.0
  * Data    :2026.09.13
*******************************************************/
static void TimeService_AddSeconds(TimeServiceTime_t *pstTime, uint32_t u32Seconds)
{
    uint16_t u16Year;

    while (u32Seconds > 0U)
    {
        pstTime->u8Second++;
        if (pstTime->u8Second >= 60U)
        {
            pstTime->u8Second = 0U;
            pstTime->u8Minute++;
            if (pstTime->u8Minute >= 60U)
            {
                pstTime->u8Minute = 0U;
                pstTime->u8Hour++;
                if (pstTime->u8Hour >= 24U)
                {
                    pstTime->u8Hour = 0U;
                    pstTime->u8Day++;
                    u16Year = (uint16_t)(2000U + pstTime->u8Year);
                    if (pstTime->u8Day > TimeService_GetMonthDays(u16Year, pstTime->u8Month))
                    {
                        pstTime->u8Day = 1U;
                        pstTime->u8Month++;
                        if (pstTime->u8Month > 12U)
                        {
                            pstTime->u8Month = 1U;
                            pstTime->u8Year++;
                        }
                    }
                }
            }
        }
        u32Seconds--;
    }
}

/*******************************************************
  * Name    :TimeService_IsCacheFresh
  * Author  :LoH
  * Function:Return whether one source cache is valid at the current OS tick.
  * Version :V1.0.0
  * Data    :2026.09.13
*******************************************************/
static uint8_t TimeService_IsCacheFresh(const TimeServiceCache_t *pstCache,
                                        uint32_t u32NowMs,
                                        uint32_t u32FreshnessMs)
{
    if (pstCache->u32SampleMs == 0U)
    {
        return 0U;
    }

    if ((u32FreshnessMs != 0U)
        && ((u32NowMs - pstCache->u32SampleMs) > u32FreshnessMs))
    {
        return 0U;
    }

    return 1U;
}

/*******************************************************
  * Name    :TimeService_Init
  * Author  :LoH
  * Function:Clear the shared UTC-time snapshot before source acquisition.
  * Version :V1.0.0
  * Data    :2026.09.13
*******************************************************/
void TimeService_Init(void)
{
    (void)memset(&s_mTimeSvcRun, 0, sizeof(s_mTimeSvcRun));
}

/*******************************************************
  * Name    :TimeService_Process
  * Author  :LoH
  * Function:Independently cache GNSS UTC and cellular UTC for immediate fallback.
  * Version :V1.0.0
  * Data    :2026.09.13
*******************************************************/
void TimeService_Process(void)
{
    EcallMsd_t stMsd;
    ModemTimeSample_t stCellularTime;

    if (GnssService_GetLatest(&stMsd) != 0U)
    {
        portENTER_CRITICAL();
        TimeService_UtcSecondsToCalendar(stMsd.utc_seconds, &s_mTimeSvcRun.stGnssCache.stTime);
        s_mTimeSvcRun.stGnssCache.u32SampleMs = osKernelSysTick();
        portEXIT_CRITICAL();
    }

    if (ModemTime_TakeSample(&stCellularTime) != 0U)
    {
        portENTER_CRITICAL();
        s_mTimeSvcRun.stCellularCache.stTime.u8Year = stCellularTime.u8Year;
        s_mTimeSvcRun.stCellularCache.stTime.u8Month = stCellularTime.u8Month;
        s_mTimeSvcRun.stCellularCache.stTime.u8Day = stCellularTime.u8Day;
        s_mTimeSvcRun.stCellularCache.stTime.u8Hour = stCellularTime.u8Hour;
        s_mTimeSvcRun.stCellularCache.stTime.u8Minute = stCellularTime.u8Minute;
        s_mTimeSvcRun.stCellularCache.stTime.u8Second = stCellularTime.u8Second;
        s_mTimeSvcRun.stCellularCache.u32SampleMs = osKernelSysTick();
        portEXIT_CRITICAL();
    }
}

/*******************************************************
  * Name    :TimeService_GetUtc
  * Author  :LoH
  * Function:Return fresh GNSS UTC first, otherwise the cached cellular UTC.
  * Version :V1.0.0
  * Data    :2026.09.13
*******************************************************/
uint8_t TimeService_GetUtc(TimeServiceTime_t *pstTime)
{
    TimeServiceTime_t stTime;
    TimeServiceCache_t stGnssCache;
    TimeServiceCache_t stCellularCache;
    TimeServiceSource_t enSource = TIME_SERVICE_SOURCE_INVALID;
    uint32_t u32SampleMs;
    uint32_t u32NowMs;

    if (pstTime == 0)
    {
        return TIME_SERVICE_SOURCE_INVALID;
    }
    portENTER_CRITICAL();
    stGnssCache = s_mTimeSvcRun.stGnssCache;
    stCellularCache = s_mTimeSvcRun.stCellularCache;
    portEXIT_CRITICAL();

    u32NowMs = osKernelSysTick();
    if (TimeService_IsCacheFresh(&stGnssCache, u32NowMs, TIME_SERVICE_GNSS_FRESHNESS_MS) != 0U)
    {
        stTime = stGnssCache.stTime;
        u32SampleMs = stGnssCache.u32SampleMs;
        enSource = TIME_SERVICE_SOURCE_GNSS;
    }
    else if (TimeService_IsCacheFresh(&stCellularCache, u32NowMs, TIME_SERVICE_CELLULAR_FRESHNESS_MS) != 0U)
    {
        stTime = stCellularCache.stTime;
        u32SampleMs = stCellularCache.u32SampleMs;
        enSource = TIME_SERVICE_SOURCE_CELLULAR;
    }
    else
    {
        (void)memset(pstTime, 0, sizeof(*pstTime));
        return TIME_SERVICE_SOURCE_INVALID;
    }
    TimeService_AddSeconds(&stTime, (u32NowMs - u32SampleMs) / 1000UL);
    *pstTime = stTime;
    return (uint8_t)enSource;
}
