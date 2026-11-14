/***********************************************************************************************************************
 * Project Name: XHSC AUTOSAR MCAL
 * File Name: Eth_Ipr_PBcfg.h
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
 *   @file       Eth_Ipr_PBcfg.h
 *   @version    1.0.3
 *   @brief      Eth IP Router Layer Configuration
 *   @addtogroup eth_ipr_config
 *   @{
 */

#ifndef ETH_IPR_PBCFG_H
#define ETH_IPR_PBCFG_H

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
 *   @brief      Single controller Max Tx FIFO number
 */
#ifndef ETH_IPR_CTRL0_MAX_TX_FIFO_NUM
    #define ETH_IPR_CTRL0_MAX_TX_FIFO_NUM    (1U)
#elif (ETH_IPR_CTRL0_MAX_TX_FIFO_NUM < 1)
    #undef ETH_IPR_CTRL0_MAX_TX_FIFO_NUM
    #define ETH_IPR_CTRL0_MAX_TX_FIFO_NUM    (1U)
#endif
/**
 *   @brief      Single controller Max Tx buffer number
 */
#ifndef ETH_IPR_CTRL0_MAX_TX_BUFFER_NUM
    #define ETH_IPR_CTRL0_MAX_TX_BUFFER_NUM    (10U)
#elif (ETH_IPR_CTRL0_MAX_TX_BUFFER_NUM < 10)
    #undef ETH_IPR_CTRL0_MAX_TX_BUFFER_NUM
    #define ETH_IPR_CTRL0_MAX_TX_BUFFER_NUM    (10U)
#endif
/**
 *   @brief      Single controller MAX Tx Buffer length 
 */
#ifndef ETH_IPR_CTRL0_MAX_TX_BUFFER_LENGTH
    #define ETH_IPR_CTRL0_MAX_TX_BUFFER_LENGTH    (1552U)
#elif (ETH_IPR_CTRL0_MAX_TX_BUFFER_LENGTH < 1552)
    #undef ETH_IPR_CTRL0_MAX_TX_BUFFER_LENGTH
    #define ETH_IPR_CTRL0_MAX_TX_BUFFER_LENGTH    (1552U)
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

#define ETH_IPR_USE_EXT_CFG_PB    extern const Eth_Ipr_ConfigType Eth_Ipr_g_CtrlConfig_aa[];
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
