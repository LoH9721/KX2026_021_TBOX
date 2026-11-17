/*******************************************************
  * Name    :ModemPlatform.h
  * Author  :LoH
  * Function:Private candidate AECS platform workflow interface.
  * Version :V1.0.0
  * Data    :2026.08.28
*******************************************************/
#ifndef __MODEM_PLATFORM_H__
#define __MODEM_PLATFORM_H__

#include <stdint.h>
#include "Ecall.h"

typedef enum
{
    MODEM_PLATFORM_EVENT_NONE = 0U,
    MODEM_PLATFORM_EVENT_REGISTERED,
    MODEM_PLATFORM_EVENT_MSD_STARTED,
    MODEM_PLATFORM_EVENT_MSD_UPLOADED,
    MODEM_PLATFORM_EVENT_FAILED
} ModemPlatformEvent_t;

/* Initialize the private platform workflow. */
void ModemPlatform_Init(void);
/* Stop active platform work while preserving a pending emergency MSD request. */
void ModemPlatform_Reset(void);
/* Start platform registration after TCP connection. */
void ModemPlatform_Start(void);
/* Latch one MSD for upload after the platform becomes available. */
void ModemPlatform_RequestMsdUpload(const EcallMsd_t *pstMsd);
/* Process one platform workflow step and optionally advance heartbeat timing. */
void ModemPlatform_Process(uint8_t u8HeartbeatEnabled);
/* Return whether the private platform workflow is stably online. */
uint8_t ModemPlatform_IsOnline(void);
/* Return whether GNSS may use the idle AT port without delaying platform work. */
uint8_t ModemPlatform_IsBackgroundAllowed(void);
/* Return and clear one platform workflow result event. */
ModemPlatformEvent_t ModemPlatform_TakeEvent(void);

/* Maintain ACK/retry time using the system clock regardless of port ownership. */
void ModemPlatform_Tick(void);
/* Return whether one data message still requires delivery. */
uint8_t ModemPlatform_HasPendingMsd(void);
/* Return whether the next frozen-MSD transmission attempt is due. */
uint8_t ModemPlatform_IsMsdAttemptDue(void);
/* Return whether voice/background work may acquire the AT port. */
uint8_t ModemPlatform_CanYieldPort(void);

#endif
