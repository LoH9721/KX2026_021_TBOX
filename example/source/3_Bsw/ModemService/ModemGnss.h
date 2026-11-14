/*******************************************************
  * Name    :ModemGnss.h
  * Author  :LoH
  * Function:Private AC7600 GNSS query workflow interface.
  * Version :V1.0.0
  * Data    :2026.08.29
*******************************************************/
#ifndef __MODEM_GNSS_H__
#define __MODEM_GNSS_H__

#include <stdint.h>

typedef struct
{
    int32_t i32LatitudeE7;
    int32_t i32LongitudeE7;
    uint32_t u32UtcSeconds;
    uint8_t u8Year;
    uint8_t u8Month;
    uint8_t u8Day;
    uint8_t u8Hour;
    uint8_t u8Minute;
    uint8_t u8Second;
} ModemGnssSample_t;

/* Initialize the private AC7600 GNSS query workflow. */
void ModemGnss_Init(void);
/* Process one GNSS state-machine step when background access is allowed. */
void ModemGnss_Process(uint8_t u8StartAllowed);
/* Return whether an active GNSS AT transaction owns the modem port. */
uint8_t ModemGnss_IsBusy(void);
/* Preempt a background GNSS AT wait for an emergency workflow without powering GNSS off. */
void ModemGnss_AbortForEmergency(void);
/* Return and clear one valid parsed GNSS sample. */
uint8_t ModemGnss_TakeSample(ModemGnssSample_t *pstSample);

#endif
