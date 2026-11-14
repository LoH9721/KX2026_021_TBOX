/***********************************************************************************************************************
 * Project Name: XHSC AUTOSAR MCAL
 * File Name: Osr_Cfg.h
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
 *   @file       Osr_Cfg.h
 *   @version    1.0.3
 *   @brief      Osr_Cfg Header File
 *   @addtogroup sytem_driver
 *   @{
 */

#ifndef OSR_CFG_H
#define OSR_CFG_H

#ifdef __cplusplus
extern "C"{
#endif

/***********************************************************************************************************************
 *  INCLUDES
 **********************************************************************************************************************/
#include "Osr_Types.h"

/***********************************************************************************************************************
 *  DEFINES & MACROS
 **********************************************************************************************************************/
#define OSR_VALIDATE_PARAMS                (STD_OFF)
#define OSR_MAX_COREIDX_SUPPORTED          (1U)

#ifdef __cplusplus
}
#endif

/**
 * @}
 */

#endif /*!< OSR_CFG_H */
