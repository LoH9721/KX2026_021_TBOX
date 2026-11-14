/***********************************************************************************************************************
 * Project Name: XHSC AUTOSAR MCAL
 * File Name: Eth_Ip_Cfg.h
 *
 * Description: Implementation of ETH IP Layer Configuration
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
 *   @file       Eth_Ip_Cfg.h
 *   @version    1.0.3
 *   @brief      ETH IP Layer Configuration
 *   @addtogroup eth_ip_config
 *   @{
 */

#ifndef ETH_IP_CFG_H
#define ETH_IP_CFG_H

#ifdef __cplusplus
extern "C" {
#endif

/***********************************************************************************************************************
 *  INCLUDES
 **********************************************************************************************************************/
#include "Eth_Ip_PBcfg.h"
#include "Common.h"
/***********************************************************************************************************************
 *  DEFINES & MACROS
 **********************************************************************************************************************/
/**
 * @brief ETH module features
 */
/**
 *   @brief      Enables / Disables enhanced Tx/Rx descriptor
 */

#define ETH_IP_ENABLE_ENHANCED_DESC    (STD_OFF)

/***********************************************************************************************************************
 *  TYPEDEFS
 **********************************************************************************************************************/

/***********************************************************************************************************************
 *  CONSTANTS
 **********************************************************************************************************************/

/***********************************************************************************************************************
 *  GLOBAL VARIABLES
 **********************************************************************************************************************/
#define ETH_IP_VARIABLE_EXT \
    extern Eth_Ip_StateType Eth_Ip_g_Ctrl0Status_st; \
extern uint8_t *Eth_Ip_g_Ctrl0TxBuffer_aa[ETH_IP_CTRL0_MAX_TX_RING_NUM][ETH_IP_CTRL0_MAX_TX_BUFFER_NUM]; \
extern ALIGNED_DATA(Eth_Ip_RxBufferDescriptorType Eth_Ip_g_Ctrl0RxDesc_aa[][ETH_IP_CTRL0_MAX_RX_BUFFER_NUM], 32U) \
extern ALIGNED_DATA(Eth_Ip_TxBufferDescriptorType Eth_Ip_g_Ctrl0TxDesc_aa[][ETH_IP_CTRL0_MAX_TX_BUFFER_NUM], 32U) \
extern ALIGNED_DATA(uint8 Eth_Ip_g_Ctrl0RxBuffer_aa[][(ETH_IP_CTRL0_MAX_RX_BUFFER_NUM * ETH_IP_CTRL0_MAX_RX_BUFFER_LENGTH)], 32U)

#define ETH_IP_TIMEOUT_METHOD_SUPPORT    (OSR_COUNTER_DUMMY)
#define ETH_IP_LOOP_TIMEOUT_LENGTH    (50000U)
/***********************************************************************************************************************
 *  GLOBAL FUNCTION
 **********************************************************************************************************************/

#ifdef __cplusplus
}
#endif
/*! @}*/

#endif /*!< ETH_IP_CFG_H */
