/*******************************************************
  * Name    :TimeService.h
  * Author  :LoH
  * Function:Unified GNSS and cellular UTC-time service interface.
  * Version :V1.0.0
  * Data    :2026.09.13
*******************************************************/
#ifndef __TIME_SERVICE_H__
#define __TIME_SERVICE_H__

#include <stdint.h>

typedef struct
{
    uint8_t u8Year;
    uint8_t u8Month;
    uint8_t u8Day;
    uint8_t u8Hour;
    uint8_t u8Minute;
    uint8_t u8Second;
} TimeServiceTime_t;

typedef enum
{
    TIME_SERVICE_SOURCE_INVALID = 0U,
    TIME_SERVICE_SOURCE_GNSS,
    TIME_SERVICE_SOURCE_CELLULAR
} TimeServiceSource_t;

void TimeService_Init(void);
void TimeService_Process(void);
/* Return current Unix UTC seconds and the selected source. */
uint8_t TimeService_GetUtcSeconds(uint32_t *pu32UtcSeconds);
uint8_t TimeService_GetUtc(TimeServiceTime_t *pstTime);

#endif
