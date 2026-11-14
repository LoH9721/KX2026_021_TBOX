/***********************************************************************************************************************
 * Project Name: XHSC AUTOSAR MCAL
 * File Name: SchM_Fee.h
 *
 * Description: Implementation of SchM_Fee
 *              Compiler:                IAR GCC
 *              AutoSAR Version:         4.4.0
 *              TechnicalReference:       AUTOSAR_SWS_RTE.pdf
 *
 *
 * Copyright (c) 2022 - 2026 Xiaohua Semiconductor Co., Ltd.
 *
 * All rights reserved.
 *
 * This software is licensed by XHSC PLATFORM SOFTWARE LICENSE AGREEMENT (the "License"). By downloading, installing, or
 * using the Software, you indicate that you accept the terms of the License, and you acknowledge that you have the
 * authority, for yourself or on behalf of your company, to bind your company to these terms. If you do not agree to all
 * of the terms of this License, you should not download or install the Software.
 *
 *
 **********************************************************************************************************************/

/**
 *   @file       SchM_Fee.h
 *   @version    1.0.3
 *   @brief      Autosar 4.4.0 RTE Driver
 *   @addtogroup rte_driver
 *   @{
 */

#ifndef SCHM_FEE_H
#define SCHM_FEE_H

#ifdef __cplusplus
extern "C"{
#endif

/***********************************************************************************************************************
 *  INCLUDES
 **********************************************************************************************************************/

/***********************************************************************************************************************
 *  VERSION CHECK
 **********************************************************************************************************************/
#define SCHM_FEE_AR_RELEASE_MAJOR_VERSION     4
#define SCHM_FEE_AR_RELEASE_MINOR_VERSION     4
#define SCHM_FEE_AR_RELEASE_REVISION_VERSION  0

/***********************************************************************************************************************
 *  DEFINES & MACROS
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

/***********************************************************************************************************************
 *  GLOBAL FUNCTION
 **********************************************************************************************************************/
/**
 *  Exclusive Area
 */
extern void SchM_Enter_Fee_FEE_EXCLUSIVE_AREA_00(void);
extern void SchM_Exit_Fee_FEE_EXCLUSIVE_AREA_00(void);
extern void SchM_Enter_Fee_FEE_EXCLUSIVE_AREA_01(void);
extern void SchM_Exit_Fee_FEE_EXCLUSIVE_AREA_01(void);
extern void SchM_Enter_Fee_FEE_EXCLUSIVE_AREA_02(void);
extern void SchM_Exit_Fee_FEE_EXCLUSIVE_AREA_02(void);
extern void SchM_Enter_Fee_FEE_EXCLUSIVE_AREA_03(void);
extern void SchM_Exit_Fee_FEE_EXCLUSIVE_AREA_03(void);
extern void SchM_Enter_Fee_FEE_EXCLUSIVE_AREA_04(void);
extern void SchM_Exit_Fee_FEE_EXCLUSIVE_AREA_04(void);
extern void SchM_Enter_Fee_FEE_EXCLUSIVE_AREA_05(void);
extern void SchM_Exit_Fee_FEE_EXCLUSIVE_AREA_05(void);
extern void SchM_Enter_Fee_FEE_EXCLUSIVE_AREA_06(void);
extern void SchM_Exit_Fee_FEE_EXCLUSIVE_AREA_06(void);


extern void Fee_MainFunction(void);

#ifdef __cplusplus
}
#endif

/**
 * @}
 */

#endif /* SCHM_FEE_H */

