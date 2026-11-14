/***********************************************************************************************************************
 * Project Name: XHSC AUTOSAR MCAL
 * File Name: Eth_PBcfg.h
 *
 * Description: Implementation of Eth High Layer Configuration
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
 *   @file       Eth_PBcfg.h
 *   @version    1.0.3
 *   @brief      Eth High Layer Configuration
 *   @addtogroup eth_config
 *   @{
 */

#ifndef ETH_PBCFG_H
#define ETH_PBCFG_H

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
 *   @brief      Max Rx Fifo number for all controller
 */
#ifndef ETH_MAX_RX_FIFO_NUM
    #define ETH_MAX_RX_FIFO_NUM    (1U)
#elif (ETH_MAX_RX_FIFO_NUM < 1)
    #undef ETH_MAX_RX_FIFO_NUM
    #define ETH_MAX_RX_FIFO_NUM    (1U)
#endif
/**
 *   @brief      Max Tx Fifo number for all controller
 */
#ifndef ETH_MAX_TX_FIFO_NUM
    #define ETH_MAX_TX_FIFO_NUM    (1U)
#elif (ETH_MAX_TX_FIFO_NUM < 1)
    #undef ETH_MAX_TX_FIFO_NUM
    #define ETH_MAX_TX_FIFO_NUM    (1U)
#endif
/**
 *   @brief      Max Tx Buffer number for all controller
 */
#ifndef ETH_MAX_TX_BUFFER_NUM
    #define ETH_MAX_TX_BUFFER_NUM    (10U)
#elif (ETH_MAX_TX_BUFFER_NUM < 10)
    #undef ETH_MAX_TX_BUFFER_NUM
    #define ETH_MAX_TX_BUFFER_NUM    (10U)
#endif
/**
 *   @brief      Symbolic Name generated for Ingress FIFO index
 */
#ifndef EthConf_EthCtrlConfigIngressFifo_EthCtrlConfigIngressFifo_0
    #define EthConf_EthCtrlConfigIngressFifo_EthCtrlConfigIngressFifo_0    (0U)
#elif (EthConf_EthCtrlConfigIngressFifo_EthCtrlConfigIngressFifo_0 != 0)
    #error "Ingress FIFO index Symbolic name values should same for all variants"
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
/***********************************************************************************************************************
 *  GLOBAL FUNCTION
 **********************************************************************************************************************/

#ifdef __cplusplus
}
#endif

/**
 * @}
 */

#endif /*!< ETH_PBCFG_H */
