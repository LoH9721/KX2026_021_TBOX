/*******************************************************
  * Name    :ModemTime.h
  * Author  :LoH
  * Function:Private AC7600 network-time query interface.
  * Version :V1.0.0
  * Data    :2026.09.13
*******************************************************/
#ifndef __MODEM_TIME_H__
#define __MODEM_TIME_H__

#include <stdint.h>

typedef struct
{
    uint8_t u8Year;
    uint8_t u8Month;
    uint8_t u8Day;
    uint8_t u8Hour;
    uint8_t u8Minute;
    uint8_t u8Second;
} ModemTimeSample_t;

void ModemTime_Init(void);
void ModemTime_Process(uint8_t u8QueryAllowed);
uint8_t ModemTime_IsBusy(void);
uint8_t ModemTime_TakeSample(ModemTimeSample_t *pstSample);

#endif
