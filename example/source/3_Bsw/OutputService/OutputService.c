/*******************************************************
  * Name    :OutputService.c
  * Author  :LoH
  * Function:Power output and indicator test service.
  * Version :V1.0.0
  * Data    :2026.09.06
*******************************************************/
#include "Gpio.h"
#include "OutputService.h"
#include "RteSys.h"

typedef struct
{
    E_GPIO_OUTPUT_INDEX enGpioOutput;
    RteSysOutputId_t enRteOutput;
    IO_Type enActiveLevel;
    IO_Type enInitialState;
} OutputServiceChannelConfig_t;

static const OutputServiceChannelConfig_t s_astOutputServiceChannelConfig[OUTPUT_SERVICE_SIG_NUM] =
{
    {E_OUTPUT_INDICATOR_1, RTESYS_OUTPUT_INDICATOR_1, HIGH, LOW},
    {E_OUTPUT_INDICATOR_2, RTESYS_OUTPUT_INDICATOR_2, HIGH, LOW},
    {E_OUTPUT_IG_KEEPON, RTESYS_OUTPUT_IG_KEEPON, HIGH, HIGH},
    {E_OUTPUT_PWR_EN_CAN, RTESYS_OUTPUT_PWR_EN_CAN, HIGH, HIGH},
    {E_OUTPUT_CAT4_EN, RTESYS_OUTPUT_CAT4_EN, HIGH, HIGH},
    {E_OUTPUT_CAT4_PWR_KEY, RTESYS_OUTPUT_CAT4_PWR_KEY, LOW, LOW},
    {E_OUTPUT_CAT4_RST, RTESYS_OUTPUT_CAT4_RST, LOW, LOW},
    {E_OUTPUT_PWR_EN_GNSS, RTESYS_OUTPUT_PWR_EN_GNSS, HIGH, HIGH},
    {E_OUTPUT_PWR_EN_3V3, RTESYS_OUTPUT_PWR_EN_3V3, HIGH, HIGH},
    {E_OUTPUT_PWR_EN_PHY, RTESYS_OUTPUT_PWR_EN_PHY, HIGH, HIGH},
    {E_OUTPUT_PWR_EN_AUDIO, RTESYS_OUTPUT_PWR_EN_AUDIO, HIGH, HIGH}
};

/*******************************************************
  * Name    :OutputService_ApplyRteRequests
  * Author  :LoH
  * Function:Apply all RTE output requests to physical GPIO levels.
  * Version :V1.0.0
  * Data    :2026.09.06
*******************************************************/
static void OutputService_ApplyRteRequests(void)
{
    uint8_t u8Index;
    IO_Type enOutputLevel;

    for (u8Index = 0U; u8Index < OUTPUT_SERVICE_SIG_NUM; u8Index++)
    {
        if (RteSys_GetOutputState(s_astOutputServiceChannelConfig[u8Index].enRteOutput)
            == RTE_SWITCH_ACTIVE)
        {
            enOutputLevel = s_astOutputServiceChannelConfig[u8Index].enActiveLevel;
        }
        else if (s_astOutputServiceChannelConfig[u8Index].enActiveLevel == HIGH)
        {
            enOutputLevel = LOW;
        }
        else
        {
            enOutputLevel = HIGH;
        }
        GPIO_SetOutputLevel(s_astOutputServiceChannelConfig[u8Index].enGpioOutput, enOutputLevel);
    }
}

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

    for (u8Index = 0U; u8Index < OUTPUT_SERVICE_SIG_NUM; u8Index++)
    {
        RteSys_SetOutputState(s_astOutputServiceChannelConfig[u8Index].enRteOutput,
                              (s_astOutputServiceChannelConfig[u8Index].enInitialState == HIGH)
                              ? RTE_SWITCH_ACTIVE : RTE_SWITCH_INACTIVE);
    }
    OutputService_ApplyRteRequests();
}

/*******************************************************
  * Name    :OutputService_Process10ms
  * Author  :LoH
  * Function:Apply pending output requests to physical GPIO levels.
  * Version :V1.0.0
  * Data    :2026.09.06
*******************************************************/
void OutputService_Process10ms(void)
{
    OutputService_ApplyRteRequests();
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
    if (enSignal < OUTPUT_SERVICE_SIG_NUM)
    {
        RteSys_SetOutputState(s_astOutputServiceChannelConfig[enSignal].enRteOutput,
                              (enState == HIGH) ? RTE_SWITCH_ACTIVE : RTE_SWITCH_INACTIVE);
    }
}

/*******************************************************
  * Name    :OutputService_SetGnssReady
  * Author  :LoH
  * Function:Set GNSS initialization indication.
  * Version :V1.0.0
  * Data    :2026.09.11
*******************************************************/
void OutputService_SetGnssReady(uint8_t u8Ready)
{
    OutputService_SetState(OUTPUT_SERVICE_SIG_INDICATOR_1, (u8Ready != 0U) ? HIGH : LOW);
}

/*******************************************************
  * Name    :OutputService_SetVoiceCallReady
  * Author  :LoH
  * Function:Set voice call availability indication.
  * Version :V1.0.0
  * Data    :2026.09.11
*******************************************************/
void OutputService_SetVoiceCallReady(uint8_t u8Ready)
{
    OutputService_SetState(OUTPUT_SERVICE_SIG_INDICATOR_2, (u8Ready != 0U) ? HIGH : LOW);
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
