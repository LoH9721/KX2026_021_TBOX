/***********************************************************************************************************************
 * Project Name: XHSC AUTOSAR MCAL
 * File Name: Eth_Ip_PBcfg.h
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
 *   @file       Eth_Ip_PBcfg.h
 *   @version    1.0.3
 *   @brief      ETH IP Layer Configuration
 *   @addtogroup eth_ip_config
 *   @{
 */

#ifndef ETH_IP_PBCFG_H
#define ETH_IP_PBCFG_H

#ifdef __cplusplus
extern "C" {
#endif

/***********************************************************************************************************************
 *  INCLUDES
 **********************************************************************************************************************/

/***********************************************************************************************************************
 *  DEFINES & MACROS
 **********************************************************************************************************************/
/**
 *   @brief      Max Rx Ring number
 */
#ifndef ETH_IP_CTRL0_MAX_RX_RING_NUM
    #define ETH_IP_CTRL0_MAX_RX_RING_NUM    (1U)
#elif (ETH_IP_CTRL0_MAX_RX_RING_NUM < 1)
    #undef ETH_IP_CTRL0_MAX_RX_RING_NUM
    #define ETH_IP_CTRL0_MAX_RX_RING_NUM    (1U)
#endif
/**
 *   @brief      Max Tx Ring number
 */
#ifndef ETH_IP_CTRL0_MAX_TX_RING_NUM
    #define ETH_IP_CTRL0_MAX_TX_RING_NUM    (1U)
#elif (ETH_IP_CTRL0_MAX_TX_RING_NUM < 1)
    #undef ETH_IP_CTRL0_MAX_TX_RING_NUM
    #define ETH_IP_CTRL0_MAX_TX_RING_NUM    (1U)
#endif
/**
 *   @brief      Max single Rx Rings buffer length
 */
#ifndef ETH_IP_CTRL0_MAX_RX_BUFFER_LENGTH
    #define ETH_IP_CTRL0_MAX_RX_BUFFER_LENGTH    (1552U)
#elif (ETH_IP_CTRL0_MAX_RX_BUFFER_LENGTH < 1552)
    #undef ETH_IP_CTRL0_MAX_RX_BUFFER_LENGTH
    #define ETH_IP_CTRL0_MAX_RX_BUFFER_LENGTH    (1552U)
#endif
/**
 *   @brief      Max Rx Rings descriptors number
 */
#ifndef ETH_IP_CTRL0_MAX_RX_BUFFER_NUM
    #define ETH_IP_CTRL0_MAX_RX_BUFFER_NUM    (10U)
#elif (ETH_IP_CTRL0_MAX_RX_BUFFER_NUM < 10)
    #undef ETH_IP_CTRL0_MAX_RX_BUFFER_NUM
    #define ETH_IP_CTRL0_MAX_RX_BUFFER_NUM    (10U)
#endif
/**
 *   @brief      Max Tx Rings descriptors number
 */
#ifndef ETH_IP_CTRL0_MAX_TX_BUFFER_NUM
    #define ETH_IP_CTRL0_MAX_TX_BUFFER_NUM    (10U)
#elif (ETH_IP_CTRL0_MAX_TX_BUFFER_NUM < 10)
    #undef ETH_IP_CTRL0_MAX_TX_BUFFER_NUM
    #define ETH_IP_CTRL0_MAX_TX_BUFFER_NUM    (10U)
#endif
/***********************************************************************************************************************
 *  TYPEDEFS
 **********************************************************************************************************************/

/***********************************************************************************************************************
 *  CONSTANTS
 **********************************************************************************************************************/

/***********************************************************************************************************************
 *  GLOBAL VARIABLES
 **********************************************************************************************************************/
#define ETH_IP_CONFIG_PB \
    extern const Eth_Ip_CtrlConfigType Eth_Ip_g_CtrlConfig_aa[];

#ifdef __cplusplus
}
#endif
/***********************************************************************************************************************
 *  GLOBAL FUNCTION
 **********************************************************************************************************************/

/**
 * @}
 */

#endif /*!< ETH_IP_PBCFG_H */
