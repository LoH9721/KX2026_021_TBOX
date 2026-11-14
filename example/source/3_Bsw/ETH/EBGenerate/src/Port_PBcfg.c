/***********************************************************************************************************************
 * Project Name: XHSC AUTOSAR MCAL
 * File Name: Port_PBcfg.c
 *
 * Description: Configuration file for PORT module
 *              Compiler:                IAR GCC
 *              AutoSAR Version:         4.4.0
 *
 *
 * Copyright (c) 2022 - 2025 Xiaohua Semiconductor Co., Ltd.
 * 
 * All rights reserved.
 *
 * This software is licensed by XHSC PLATFORM SOFTWARE LICENSE AGREEMENT (the "License"). By downloading, installing, or
 * using the Software, you indicate that you accept the terms of the License, and you acknowledge that you have the
 * authority, for yourself or on behalf of your company, to bind your company to these terms. If you do not agree to all
 * of the terms of this License, you should not download or install the Software.
 *
 **********************************************************************************************************************/

/**
 *   @file       Port_PBcfg.c
 *   @version    V1.0.3
 *   @brief      Autosar 4.4.0 PORT Driver
 *   @addtogroup port_driver
 *   @{
 */

#ifdef __cplusplus
extern "C"{
#endif

/***********************************************************************************************************************
 * INCLUDES
 **********************************************************************************************************************/
#include "Port.h"

/***********************************************************************************************************************
 *  VERSION CHECK
 **********************************************************************************************************************/

/***********************************************************************************************************************
 *  DEFINES & MACROS
 **********************************************************************************************************************/

/***********************************************************************************************************************
 *  EXTERN
 **********************************************************************************************************************/

/***********************************************************************************************************************
 *  LOCAL TYPEDEFS
 **********************************************************************************************************************/

/***********************************************************************************************************************
 *  LOCAL CONSTANTS
 **********************************************************************************************************************/
#define PORT_START_SEC_CONFIG_DATA_UNSPECIFIED
#include "Port_MemMap.h"
static const Port_Gpio_PinConfigType  s_GpioPortPinsConfig_PB_st[19U]=
{
    /* PA2_SMI_MDIO */
    {
        /* Port pin mode from mode list for use with Port_Init() function */
        PORT_GPIO_MODE,
        /* Port pin mode from mode list */
        PORT_PA2_ETH_SMI_MDIO_MODE,
        /* Port pin Sub-mode from mode list */
        PORT_GPIO_MODE,
        /* Pin Id of the GPIO pin */
        1U,
        /* Pin name of the GPIO pin */
        PORT_GPIO_PA2,
        /* Indicate if the dual mode is enable */
        (boolean)FALSE,
        /* The initial direction of the pin */
        (uint8)PORT_PIN_INOUT,
        /* Indicate if the direction is changeable on a port pin during runtime */
        (boolean)TRUE,
        /* Port Pin Level value from Port pin list */
        PORT_PIN_LEVEL_LOW,
        /* Indicate if the mode is changeable on a port pin during runtime */
        (boolean)TRUE,
        /* Output state latch enable */
        (boolean)FALSE,
        /* Input mode Schmitt/CMOS */
        PORT_GPIO_PIN_SCHMITT_INPUT_MODE,
        /* Pull-up enable/disable */
        (boolean)FALSE,
        /* Select driving force mode */
        PORT_GPIO_PIN_LOW_DRIVING_FORCE_MODE,
        /* Open-drain enable/disable */
        (boolean)FALSE
    },
    /* PC1_SMI_MDC */
    {
        /* Port pin mode from mode list for use with Port_Init() function */
        PORT_GPIO_MODE,
        /* Port pin mode from mode list */
        PORT_PC1_ETH_SMI_MDC_MODE,
        /* Port pin Sub-mode from mode list */
        PORT_GPIO_MODE,
        /* Pin Id of the GPIO pin */
        2U,
        /* Pin name of the GPIO pin */
        PORT_GPIO_PC1,
        /* Indicate if the dual mode is enable */
        (boolean)FALSE,
        /* The initial direction of the pin */
        (uint8)PORT_PIN_OUT,
        /* Indicate if the direction is changeable on a port pin during runtime */
        (boolean)TRUE,
        /* Port Pin Level value from Port pin list */
        PORT_PIN_LEVEL_LOW,
        /* Indicate if the mode is changeable on a port pin during runtime */
        (boolean)TRUE,
        /* Output state latch enable */
        (boolean)FALSE,
        /* Input mode Schmitt/CMOS */
        PORT_GPIO_PIN_SCHMITT_INPUT_MODE,
        /* Pull-up enable/disable */
        (boolean)FALSE,
        /* Select driving force mode */
        PORT_GPIO_PIN_LOW_DRIVING_FORCE_MODE,
        /* Open-drain enable/disable */
        (boolean)FALSE
    },
    /* PH15_USART1_TX */
    {
        /* Port pin mode from mode list for use with Port_Init() function */
        PORT_GPIO_MODE,
        /* Port pin mode from mode list */
        PORT_PH15_USART1_TX_MODE,
        /* Port pin Sub-mode from mode list */
        PORT_GPIO_MODE,
        /* Pin Id of the GPIO pin */
        3U,
        /* Pin name of the GPIO pin */
        PORT_GPIO_PH15,
        /* Indicate if the dual mode is enable */
        (boolean)FALSE,
        /* The initial direction of the pin */
        (uint8)PORT_PIN_OUT,
        /* Indicate if the direction is changeable on a port pin during runtime */
        (boolean)TRUE,
        /* Port Pin Level value from Port pin list */
        PORT_PIN_LEVEL_LOW,
        /* Indicate if the mode is changeable on a port pin during runtime */
        (boolean)TRUE,
        /* Output state latch enable */
        (boolean)FALSE,
        /* Input mode Schmitt/CMOS */
        PORT_GPIO_PIN_SCHMITT_INPUT_MODE,
        /* Pull-up enable/disable */
        (boolean)FALSE,
        /* Select driving force mode */
        PORT_GPIO_PIN_LOW_DRIVING_FORCE_MODE,
        /* Open-drain enable/disable */
        (boolean)FALSE
    },
    /* PH13_USART1_RX */
    {
        /* Port pin mode from mode list for use with Port_Init() function */
        PORT_GPIO_MODE,
        /* Port pin mode from mode list */
        PORT_PH13_USART1_RX_MODE,
        /* Port pin Sub-mode from mode list */
        PORT_GPIO_MODE,
        /* Pin Id of the GPIO pin */
        4U,
        /* Pin name of the GPIO pin */
        PORT_GPIO_PH13,
        /* Indicate if the dual mode is enable */
        (boolean)FALSE,
        /* The initial direction of the pin */
        (uint8)PORT_PIN_IN,
        /* Indicate if the direction is changeable on a port pin during runtime */
        (boolean)TRUE,
        /* Port Pin Level value from Port pin list */
        PORT_PIN_LEVEL_LOW,
        /* Indicate if the mode is changeable on a port pin during runtime */
        (boolean)TRUE,
        /* Output state latch enable */
        (boolean)FALSE,
        /* Input mode Schmitt/CMOS */
        PORT_GPIO_PIN_SCHMITT_INPUT_MODE,
        /* Pull-up enable/disable */
        (boolean)FALSE,
        /* Select driving force mode */
        PORT_GPIO_PIN_LOW_DRIVING_FORCE_MODE,
        /* Open-drain enable/disable */
        (boolean)FALSE
    },
    /* PH2_MII_CRS */
    {
        /* Port pin mode from mode list for use with Port_Init() function */
        PORT_GPIO_MODE,
        /* Port pin mode from mode list */
        PORT_PH2_ETH_MII_CRS_MODE,
        /* Port pin Sub-mode from mode list */
        PORT_GPIO_MODE,
        /* Pin Id of the GPIO pin */
        5U,
        /* Pin name of the GPIO pin */
        PORT_GPIO_PH2,
        /* Indicate if the dual mode is enable */
        (boolean)FALSE,
        /* The initial direction of the pin */
        (uint8)PORT_PIN_IN,
        /* Indicate if the direction is changeable on a port pin during runtime */
        (boolean)TRUE,
        /* Port Pin Level value from Port pin list */
        PORT_PIN_LEVEL_LOW,
        /* Indicate if the mode is changeable on a port pin during runtime */
        (boolean)TRUE,
        /* Output state latch enable */
        (boolean)FALSE,
        /* Input mode Schmitt/CMOS */
        PORT_GPIO_PIN_SCHMITT_INPUT_MODE,
        /* Pull-up enable/disable */
        (boolean)FALSE,
        /* Select driving force mode */
        PORT_GPIO_PIN_LOW_DRIVING_FORCE_MODE,
        /* Open-drain enable/disable */
        (boolean)FALSE
    },
    /* PH3_MII_COL */
    {
        /* Port pin mode from mode list for use with Port_Init() function */
        PORT_GPIO_MODE,
        /* Port pin mode from mode list */
        PORT_PH3_ETH_MII_COL_MODE,
        /* Port pin Sub-mode from mode list */
        PORT_GPIO_MODE,
        /* Pin Id of the GPIO pin */
        6U,
        /* Pin name of the GPIO pin */
        PORT_GPIO_PH3,
        /* Indicate if the dual mode is enable */
        (boolean)FALSE,
        /* The initial direction of the pin */
        (uint8)PORT_PIN_IN,
        /* Indicate if the direction is changeable on a port pin during runtime */
        (boolean)TRUE,
        /* Port Pin Level value from Port pin list */
        PORT_PIN_LEVEL_LOW,
        /* Indicate if the mode is changeable on a port pin during runtime */
        (boolean)TRUE,
        /* Output state latch enable */
        (boolean)FALSE,
        /* Input mode Schmitt/CMOS */
        PORT_GPIO_PIN_SCHMITT_INPUT_MODE,
        /* Pull-up enable/disable */
        (boolean)FALSE,
        /* Select driving force mode */
        PORT_GPIO_PIN_LOW_DRIVING_FORCE_MODE,
        /* Open-drain enable/disable */
        (boolean)FALSE
    },
    /* PA1_MII_RXCLK */
    {
        /* Port pin mode from mode list for use with Port_Init() function */
        PORT_GPIO_MODE,
        /* Port pin mode from mode list */
        PORT_PA1_ETH_MII_RMII_RXCLK_MODE,
        /* Port pin Sub-mode from mode list */
        PORT_GPIO_MODE,
        /* Pin Id of the GPIO pin */
        7U,
        /* Pin name of the GPIO pin */
        PORT_GPIO_PA1,
        /* Indicate if the dual mode is enable */
        (boolean)FALSE,
        /* The initial direction of the pin */
        (uint8)PORT_PIN_IN,
        /* Indicate if the direction is changeable on a port pin during runtime */
        (boolean)TRUE,
        /* Port Pin Level value from Port pin list */
        PORT_PIN_LEVEL_LOW,
        /* Indicate if the mode is changeable on a port pin during runtime */
        (boolean)TRUE,
        /* Output state latch enable */
        (boolean)FALSE,
        /* Input mode Schmitt/CMOS */
        PORT_GPIO_PIN_SCHMITT_INPUT_MODE,
        /* Pull-up enable/disable */
        (boolean)FALSE,
        /* Select driving force mode */
        PORT_GPIO_PIN_LOW_DRIVING_FORCE_MODE,
        /* Open-drain enable/disable */
        (boolean)FALSE
    },
    /* PI10_MII_RXER */
    {
        /* Port pin mode from mode list for use with Port_Init() function */
        PORT_GPIO_MODE,
        /* Port pin mode from mode list */
        PORT_PI10_ETH_MII_RXER_MODE,
        /* Port pin Sub-mode from mode list */
        PORT_GPIO_MODE,
        /* Pin Id of the GPIO pin */
        8U,
        /* Pin name of the GPIO pin */
        PORT_GPIO_PI10,
        /* Indicate if the dual mode is enable */
        (boolean)FALSE,
        /* The initial direction of the pin */
        (uint8)PORT_PIN_IN,
        /* Indicate if the direction is changeable on a port pin during runtime */
        (boolean)TRUE,
        /* Port Pin Level value from Port pin list */
        PORT_PIN_LEVEL_LOW,
        /* Indicate if the mode is changeable on a port pin during runtime */
        (boolean)TRUE,
        /* Output state latch enable */
        (boolean)FALSE,
        /* Input mode Schmitt/CMOS */
        PORT_GPIO_PIN_SCHMITT_INPUT_MODE,
        /* Pull-up enable/disable */
        (boolean)FALSE,
        /* Select driving force mode */
        PORT_GPIO_PIN_LOW_DRIVING_FORCE_MODE,
        /* Open-drain enable/disable */
        (boolean)FALSE
    },
    /* PA7_MII_MII_RXDV */
    {
        /* Port pin mode from mode list for use with Port_Init() function */
        PORT_GPIO_MODE,
        /* Port pin mode from mode list */
        PORT_PA7_ETH_MII_RMII_RXDV_MODE,
        /* Port pin Sub-mode from mode list */
        PORT_GPIO_MODE,
        /* Pin Id of the GPIO pin */
        9U,
        /* Pin name of the GPIO pin */
        PORT_GPIO_PA7,
        /* Indicate if the dual mode is enable */
        (boolean)FALSE,
        /* The initial direction of the pin */
        (uint8)PORT_PIN_IN,
        /* Indicate if the direction is changeable on a port pin during runtime */
        (boolean)TRUE,
        /* Port Pin Level value from Port pin list */
        PORT_PIN_LEVEL_LOW,
        /* Indicate if the mode is changeable on a port pin during runtime */
        (boolean)TRUE,
        /* Output state latch enable */
        (boolean)FALSE,
        /* Input mode Schmitt/CMOS */
        PORT_GPIO_PIN_SCHMITT_INPUT_MODE,
        /* Pull-up enable/disable */
        (boolean)FALSE,
        /* Select driving force mode */
        PORT_GPIO_PIN_LOW_DRIVING_FORCE_MODE,
        /* Open-drain enable/disable */
        (boolean)FALSE
    },
    /* PC4_MII_MII_RXD0 */
    {
        /* Port pin mode from mode list for use with Port_Init() function */
        PORT_GPIO_MODE,
        /* Port pin mode from mode list */
        PORT_PC4_ETH_MII_RMII_RXD0_MODE,
        /* Port pin Sub-mode from mode list */
        PORT_GPIO_MODE,
        /* Pin Id of the GPIO pin */
        10U,
        /* Pin name of the GPIO pin */
        PORT_GPIO_PC4,
        /* Indicate if the dual mode is enable */
        (boolean)FALSE,
        /* The initial direction of the pin */
        (uint8)PORT_PIN_IN,
        /* Indicate if the direction is changeable on a port pin during runtime */
        (boolean)TRUE,
        /* Port Pin Level value from Port pin list */
        PORT_PIN_LEVEL_LOW,
        /* Indicate if the mode is changeable on a port pin during runtime */
        (boolean)TRUE,
        /* Output state latch enable */
        (boolean)FALSE,
        /* Input mode Schmitt/CMOS */
        PORT_GPIO_PIN_SCHMITT_INPUT_MODE,
        /* Pull-up enable/disable */
        (boolean)FALSE,
        /* Select driving force mode */
        PORT_GPIO_PIN_LOW_DRIVING_FORCE_MODE,
        /* Open-drain enable/disable */
        (boolean)FALSE
    },
    /* PC5_MII_MII_RXD1 */
    {
        /* Port pin mode from mode list for use with Port_Init() function */
        PORT_GPIO_MODE,
        /* Port pin mode from mode list */
        PORT_PC5_ETH_MII_RMII_RXD1_MODE,
        /* Port pin Sub-mode from mode list */
        PORT_GPIO_MODE,
        /* Pin Id of the GPIO pin */
        11U,
        /* Pin name of the GPIO pin */
        PORT_GPIO_PC5,
        /* Indicate if the dual mode is enable */
        (boolean)FALSE,
        /* The initial direction of the pin */
        (uint8)PORT_PIN_IN,
        /* Indicate if the direction is changeable on a port pin during runtime */
        (boolean)TRUE,
        /* Port Pin Level value from Port pin list */
        PORT_PIN_LEVEL_LOW,
        /* Indicate if the mode is changeable on a port pin during runtime */
        (boolean)TRUE,
        /* Output state latch enable */
        (boolean)FALSE,
        /* Input mode Schmitt/CMOS */
        PORT_GPIO_PIN_SCHMITT_INPUT_MODE,
        /* Pull-up enable/disable */
        (boolean)FALSE,
        /* Select driving force mode */
        PORT_GPIO_PIN_LOW_DRIVING_FORCE_MODE,
        /* Open-drain enable/disable */
        (boolean)FALSE
    },
    /* PB0_MII_RXD2 */
    {
        /* Port pin mode from mode list for use with Port_Init() function */
        PORT_GPIO_MODE,
        /* Port pin mode from mode list */
        PORT_PB0_ETH_MII_RXD2_MODE,
        /* Port pin Sub-mode from mode list */
        PORT_GPIO_MODE,
        /* Pin Id of the GPIO pin */
        12U,
        /* Pin name of the GPIO pin */
        PORT_GPIO_PB0,
        /* Indicate if the dual mode is enable */
        (boolean)FALSE,
        /* The initial direction of the pin */
        (uint8)PORT_PIN_IN,
        /* Indicate if the direction is changeable on a port pin during runtime */
        (boolean)TRUE,
        /* Port Pin Level value from Port pin list */
        PORT_PIN_LEVEL_LOW,
        /* Indicate if the mode is changeable on a port pin during runtime */
        (boolean)TRUE,
        /* Output state latch enable */
        (boolean)FALSE,
        /* Input mode Schmitt/CMOS */
        PORT_GPIO_PIN_SCHMITT_INPUT_MODE,
        /* Pull-up enable/disable */
        (boolean)FALSE,
        /* Select driving force mode */
        PORT_GPIO_PIN_LOW_DRIVING_FORCE_MODE,
        /* Open-drain enable/disable */
        (boolean)FALSE
    },
    /* PB1_MII_RXD3 */
    {
        /* Port pin mode from mode list for use with Port_Init() function */
        PORT_GPIO_MODE,
        /* Port pin mode from mode list */
        PORT_PB1_ETH_MII_RXD3_MODE,
        /* Port pin Sub-mode from mode list */
        PORT_GPIO_MODE,
        /* Pin Id of the GPIO pin */
        13U,
        /* Pin name of the GPIO pin */
        PORT_GPIO_PB1,
        /* Indicate if the dual mode is enable */
        (boolean)FALSE,
        /* The initial direction of the pin */
        (uint8)PORT_PIN_IN,
        /* Indicate if the direction is changeable on a port pin during runtime */
        (boolean)TRUE,
        /* Port Pin Level value from Port pin list */
        PORT_PIN_LEVEL_LOW,
        /* Indicate if the mode is changeable on a port pin during runtime */
        (boolean)TRUE,
        /* Output state latch enable */
        (boolean)FALSE,
        /* Input mode Schmitt/CMOS */
        PORT_GPIO_PIN_SCHMITT_INPUT_MODE,
        /* Pull-up enable/disable */
        (boolean)FALSE,
        /* Select driving force mode */
        PORT_GPIO_PIN_LOW_DRIVING_FORCE_MODE,
        /* Open-drain enable/disable */
        (boolean)FALSE
    },
    /* PB6_MII_TXCLK */
    {
        /* Port pin mode from mode list for use with Port_Init() function */
        PORT_GPIO_MODE,
        /* Port pin mode from mode list */
        PORT_PB6_ETH_MII_TXCLK_MODE,
        /* Port pin Sub-mode from mode list */
        PORT_GPIO_MODE,
        /* Pin Id of the GPIO pin */
        14U,
        /* Pin name of the GPIO pin */
        PORT_GPIO_PB6,
        /* Indicate if the dual mode is enable */
        (boolean)FALSE,
        /* The initial direction of the pin */
        (uint8)PORT_PIN_IN,
        /* Indicate if the direction is changeable on a port pin during runtime */
        (boolean)TRUE,
        /* Port Pin Level value from Port pin list */
        PORT_PIN_LEVEL_LOW,
        /* Indicate if the mode is changeable on a port pin during runtime */
        (boolean)TRUE,
        /* Output state latch enable */
        (boolean)FALSE,
        /* Input mode Schmitt/CMOS */
        PORT_GPIO_PIN_SCHMITT_INPUT_MODE,
        /* Pull-up enable/disable */
        (boolean)FALSE,
        /* Select driving force mode */
        PORT_GPIO_PIN_LOW_DRIVING_FORCE_MODE,
        /* Open-drain enable/disable */
        (boolean)FALSE
    },
    /* PG11_MII_TX_EN */
    {
        /* Port pin mode from mode list for use with Port_Init() function */
        PORT_GPIO_MODE,
        /* Port pin mode from mode list */
        PORT_PG11_ETH_MII_RMII_TXEN_MODE,
        /* Port pin Sub-mode from mode list */
        PORT_GPIO_MODE,
        /* Pin Id of the GPIO pin */
        15U,
        /* Pin name of the GPIO pin */
        PORT_GPIO_PG11,
        /* Indicate if the dual mode is enable */
        (boolean)FALSE,
        /* The initial direction of the pin */
        (uint8)PORT_PIN_OUT,
        /* Indicate if the direction is changeable on a port pin during runtime */
        (boolean)TRUE,
        /* Port Pin Level value from Port pin list */
        PORT_PIN_LEVEL_LOW,
        /* Indicate if the mode is changeable on a port pin during runtime */
        (boolean)TRUE,
        /* Output state latch enable */
        (boolean)FALSE,
        /* Input mode Schmitt/CMOS */
        PORT_GPIO_PIN_SCHMITT_INPUT_MODE,
        /* Pull-up enable/disable */
        (boolean)FALSE,
        /* Select driving force mode */
        PORT_GPIO_PIN_LOW_DRIVING_FORCE_MODE,
        /* Open-drain enable/disable */
        (boolean)FALSE
    },
    /* PG13_MII_TXD0 */
    {
        /* Port pin mode from mode list for use with Port_Init() function */
        PORT_GPIO_MODE,
        /* Port pin mode from mode list */
        PORT_PG13_ETH_MII_RMII_TXD0_MODE,
        /* Port pin Sub-mode from mode list */
        PORT_GPIO_MODE,
        /* Pin Id of the GPIO pin */
        16U,
        /* Pin name of the GPIO pin */
        PORT_GPIO_PG13,
        /* Indicate if the dual mode is enable */
        (boolean)FALSE,
        /* The initial direction of the pin */
        (uint8)PORT_PIN_OUT,
        /* Indicate if the direction is changeable on a port pin during runtime */
        (boolean)TRUE,
        /* Port Pin Level value from Port pin list */
        PORT_PIN_LEVEL_LOW,
        /* Indicate if the mode is changeable on a port pin during runtime */
        (boolean)TRUE,
        /* Output state latch enable */
        (boolean)FALSE,
        /* Input mode Schmitt/CMOS */
        PORT_GPIO_PIN_SCHMITT_INPUT_MODE,
        /* Pull-up enable/disable */
        (boolean)FALSE,
        /* Select driving force mode */
        PORT_GPIO_PIN_LOW_DRIVING_FORCE_MODE,
        /* Open-drain enable/disable */
        (boolean)FALSE
    },
    /* PG14_MII_TXD1 */
    {
        /* Port pin mode from mode list for use with Port_Init() function */
        PORT_GPIO_MODE,
        /* Port pin mode from mode list */
        PORT_PG14_ETH_MII_RMII_TXD1_MODE,
        /* Port pin Sub-mode from mode list */
        PORT_GPIO_MODE,
        /* Pin Id of the GPIO pin */
        17U,
        /* Pin name of the GPIO pin */
        PORT_GPIO_PG14,
        /* Indicate if the dual mode is enable */
        (boolean)FALSE,
        /* The initial direction of the pin */
        (uint8)PORT_PIN_OUT,
        /* Indicate if the direction is changeable on a port pin during runtime */
        (boolean)TRUE,
        /* Port Pin Level value from Port pin list */
        PORT_PIN_LEVEL_LOW,
        /* Indicate if the mode is changeable on a port pin during runtime */
        (boolean)TRUE,
        /* Output state latch enable */
        (boolean)FALSE,
        /* Input mode Schmitt/CMOS */
        PORT_GPIO_PIN_SCHMITT_INPUT_MODE,
        /* Pull-up enable/disable */
        (boolean)FALSE,
        /* Select driving force mode */
        PORT_GPIO_PIN_LOW_DRIVING_FORCE_MODE,
        /* Open-drain enable/disable */
        (boolean)FALSE
    },
    /* PB9_MII_TXD2 */
    {
        /* Port pin mode from mode list for use with Port_Init() function */
        PORT_GPIO_MODE,
        /* Port pin mode from mode list */
        PORT_PB9_ETH_MII_TXD2_MODE,
        /* Port pin Sub-mode from mode list */
        PORT_GPIO_MODE,
        /* Pin Id of the GPIO pin */
        18U,
        /* Pin name of the GPIO pin */
        PORT_GPIO_PB9,
        /* Indicate if the dual mode is enable */
        (boolean)FALSE,
        /* The initial direction of the pin */
        (uint8)PORT_PIN_OUT,
        /* Indicate if the direction is changeable on a port pin during runtime */
        (boolean)TRUE,
        /* Port Pin Level value from Port pin list */
        PORT_PIN_LEVEL_LOW,
        /* Indicate if the mode is changeable on a port pin during runtime */
        (boolean)TRUE,
        /* Output state latch enable */
        (boolean)FALSE,
        /* Input mode Schmitt/CMOS */
        PORT_GPIO_PIN_SCHMITT_INPUT_MODE,
        /* Pull-up enable/disable */
        (boolean)FALSE,
        /* Select driving force mode */
        PORT_GPIO_PIN_LOW_DRIVING_FORCE_MODE,
        /* Open-drain enable/disable */
        (boolean)FALSE
    },
    /* PB8_MII_TXD3 */
    {
        /* Port pin mode from mode list for use with Port_Init() function */
        PORT_GPIO_MODE,
        /* Port pin mode from mode list */
        PORT_PB8_ETH_MII_TXD3_MODE,
        /* Port pin Sub-mode from mode list */
        PORT_GPIO_MODE,
        /* Pin Id of the GPIO pin */
        19U,
        /* Pin name of the GPIO pin */
        PORT_GPIO_PB8,
        /* Indicate if the dual mode is enable */
        (boolean)FALSE,
        /* The initial direction of the pin */
        (uint8)PORT_PIN_OUT,
        /* Indicate if the direction is changeable on a port pin during runtime */
        (boolean)TRUE,
        /* Port Pin Level value from Port pin list */
        PORT_PIN_LEVEL_LOW,
        /* Indicate if the mode is changeable on a port pin during runtime */
        (boolean)TRUE,
        /* Output state latch enable */
        (boolean)FALSE,
        /* Input mode Schmitt/CMOS */
        PORT_GPIO_PIN_SCHMITT_INPUT_MODE,
        /* Pull-up enable/disable */
        (boolean)FALSE,
        /* Select driving force mode */
        PORT_GPIO_PIN_LOW_DRIVING_FORCE_MODE,
        /* Open-drain enable/disable */
        (boolean)FALSE
    }
};


/**
 *  @brief   Data structure containing the initialization data for the PORT Driver
 *  @details SRS ID:     SWS_Port_00073
 */
const Port_ConfigType  Port_Config =
{
    /* The number of configured pins. */
    19U,
    /* Pointer to the list of Port configured pins */
    &s_GpioPortPinsConfig_PB_st[0]
};

#define PORT_STOP_SEC_CONFIG_DATA_UNSPECIFIED
#include "Port_MemMap.h"

/***********************************************************************************************************************
 *  LOCAL VARIABLES
 **********************************************************************************************************************/

/***********************************************************************************************************************
 *  GLOBAL VARIABLES
 **********************************************************************************************************************/

/***********************************************************************************************************************
 *  LOCAL FUNCTION PROTOTYPES
 **********************************************************************************************************************/

/***********************************************************************************************************************
 *  LOCAL FUNCTION
 **********************************************************************************************************************/

/***********************************************************************************************************************
 *  GLOBAL FUNCTION
 **********************************************************************************************************************/

#ifdef __cplusplus
}
#endif

/**
 * @}
 */

/*---------------------- End of File -----------------------------------------*/
