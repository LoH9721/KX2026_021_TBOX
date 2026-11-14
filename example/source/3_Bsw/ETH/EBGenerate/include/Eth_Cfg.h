/***********************************************************************************************************************
 * Project Name: XHSC AUTOSAR MCAL
 * File Name: Eth_Cfg.h
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
 *   @file       Eth_Cfg.h
 *   @version    1.0.3
 *   @brief      Eth High Layer Configuration
 *   @addtogroup eth_config
 *   @{
 */

#ifndef ETH_CFG_H
#define ETH_CFG_H

#ifdef __cplusplus
extern "C" {
#endif

/***********************************************************************************************************************
 *  INCLUDES
 **********************************************************************************************************************/
#include "Eth_PBcfg.h"

/***********************************************************************************************************************
 *  DEFINES & MACROS
 **********************************************************************************************************************/
/**
 *   @brief      File version information.
 */
#define ETH_CFG_SW_MAJOR_VERSION        1UL
#define ETH_CFG_SW_MINOR_VERSION        0UL
#define ETH_CFG_SW_PATCH_VERSION        3UL

/**
 *   @brief      Enable/Disable Precompile Support.
 */
#define ETH_PRECOMPILE_CFG_ENABLED    (STD_ON)

/**
 *   @brief      MAX Number Of Eth Controller.
 */
#define ETH_MAX_CONTROLLER_COUNT    (1U)

/**
 *   @brief      The definition represent for number of ECUC partition configured.
 */

#define ETH_MAX_PARTITIONS    (1U)

/**
 *   @brief      The definition used for guarding Multicore
 */
#define ETH_MULTICORE_ENABLED    (STD_OFF)

/**
 *   @brief      Enable/Disable Development Error Detection and Notification.
 */
#define ETH_DEV_ERROR_DETECT    (STD_OFF)

/**
 * @brief        Production error detection enable/disable.
 */
#define ETH_DEM_EVENT_ENABLE    (STD_OFF)

/**
 *   @brief      Enable/Disable Eth_GetCounterValues API.
 */
#define ETH_GET_COUNTER_VALUES_API    (STD_ON)

/**
 *   @brief      Enable/Disable Eth_GetTxErrorCounterValues API.
 */
#define ETH_GET_TX_ERROR_COUNTER_VALUES_API    (STD_ON)

/**
 *   @brief      Enable/Disable Eth_GetRxStats API.
 */
#define ETH_GET_RECEIVED_STATS_API    (STD_ON)

/**
 *   @brief      Enable/Disable Eth_GetTxStats API.
 */
#define ETH_GET_TRANSMIT_STATS_API    (STD_ON)

/**
 *   @brief      Enable/Disable the GlobalTime APIs used amongst others by Global Time Synchronization over Ethernet.
 */
#define ETH_GLOBAL_TIME_SUPPORT_API    (STD_OFF)

/**
 *   @brief      Enable/Disable Eth_GetVersionInfo API.
 */
#define ETH_GET_VERSION_INFO_API    (STD_ON)

/**
 *   @brief      Instance index of the Eth module.
 */
#define ETH_INSTANCE_ID    (0U)

/**
 *   @brief      Enables / Disables hardware offloading for IP checksums
 */
#define ETH_ENABLE_L3_OFFLOAD    (STD_OFF)

/**
 *   @brief      Enables / Disables hardware offloading for TCP/UDP/ICMP checksums
 */

#define ETH_ENABLE_L4_OFFLOAD    (STD_OFF)

/**
 *   @brief      Symbolic Name generated for Eth controller
 */

#define EthConf_EthCtrlConfig_EthCtrlConfig_0    ((uint8)0U)


/**
 *   @brief      Enables / Disables Media Independent Interface (MII) for transceiver access.
 */
#define ETH_ENABLE_MII    (STD_ON)

/**
 *   @brief      Enables / Disables receive interrupt.
 */
#define ETH_RECEIVED_IRQ_ENABLED    (STD_OFF)

/**
 *   @brief      Enables / Disables transmit interrupt.
 */
#define ETH_TRANSMIT_IRQ_ENABLED    (STD_OFF)

/**
 *   @brief      Definition which contain all Eth driver Config
 */
#define ETH_USE_EXT_CFG

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
/*! @}*/

#endif /*!< ETH_CFG_H */
