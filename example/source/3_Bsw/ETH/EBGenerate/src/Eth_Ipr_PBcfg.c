/***********************************************************************************************************************
 * Project Name: XHSC AUTOSAR MCAL
 * File Name: Eth_Ipr_PBcfg.c
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
 *   @file       Eth_Ipr_PBcfg.c
 *   @version    1.0.3
 *   @brief      Eth IP Router Layer Configuration
 *   @addtogroup eth_ipr_config
 *   @{
 */

#ifdef __cplusplus
extern "C"{
#endif

/***********************************************************************************************************************
 *  INCLUDES
 **********************************************************************************************************************/
#include "Eth_Ipr_Cfg.h"
#include "Eth_Ipr_Types.h"
#include "Eth_Ip_PBcfg.h"

/***********************************************************************************************************************
 *  DEFINES & MACROS
 **********************************************************************************************************************/

/***********************************************************************************************************************
 *  EXTERN
 **********************************************************************************************************************/
/* Import IPR buffer variables */
ETH_IPR_VARIABLE_EXT

/***********************************************************************************************************************
 *  LOCAL TYPEDEFS
 **********************************************************************************************************************/

/***********************************************************************************************************************
 *  LOCAL CONSTANTS
 **********************************************************************************************************************/
/* The configuration of the Tx buffer pool. */
static const Eth_Ipr_TxBufferPoolType s_EthIprCtrl0TxBufferPool_aa[1U] =
{
    {
        .TxBuffer_ptr  = &Eth_Ipr_g_Ctrl0TxBuffer_aa[0U][0U],
        .TxBuffLen_u16 = 1552U,
    }
};
/***********************************************************************************************************************
 *  LOCAL VARIABLES
 **********************************************************************************************************************/

/***********************************************************************************************************************
 *  GLOBAL VARIABLES
 **********************************************************************************************************************/
/* Import IP config */

ETH_IP_CONFIG_PB

#define ETH_START_SEC_CONFIG_DATA_UNSPECIFIED
#include "Eth_MemMap.h"

const Eth_Ipr_ConfigType Eth_Ipr_g_CtrlConfig_aa[1] =
{
    /* The configuration structure for Eth_Ipr_ConfigType_aa[0U] */
    {
        .PriorityToEgressFifoIdx_aa =
        {
            0U, 0U, 0U, 0U, 0U, 0U, 0U, 0U, 
        },
        .CtrlCfg_ptr    = &Eth_Ip_g_CtrlConfig_aa[0U],
        .TxPool_ptr     = &s_EthIprCtrl0TxBufferPool_aa[0]
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
