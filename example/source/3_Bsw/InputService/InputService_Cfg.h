/*******************************************************
  * Name    :InputService_Cfg.h
  * Author  :LoH
  * Function:Input service compile-time channel configuration.
  * Version :V1.0.0
  * Data    :2026.09.06
*******************************************************/
#ifndef __INPUT_SERVICE_CFG_H__
#define __INPUT_SERVICE_CFG_H__

#include "Gpio.h"
#include "InputService.h"

typedef struct
{
    E_GPIO_INPUT_INDEX enGpioInput;
    IO_Type enActiveLevel;
    RteSysSwitchId_t enRteSwitch;
    uint16_t u16DebounceMs;
    const char *pcActiveLog;
} InputServiceChannelConfig_t;

extern const InputServiceChannelConfig_t g_astInputServiceChannelConfig[INPUT_SERVICE_SIG_NUM];

#endif
