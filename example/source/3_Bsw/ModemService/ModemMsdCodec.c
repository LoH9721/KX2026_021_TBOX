/*******************************************************
  * Name    :ModemMsdCodec.c
  * Author  :LoH
  * Function:Standard 111-byte MSD with V0.2 TLV and correlated JSON ACK.
  * Version :V1.0.0
  * Data    :2026.11.14
*******************************************************/
#include "ModemMsdCodec.h"
#include <stdio.h>
#include <string.h>
#include "ModemAt_Cfg.h"

typedef struct
{
    char acType[32];
    char acResult[16];
    char acDeviceId[16];
    uint32_t u32Ser;
    uint32_t u32Identifier;
    uint32_t u32TriggerTime;
    uint8_t u8Seen;
} ModemMsdReply_t;

/*******************************************************
  * Name    :ModemMsdCodec_PackU32
  * Author  :LoH
  * Function:Pack U32 within the bounded MSD protocol workflow.
  * Version :V1.0.0
  * Data    :2026.11.14
*******************************************************/
static void ModemMsdCodec_PackU32(uint8_t *pu8Value, uint32_t u32Value)
{
    pu8Value[0] = (uint8_t)(u32Value >> 24U);
    pu8Value[1] = (uint8_t)(u32Value >> 16U);
    pu8Value[2] = (uint8_t)(u32Value >> 8U);
    pu8Value[3] = (uint8_t)u32Value;
}

/*******************************************************
  * Name    :ModemMsdCodec_CoordinateRaw
  * Author  :LoH
  * Function:Coordinate Raw within the bounded MSD protocol workflow.
  * Version :V1.0.0
  * Data    :2026.11.14
*******************************************************/
static int32_t ModemMsdCodec_CoordinateRaw(int32_t i32Coordinate)
{
    int64_t i64Scaled;
    if (i32Coordinate == INT32_MAX)
    {
        return INT32_MAX;
    }
    i64Scaled = (int64_t)i32Coordinate * 9;
    return (int32_t)((i64Scaled >= 0) ? ((i64Scaled + 12) / 25) : -(((-i64Scaled) + 12) / 25));
}

/*******************************************************
  * Name    :ModemMsdCodec_AppendString
  * Author  :LoH
  * Function:Append String within the bounded MSD protocol workflow.
  * Version :V1.0.0
  * Data    :2026.11.14
*******************************************************/
static uint16_t ModemMsdCodec_AppendString(uint8_t *pu8Payload, uint16_t u16Offset, uint8_t u8Tag,
                                          const char *pcValue, uint8_t u8Maximum)
{
    uint8_t u8Length = 0U;
    uint8_t u8Index;
    if (pcValue == 0)
    {
        return u16Offset;
    }
    while ((u8Length <= u8Maximum) && (pcValue[u8Length] != '\0'))
    {
        u8Length++;
    }
    if ((u8Length == 0U) || (u8Length > u8Maximum))
    {
        return u16Offset;
    }
    for (u8Index = 0U; u8Index < u8Length; u8Index++)
    {
        if ((pcValue[u8Index] < 0x20) || (pcValue[u8Index] > 0x7E))
        {
            return u16Offset;
        }
        if ((u8Tag <= 4U)
            && (!((pcValue[u8Index] >= '0') && (pcValue[u8Index] <= '9')))
            && (!((u8Tag == 2U) && (u8Index == 0U) && (pcValue[u8Index] == '+'))))
        {
            return u16Offset;
        }
    }
    if (((u8Tag == 2U) && (u8Length == 1U) && (pcValue[0] == '+'))
        || ((u8Tag == 3U) && (u8Length != 19U) && (u8Length != 20U))
        || ((u8Tag == 4U) && (u8Length != 15U)))
    {
        return u16Offset;
    }
    pu8Payload[u16Offset++] = u8Tag;
    pu8Payload[u16Offset++] = u8Length;
    (void)memcpy(&pu8Payload[u16Offset], pcValue, u8Length);
    return (uint16_t)(u16Offset + u8Length);
}

