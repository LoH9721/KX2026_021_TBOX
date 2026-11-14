/***********************************************************************************************************************
 * Project Name: XHSC AUTOSAR MCAL
 * File Name: Eth_Ip_Cfg.c
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
 *   @file       Eth_Ip_Cfg.c
 *   @version    1.0.3
 *   @brief      ETH IP Layer Configuration
 *   @addtogroup eth_ip_config
 *   @{
 */

#ifdef __cplusplus
extern "C" {
#endif

/***********************************************************************************************************************
 *  INCLUDES
 **********************************************************************************************************************/
#include "Eth_Ip_Cfg.h"
#include "Eth_Ip_Types.h"

/***********************************************************************************************************************
 *  DEFINES & MACROS
 **********************************************************************************************************************/

/***********************************************************************************************************************
 *  EXTERN
 **********************************************************************************************************************/

/***********************************************************************************************************************
 *  LOCAL TYPEDEFS
 **********************************************************************************************************************/

/***********************************************************************************************************************
 *  LOCAL CONSTANTS
 **********************************************************************************************************************/

/***********************************************************************************************************************
 *  LOCAL VARIABLES
 **********************************************************************************************************************/

/***********************************************************************************************************************
 *  GLOBAL VARIABLES
 **********************************************************************************************************************/
#define ETH_START_SEC_VAR_CLEARED_UNSPECIFIED
#include "Eth_MemMap.h"
#include "Common.h"
/** @brief Status of ETH IP */
Eth_Ip_StateType Eth_Ip_g_Ctrl0Status_st;
/** @brief Record the Tx BD transmit buffer start address. */
uint8_t *Eth_Ip_g_Ctrl0TxBuffer_aa[ETH_IP_CTRL0_MAX_TX_RING_NUM][ETH_IP_CTRL0_MAX_TX_BUFFER_NUM];
/** @brief Reception buffer descriptors for Rx Ring */
ALIGNED_DATA(Eth_Ip_RxBufferDescriptorType Eth_Ip_g_Ctrl0RxDesc_aa[ETH_IP_CTRL0_MAX_RX_RING_NUM][ETH_IP_CTRL0_MAX_RX_BUFFER_NUM], 32U)
/** @brief Transmission buffer descriptors for Tx Ring */
ALIGNED_DATA(Eth_Ip_TxBufferDescriptorType Eth_Ip_g_Ctrl0TxDesc_aa[ETH_IP_CTRL0_MAX_TX_RING_NUM][ETH_IP_CTRL0_MAX_TX_BUFFER_NUM], 32U)
#define ETH_STOP_SEC_VAR_CLEARED_UNSPECIFIED
#include "Eth_MemMap.h"

#define ETH_START_SEC_VAR_CLEARED_8
#include "Eth_MemMap.h"
/** @brief Reception data buffers for all Rx Ring */
ALIGNED_DATA(uint8 Eth_Ip_g_Ctrl0RxBuffer_aa[ETH_IP_CTRL0_MAX_RX_RING_NUM][(ETH_IP_CTRL0_MAX_RX_BUFFER_NUM * ETH_IP_CTRL0_MAX_RX_BUFFER_LENGTH)], 32U)
#define ETH_STOP_SEC_VAR_CLEARED_8
#include "Eth_MemMap.h"
/***********************************************************************************************************************
 *  LOCAL FUNCTION PROTOTYPES
 **********************************************************************************************************************/

/***********************************************************************************************************************
 *  LOCAL FUNCTION
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
