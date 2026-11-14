/***********************************************************************************************************************
 * Project Name: XHSC AUTOSAR MCAL
 * File Name: Eth_Irq.h
 *
 * Description: Implementation of Eth High Layer Driver
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
 *   @file       Eth_Irq.h
 *   @version    1.0.3
 *   @brief      Eth High Layer Driver
 *   @addtogroup eth_driver
 *   @{
 */

#ifndef ETH_IRQ_H
#define ETH_IRQ_H

#ifdef __cplusplus
extern "C" {
#endif

/***********************************************************************************************************************
 *  INCLUDES
 **********************************************************************************************************************/
#include "Eth.h"

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

#define ETH_START_SEC_CODE
#include "Eth_MemMap.h"

#if (STD_ON == ETH_TRANSMIT_IRQ_ENABLED)
/**
 *  @brief          Transmit IRQ callback.
 *
 *  @details        Service ID:        None
 *                  SRS ID:            SWS_Eth_00243
 *                  Design ID:         SDD_ETH_50
 *                  Sync/Async:        None
 *                  Reentrancy:        None
 *
 *  @param[in]      CtrlIdx    None
 *  @param[in]      Channel    None
 *
 *  @return         void
 *
 *  @note           None
 *
 *  @pre            None
 *
 */
void Eth_TxIrqCallback(
    uint8_t CtrlIdx_u8,
    uint8_t Channel_u8
);
#endif /*!< STD_ON == ETH_TRANSMIT_IRQ_ENABLED */

#if (STD_ON == ETH_RECEIVED_IRQ_ENABLED)
/**
 *  @brief          Received IRQ callback.
 *
 *  @details        Service ID:        None
 *                  SRS ID:            SWS_Eth_00244
 *                  Design ID:         SDD_ETH_51
 *                  Sync/Async:        None
 *                  Reentrancy:        None
 *
 *  @param[in]      CtrlIdx    None
 *  @param[in]      Channel    None
 *
 *  @return         void
 *
 *  @note           None
 *
 *  @pre            None
 *
 */
void Eth_RxIrqCallback(
    uint8_t CtrlIdx_u8,
    uint8_t Channel_u8
);
#endif

#define ETH_STOP_SEC_CODE
#include "Eth_MemMap.h"

#ifdef __cplusplus
}
#endif /*!< STD_ON == ETH_RECEIVED_IRQ_ENABLED */
/*! @}*/

#endif /*!< ETH_IRQ_H */