/*******************************************************
  * Name    :ModemMsdCodec_BuildFrame
  * Author  :LoH
  * Function:Build Frame within the bounded MSD protocol workflow.
  * Version :V1.0.0
  * Data    :2026.11.14
*******************************************************/
uint16_t ModemMsdCodec_BuildFrame(const EcallMsd_t *pstMsd, const ModemIdentitySnapshot_t *pstIdentity,
                                uint32_t u32Ser, uint8_t u8Identifier, uint8_t u8ExtensionEnabled,
                                uint8_t *pu8Frame, uint16_t u16Capacity)
{
    uint8_t *pu8Payload;
    uint8_t u8Index;
    uint16_t u16PayloadLength = MODEM_MSD_STANDARD_LENGTH;
    uint16_t u16TlvLength;
    if ((pstMsd == 0) || (pu8Frame == 0) || (u8Identifier == 0U)
        || (sizeof(MODEM_PLATFORM_TEST_VIN) != 18U)
        || (u16Capacity < ((u8ExtensionEnabled != 0U) ? 250U : 116U))
        || ((u8ExtensionEnabled != 0U) && ((pstIdentity == 0) || (u32Ser == 0U))))
    {
        return 0U;
    }
    pu8Payload = &pu8Frame[5];
    (void)memset(pu8Payload, 0, MODEM_MSD_STANDARD_LENGTH);
    pu8Payload[0] = MODEM_PLATFORM_MSD_VERSION;
    pu8Payload[1] = u8Identifier;
    pu8Payload[2] = ((pstMsd->trigger_mask & (ECALL_TRIGGER_MASK_AUTO | ECALL_TRIGGER_MASK_CAN)) != 0U) ? 1U : 0U;
    pu8Payload[3] = MODEM_PLATFORM_MSD_TEST_CALL;
    pu8Payload[4] = MODEM_PLATFORM_MSD_VEHICLE_TYPE;
    (void)memcpy(&pu8Payload[5], MODEM_PLATFORM_TEST_VIN, 17U);
    pu8Payload[22] = MODEM_PLATFORM_MSD_POWER_TYPE;
    ModemMsdCodec_PackU32(&pu8Payload[23], pstMsd->utc_seconds);
    ModemMsdCodec_PackU32(&pu8Payload[27], (uint32_t)ModemMsdCodec_CoordinateRaw(pstMsd->longitude_e7));
    ModemMsdCodec_PackU32(&pu8Payload[31], (uint32_t)ModemMsdCodec_CoordinateRaw(pstMsd->latitude_e7));
    for (u8Index = 0U; u8Index < ECALL_GNSS_HISTORY_COUNT; u8Index++)
    {
        ModemMsdCodec_PackU32(&pu8Payload[35U + ((uint16_t)u8Index * 8U)], (uint32_t)ModemMsdCodec_CoordinateRaw(pstMsd->history_longitude_e7[u8Index]));
        ModemMsdCodec_PackU32(&pu8Payload[39U + ((uint16_t)u8Index * 8U)], (uint32_t)ModemMsdCodec_CoordinateRaw(pstMsd->history_latitude_e7[u8Index]));
    }
    pu8Payload[107] = pstMsd->delta_v_longitudinal_raw;
    pu8Payload[108] = pstMsd->delta_v_lateral_raw;
    pu8Payload[109] = pstMsd->occupants;
    pu8Payload[110] = pstMsd->rollover;
    if (u8ExtensionEnabled != 0U)
    {
        pu8Payload[111] = 1U;
        pu8Payload[114] = 1U;
        pu8Payload[115] = 4U;
        ModemMsdCodec_PackU32(&pu8Payload[116], u32Ser);
        u16PayloadLength = 120U;
        u16PayloadLength = ModemMsdCodec_AppendString(pu8Payload, u16PayloadLength, 2U, pstIdentity->acSimPhoneNumber, 16U);
        u16PayloadLength = ModemMsdCodec_AppendString(pu8Payload, u16PayloadLength, 3U, pstIdentity->acIccid, 20U);
        u16PayloadLength = ModemMsdCodec_AppendString(pu8Payload, u16PayloadLength, 4U, pstIdentity->acImei, 15U);
        u16PayloadLength = ModemMsdCodec_AppendString(pu8Payload, u16PayloadLength, 5U, pstIdentity->acHardwareVersion, 32U);
        u16PayloadLength = ModemMsdCodec_AppendString(pu8Payload, u16PayloadLength, 6U, pstIdentity->acSoftwareVersion, 32U);
        u16TlvLength = (uint16_t)(u16PayloadLength - 114U);
        pu8Payload[112] = (uint8_t)(u16TlvLength >> 8U);
        pu8Payload[113] = (uint8_t)u16TlvLength;
    }
    pu8Frame[0] = MODEM_PLATFORM_MSG_MSD;
    ModemMsdCodec_PackU32(&pu8Frame[1], u16PayloadLength);
    return (uint16_t)(u16PayloadLength + 5U);
}

