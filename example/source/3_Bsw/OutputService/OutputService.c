/*******************************************************
  * Name    :OutputService.c
  * Author  :LoH
  * Function:Power output and indicator test service.
  * Version :V1.0.0
  * Data    :2026.09.06
*******************************************************/
#include "Gpio.h"
#include "OutputService.h"
#include <string.h>

#define OUTPUT_SERVICE_PERIOD_MS              10U
#define OUTPUT_SERVICE_INDICATOR_TOGGLE_MS   500U

typedef struct
{
    E_GPIO_OUTPUT_INDEX enGpioOutput;
    IO_Type enActiveLevel;
    IO_Type enInitialState;
} OutputServiceChannelConfig_t;

typedef struct
{
    IO_Type enIndicatorState;
    uint16_t u16IndicatorElapsedMs;
} OutputServiceRuntime_t;

static const OutputServiceChannelConfig_t s_astOutputServiceChannelConfig[OUTPUT_SERVICE_SIG_NUM] =
{
    {E_OUTPUT_INDICATOR_1, HIGH, LOW},
    {E_OUTPUT_INDICATOR_2, HIGH, LOW},
    {E_OUTPUT_IG_KEEPON, HIGH, HIGH},
    {E_OUTPUT_PWR_EN_CAN, HIGH, HIGH},
    {E_OUTPUT_CAT4_EN, HIGH, HIGH}
};

static OutputServiceRuntime_t s_mOutputRun;

/*******************************************************
  * Name    :OutputService_Init
  * Author  :LoH
  * Function:Initialize configured output states.
  * Version :V1.0.0
  * Data    :2026.09.06
*******************************************************/
void OutputService_Init(void)
{
    uint8_t u8Index;

    (void)memset(&s_mOutputRun, 0, sizeof(s_mOutputRun));
    for (u8Index = 0U; u8Index < OUTPUT_SERVICE_SIG_NUM; u8Index++)
    {
        OutputService_SetState((OutputServiceSignal_t)u8Index,
                               s_astOutputServiceChannelConfig[u8Index].enInitialState);
    }
}

/*******************************************************
  * Name    :OutputService_Process10ms
  * Author  :LoH
  * Function:Synchronously toggle both indicator outputs during test.
  * Version :V1.0.0
  * Data    :2026.09.06
*******************************************************/
void OutputService_Process10ms(void)
{
    if (s_mOutputRun.u16IndicatorElapsedMs < OUTPUT_SERVICE_INDICATOR_TOGGLE_MS)
    {
        s_mOutputRun.u16IndicatorElapsedMs += OUTPUT_SERVICE_PERIOD_MS;
    }

    if (s_mOutputRun.u16IndicatorElapsedMs >= OUTPUT_SERVICE_INDICATOR_TOGGLE_MS)
    {
        s_mOutputRun.enIndicatorState = (s_mOutputRun.enIndicatorState == LOW) ? HIGH : LOW;
        s_mOutputRun.u16IndicatorElapsedMs = 0U;
        OutputService_SetState(OUTPUT_SERVICE_SIG_INDICATOR_1, s_mOutputRun.enIndicatorState);
        OutputService_SetState(OUTPUT_SERVICE_SIG_INDICATOR_2, s_mOutputRun.enIndicatorState);
    }
}

/*******************************************************
  * Name    :OutputService_SetState
  * Author  :LoH
  * Function:Set one configured output service state.
  * Version :V1.0.0
  * Data    :2026.09.06
*******************************************************/
void OutputService_SetState(OutputServiceSignal_t enSignal, IO_Type enState)
{
    IO_Type enOutputLevel;

    if (enSignal < OUTPUT_SERVICE_SIG_NUM)
    {
        if (enState == HIGH)
        {
            enOutputLevel = s_astOutputServiceChannelConfig[enSignal].enActiveLevel;
        }
        else if (s_astOutputServiceChannelConfig[enSignal].enActiveLevel == HIGH)
        {
            enOutputLevel = LOW;
        }
        else
        {
            enOutputLevel = HIGH;
        }
        GPIO_SetOutputLevel(s_astOutputServiceChannelConfig[enSignal].enGpioOutput, enOutputLevel);
    }
}

/*******************************************************
  * Name    :OutputService_SetEcallCancelWindow
  * Author  :LoH
  * Function:Accept the current eCall LED request during GPIO test mode.
  * Version :V1.0.0
  * Data    :2026.09.06
*******************************************************/
void OutputService_SetEcallCancelWindow(uint8_t u8Active)
{
    (void)u8Active;
}
