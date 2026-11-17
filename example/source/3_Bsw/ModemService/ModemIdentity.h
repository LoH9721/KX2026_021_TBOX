/*******************************************************
  * Name    :ModemIdentity.h
  * Author  :LoH
  * Function:Cached SIM number and configured TBOX identity snapshot.
  * Version :V1.0.0
  * Data    :2026.11.14
*******************************************************/
#ifndef __MODEM_IDENTITY_H__
#define __MODEM_IDENTITY_H__
#include <stdint.h>

typedef struct
{
    char acSimPhoneNumber[17];
    char acIccid[21];
    char acImei[16];
    char acHardwareVersion[33];
    char acSoftwareVersion[33];
} ModemIdentitySnapshot_t;

/* Initialize configured identity strings without inventing unavailable values. */
void ModemIdentity_Init(void);
/* Query CNUM once after initialization while background work is allowed. */
void ModemIdentity_Process(uint8_t u8StartAllowed);
/* Return whether this module owns one outstanding AT query. */
uint8_t ModemIdentity_IsBusy(void);
/* Copy one stable identity snapshot in the single ModemTask context. */
void ModemIdentity_GetSnapshot(ModemIdentitySnapshot_t *pstSnapshot);
#endif
