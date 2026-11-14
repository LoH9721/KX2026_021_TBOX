/***********************************************************************************************************************
 * Project Name: XHSC AUTOSAR MCAL
 * File Name: Modules.h
 *
 * Description: Configuration file for SYSTEM module
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
 *   @file       Modules.h
 *   @version    V1.0.3
 *   @brief      None
 *   @addtogroup sytem_driver
 *   @{
 */

#ifndef MODULES_H
#define MODULES_H

#ifdef __cplusplus
extern "C"{
#endif


#include "Std_Types.h"

/***********************************************************************************************************************
 *  DEFINES & MACROS
 **********************************************************************************************************************/
/**
 * @brief   Check if ADC is present in the project
 */
#define USE_ADC_MODULE              (STD_OFF)

/**
 * @brief   Check if CAN is present in the project
 */
#define USE_CAN_MODULE              (STD_OFF)

/**
 * @brief   Check if CANIF is present in the project
 */
#define USE_CANIF_MODULE            (STD_OFF)

/**
 * @brief   Check if DEM is present in the project
 */
#define USE_DEM_MODULE              (STD_OFF)

/**
 * @brief   Check if DET is present in the project
 */
#define USE_DET_MODULE              (STD_OFF)

/**
 * @brief   Check if DIO is present in the project
 */
#define USE_DIO_MODULE              (STD_OFF)

/**
 * @brief   Check if ECUC is present in the project
 */
#define USE_ECUC_MODULE             (STD_OFF)

/**
 * @brief   Check if ECUM is present in the project
 */
#define USE_ECUM_MODULE             (STD_OFF)

/**
 * @brief   Check if FLS is present in the project
 */
#define USE_FLS_MODULE              (STD_OFF)

/**
 * @brief   Check if GPT is present in the project
 */
#define USE_GPT_MODULE              (STD_OFF)

/**
 * @brief   Check if I2C is present in the project
 */
#define USE_I2C_MODULE              (STD_OFF)

/**
 * @brief   Check if ICU is present in the project
 */
#define USE_ICU_MODULE              (STD_OFF)

/**
 * @brief   Check if LIN is present in the project
 */
#define USE_LIN_MODULE              (STD_OFF)

/**
 * @brief   Check if LINIF is present in the project
 */
#define USE_LINIF_MODULE            (STD_OFF)

/**
 * @brief   Check if MCU is present in the project
 */
#define USE_MCU_MODULE              (STD_ON)

/**
 * @brief   Check if MEMIF is present in the project
 */
#define USE_MEMIF_MODULE            (STD_OFF)

/**
 * @brief   Check if OS is present in the project
 */
#define USE_OS_MODULE               (STD_ON)

/**
 * @brief   Check if PORT is present in the project
 */
#define USE_PORT_MODULE             (STD_ON)

/**
 * @brief   Check if PWM is present in the project
 */
#define USE_PWM_MODULE              (STD_OFF)

/**
 * @brief   Check if RESOURCE  is present in the project
 */
#define USE_RESOURCE_MODULE         (STD_ON)

/**
 * @brief   Check if RTE  is present in the project
 */
#define USE_RTE_MODULE              (STD_OFF)

/**
 * @brief   Check if SPI is present in the project
 */
#define USE_SPI_MODULE              (STD_OFF)

/**
 * @brief   Check if SYSTEM is present in the project
 */
#define USE_SYSTEM_MODULE           (STD_ON)

/**
 * @brief   Check if UART is present in the project
 */
#define USE_UART_MODULE             (STD_OFF)

/**
 * @brief   Check if WDG is present in the project
 */
#define USE_WDG_MODULE              (STD_OFF)

/**
 * @brief   Check if WDG_168_SWDT is present in the project
 */
#define USE_WDG_168_SWDT_MODULE     (STD_OFF)

/**
 * @brief   Check if WDG_168_WDT is present in the project
 */
#define USE_WDG_168_WDT_MODULE      (STD_OFF)

/**
 * @brief   Check if WDGIF is present in the project
 */
#define USE_WDGIF_MODULE            (STD_OFF)


#ifdef __cplusplus
}
#endif

#endif /* MODULES_H */

/** @} */

