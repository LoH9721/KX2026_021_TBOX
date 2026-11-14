/***********************************************************************************************************************
 * Project Name: XHSC AUTOSAR MCAL
 * File Name: Mcu_Cfg.h
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
 *   @file       Mcu_Cfg.h
 *   @version    V1.0.3
 *   @brief      Autosar 4.4.0 MCU Driver
 *   @addtogroup mcu_driver
 *   @{
 */

#ifndef MCU_CFG_H
#define MCU_CFG_H

#ifdef __cplusplus
extern "C"{
#endif

/***********************************************************************************************************************
 *  Includes
 **********************************************************************************************************************/
#include "Clock_Ip_Cfg.h"
#include "Power_Ip_Cfg.h"
#include "Ram_Ip_Cfg.h"
#include "Mcu_PBcfg.h"

/***********************************************************************************************************************
 *                              VERSION INFORMATION
 **********************************************************************************************************************/
#define MCU_CFG_SW_MAJOR_VERSION               1U
#define MCU_CFG_SW_MINOR_VERSION               0U
#define MCU_CFG_SW_PATCH_VERSION               3U


/***********************************************************************************************************************
 *  Define & Macros
 **********************************************************************************************************************/

#define MCU_CONFIG_DECLARE \
  MCU_PARAM_CONFIG_PB



/**
 * @brief             Pre-compile Support.
 */
#define MCU_PRECOMPILE_ENABLE                          (STD_OFF)

/**
 * @brief            Pre-processor switch to enable/disable the API Mcu_GetRamState.
 */
#define MCU_ENABLE_GET_RAM_STATE_API            (STD_ON)

/**
 * @brief            If this parameter is FALSE, disable clock initialization in the MCU driver.
 */
#define MCU_ENABLE_INIT_CLOCK                   (STD_ON)

/**
 * @brief            This parameter shall be set True, if the H/W does not have a PLL or the PLL circuitry is enabled
 *                   after the power on without S/W intervention.
 */
#define MCU_SUPPORT_PLL                         (STD_ON)

/**
 * @brief            Pre-processor switch to enable/disable the API Mcu_PerformReset.
 */
#define MCU_ENABLE_PERFORM_RESET_API            (STD_ON)

/**
 * @brief            Pre-processor switch for enabling the default error detection and reporting to the DET.
 *                   The detection of default errors is configurable (ON / OFF) at pre-compile time.
 */
#define MCU_ENABLE_DEV_ERROR_DETECT             (STD_OFF)

/**
 * @brief            Pre-processor switch to enable/disable the API to read out the modules version information.
 */
#define MCU_ENABLE_VERSION_INFO_API             (STD_ON)

/**
 * @brief            Enable/Disable external reset
 */
#define MCU_EXTERNAL_RESET_ENABLE               (STD_OFF)

/**
 * @brief            Enable/Disable the API for reporting the Dem Error.
 */
#define MCU_ENABLE_DEM_REPORT_ERROR_STATUS      (STD_OFF)

/**
 * @brief            Enable Clock Failure DEM error
 */
#define MCU_CLOCK_SOURCE_FAILURE_NOTIFICATION   (STD_OFF)

/**
 * @brief            Enable/Disable clock failure notification
 */
#define MCU_CLOCK_FAIL_PB_CONFIGURATION         (STD_OFF)

/**
 * @brief            Enable/Disable Lookup Reset
 */
#define MCU_LOOKUP_RESET_ENABLE                 (STD_OFF)

/**
 * @brief            Number of clock configured.
 */
#define MCU_NUM_OF_CLOCK_CONFIGURED             (8U)

/**
 *  @brief          Pre-processor switch for enabling or disabling the API Mcu_PeripheralsPerformReset.
 */
#define MCU_SUPPORT_PERFORM_SOFTWARE_MODELE_RESET_API  (STD_OFF)

#if (STD_ON == MCU_ENABLE_PERFORM_RESET_API)
#define MCU_PROC_RST                            (0x00U)
#endif

/* Mcu_InitClock API Argument macros */
#define McuConf_McuClockSettingConf_McuClockSettingConfig_0  ((Mcu_ClockType)0U)

/* McuRamSectorSettingConf RAM sectors */
#define McuConf_McuRamSectorSettingConf_McuRamSectorSettingConf_0  ((Mcu_RamSectionType)0U)

/* MCU Mode macros */
#define McuConf_McuModeSettingConf_Sleep  ((Mcu_ModeType)0U)
#define McuConf_McuModeSettingConf_Stop  ((Mcu_ModeType)1U)
#define McuConf_McuModeSettingConf_Power_down  ((Mcu_ModeType)2U)
#define McuConf_McuModeSettingConf_McuModeSettingConf_0  ((Mcu_ModeType)3U)

/* MCU Reset Reason macros */
#define McuConf_McuResetReasonConf_MCU_PO_RESET  ((uint32)0U)
#define McuConf_McuResetReasonConf_MCU_PIN_RESET  ((uint32)1U)
#define McuConf_McuResetReasonConf_MCU_BO_RESET  ((uint32)2U)
#define McuConf_McuResetReasonConf_MCU_PVD1_RESET  ((uint32)3U)
#define McuConf_McuResetReasonConf_MCU_PVD2_RESET  ((uint32)4U)
#define McuConf_McuResetReasonConf_MCU_WD_RESET  ((uint32)5U)
#define McuConf_McuResetReasonConf_MCU_SWD_RESET  ((uint32)6U)
#define McuConf_McuResetReasonConf_MCU_PD_RESET  ((uint32)7U)
#define McuConf_McuResetReasonConf_MCU_SW_RESET  ((uint32)8U)
#define McuConf_McuResetReasonConf_MCU_MPUE_RESET  ((uint32)9U)
#define McuConf_McuResetReasonConf_MCU_RAPE_RESET  ((uint32)10U)
#define McuConf_McuResetReasonConf_MCU_RAEC_RESET  ((uint32)11U)
#define McuConf_McuResetReasonConf_MCU_CKFE_RESET  ((uint32)12U)
#define McuConf_McuResetReasonConf_MCU_XTALE_RESET  ((uint32)13U)
#define McuConf_McuResetReasonConf_MCU_LKUP_RESET  ((uint32)14U)
#define McuConf_McuResetReasonConf_MCU_FLEC_RESET  ((uint32)15U)
#define McuConf_McuResetReasonConf_MCU_ERMU_RESET  ((uint32)16U)
#define McuConf_McuResetReasonConf_MCU_MULTIPLE_RESET  ((uint32)17U)
#define McuConf_McuResetReasonConf_MCU_RESET_UNDEFINED  ((uint32)18U)


/***********************************************************************************************************************
 *  TYPEDEFS
 **********************************************************************************************************************/

/***********************************************************************************************************************
 *  ENUMS
 **********************************************************************************************************************/

/***********************************************************************************************************************
 *  GLOBAL VARIABLES
 **********************************************************************************************************************/

/***********************************************************************************************************************
 *  FUNCTION PROTOTYPES
 **********************************************************************************************************************/
#ifdef __cplusplus
}
#endif

/**
 * @}
 */

#endif /* MCU_CFG_H */

/*---------------------- End of File ----------------------------------------*/
