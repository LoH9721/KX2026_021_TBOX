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
    uint32_t u32UtcSeconds;
    uint32_t u32SampleMs;
    uint8_t u8Valid;
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
  * Name    :TimeService_CalendarToUtcSeconds
  * Author  :LoH
  * Function:Convert one validated UTC calendar value to seconds since 1970.
  * Version :V1.0.0
  * Data    :2026.11.17
*******************************************************/
static uint8_t TimeService_CalendarToUtcSeconds(const TimeServiceTime_t *pstTime,
                                                uint32_t *pu32UtcSeconds)
{
    uint16_t u16Year;
    uint16_t u16LoopYear;
    uint32_t u32Days = 0U;
    uint8_t u8Month;

    if ((pstTime == 0) || (pu32UtcSeconds == 0)
        || (pstTime->u8Year < 24U) || (pstTime->u8Month == 0U)
        || (pstTime->u8Month > 12U) || (pstTime->u8Day == 0U)
        || (pstTime->u8Hour > 23U) || (pstTime->u8Minute > 59U)
        || (pstTime->u8Second > 59U)
        || (pstTime->u8Day > TimeService_GetMonthDays((uint16_t)(2000U + pstTime->u8Year), pstTime->u8Month)))
    {
        return 0U;
    }

    u16Year = (uint16_t)(2000U + pstTime->u8Year);
    for (u16LoopYear = 1970U; u16LoopYear < u16Year; u16LoopYear++)
    {
        u32Days += (TimeService_IsLeapYear(u16LoopYear) != 0U) ? 366U : 365U;
    }
    for (u8Month = 1U; u8Month < pstTime->u8Month; u8Month++)
    {
        u32Days += TimeService_GetMonthDays(u16Year, u8Month);
    }

    *pu32UtcSeconds = ((u32Days + (uint32_t)(pstTime->u8Day - 1U)) * 86400UL)
                      + ((uint32_t)pstTime->u8Hour * 3600UL)
                      + ((uint32_t)pstTime->u8Minute * 60UL)
                      + pstTime->u8Second;
    return 1U;
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
    if (pstCache->u8Valid == 0U)
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
    TimeServiceTime_t stUtcTime;
    uint32_t u32UtcSeconds;
    uint32_t u32SampleMs;

    if (GnssService_GetLatest(&stMsd) != 0U)
    {
        u32SampleMs = osKernelSysTick();
        portENTER_CRITICAL();
        if ((s_mTimeSvcRun.stGnssCache.u8Valid == 0U)
            || (s_mTimeSvcRun.stGnssCache.u32UtcSeconds != stMsd.utc_seconds))
        {
            s_mTimeSvcRun.stGnssCache.u32UtcSeconds = stMsd.utc_seconds;
            s_mTimeSvcRun.stGnssCache.u32SampleMs = u32SampleMs;
            s_mTimeSvcRun.stGnssCache.u8Valid = 1U;
        }
        portEXIT_CRITICAL();
    }

    if (ModemTime_TakeSample(&stCellularTime) != 0U)
    {
        stUtcTime.u8Year = stCellularTime.u8Year;
        stUtcTime.u8Month = stCellularTime.u8Month;
        stUtcTime.u8Day = stCellularTime.u8Day;
        stUtcTime.u8Hour = stCellularTime.u8Hour;
        stUtcTime.u8Minute = stCellularTime.u8Minute;
        stUtcTime.u8Second = stCellularTime.u8Second;
        if (TimeService_CalendarToUtcSeconds(&stUtcTime, &u32UtcSeconds) != 0U)
        {
            portENTER_CRITICAL();
            s_mTimeSvcRun.stCellularCache.u32UtcSeconds = u32UtcSeconds;
            s_mTimeSvcRun.stCellularCache.u32SampleMs = osKernelSysTick();
            s_mTimeSvcRun.stCellularCache.u8Valid = 1U;
            portEXIT_CRITICAL();
        }
    }
}

/*******************************************************
  * Name    :TimeService_GetUtcSeconds
  * Author  :LoH
  * Function:Return current Unix UTC seconds from fresh GNSS or cached cellular time.
  * Version :V1.0.0
  * Data    :2026.11.17
*******************************************************/
uint8_t TimeService_GetUtcSeconds(uint32_t *pu32UtcSeconds)
{
    TimeServiceCache_t stGnssCache;
    TimeServiceCache_t stCellularCache;
    TimeServiceCache_t stSelectedCache;
    TimeServiceSource_t enSource = TIME_SERVICE_SOURCE_INVALID;
    uint32_t u32NowMs;

    if (pu32UtcSeconds == 0)
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
        stSelectedCache = stGnssCache;
        enSource = TIME_SERVICE_SOURCE_GNSS;
    }
    else if (TimeService_IsCacheFresh(&stCellularCache, u32NowMs, TIME_SERVICE_CELLULAR_FRESHNESS_MS) != 0U)
    {
        stSelectedCache = stCellularCache;
        enSource = TIME_SERVICE_SOURCE_CELLULAR;
    }
    else
    {
        *pu32UtcSeconds = 0U;
        return TIME_SERVICE_SOURCE_INVALID;
    }

    *pu32UtcSeconds = stSelectedCache.u32UtcSeconds
                      + ((u32NowMs - stSelectedCache.u32SampleMs) / 1000UL);
    return (uint8_t)enSource;
}

/*******************************************************
  * Name    :TimeService_GetUtc
  * Author  :LoH
  * Function:Return current UTC calendar from the selected trusted time source.
  * Version :V1.0.0
  * Data    :2026.11.17
*******************************************************/
uint8_t TimeService_GetUtc(TimeServiceTime_t *pstTime)
{
    uint32_t u32UtcSeconds;
    uint8_t u8Source;

    if (pstTime == 0)
    {
        return TIME_SERVICE_SOURCE_INVALID;
    }

    u8Source = TimeService_GetUtcSeconds(&u32UtcSeconds);
    if (u8Source == TIME_SERVICE_SOURCE_INVALID)
    {
        (void)memset(pstTime, 0, sizeof(*pstTime));
        return TIME_SERVICE_SOURCE_INVALID;
    }

    TimeService_UtcSecondsToCalendar(u32UtcSeconds, pstTime);
    return u8Source;
}
