/*******************************************************
  * Name    :ModemMsdCodec.h
  * Author  :LoH
  * Function:MSD V0.2 bounded frame encoding and business receipt validation.
  * Version :V1.0.0
  * Data    :2026.11.14
*******************************************************/
#ifndef __MODEM_MSD_CODEC_H__
#define __MODEM_MSD_CODEC_H__
#include <stdint.h>
#include "Ecall.h"
#include "ModemIdentity.h"

#define MODEM_MSD_STANDARD_LENGTH 111U
#define MODEM_MSD_MAX_PAYLOAD_LENGTH 512U
#define MODEM_MSD_MAX_FRAME_LENGTH (MODEM_MSD_MAX_PAYLOAD_LENGTH + 5U)

/* Build a complete immutable network frame; return zero for invalid inputs. */
uint16_t ModemMsdCodec_BuildFrame(const EcallMsd_t *pstMsd, const ModemIdentitySnapshot_t *pstIdentity,
                                uint32_t u32Ser, uint8_t u8Identifier, uint8_t u8ExtensionEnabled,
                                uint8_t *pu8Frame, uint16_t u16Capacity);
/* Validate one complete flat JSON reply and copy its type for logging. */
uint8_t ModemMsdCodec_ReadReplyType(const uint8_t *pu8Json, uint16_t u16Length,
                                  char *pcType, uint16_t u16Capacity);
/* Match a successful registration/control receipt to this configured device. */
uint8_t ModemMsdCodec_IsMatchingControlAck(const uint8_t *pu8Json, uint16_t u16Length,
                                          const char *pcExpectedType);
/* Match a success receipt to one pending MSD, including all V0.2 correlation fields. */
uint8_t ModemMsdCodec_IsMatchingAck(const uint8_t *pu8Json, uint16_t u16Length, uint32_t u32Ser,
                                  uint8_t u8Identifier, uint32_t u32TriggerTime, uint8_t u8RequireSer);
#endif
