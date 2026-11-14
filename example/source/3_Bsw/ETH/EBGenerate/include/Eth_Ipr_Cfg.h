/***********************************************************************************************************************
 * Project Name: XHSC AUTOSAR MCAL
 * File Name: Eth_Ipr_Cfg.h
 *
 * Description: Implementation of Eth IP Router Layer Configuration
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
 *   @file       Eth_Ipr_Cfg.h
 *   @version    1.0.3
 *   @brief      Eth IP Router Layer Configuration
 *   @addtogroup eth_ipr_config
 *   @{
 */

#ifndef ETH_IPR_CFG_H
#define ETH_IPR_CFG_H

#ifdef __cplusplus
extern "C" {
#endif

/***********************************************************************************************************************
 *  INCLUDES
 **********************************************************************************************************************/
#include "Eth_Ipr_PBcfg.h"

/***********************************************************************************************************************
 *  DEFINES & MACROS
 **********************************************************************************************************************/
#define ETH_IPR_VARIABLE_EXT \
extern ALIGNED_DATA(uint8 Eth_Ipr_g_Ctrl0TxBuffer_aa[][(ETH_IPR_CTRL0_MAX_TX_BUFFER_NUM * ETH_IPR_CTRL0_MAX_TX_BUFFER_LENGTH)], 32U)

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

#ifdef __cplusplus
}
#endif

/**
 * @}
 */

#endif /*!< ETH_IPR_PBCFG_H */
