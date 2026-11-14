/***********************************************************************************************************************
 * Project Name: XHSC AUTOSAR MCAL
 * File Name: Eth_Ip_Irq.h
 *
 * Description: Implementation of ETH IP layer
 *              Compiler:                IAR GCC
 *              AutoSAR Version:         4.4.0
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
 **********************************************************************************************************************/

/**
 *   @file       Eth_Ip_Irq.h
 *   @version    1.0.3
 *   @brief      ETH IP Layer Driver
 *   @addtogroup eth_ip
 *   @{
 */

#ifndef ETH_IP_IRQ_H
#define ETH_IP_IRQ_H

#ifdef __cplusplus
extern "C" {
#endif

/***********************************************************************************************************************
 *  INCLUDES
 **********************************************************************************************************************/
#include "Common.h"
#include "Osr_Internal.h"

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
 *  @brief          ETH IRQ Handler.
 *
 *  @details        SRS ID:            None
 *                  Design ID:         ETH_GLB_INT_IRQHandler_Activity
 *                  Sync/Async:        None
 *                  Reentrancy:        None
 *
 *
 *  @note           None
 *
 *  @pre            None
 *
 */
ISR(
    ETH_GLB_INT_IRQHandler
);
#ifdef __cplusplus
}
#endif
/*! @}*/

#endif /*!< ETH_IP_IRQ_H */
