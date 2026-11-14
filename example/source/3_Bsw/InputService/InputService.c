/*******************************************************
  * Name    :InputService.c
  * Author  :LoH
  * Function:SOS debounce, SRS input observation and RTE state update.
  * Version :V1.0.0
  * Data    :2026.08.26
*******************************************************/
#include "Hmi.h"
#include "InputService.h"
#include "InputService_Cfg.h"
#include <string.h>

#define INPUT_SERVICE_PERIOD_MS             10U

typedef struct
{
    RteSwitchState_t enState;
    uint16_t u16DebounceMs;
} InputServiceChannelRuntime_t;

typedef struct
{
    InputServiceChannelRuntime_t astChannel[INPUT_SERVICE_SIG_NUM];
} InputServiceRuntime_t;

static InputServiceRuntime_t s_mInputRun;

/*******************************************************
  * Name    :InputService_Init
  * Author  :LoH
  * Function:Initialize all debounced input service states.
  * Version :V1.0.0
  * Data    :2026.08.26
*******************************************************/
void InputService_Init(void)
{
    uint8_t u8Index;

    (void)memset(&s_mInputRun, 0, sizeof(s_mInputRun));
    for (u8Index = 0U; u8Index < INPUT_SERVICE_SIG_NUM; u8Index++)
    {
        s_mInputRun.astChannel[u8Index].enState = RTE_SWITCH_INACTIVE;
        RteSys_SetSwitchState(g_astInputServiceChannelConfig[u8Index].enRteSwitch,
                               RTE_SWITCH_INACTIVE);
    }
}

/*******************************************************
  * Name    :InputService_Process10ms
  * Author  :LoH
  * Function:Debounce all configured inputs and publish stable RTE states.
  * Version :V1.0.0
  * Data    :2026.08.26
*******************************************************/
void InputService_Process10ms(void)
{
    uint8_t u8Index;
    IO_Type enRawLevel;
    RteSwitchState_t enRawState;

    for (u8Index = 0U; u8Index < INPUT_SERVICE_SIG_NUM; u8Index++)
    {
        enRawLevel = GPIO_GetInputLevel(g_astInputServiceChannelConfig[u8Index].enGpioInput);
        enRawState = (enRawLevel == g_astInputServiceChannelConfig[u8Index].enActiveLevel)
                     ? RTE_SWITCH_ACTIVE : RTE_SWITCH_INACTIVE;
        if (enRawState == s_mInputRun.astChannel[u8Index].enState)
        {
            s_mInputRun.astChannel[u8Index].u16DebounceMs = 0U;
        }
        else
        {
            s_mInputRun.astChannel[u8Index].u16DebounceMs += INPUT_SERVICE_PERIOD_MS;
            if (s_mInputRun.astChannel[u8Index].u16DebounceMs
                >= g_astInputServiceChannelConfig[u8Index].u16DebounceMs)
            {
                s_mInputRun.astChannel[u8Index].enState = enRawState;
                s_mInputRun.astChannel[u8Index].u16DebounceMs = 0U;
                RteSys_SetSwitchState(g_astInputServiceChannelConfig[u8Index].enRteSwitch, enRawState);
                if ((enRawState == RTE_SWITCH_ACTIVE)
                    && (g_astInputServiceChannelConfig[u8Index].pcActiveLog != NULL))
                {
                    HMI_LOG_APP(g_astInputServiceChannelConfig[u8Index].pcActiveLog);
                }
            }
        }
    }
}

/*******************************************************
  * Name    :InputService_GetState
  * Author  :LoH
  * Function:Get one debounced input service state.
  * Version :V1.0.0
  * Data    :2026.09.06
*******************************************************/
RteSwitchState_t InputService_GetState(InputServiceSignal_t enSignal)
{
    if (enSignal < INPUT_SERVICE_SIG_NUM)
    {
        return s_mInputRun.astChannel[enSignal].enState;
    }
    return RTE_SWITCH_INACTIVE;
}
