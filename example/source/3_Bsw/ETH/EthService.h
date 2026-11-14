#ifndef ETHSERVICE_H
#define ETHSERVICE_H
#include <stdint.h>

typedef struct 
{
    uint8_t gpsTime_Year;
    uint8_t gpsTime_Month;
    uint8_t gpsTime_Day;
    uint8_t gpsTime_Hour;
    uint8_t gpsTime_Minute;
    uint8_t gpsTime_Second;
}Eth_GpsTime_B_Def;

typedef union 
{
    Eth_GpsTime_B_Def B;
    uint8_t           U[sizeof(Eth_GpsTime_B_Def)];
}Eth_GpsTime_Def;

void EthService_Init(void);
void EthTask_Create(void);

#endif