/***********************************************************************************************************************
 * Project Name: XHSC AUTOSAR MCAL
 * File Name: EthTrcv.h
 *
 * Description: Implementation of EthTrcv 
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
 *   @file       EthTrcv.h
 *   @version    1.0.3
 *   @brief      Autosar 4.4.0 EthTrcv Driver
 *   @addtogroup ethtrcv_driver
 *   @{
 */

#ifndef ETHTRCV_H
#define ETHTRCV_H

#ifdef __cplusplus
extern "C" {
#endif

/***********************************************************************************************************************
 *  INCLUDES
 **********************************************************************************************************************/
#include "EthTrcv_Types.h"

/***********************************************************************************************************************
 *  DEFINES & MACROS
 **********************************************************************************************************************/
#define ETHTRCV_AR_RELEASE_MAJOR_VERSION    4
#define ETHTRCV_AR_RELEASE_MINOR_VERSION    4

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
 *  @brief          Called when information has been read out via MII interface.
 *                  Triggered by previous Eth_ReadMii call. Can directly be called within Eth_ReadMii.
 *
 *  @details        Service ID:        None
 *                  SRS ID:            SWS_EthTrcv_00108
 *                  Design ID:         EthTrcv_ReadMiiIndication_Activity
 *                  Sync/Async:        Synchronous
 *                  Reentrancy:        Non Reentrant for the same CtrlIdx, reentrant for different
 *
 *  @param[in]      CtrlIdx     Index of the controller within the context of the Ethernet Driver
 *  @param[in]      TrcvIdx     Index of the transceiver on the MII
 *  @param[in]      RegIdx      Index of the transceiver register on the MII
 *  @param[in]      RegVal      Value contained in the indexed register
 *
 *  @return         void
 *
 *  @note           None.
 *
 *  @pre            None
 *
 **/
void EthTrcv_ReadMiiIndication(
    uint8_t CtrlIdx,
    uint8_t TrcvIdx,
    uint8_t RegIdx,
    uint8_t RegVal
);

/**
 *  @brief          Called when information has been written via MII interface.
 *                  Triggered by previous Eth_WriteMii call. Can directly be called within Eth_WriteMii.
 *
 *  @details        Service ID:        None
 *                  SRS ID:            SWS_EthTrcv_00109
 *                  Design ID:         EthTrcv_WriteMiiIndication_Activity
 *                  Sync/Async:        Synchronous
 *                  Reentrancy:        Non Reentrant for the same CtrlIdx, reentrant for different
 *
 *  @param[in]      CtrlIdx     Index of the controller within the context of the Ethernet Driver
 *  @param[in]      TrcvIdx     Index of the transceiver on the MII
 *  @param[in]      RegIdx      Index of the transceiver register on the MII
 *
 *  @return         void
 *
 *  @note           None.
 *
 *  @pre            None
 *
 **/
void EthTrcv_WriteMiiIndication(
    uint8_t CtrlIdx,
    uint8_t TrcvIdx,
    uint8_t RegIdx
);

#ifdef __cplusplus
}
#endif

/**
 * @}
 */

#endif /*!< ETHTRCV_H */
