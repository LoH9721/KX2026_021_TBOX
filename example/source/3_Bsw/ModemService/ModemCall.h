/*******************************************************
  * Name    :ModemCall.h
  * Author  :LoH
  * Function:Private modem call state processing interface.
  * Version :V1.0.0
  * Data    :2026.08.28
*******************************************************/
#ifndef __MODEM_CALL_H__
#define __MODEM_CALL_H__

#include "ModemService.h"

void ModemCall_Init(void);
void ModemCall_Reset(void);
void ModemCall_Start(ModemCallState_t *penCallState);
/* Process URCs and timers every cycle; queue AT work only when granted ownership. */
void ModemCall_Process(ModemCallState_t *penCallState, uint8_t u8PortAllowed);
/* Return whether voice setup or release currently needs exclusive AT ownership. */
uint8_t ModemCall_NeedsPort(void);

#endif
