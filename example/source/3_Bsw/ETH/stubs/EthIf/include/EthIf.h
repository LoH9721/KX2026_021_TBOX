/***********************************************************************************************************************
 * Project Name: XHSC AUTOSAR MCAL
 * File Name: EthIf.h
 *
 * Description: Implementation of EthIf 
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
 *   @file       EthIf.h
 *   @version    1.0.3
 *   @brief      Autosar 4.4.0 EthIf Driver
 *   @addtogroup ethif_driver
 *   @{
 */

#ifndef ETHIF_H
#define ETHIF_H

#ifdef __cplusplus
extern "C" {
#endif

/***********************************************************************************************************************
 *  INCLUDES
 **********************************************************************************************************************/
#include "EthIf_Types.h"

/***********************************************************************************************************************
 *  DEFINES & MACROS
 **********************************************************************************************************************/
#define ETHIF_AR_RELEASE_MAJOR_VERSION    4
#define ETHIF_AR_RELEASE_MINOR_VERSION    4

#define ETHIF_NUMBER_OF_CTRL ((uint8_t)10U)

/***********************************************************************************************************************
 *  TYPEDEFS
 **********************************************************************************************************************/

/***********************************************************************************************************************
 *  CONSTANTS
 **********************************************************************************************************************/

/***********************************************************************************************************************
 *  GLOBAL VARIABLES
 **********************************************************************************************************************/
extern volatile uint32_t g_EthIfRxIndications_aa[ETHIF_NUMBER_OF_CTRL];
extern volatile uint32_t g_EthIfTxConfirmations_aa[ETHIF_NUMBER_OF_CTRL];
extern volatile uint32_t g_EthIfChecksumValue_aa[ETHIF_NUMBER_OF_CTRL];
extern volatile boolean g_EthIfModeIndications_aa[ETHIF_NUMBER_OF_CTRL];

/***********************************************************************************************************************
 *  GLOBAL FUNCTION
 **********************************************************************************************************************/
/**
 *  @brief          Handles a received frame received by the indexed controller.
 *
 *  @details        Service ID:        None
 *                  SRS ID:            SWS_EthIf_00085
 *                  Design ID:         EthIf_RxIndication_Activity
 *                  Sync/Async:        Synchronous
 *                  Reentrancy:        Non Reentrant
 *
 *  @param[in]      CtrlIdx     Index of the physical Ethernet controller within the context of the Ethernet Interface
 *  @param[in]      FrameType   Frame type of received Ethernet frame
 *  @param[in]      IsBroadcast Parameter to indicate a broadcast frame
 *  @param[in]      PhysAddrPtr Pointer to Physical source address (MAC address in network byte
 *                              order) of received Ethernet frame
 *  @param[in]      DataPtr     Pointer to payload of received Ethernet frame
 *  @param[in]      LenByte     Length (bytes) of the payload in received frame
 *
 *  @return         void
 *
 *  @note           The passed data buffer is no longer valid after the function is exited.
 *
 *  @pre            None
 *
 **/
void EthIf_RxIndication(
    uint8_t CtrlIdx,
    Eth_FrameType FrameType,
    boolean IsBroadcast,
    const uint8_t *PhysAddrPtr,
    const Eth_DataType *DataPtr,
    uint16_t LenByte
);

/**
 *  @brief          Confirms frame transmission by the indexed controller.
 *
 *  @details        Service ID:        None
 *                  SRS ID:            SWS_EthIf_00091
 *                  Design ID:         EthIf_TxConfirmation_Activity
 *                  Sync/Async:        Synchronous
 *                  Reentrancy:        Non Reentrant
 *
 *  @param[in]      CtrlIdx     Index of the physical Ethernet controller within the context of the Ethernet Interface
 *  @param[in]      BufIdx      Index of the transmitted buffer
 *  @param[in]      Result      E_OK: The transmission was successful,
 *                              E_NOT_OK: The transmission failed.
 *
 *  @return         void
 *
 *  @note           None
 *
 *  @pre            None
 *
 **/
void EthIf_TxConfirmation(
    uint8_t CtrlIdx,
    Eth_BufIdxType BufIdx,
    Std_ReturnType Result
);

/**
 *  @brief          Called asynchronously when mode has been read out.
 *                  Triggered by previous Eth_SetControllerMode call. Can directly be called within the trigger functions.
 *
 *  @details        Service ID:        None
 *                  SRS ID:            SWS_EthIf_00231
 *                  Design ID:         EthIf_CtrlModeIndication_Activity
 *                  Sync/Async:        Synchronous
 *                  Reentrancy:        Non Reentrant for the same CtrlIdx, reentrant for different
 *
 *  @param[in]      CtrlIdx     Index of the physical Ethernet controller within the context of the Ethernet Interface
 *  @param[in]      CtrlMode    Notified Ethernet controller mode
 *
 *  @return         void
 *
 *  @note           None
 *
 *  @pre            None
 *
 **/
void EthIf_CtrlModeIndication(
    uint8_t CtrlIdx,
    Eth_ModeType CtrlMode
);

#ifdef __cplusplus
}
#endif

/**
 * @}
 */

#endif /* ETHIF_H */
