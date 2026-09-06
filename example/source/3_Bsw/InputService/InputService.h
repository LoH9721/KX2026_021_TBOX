/*******************************************************
  * Name    :InputService.h
  * Author  :LoH
  * Function:Debounced digital input service interface.
  * Version :V1.0.0
  * Data    :2026.08.26
*******************************************************/
#ifndef __INPUT_SERVICE_H__
#define __INPUT_SERVICE_H__

#include "RteSys.h"

typedef enum
{
    INPUT_SERVICE_SIG_SOS = 0U,
    INPUT_SERVICE_SIG_SRS,
    INPUT_SERVICE_SIG_PBATTERY_SRS,
    INPUT_SERVICE_SIG_KL15,
    INPUT_SERVICE_SIG_NUM
} InputServiceSignal_t;

/* Initialize input service state and RTE switch value. */
void InputService_Init(void);
/* Sample and debounce input signals at a 10 ms period. */
void InputService_Process10ms(void);
/* Return one debounced normalized input state. */
RteSwitchState_t InputService_GetState(InputServiceSignal_t enSignal);

#endif
