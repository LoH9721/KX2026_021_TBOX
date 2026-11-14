/*******************************************************
  * Name    :CanService.h
  * Author  :LoH
  * Function:Controller-independent CAN FD service interface.
  * Version :V1.0.0
  * Data    :2026.08.29
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

typedef union
{
    uint8_t U[8];
    struct
    {
        uint8_t u8ReservedByte0;
        uint8_t u8ReservedByte1;
        uint8_t u8CrashOutputStatus : 1;
        uint8_t u8ReservedByte2 : 7;
        uint8_t u8ReservedByte3;
        uint8_t u8ReservedByte4;
        uint8_t u8ReservedByte5;
        uint8_t u8RollingCounter : 4;
        uint8_t u8ReservedByte6 : 4;
        uint8_t u8Checksum;
    } B;
} CanServiceSrsInfoFrame_t;

typedef union
{
    uint8_t U[8];
    struct
    {
        uint8_t u8EcallStatus : 3;
        uint8_t u8ReservedByte0 : 5;
        uint8_t u8DurationSecond;
        uint8_t u8DurationMinute;
        uint8_t u8DurationHourRaw;
        uint8_t u8ReservedByte4;
        uint8_t u8ReservedByte5;
        uint8_t u8ReservedByte6;
        uint8_t u8ReservedByte7;
    } B;
} CanServiceEcallInfoFrame_t;

typedef union
{
    uint8_t U[8];
    struct
    {
        uint8_t u8YearHigh;
        uint8_t u8YearLow;
        uint8_t u8Month;
        uint8_t u8DayRaw;
        uint8_t u8Hour;
        uint8_t u8Minute;
        uint8_t u8SecondRaw;
        uint8_t u8ReservedByte7;
    } B;
} CanServiceTimeFrame_t;

/* Initialize CAN2 hardware, PHY and all CAN service state. */
void CanService_Init(void);
/* Drain received frames, manage errors and send the 100 ms eCall status and time frames. */
void CanService_Process(void);
/* Consume one rising-edge crash trigger from DBC SRS_Info 0x10C. */
uint8_t CanService_TakeEcallTrigger(void);
/* Return any future CAN-sourced eCall data; SRS_Info currently carries crash state only. */
uint8_t CanService_GetEcallData(CanServiceEcallData_t *pstData);
/* Publish application call state for DBC message TBOX_EcallInfo 0x21C. */
void CanService_SetEcallStatus(CanServiceEcallStatus_t enStatus);

#endif
