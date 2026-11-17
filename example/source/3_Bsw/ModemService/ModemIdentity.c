/*******************************************************
  * Name    :ModemIdentity.c
  * Author  :LoH
  * Function:Nonblocking CNUM query with macro-defined equipment identifiers.
  * Version :V1.0.0
  * Data    :2026.11.14
*******************************************************/
#include "ModemIdentity.h"
#include <stdio.h>
#include <string.h>
#include "ModemAt_Cfg.h"
#include "ModemPort.h"

typedef struct
{
    uint8_t u8Waiting;
    uint8_t u8Queried;
} ModemIdentityRuntime_t;

static ModemIdentityRuntime_t s_mIdentRun;
static ModemIdentitySnapshot_t s_mIdentSts;

/*******************************************************
  * Name    :ModemIdentity_CopyConfigured
  * Author  :LoH
  * Function:Omit oversized configuration instead of truncating equipment identifiers.
  * Version :V1.0.0
  * Data    :2026.11.14
*******************************************************/
static void ModemIdentity_CopyConfigured(char *pcTarget, uint16_t u16Capacity, const char *pcSource)
{
    size_t u32Length = strlen(pcSource);
    pcTarget[0] = '\0';
    if (u32Length < u16Capacity)
    {
        (void)memcpy(pcTarget, pcSource, u32Length + 1U);
    }
}

/*******************************************************
  * Name    :ModemIdentity_ParseNumber
  * Author  :LoH
  * Function:Parse Number within the bounded MSD protocol workflow.
  * Version :V1.0.0
  * Data    :2026.11.14
*******************************************************/
static void ModemIdentity_ParseNumber(const char *pcLine)
{
    char acNumber[17];
    const char *pcNumber;
    uint8_t u8Length = 0U;
    uint8_t u8Quoted = 0U;
    unsigned int u32Type;

    if ((pcLine == 0) || (strncmp(pcLine, "+CNUM:", 6U) != 0))
    {
        return;
    }
    pcNumber = &pcLine[6];
    while (*pcNumber != '\0')
    {
        if (*pcNumber == '"')
        {
            u8Quoted = (u8Quoted == 0U) ? 1U : 0U;
        }
        else if ((*pcNumber == ',') && (u8Quoted == 0U))
        {
            break;
        }
        pcNumber++;
    }
    if (*pcNumber != ',')
    {
        return;
    }
    pcNumber++;
    while (*pcNumber == ' ')
    {
        pcNumber++;
    }
    if (*pcNumber == '"')
    {
        pcNumber++;
    }
    while ((*pcNumber != '\0') && (*pcNumber != '"') && (*pcNumber != ','))
    {
        if ((u8Length >= 16U)
            || (!((*pcNumber >= '0') && (*pcNumber <= '9')) && !((u8Length == 0U) && (*pcNumber == '+'))))
        {
            return;
        }
        acNumber[u8Length++] = *pcNumber++;
    }
    if ((u8Length == 0U) || ((u8Length == 1U) && (acNumber[0] == '+')))
    {
        return;
    }
    acNumber[u8Length] = '\0';
    if (*pcNumber == '"')
    {
        pcNumber++;
    }
    if (sscanf(pcNumber, ",%u", &u32Type) != 1)
    {
        return;
    }
    if ((u32Type == 145U) && (acNumber[0] != '+'))
    {
        if (u8Length >= 16U)
        {
            return;
        }
        (void)memmove(&acNumber[1], acNumber, (size_t)u8Length + 1U);
        acNumber[0] = '+';
    }
    (void)strcpy(s_mIdentSts.acSimPhoneNumber, acNumber);
}

/*******************************************************
  * Name    :ModemIdentity_Init
  * Author  :LoH
  * Function:Init within the bounded MSD protocol workflow.
  * Version :V1.0.0
  * Data    :2026.11.14
*******************************************************/
void ModemIdentity_Init(void)
{
    (void)memset(&s_mIdentRun, 0, sizeof(s_mIdentRun));
    (void)memset(&s_mIdentSts, 0, sizeof(s_mIdentSts));
    ModemIdentity_CopyConfigured(s_mIdentSts.acSimPhoneNumber, sizeof(s_mIdentSts.acSimPhoneNumber), MODEM_PLATFORM_SIM_PHONE_NUMBER);
    ModemIdentity_CopyConfigured(s_mIdentSts.acIccid, sizeof(s_mIdentSts.acIccid), MODEM_PLATFORM_ICCID);
    ModemIdentity_CopyConfigured(s_mIdentSts.acImei, sizeof(s_mIdentSts.acImei), MODEM_PLATFORM_IMEI);
    ModemIdentity_CopyConfigured(s_mIdentSts.acHardwareVersion, sizeof(s_mIdentSts.acHardwareVersion), MODEM_PLATFORM_HARDWARE_VERSION);
    ModemIdentity_CopyConfigured(s_mIdentSts.acSoftwareVersion, sizeof(s_mIdentSts.acSoftwareVersion), MODEM_PLATFORM_SOFTWARE_VERSION);
}

/*******************************************************
  * Name    :ModemIdentity_Process
  * Author  :LoH
  * Function:Query the SIM subscriber number once after each device initialization.
  * Version :V1.0.0
  * Data    :2026.11.16
*******************************************************/
void ModemIdentity_Process(uint8_t u8StartAllowed)
{
    if (s_mIdentRun.u8Waiting != 0U)
    {
        if (ModemPort_IsIdle() != 0U)
        {
            if (ModemPort_HasFailure() == 0U)
            {
                ModemIdentity_ParseNumber(ModemPort_GetLastInformationLine());
            }
            ModemPort_ClearTransactionFailure();
            s_mIdentRun.u8Waiting = 0U;
        }
    }
    else if ((u8StartAllowed != 0U)
        && (s_mIdentRun.u8Queried == 0U)
        && (ModemPort_IsIdle() != 0U))
    {
        ModemPort_ClearTransactionFailure();
        if (ModemPort_QueueQueryCommand(MODEM_AT_CMD_SUBSCRIBER_NUMBER, "+CNUM:", 9000U) != 0U)
        {
            s_mIdentRun.u8Waiting = 1U;
            s_mIdentRun.u8Queried = 1U;
            /* A fresh empty response must not keep a previous SIM's telephone number. */
            ModemIdentity_CopyConfigured(s_mIdentSts.acSimPhoneNumber, sizeof(s_mIdentSts.acSimPhoneNumber), MODEM_PLATFORM_SIM_PHONE_NUMBER);
        }
    }
}

/*******************************************************
  * Name    :ModemIdentity_IsBusy
  * Author  :LoH
  * Function:Is Busy within the bounded MSD protocol workflow.
  * Version :V1.0.0
  * Data    :2026.11.14
*******************************************************/
uint8_t ModemIdentity_IsBusy(void)
{
    return s_mIdentRun.u8Waiting;
}

/*******************************************************
  * Name    :ModemIdentity_GetSnapshot
  * Author  :LoH
  * Function:Get Snapshot within the bounded MSD protocol workflow.
  * Version :V1.0.0
  * Data    :2026.11.14
*******************************************************/
void ModemIdentity_GetSnapshot(ModemIdentitySnapshot_t *pstSnapshot)
{
    if (pstSnapshot != 0)
    {
        *pstSnapshot = s_mIdentSts;
    }
}
