/*******************************************************
  * Name    :OutputService.h
  * Author  :LoH
  * Function:Output service interface for test and power signals.
  * Version :V1.0.0
  * Data    :2026.09.06
*******************************************************/
#ifndef __OUTPUT_SERVICE_H__
#define __OUTPUT_SERVICE_H__

#include "Types.h"

typedef enum
{
    OUTPUT_SERVICE_SIG_INDICATOR_1 = 0U,
    OUTPUT_SERVICE_SIG_INDICATOR_2,
    OUTPUT_SERVICE_SIG_IG_KEEPON,
    OUTPUT_SERVICE_SIG_PWR_EN_CAN,
    OUTPUT_SERVICE_SIG_CAT4_EN,
    OUTPUT_SERVICE_SIG_CAT4_PWR_KEY,
    OUTPUT_SERVICE_SIG_CAT4_RST,
    OUTPUT_SERVICE_SIG_PWR_EN_GNSS,
    OUTPUT_SERVICE_SIG_PWR_EN_3V3,
    OUTPUT_SERVICE_SIG_PWR_EN_PHY,
    OUTPUT_SERVICE_SIG_PWR_EN_AUDIO,
    OUTPUT_SERVICE_SIG_NUM
} OutputServiceSignal_t;

void OutputService_Init(void);
void OutputService_Process10ms(void);
void OutputService_SetState(OutputServiceSignal_t enSignal, IO_Type enState);
/* Set indicator 1 according to GNSS initialization status. */
void OutputService_SetGnssReady(uint8_t u8Ready);
/* Set indicator 2 according to voice call availability status. */
void OutputService_SetVoiceCallReady(uint8_t u8Ready);
void OutputService_SetEcallCancelWindow(uint8_t u8Active);

#endif