/*******************************************************
  * Name    :ModemMsdCodec_SkipSpace
  * Author  :LoH
  * Function:Skip Space within the bounded MSD protocol workflow.
  * Version :V1.0.0
  * Data    :2026.11.14
*******************************************************/
static void ModemMsdCodec_SkipSpace(const uint8_t *pu8Json, uint16_t u16Length, uint16_t *pu16Offset)
{
    while ((*pu16Offset < u16Length)
        && ((pu8Json[*pu16Offset] == ' ') || (pu8Json[*pu16Offset] == '\r') || (pu8Json[*pu16Offset] == '\n') || (pu8Json[*pu16Offset] == '\t')))
    {
        (*pu16Offset)++;
    }
}

/*******************************************************
  * Name    :ModemMsdCodec_ReadString
  * Author  :LoH
  * Function:Read String within the bounded MSD protocol workflow.
  * Version :V1.0.0
  * Data    :2026.11.14
*******************************************************/
static uint8_t ModemMsdCodec_ReadString(const uint8_t *pu8Json, uint16_t u16Length, uint16_t *pu16Offset,
                                      char *pcValue, uint16_t u16Capacity)
{
    uint16_t u16Count = 0U;
    uint8_t u8Value;
    if ((*pu16Offset >= u16Length) || (pu8Json[(*pu16Offset)++] != '"'))
    {
        return 0U;
    }
    while (*pu16Offset < u16Length)
    {
        u8Value = pu8Json[(*pu16Offset)++];
        if (u8Value == '"')
        {
            if ((pcValue != 0) && (u16Count < u16Capacity))
            {
                pcValue[u16Count] = '\0';
            }
            return ((pcValue == 0) || (u16Count < u16Capacity)) ? 1U : 0U;
        }
        if (u8Value < 0x20U)
        {
            return 0U;
        }
        if (u8Value == '\\')
        {
            /* Correlation strings are ASCII; unknown strings may contain valid JSON escapes. */
            if ((pcValue != 0) || (*pu16Offset >= u16Length))
            {
                return 0U;
            }
            u8Value = pu8Json[(*pu16Offset)++];
            if (u8Value == 'u')
            {
                uint8_t u8Index;
                for (u8Index = 0U; u8Index < 4U; u8Index++)
                {
                    if ((*pu16Offset >= u16Length)
                        || !(((pu8Json[*pu16Offset] >= '0') && (pu8Json[*pu16Offset] <= '9')) || ((pu8Json[*pu16Offset] >= 'A') && (pu8Json[*pu16Offset] <= 'F')) || ((pu8Json[*pu16Offset] >= 'a') && (pu8Json[*pu16Offset] <= 'f'))))
                    {
                        return 0U;
                    }
                    (*pu16Offset)++;
                }
            }
            else if ((u8Value == 0U) || (strchr("\"\\/bfnrt", (int)u8Value) == 0))
            {
                return 0U;
            }
        }
        if ((pcValue != 0) && (u16Count < u16Capacity))
        {
            pcValue[u16Count] = (char)u8Value;
        }
        u16Count++;
    }
    return 0U;
}

