/***********************************************************************************************************************
 * Project Name: XHSC AUTOSAR MCAL
 * File Name: Eth_Ipr_Types.h
 *
 * Description: Implementation of Eth IP Router Layer Driver
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
 *   @file       Eth_Ipr_Types.h
 *   @version    1.0.3
 *   @brief      Eth IP Router Layer Driver
 *   @addtogroup eth_ipr_driver
 *   @{
 */

#ifndef ETH_IPR_TYPES_H
#define ETH_IPR_TYPES_H

#ifdef __cplusplus
extern "C" {
#endif

/***********************************************************************************************************************
 *  INCLUDES
 **********************************************************************************************************************/
#include "Eth_GeneralTypes.h"
#include "Eth_Ip_Types.h"
#include "Eth_Cfg.h"

/***********************************************************************************************************************
 *  DEFINES & MACROS
 **********************************************************************************************************************/
/**
 * @brief      MAX FIFO priority number.
 */
#define ETH_MAX_PRIORITY (8U)
/***********************************************************************************************************************
 *  TYPEDEFS
 **********************************************************************************************************************/
/**
 *  @brief   Defines the Tx Buffer Info.
 *  @details SRS ID:
 *           Design ID:
 */
typedef struct
{
    uint8_t *TxBuffer_ptr;         /*!< Transmit buffer data pool start address. */
    uint16_t TxBuffLen_u16;        /*!< Length of each individual buffer in buffer data pool. */
} Eth_Ipr_TxBufferPoolType;

/**
 *  @brief   Defines the config for IPR.
 *  @details SRS ID:
 *           Design ID:
 */
typedef struct
{
    uint8_t PriorityToEgressFifoIdx_aa[ETH_MAX_PRIORITY]; /*!< Map between prority and egress FIFOs.*/
    const Eth_Ip_CtrlConfigType *CtrlCfg_ptr;           /*!< Eth IP Ctrl config Type.*/
    const Eth_Ipr_TxBufferPoolType *TxPool_ptr;         /*!< Internal Tx buffer pool info. */
} Eth_Ipr_ConfigType;

/**
 *  @brief   Defines the Tx Buffer Info.
 *  @details SRS ID:
 *           Design ID:
 */
typedef struct Eth_Ipr_TxBufferInfoType
{
    uint8_t *Buffer_ptr;                                  /*!< Buffer pointer.*/
    Eth_BufIdxType BufferIdx_en;                        /*!< Buffer Index.*/
    uint8_t FifoIdx_u8;                                   /*!< Fifo Index.*/
    boolean TxConfirmation_bool;                        /*!< Disable/Enable Tx Confirmation.*/
    boolean Lock_bool;                                  /*!< Disable/Enable Look.*/
    Std_ReturnType TxResult_en;                         /*!< Indicate transmission success or not.*/
#if (STD_ON == ETH_GLOBAL_TIME_SUPPORT_API)
    boolean TimeStamp_bool;                             /*!< Disable/Enable TimeStamp.*/
    Eth_TimeStampType TxTimeStamp_st;                   /*!< Tx buffer timestamp.*/
#endif /*!< (STD_ON == ETH_GLOBAL_TIME_SUPPORT_API) */
    struct Eth_Ipr_TxBufferInfoType *NextTxBuffer_ptr; /*!< Point to the next transmitted Tx buffer.*/
} Eth_Ipr_TxBufferInfoType;

/**
 *  @brief   Defines the Tx Buffer List.
 *  @details SRS ID:
 *           Design ID:
 */
typedef struct
{
    uint8_t *Buffer_ptr;                          /*!< Transmit buffer data pool start address. */
    uint16_t BuffLen_u16;                         /*!< Length of each individual buffer in buffer data pool. */
    uint16_t BufferNum_u16;                       /*!< Transmited Buffer Number.*/
    Eth_Ipr_TxBufferInfoType *TxBufferHead_ptr; /*!< Point to the transmitted Tx buffer head.*/
    Eth_Ipr_TxBufferInfoType *TxBufferTail_ptr; /*!< Point to the transmitted Tx buffer Tail.*/
} Eth_Ipr_TxBufferListType;

/**
 *  @brief   Defines the Rx Buffer Info.
 *  @details SRS ID:
 *           Design ID:
 */
typedef struct
{
    uint8_t *Buffer_ptr;              /*!< Rx Buffer pointer.*/
    Eth_TimeStampType Timestamp_st; /*!< Rx TimeStamp.*/
} Eth_Ipr_RxBufferInfoType;
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

#endif /*!< ETH_IPR_TYPES_H */
