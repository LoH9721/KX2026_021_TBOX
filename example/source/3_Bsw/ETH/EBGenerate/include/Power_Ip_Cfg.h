/***********************************************************************************************************************
 * Project Name: XHSC AUTOSAR MCAL
 * File Name: Power_Ip_Cfg.h
 *
 * Description: Configuration file for MCU module
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
 *   @file       Power_Ip_Cfg.h
 *   @version    V1.0.3
 *   @brief      Autosar 4.4.0 MCU Driver
 *   @addtogroup mcu_driver
 *   @{
 */


#ifndef POWER_IP_CFG_H
#define POWER_IP_CFG_H

#ifdef __cplusplus
extern "C"{
#endif

/***********************************************************************************************************************
 *  INCLUDES
 **********************************************************************************************************************/
#include "Power_Ip_PBcfg.h"
#include "Osr.h"

/***********************************************************************************************************************
 *   VERSION CHECK
 **********************************************************************************************************************/

/***********************************************************************************************************************
 *  DEFINES & MACROS
 **********************************************************************************************************************/
/**
 *  @brief          Define timeout method.
 */
#define POWER_IP_TIMEOUT_TYPE                        (OSR_COUNTER_DUMMY)

/**
 *  @brief          Define timeout value.
 */
#define POWER_IP_TIMEOUT_VALUE                       (100000U)

/**
 * @brief  ISR Mcu_VoltageError_ISR is/isn't available (STD_ON/STD_OFF)
 */
#define POWER_IP_VOLTAGE_ERROR_ISR_USED         (STD_OFF)

/***********************************************************************************************************************
 *  EXTERN
 **********************************************************************************************************************/
/***********************************************************************************************************************
 *  TYPEDEFS
 **********************************************************************************************************************/

/***********************************************************************************************************************
 *  CONSTANTS
 **********************************************************************************************************************/

/***********************************************************************************************************************
 *  GLOBAL VARIABLES
 **********************************************************************************************************************/

#ifdef __cplusplus
}
#endif

/**
 * @}
 */

#endif /* #ifndef POWER_IP_CFG_H */

/*---------------------- End of File ----------------------------------------*/