/*******************************************************
  * Name    :ModemMsdCodec_ReadU32
  * Author  :LoH
  * Function:Read U32 within the bounded MSD protocol workflow.
  * Version :V1.0.0
  * Data    :2026.11.14
*******************************************************/
static uint8_t ModemMsdCodec_ReadU32(const uint8_t *pu8Json, uint16_t u16Length, uint16_t *pu16Offset, uint32_t *pu32Value)
{
    uint32_t u32Value = 0U;
    uint16_t u16Start = *pu16Offset;
    uint8_t u8Digit;
    while ((*pu16Offset < u16Length) && (pu8Json[*pu16Offset] >= '0') && (pu8Json[*pu16Offset] <= '9'))
    {
        u8Digit = (uint8_t)(pu8Json[*pu16Offset] - '0');
        if (u32Value > ((UINT32_MAX - u8Digit) / 10U))
        {
            return 0U;
        }
        u32Value = (u32Value * 10U) + u8Digit;
        (*pu16Offset)++;
    }
    if ((*pu16Offset == u16Start) || (((uint16_t)(*pu16Offset - u16Start) > 1U) && (pu8Json[u16Start] == '0')))
    {
        return 0U;
    }
    *pu32Value = u32Value;
    return 1U;
}

/*******************************************************
  * Name    :ModemMsdCodec_ParseReply
  * Author  :LoH
  * Function:Parse Reply within the bounded MSD protocol workflow.
  * Version :V1.0.0
  * Data    :2026.11.14
*******************************************************/
static uint8_t ModemMsdCodec_ParseReply(const uint8_t *pu8Json, uint16_t u16Length, ModemMsdReply_t *pstReply)
{
    char acKey[32];
    uint16_t u16Offset = 0U;
    uint8_t u8Bit;
    char *pcText;
    uint16_t u16Capacity;
    uint32_t *pu32Value;
    uint32_t u32Ignored;
    if ((pu8Json == 0) || (pstReply == 0) || (u16Length == 0U) || (u16Length > MODEM_MSD_MAX_PAYLOAD_LENGTH))
    {
        return 0U;
    }
    (void)memset(pstReply, 0, sizeof(*pstReply));
    ModemMsdCodec_SkipSpace(pu8Json, u16Length, &u16Offset);
    if ((u16Offset >= u16Length) || (pu8Json[u16Offset++] != '{'))
    {
        return 0U;
    }
    for (;;)
    {
        ModemMsdCodec_SkipSpace(pu8Json, u16Length, &u16Offset);
        if (ModemMsdCodec_ReadString(pu8Json, u16Length, &u16Offset, acKey, sizeof(acKey)) == 0U)
        {
            return 0U;
        }
        ModemMsdCodec_SkipSpace(pu8Json, u16Length, &u16Offset);
        if ((u16Offset >= u16Length) || (pu8Json[u16Offset++] != ':'))
        {
            return 0U;
        }
        u8Bit = 0U;
        pcText = 0;
        u16Capacity = 0U;
        pu32Value = 0;
        if (strcmp(acKey, "type") == 0)
        {
            u8Bit = 1U;
            pcText = pstReply->acType;
            u16Capacity = sizeof(pstReply->acType);
        }
        else if (strcmp(acKey, "result") == 0)
        {
            u8Bit = 2U;
            pcText = pstReply->acResult;
            u16Capacity = sizeof(pstReply->acResult);
        }
        else if (strcmp(acKey, "deviceId") == 0)
        {
            u8Bit = 4U;
            pcText = pstReply->acDeviceId;
            u16Capacity = sizeof(pstReply->acDeviceId);
        }
        else if (strcmp(acKey, "ser") == 0)
        {
            u8Bit = 8U;
            pu32Value = &pstReply->u32Ser;
        }
        else if (strcmp(acKey, "identifier") == 0)
        {
            u8Bit = 16U;
            pu32Value = &pstReply->u32Identifier;
        }
        else if (strcmp(acKey, "triggerTimeUtc") == 0)
        {
            u8Bit = 32U;
            pu32Value = &pstReply->u32TriggerTime;
        }
        if ((pstReply->u8Seen & u8Bit) != 0U)
        {
            return 0U;
        }
        pstReply->u8Seen |= u8Bit;
        ModemMsdCodec_SkipSpace(pu8Json, u16Length, &u16Offset);
        if ((pu32Value != 0) || ((pcText == 0) && (u16Offset < u16Length) && (pu8Json[u16Offset] != '"')))
        {
            if (ModemMsdCodec_ReadU32(pu8Json, u16Length, &u16Offset, (pu32Value != 0) ? pu32Value : &u32Ignored) == 0U)
            {
                return 0U;
            }
        }
        else if (ModemMsdCodec_ReadString(pu8Json, u16Length, &u16Offset, pcText, u16Capacity) == 0U)
        {
            return 0U;
        }
        ModemMsdCodec_SkipSpace(pu8Json, u16Length, &u16Offset);
        if (u16Offset >= u16Length)
        {
            return 0U;
        }
        if (pu8Json[u16Offset++] == '}')
        {
            break;
        }
        if (pu8Json[u16Offset - 1U] != ',')
        {
            return 0U;
        }
    }
    ModemMsdCodec_SkipSpace(pu8Json, u16Length, &u16Offset);
    return ((u16Offset == u16Length) && ((pstReply->u8Seen & 1U) != 0U)) ? 1U : 0U;
}

