/*******************************************************
  * Name    :Gpio.c
  * Author  :WangHu
  * Function:****
  * Version :V1.0.0
  * Data    :2023.12.12
*******************************************************/
/*-------------include file---------------------------*/
#include "Gpio_Cfg.h"
#include "hc32_ll_interrupts.h"
/*-------------function-------------------------------*/
/*******************************************************
  * Name    :GPIO_SetOutputLevel
  * Author  :LoH
  * Function:Write one physical GPIO output level.
  * Version :V1.0.0
  * Data    :2026.09.06
*******************************************************/
void GPIO_SetOutputLevel(E_GPIO_OUTPUT_INDEX index, IO_Type state)
{
    if (index >= E_OUTPUT_GPIO_NUM)
    {
        return;
    }

    if (state == HIGH)
    {
        GPIO_SetPins(g_astGpioOutputConfig[index].u8Port, g_astGpioOutputConfig[index].u16Pin);
    }
    else
    {
        GPIO_ResetPins(g_astGpioOutputConfig[index].u8Port, g_astGpioOutputConfig[index].u16Pin);
    }
}
/*******************************************************
  * Name    :Gpio_Init
  * Author  :LoH
  * Function:****
  * Version :V1.0.0
  * Data    :2026.09.06
*******************************************************/
void Gpio_Init(void)
{
    uint8_t         index = 0;
    stc_gpio_init_t stcGpioInit;

//    GPIO_SetDebugPort(GPIO_PIN_TDI, DISABLE);    //PA15
//    GPIO_SetDebugPort(GPIO_PIN_TDO, DISABLE);    //PB3
//    GPIO_SetDebugPort(GPIO_PIN_TRST, DISABLE);   //PB4

    (void)GPIO_StructInit(&stcGpioInit);
    stcGpioInit.u16PinAttr = PIN_ATTR_DIGITAL;
    for (index = 0; index < E_INPUT_GPIO_NUM; index++)
    {
        (void)GPIO_Init(g_astGpioInputConfig[index].u8Port, g_astGpioInputConfig[index].u16Pin, &stcGpioInit);
    }

    (void)GPIO_StructInit(&stcGpioInit);
    stcGpioInit.u16PinDir  = PIN_DIR_OUT;
    stcGpioInit.u16PinAttr = PIN_ATTR_DIGITAL;
    stcGpioInit.u16PinDrv  = PIN_HIGH_DRV;
    for (index = 0; index < E_OUTPUT_GPIO_NUM; index++)
    {
        (void)GPIO_Init(g_astGpioOutputConfig[index].u8Port, g_astGpioOutputConfig[index].u16Pin, &stcGpioInit);
    }

}


/*******************************************************
  * Name    :Gpio_DeInit
  * Author  :LoH
  * Function:****
  * Version :V1.0.0
  * Data    :2026.09.06
*******************************************************/
void Mcal_Gpio_DeInit(void)
{

}


/*******************************************************
  * Name    :GPIO_GetInputLevel
  * Author  :ZhuSenRong
  * Function:Read one physical GPIO input level.
  * Version :V1.0.0
  * Data    :2023.12.12
*******************************************************/
IO_Type GPIO_GetInputLevel(E_GPIO_INPUT_INDEX index)
{
    if (index >= E_INPUT_GPIO_NUM)
    {
        return LOW;
    }

    return (IO_Type)GPIO_ReadInputPins(g_astGpioInputConfig[index].u8Port,
                                        g_astGpioInputConfig[index].u16Pin);
}

/*******************************************************
  * Name    :GpioScanTask
  * Author  :LoH
  * Function:Retain the legacy scheduler hook; filtering runs in InputService.
  * Version :V1.0.0
  * Data    :2026.09.06
*******************************************************/
void GpioScanTask(void)
{
}

/*******************************************************
  * Name    :GPIO_GetOutputLevel
  * Author  :ZhuSenRong
  * Function:Read one physical GPIO output level.
  * Version :V1.0.0
  * Data    :2023.12.12
*******************************************************/
IO_Type GPIO_GetOutputLevel(E_GPIO_OUTPUT_INDEX index)
{
    return (IO_Type)GPIO_ReadOutputPins(g_astGpioOutputConfig[index].u8Port,
                                         g_astGpioOutputConfig[index].u16Pin);
}
