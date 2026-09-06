/*******************************************************
  * Name    :ModemPower.h
  * Author  :LoH
  * Function:AC7600 physical power and UART sleep control interface.
  * Version :V1.0.0
  * Data    :2026.09.06
*******************************************************/
#ifndef __MODEM_POWER_H__
#define __MODEM_POWER_H__

#include <stdint.h>

/* Set CAT4 power controls to their safe startup state and begin one power-on sequence. */
void ModemPower_Init(void);
/* Request UART wake or physical startup before ModemNetwork sends AT commands. */
void ModemPower_RequestOnline(void);
/* Request AC7600 RX-UART sleep without removing CAT4 supply power. */
void ModemPower_RequestUartSleep(void);
/* Process power-key pulse, boot delay and CSCLK command completion in ModemTask context. */
void ModemPower_Process(void);
/* Return whether the AC7600 UART may receive normal network commands. */
uint8_t ModemPower_IsOnlineReady(void);

#endif