/*******************************************************
  * Name    :ModemMsdCodec_ReadReplyType
  * Author  :LoH
  * Function:Read Reply Type within the bounded MSD protocol workflow.
  * Version :V1.0.0
  * Data    :2026.11.14
*******************************************************/
uint8_t ModemMsdCodec_ReadReplyType(const uint8_t *pu8Json, uint16_t u16Length, char *pcType, uint16_t u16Capacity)
{
    ModemMsdReply_t stReply;
    if ((pcType == 0) || (u16Capacity == 0U) || (ModemMsdCodec_ParseReply(pu8Json, u16Length, &stReply) == 0U)
        || (strlen(stReply.acType) >= u16Capacity))
    {
        return 0U;
    }
    (void)strcpy(pcType, stReply.acType);
    return 1U;
}

/*******************************************************
  * Name    :ModemMsdCodec_IsMatchingControlAck
  * Author  :LoH
  * Function:Validate a successful control reply for the configured device.
  * Version :V1.0.0
  * Data    :2026.11.17
*******************************************************/
uint8_t ModemMsdCodec_IsMatchingControlAck(const uint8_t *pu8Json, uint16_t u16Length,
                                          const char *pcExpectedType)
{
    ModemMsdReply_t stReply;
    char acDeviceId[16];

    if ((pcExpectedType == 0)
        || (ModemMsdCodec_ParseReply(pu8Json, u16Length, &stReply) == 0U))
    {
        return 0U;
    }

    (void)snprintf(acDeviceId, sizeof(acDeviceId), "%04X", MODEM_PLATFORM_TEST_DEVICE_ID);
    return (((stReply.u8Seen & 7U) == 7U)
        && (strcmp(stReply.acType, pcExpectedType) == 0)
        && (strcmp(stReply.acResult, "ok") == 0)
        && (strcmp(stReply.acDeviceId, acDeviceId) == 0)) ? 1U : 0U;
}

/*******************************************************
  * Name    :ModemMsdCodec_IsMatchingAck
  * Author  :LoH
  * Function:Is Matching Ack within the bounded MSD protocol workflow.
  * Version :V1.0.0
  * Data    :2026.11.14
*******************************************************/
uint8_t ModemMsdCodec_IsMatchingAck(const uint8_t *pu8Json, uint16_t u16Length, uint32_t u32Ser,
                                  uint8_t u8Identifier, uint32_t u32TriggerTime, uint8_t u8RequireSer)
{
    ModemMsdReply_t stReply;
    char acDeviceId[16];
    if ((ModemMsdCodec_ParseReply(pu8Json, u16Length, &stReply) == 0U) || (strcmp(stReply.acType, "msd_ack") != 0))
    {
        return 0U;
    }
    if (u8RequireSer == 0U)
    {
        return (((stReply.u8Seen & 2U) == 0U) || (strcmp(stReply.acResult, "ok") == 0)) ? 1U : 0U;
    }
    (void)snprintf(acDeviceId, sizeof(acDeviceId), "%04X", MODEM_PLATFORM_TEST_DEVICE_ID);
    return ((stReply.u8Seen == 63U) && (strcmp(stReply.acResult, "ok") == 0)
        && (strcmp(stReply.acDeviceId, acDeviceId) == 0) && (stReply.u32Ser == u32Ser)
        && (stReply.u32Identifier == u8Identifier) && (stReply.u32TriggerTime == u32TriggerTime)) ? 1U : 0U;
}
