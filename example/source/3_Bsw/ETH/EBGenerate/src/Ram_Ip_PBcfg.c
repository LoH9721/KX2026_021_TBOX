/***********************************************************************************************************************
 * Project Name: XHSC AUTOSAR MCAL
 * File Name: Ram_Ip_PBcfg.c
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
 *   @file       Ram_Ip_PBcfg.c
 *   @version    V1.0.3
 *   @brief      Autosar 4.4.0 MCU Driver
 *   @addtogroup mcu_driver
 *   @{
 */

#ifdef __cplusplus
extern "C"{
#endif

#include "StandardTypes.h"
#include "Ram_Ip_Cfg.h"

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




/* Mcu_RamSectorConfig */
const Ram_Ip_HwRamConfigType g_McuRamConfigPB_aa[MCU_NUMBER_OF_RAMCONFIGS] = {
  {
      .McuRamSectorId_u32 = (uint32)0U,
      .McuRamSectionBase_ptr = (uint8 *)0x200f0000U,
      .McuRamSectionSize_u32 = 0x00000400U,
      .McuRamDefaultValue_u64 = (uint64)( ((uint64)0x00U << 0U) \
                                             | ((uint64)0x00U << 8U) \
                                             | ((uint64)0x00U << 16U) \
                                             | ((uint64)0x00U << 24U) \
                                             | ((uint64)0x00U << 32U) \
                                             | ((uint64)0x00U << 40U) \
                                             | ((uint64)0x00U << 48U) \
                                             | ((uint64)0x00U << 56U) \
                                             ),
      .McuRamWriteSize_u32 = 8U
  }
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
