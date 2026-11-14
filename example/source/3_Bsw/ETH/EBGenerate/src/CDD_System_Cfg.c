/***********************************************************************************************************************
 * Project Name: XHSC AUTOSAR MCAL
 * File Name: CDD_System_Cfg.c
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
 *   @file       CDD_System_Cfg.c
 *   @version    V1.0.3
 *   @brief      Autosar 4.4.0 SYSTEM Driver
 *   @addtogroup system_driver
 *   @{
 */

#ifdef __cplusplus
extern "C"{
#endif
#include "CDD_System_Cfg.h"
#include "System_Types.h"

/***********************************************************************************************************************
 *   SOURCE FILE VERSION INFORMATION
 **********************************************************************************************************************/

/***********************************************************************************************************************
 * VERSION CHECKS
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

#define SYSTEM_START_SEC_CONFIG_DATA_UNSPECIFIED
#include "System_MemMap.h"

/* INTC Configuration Set */
  /* INTC Config Set 0 */
static const System_IntConfigType  l_SystemIntListConfigSet0IntcConfig_aa[1] = 
{
  {
    .SystemIntReqSource_en = INTC_SRC_ETH_GLB_INT,
    .SystemIntIrqNumber_en = INT016_IRQn,
    .SystemIntPriority_en = PRIORITY_0,
    .SystemIntIrqHandler_dd = ETH_GLB_INT_IRQHandler
  } 
};

static const System_IntConfigSetType  l_SystemINTCListConfigSet_aa[1] = 
{
  {
    /* Index of INTC ConfigSet */
    .SystemIntConfigSetIdx_u32 = ((uint32)0U),
    /* Total number of interrupt request vector configured */
    .SystemIntNumberOfIntcConfig_u8 = 1,
    /* List of Trigger target */
    .SystemIntListConfig_ptr = l_SystemIntListConfigSet0IntcConfig_aa
  } 
};

#define SYSTEM_STOP_SEC_CONFIG_DATA_UNSPECIFIED
#include "System_MemMap.h"


#define SYSTEM_START_SEC_CONFIG_DATA_UNSPECIFIED
#include "System_MemMap.h"
/**
 * @SWS     ECUC_System_00100, ECUC_System_00302
 * @Design
 */
const System_ConfigType  System_Config = {
  /*Number of INTC Config Set*/
  .SystemIntNumberOfIntcConfigSet_u32 = ((uint32)1U),
  /*List of Config Set*/
  .SystemIntConfigSet_ptr = l_SystemINTCListConfigSet_aa,
  /*AOS has not been configured */
  .SystemAosNumberOfAosSetConfig_u32 = 0U,
  .SystemAosConfigSet_ptr = NULL_PTR
};

#define SYSTEM_STOP_SEC_CONFIG_DATA_UNSPECIFIED
#include "System_MemMap.h"

/* ======================Value for ICG- Initial Configuration=================== */

/* System - ICG0 */
#if defined (__GNUC__)
const uint32 l_SystemICG0Value_u32 __attribute__((section(".icg0_sec"))) =
#elif defined (__ICCARM__)
#pragma location = SYSTEM_ICG0_START_ADDR
const uint32 l_SystemICG0Value_u32 =
#endif
SYSTEM_ICG0_NOT_CONFIGURED;

/* System - ICG1 */
#if defined (__GNUC__)
const uint32 l_SystemICG1Value_u32 __attribute__((section(".icg1_sec"))) =
#elif defined (__ICCARM__)
#pragma location = SYSTEM_ICG1_START_ADDR
const uint32 l_SystemICG1Value_u32 =
#endif
  SYSTEM_ICG1_NOT_CONFIGURED;

/* System - ICG3 */
#if defined (__GNUC__)
const uint32 l_SystemICG3Value_u32 __attribute__((section(".icg3_sec"))) =
#elif defined (__ICCARM__)
#pragma location = SYSTEM_ICG3_START_ADDR
const uint32 l_SystemICG3Value_u32 =
#endif
  SYSTEM_ICG3_NOT_CONFIGURED;

/* System - ICG8x */
#if defined (__GNUC__)
const uint32 l_SystemICG8AValue_u32 __attribute__((section(".icg8a_sec"))) =
#elif defined (__ICCARM__)
#pragma location = SYSTEM_ICG8A_START_ADDR
const uint32 l_SystemICG8AValue_u32 =
#endif
  SYSTEM_ICG8X_NOT_CONFIGURED;

#if defined (__GNUC__)
const uint32 l_SystemICG8BValue_u32 __attribute__((section(".icg8b_sec"))) =
#elif defined (__ICCARM__)
#pragma location = SYSTEM_ICG8B_START_ADDR
const uint32 l_SystemICG8BValue_u32 =
#endif
  SYSTEM_ICG8X_NOT_CONFIGURED;

#if defined (__GNUC__)
const uint32 l_SystemICG8CValue_u32 __attribute__((section(".icg8c_sec"))) =
#elif defined (__ICCARM__)
#pragma location = SYSTEM_ICG8C_START_ADDR
const uint32 l_SystemICG8CValue_u32 =
#endif
  SYSTEM_ICG8X_NOT_CONFIGURED;


#if defined (__GNUC__)
const uint32 l_SystemICGReserve11Value_u32 __attribute__((section(".icgReserval1_sec"))) = 0xFFFFFFFFUL;
const uint32 l_SystemICGReserve12Value_u32[4] __attribute__((section(".icgReserval2_sec"))) = {0xFFFFFFFFUL, 
                                                                                               0xFFFFFFFFUL, 
                                                                                               0xFFFFFFFFUL, 
                                                                                               0xFFFFFFFFUL
                                                                                              };                                                                                       
const uint32 l_SystemICGReserve13Value_u32[10] __attribute__((section(".icgReserval3_sec"))) = {0xFFFFFFFFUL, 
                                                                                                0xFFFFFFFFUL, 
                                                                                                0xFFFFFFFFUL, 
                                                                                                0xFFFFFFFFUL,
                                                                                                0xFFFFFFFFUL, 
                                                                                                0xFFFFFFFFUL, 
                                                                                                0xFFFFFFFFUL,
                                                                                                0xFFFFFFFFUL, 
                                                                                                0xFFFFFFFFUL, 
                                                                                                0xFFFFFFFFUL                                                                                                                                                                                                                                                                                                                                                           
                                                                                               };
#elif defined (__ICCARM__)
#pragma location = 0x408
const uint32 l_SystemICGReserve11Value_u32  = 0xFFFFFFFFUL;

#pragma location = 0x410
const uint32 l_SystemICGReserve12Value_u32[4]  = {0xFFFFFFFFUL, 
                                                  0xFFFFFFFFUL, 
                                                  0xFFFFFFFFUL, 
                                                  0xFFFFFFFFUL
                                                 };

#pragma location = 0x438
const uint32 l_SystemICGReserve13Value_u32[10]  = {0xFFFFFFFFUL, 
                                                   0xFFFFFFFFUL, 
                                                   0xFFFFFFFFUL,
                                                   0xFFFFFFFFUL, 
                                                   0xFFFFFFFFUL, 
                                                   0xFFFFFFFFUL,
                                                   0xFFFFFFFFUL, 
                                                   0xFFFFFFFFUL, 
                                                   0xFFFFFFFFUL,
                                                   0xFFFFFFFFUL                                                      
                                                 };
#endif

#ifdef __cplusplus
}
#endif

/**
 * @}
 */

/*---------------------- End of File ----------------------------------------*/
