/*******************************************************
  * Name    :CanService.h
  * Author  :LoH
  * Function:Controller-independent CAN FD service interface.
  * Version :V1.0.0
  * Data    :2026.09.06
*******************************************************/
#ifndef __CAN_SERVICE_H__
#define __CAN_SERVICE_H__

#include <stdint.h>

typedef enum
{
    CAN_SERVICE_ECALL_NO_DISPLAY = 0U,
    CAN_SERVICE_ECALL_OUTGOING_CALL = 1U,
    CAN_SERVICE_ECALL_INCOMING_CALL = 2U,
    CAN_SERVICE_ECALL_ON_THE_PHONE = 3U,
    CAN_SERVICE_ECALL_END_OF_CALL = 4U
} CanServiceEcallStatus_t;

typedef struct
{
    uint8_t u8DeltaVLongitudinalRaw;
    uint8_t u8DeltaVLateralRaw;
    uint8_t u8Occupants;
    uint8_t u8Rollover;
} CanServiceEcallData_t;

/* Initialize CAN2 hardware, PHY and all CAN service state. */
void CanService_Init(void);
/* Drain received frames, manage errors and send the 100 ms eCall status frame. */
void CanService_Process(void);
/* Consume one rising-edge crash trigger from the sample-only 0x126 frame. */
uint8_t CanService_TakeEcallTrigger(void);
/* Consume trigger-frozen data first, or copy fresh sample-only 0x126 data. */
uint8_t CanService_GetEcallData(CanServiceEcallData_t *pstData);
/* Publish application call state for DBC message TBOX_EcallInfo 0x21C. */
void CanService_SetEcallStatus(CanServiceEcallStatus_t enStatus);

#endif
