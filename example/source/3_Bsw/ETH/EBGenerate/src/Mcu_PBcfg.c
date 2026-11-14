/***********************************************************************************************************************
 * Project Name: XHSC AUTOSAR MCAL
 * File Name: Mcu_PBcfg.c
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
 *   @file       Mcu_PBcfg.c
 *   @version    V1.0.3
 *   @brief      Autosar 4.4.0 MCU Driver
 *   @addtogroup mcu_driver
 *   @{
 */

#ifdef __cplusplus
extern "C"{
#endif

/***********************************************************************************************************************
 *  INCLUDES
 **********************************************************************************************************************/
#include "Mcu.h"

/***********************************************************************************************************************
 *   VERSION CHECK
 **********************************************************************************************************************/

/***********************************************************************************************************************
 *  DEFINES & MACROS
 **********************************************************************************************************************/

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
 *  GLOBAL DATA TYPES
 **********************************************************************************************************************/
#define MCU_START_SEC_CONFIG_DATA_UNSPECIFIED
#include "Mcu_MemMap.h"



/**
 * @brief          Initialization data for the MCU driver.
 * @details        A pointer to such a structure is provided to the MCU initialization routines for configuration.
 *
 */

const Mcu_ConfigType Mcu_Config = {
#if (STD_ON == MCU_CLOCK_FAIL_PB_CONFIGURATION)
    .McuClockSrcFailureNotification_bool = (FALSE),
#endif

#if (MCU_ENABLE_INIT_CLOCK == STD_ON)
    .McuNumberOfClockConfig_u8 = (Mcu_ClockType)1U,
    .McuClockConfigSet_ptr = &g_McuClockConfigPB_aa,
#endif /* (MCU_ENABLE_INIT_CLOCK == STD_ON) */

    .McuNumberOfRamConfigs_u32 = 1U,
#ifndef MCU_MAX_NORAMCONFIGS
    .McuRamSectorConfigList_ptr = &g_McuRamConfigPB_aa,
#endif

    .McuNumberOfMcuModes_u8 = 4U,
    .McuModeConfig_Ptr = &g_McuModeConfigPB_aa,
    .Mcu_HwIPsConfig_Ptr = &g_McuHwIPsConfigPB_st
};


#define MCU_STOP_SEC_CONFIG_DATA_UNSPECIFIED
#include "Mcu_MemMap.h"

#ifdef __cplusplus
}
#endif

/**
 * @}
 */

/*---------------------- End of File ----------------------------------------*/
