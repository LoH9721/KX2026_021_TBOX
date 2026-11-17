/*******************************************************
  * Name    :GnssService.c
  * Author  :LoH
  * Function:GNSS current-position and nine-point history service.
  * Version :V1.0.0
  * Data    :2026.08.29
*******************************************************/
#include "GnssService.h"
#include "cmsis_os.h"
#include "FreeRTOS.h"
#include <string.h>
#include "ModemGnss.h"

#define GNSS_SERVICE_FRESHNESS_MS 5000U

typedef struct
{
    EcallMsd_t stMsd;
    uint32_t u32LastUpdateMs;
} GnssServiceRuntime_t;

static GnssServiceRuntime_t s_mGnssRun;

/*******************************************************
  * Name    :GnssService_SetInvalidPosition
  * Author  :LoH
  * Function:Apply the GB 45672 invalid coordinate value to current and historical points.
  * Version :V1.0.0
  * Data    :2026.11.16
*******************************************************/
static void GnssService_SetInvalidPosition(EcallMsd_t *pstMsd)
{
    uint8_t u8Index;

    pstMsd->gnss_valid = 0U;
    pstMsd->latitude_e7 = INT32_MAX;
    pstMsd->longitude_e7 = INT32_MAX;
    pstMsd->utc_seconds = 0U;
    for (u8Index = 0U; u8Index < ECALL_GNSS_HISTORY_COUNT; u8Index++)
    {
        pstMsd->history_latitude_e7[u8Index] = INT32_MAX;
        pstMsd->history_longitude_e7[u8Index] = INT32_MAX;
    }
}

/*******************************************************
  * Name    :GnssService_Init
  * Author  :LoH
  * Function:Initialize current and historical positions to the invalid value.
  * Version :V1.0.0
  * Data    :2026.11.16
*******************************************************/
void GnssService_Init(void)
{
    (void)memset(&s_mGnssRun, 0, sizeof(s_mGnssRun));
    GnssService_SetInvalidPosition(&s_mGnssRun.stMsd);
}

/*******************************************************
  * Name    :GnssService_Process
  * Author  :LoH
  * Function:Consume a valid modem sample and shift T-1 through T-9 history.
  * Version :V1.0.0
  * Data    :2026.08.29
*******************************************************/
void GnssService_Process(void)
{
    ModemGnssSample_t stSample;
    uint8_t u8Index;

    if (ModemGnss_TakeSample(&stSample) == 0U)
    {
        return;
    }

    portENTER_CRITICAL();
    if (s_mGnssRun.stMsd.gnss_valid != 0U)
    {
        for (u8Index = (ECALL_GNSS_HISTORY_COUNT - 1U); u8Index > 0U; u8Index--)
        {
            s_mGnssRun.stMsd.history_latitude_e7[u8Index] =
                s_mGnssRun.stMsd.history_latitude_e7[u8Index - 1U];
            s_mGnssRun.stMsd.history_longitude_e7[u8Index] =
                s_mGnssRun.stMsd.history_longitude_e7[u8Index - 1U];
        }
        s_mGnssRun.stMsd.history_latitude_e7[0] = s_mGnssRun.stMsd.latitude_e7;
        s_mGnssRun.stMsd.history_longitude_e7[0] = s_mGnssRun.stMsd.longitude_e7;
    }
    s_mGnssRun.stMsd.gnss_valid = 1U;
    s_mGnssRun.stMsd.latitude_e7 = stSample.i32LatitudeE7;
    s_mGnssRun.stMsd.longitude_e7 = stSample.i32LongitudeE7;
    s_mGnssRun.stMsd.utc_seconds = stSample.u32UtcSeconds;
    s_mGnssRun.u32LastUpdateMs = osKernelSysTick();
    portEXIT_CRITICAL();
}

/*******************************************************
  * Name    :GnssService_GetLatest
  * Author  :LoH
  * Function:Copy a fresh snapshot or return GB 45672 invalid coordinates.
  * Version :V1.0.0
  * Data    :2026.11.16
*******************************************************/
uint8_t GnssService_GetLatest(EcallMsd_t *pstMsd)
{
    uint8_t u8Valid;

    if (pstMsd == 0)
    {
        return 0U;
    }

    portENTER_CRITICAL();
    *pstMsd = s_mGnssRun.stMsd;
    u8Valid = ((s_mGnssRun.stMsd.gnss_valid != 0U)
               && ((osKernelSysTick() - s_mGnssRun.u32LastUpdateMs) <= GNSS_SERVICE_FRESHNESS_MS)) ? 1U : 0U;
    portEXIT_CRITICAL();
    pstMsd->gnss_valid = u8Valid;
    if (u8Valid == 0U)
    {
        GnssService_SetInvalidPosition(pstMsd);
    }
    return u8Valid;
}
