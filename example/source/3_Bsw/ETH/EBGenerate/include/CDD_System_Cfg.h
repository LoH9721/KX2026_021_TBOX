/***********************************************************************************************************************
 * Project Name: XHSC AUTOSAR MCAL
 * File Name: CDD_System_Cfg.h
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
 *   @file       CDD_System_Cfg.h
 *   @version    V1.0.3
 *   @brief      Autosar 4.4.0 SYSTEM Driver
 *   @addtogroup system_driver
 *   @{
 */

#ifndef SYSTEM_CFG_H
#define SYSTEM_CFG_H

#ifdef __cplusplus
extern "C"{
#endif

/***********************************************************************************************************************
 *  Includes
 **********************************************************************************************************************/
#include "System_Types.h"

/***********************************************************************************************************************
 *                              VERSION INFORMATION
 **********************************************************************************************************************/
#define SYSTEM_CFG_VENDOR_ID                      168U
#define SYSTEM_CFG_SW_MAJOR_VERSION               1U
#define SYSTEM_CFG_SW_MINOR_VERSION               0U
#define SYSTEM_CFG_SW_PATCH_VERSION               3U

/***********************************************************************************************************************
 *  Define & Macros
 **********************************************************************************************************************/
/**
 * @brief      Pre-compile configuration constants
 */
#define SYSTEM_PRECOMPILE_SUPPORT           (STD_OFF)

#define SYSTEM_ICG0_START_ADDR                  0x400
#define SYSTEM_ICG1_START_ADDR                  0x404
#define SYSTEM_ICG3_START_ADDR                  0x40C
#define SYSTEM_ICG8A_START_ADDR                 0x460
#define SYSTEM_ICG8B_START_ADDR                 0x464
#define SYSTEM_ICG8C_START_ADDR                 0x468

#define SYSTEM_ICG0_NOT_CONFIGURED              0xFFFFFFFFUL

#define SYSTEM_ICG1_NOT_CONFIGURED              0xFFFFFFFFUL

#define SYSTEM_ICG3_NOT_CONFIGURED              0xFFFFFFFFUL

#define SYSTEM_ICG8X_NOT_CONFIGURED              0xFFFFFFFFUL

/**
 * @brief   ICG1
 */
#define ICG1_BORDIS_ALLOWED                     0U
#define ICG1_BORDIS_DISABLED                    1U
#define ICG1_BORLEV_20                          0U
#define ICG1_BORLEV_21                          1U
#define ICG1_BORLEV_22                          2U
#define ICG1_BORLEV_24                          3U
#define ICG1_HRCSTOP_OSCILLATES                 0U
#define ICG1_HRCSTOP_STOPS                      1U
#define ICG1_HRCFREQSEL_20MHZ                   0U
#define ICG1_HRCFREQSEL_16MHZ                   1U

/**
 * @brief   ICG3
 */
#define ICG3_DBUSPRT_READPROTECTION_ENABLED     (0x4450U)

/**
 * @SWS     ECUC_System_00300
 * @Design
 */
#define SYSTEM_DEV_ERROR_DETECT           (STD_OFF)
/**
 * @SWS
 * ECUC_System_00301
 */
#define SYSTEM_AOS_ENABLE                 (STD_OFF)

/**
 * @brief   Total number of AOS Configset
 */
#define SYSTEM_AOS_TOTALNUMBER_CONFIGSET              ((uint32)0U)

/**
 * @brief   System Get version info API Support
 */
#define SYSTEM_VERSION_INFO_API           (STD_ON)



/***********************************************************************************************************************
 *  ENUMS
 **********************************************************************************************************************/

/***********************************************************************************************************************
 *  GLOBAL VARIABLES
 **********************************************************************************************************************/
#define SYSTEM_START_SEC_CONFIG_DATA_UNSPECIFIED
#include "System_MemMap.h"

extern const System_ConfigType System_Config;

#define SYSTEM_STOP_SEC_CONFIG_DATA_UNSPECIFIED
#include "System_MemMap.h"

/***********************************************************************************************************************
 *  GLOBAL FUNCTION
 **********************************************************************************************************************/
void ETH_GLB_INT_IRQHandler(void);

#ifdef __cplusplus
}
#endif

/**
 * @}
 */

#endif /* SYSTEM_CFG_H */

/*---------------------- End of File ----------------------------------------*/
