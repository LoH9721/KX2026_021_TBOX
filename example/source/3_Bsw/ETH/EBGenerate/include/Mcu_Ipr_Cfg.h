/***********************************************************************************************************************
 * Project Name: XHSC AUTOSAR MCAL
 * File Name: Mcu_Ipr_Cfg.h
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
 *   @file       Mcu_Ipr_Cfg.h
 *   @version    V1.0.3
 *   @brief      Autosar 4.4.0 MCU Driver
 *   @addtogroup mcu_driver
 *   @{
 */

#ifndef MCU_IPR_CFG_H
#define MCU_IPR_CFG_H

#ifdef __cplusplus
extern "C"{
#endif

/***********************************************************************************************************************
 *  Includes
 **********************************************************************************************************************/
#include "StandardTypes.h"
#include "System_Hw_Reg.h"
    
/***********************************************************************************************************************
 *   VERSION CHECK
 **********************************************************************************************************************/


/***********************************************************************************************************************
 *  DEFINES & MACROS
 **********************************************************************************************************************/
/**
 * @brief            Maximum number of MCU Clock configurations.
 */
#define MCU_NUMBER_OF_CLKCONFIGS                      ((uint32)1U)

/**
 * @brief            The maximum count of MCU mode configurations.
 */
#define MCU_NUMBER_OF_MODECONFIGS                     ((uint32)4U)

/**
 * @brief            The maximum count of MCU Ram configurations.
 */
#define MCU_NUMBER_OF_RAMCONFIGS                      ((uint32)1U)


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

#endif /* MCU_IPR_CFG_H */

/*---------------------- End of File ----------------------------------------*/
