/***********************************************************************************************************************
 * Project Name: XHSC AUTOSAR MCAL
 * File Name: Eth_Ip_PBcfg.c
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
 *   @file       Eth_Ip_PBcfg.c
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
#include "Eth_Ip_Types.h"
#include "Eth_Ip_Cfg.h"
/***********************************************************************************************************************
 *  DEFINES & MACROS
 **********************************************************************************************************************/

/***********************************************************************************************************************
 *  EXTERN
 **********************************************************************************************************************/

/***********************************************************************************************************************
 *  LOCAL TYPEDEFS
 **********************************************************************************************************************/
/* Channel callbacks declarations. */
/***********************************************************************************************************************
 *  LOCAL CONSTANTS
 **********************************************************************************************************************/
/***********************************************************************************************************************
 *  LOCAL VARIABLES
 **********************************************************************************************************************/
/* Import IP variables */
ETH_IP_VARIABLE_EXT

#define ETH_START_SEC_CONFIG_DATA_UNSPECIFIED
#include "Eth_MemMap.h"
/* The configuration of the ETH_IP MAC part. */
static const Eth_Ip_MacConfigType s_EthIpCtrl0MacConfig_st =
{
    .DisWatchdog_bool        = FALSE,
    .DisJabber_bool          = FALSE,
    .InterPacketGap_u16      = 96U,
    .EnLoopbackMode_bool     = FALSE,
    .EnFullDuplexMode_bool   = TRUE,
    .EnChecksumOffload_bool  = FALSE,
    .DisBroadcastPacket_bool = FALSE
};

/* The MAC address of the configured controller(s) */
static const uint8 s_EthIpCtrl0PhysAddr_aa[ETH_IP_MAC_ADDR_SIZE] = { 0x62U, 0x66U, 0x64U, 0x61U, 0x10U, 0x02U};

/* The configuration of the Rx channel (DMA + MTL). */
static const Eth_Ip_RxChannelConfigType s_EthIpCtrl0RxChannelConfig_st =
{
    .RxDescAddr_ptr                 = &Eth_Ip_g_Ctrl0RxDesc_aa[0U][0U],
    .EnRxInterrupt_bool             = FALSE,
    .DescNum_16                     = 10U,
    .Buffer_ptr                     = &Eth_Ip_g_Ctrl0RxBuffer_aa[0U][0U],
    .BuffLen_u16                    = 1552U,
    .EnStoreForward_bool            = TRUE,
    .ThresholdLevel_en              = ETH_IP_RX_THRESHOLD_64_BYTES,
    .EnFwUndersizeGoodPackets_bool  = TRUE,
    .DisDropChecksumErrPackets_bool = TRUE,
    .Callback_pfn                   = NULL_PTR
};

/* The configuration of the Tx Channel (DMA + MTL). */
static const Eth_Ip_TxChannelConfigType s_EthIpCtrl0TxChannelConfig_st =
{
    .TxDescAddr_ptr      = &Eth_Ip_g_Ctrl0TxDesc_aa[0U][0U],
    .TxBuffer_pp         = &Eth_Ip_g_Ctrl0TxBuffer_aa[0U][0U],
    .EnTxInterrupt_bool  = FALSE,
    .DescNum_16          = 10U,
    .EnStoreForward_bool = TRUE,
    .ThresholdLevel_en   = ETH_IP_TX_THRESHOLD_32_BYTES,
    .Callback_pfn        = NULL_PTR
};

/* The normal configuration of the ETH_IP 0. */
static const Eth_Ip_NormalConfigType s_EthIpCtrl0NormalConfig_st =
{
    .Interface_en   = ETH_IP_INTERFACE_RMII,
    .Speed_en       = ETH_IP_SPEED_100M,
    .CsrClockDiv_en = ETH_IP_CSR_CLOCK_DIV_100_120M,
    .PhysAddr_ptr   = &s_EthIpCtrl0PhysAddr_aa[0U],
    .MacCfg_ptr     = &s_EthIpCtrl0MacConfig_st
};
/***********************************************************************************************************************
 *  GLOBAL VARIABLES
 **********************************************************************************************************************/
/* The configuration of Eth controller */
const Eth_Ip_CtrlConfigType Eth_Ip_g_CtrlConfig_aa[1] =
{
    /* The configuration structure for Eth_Ip_g_CtrlConfig_aa[0U] */
    {
        .NormalCfg_ptr     = &s_EthIpCtrl0NormalConfig_st,
        .RxChannelCfg_ptr  = &s_EthIpCtrl0RxChannelConfig_st,
        .TxChannelCfg_ptr  = &s_EthIpCtrl0TxChannelConfig_st,
        .PtpCfg_ptr        = NULL_PTR,
        .VlanCfg_ptr       = NULL_PTR,
        .VlanFilterCfg_ptr = NULL_PTR,
        .L3L4FilterCfg_ptr = NULL_PTR,
        .CtrlState_ptr     = &Eth_Ip_g_Ctrl0Status_st
    }
};

#define ETH_STOP_SEC_CONFIG_DATA_UNSPECIFIED
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
