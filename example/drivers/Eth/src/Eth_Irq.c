/***********************************************************************************************************************
 * Project Name: XHSC AUTOSAR MCAL
 * File Name: Eth_Irq.c
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
 *   @file       Eth_Irq.c
 *   @version    1.0.3
 *   @brief      Eth High Layer Driver
 *   @addtogroup eth_driver
 *   @{
 */
#ifdef __cplusplus
extern "C" {
#endif

/***********************************************************************************************************************
 *  INCLUDES
 **********************************************************************************************************************/
#include "Eth_Irq.h"
#include "Eth_Ipr.h"
#include "EthIf.h"

/***********************************************************************************************************************
 *  DEFINES & MACROS
 **********************************************************************************************************************/

/***********************************************************************************************************************
 *  EXTERN
 **********************************************************************************************************************/

/***********************************************************************************************************************
 *  LOCAL TYPEDEFS
 **********************************************************************************************************************/

/***********************************************************************************************************************
 *  LOCAL CONSTANTS
 **********************************************************************************************************************/

/***********************************************************************************************************************
 *  LOCAL VARIABLES
 **********************************************************************************************************************/

/***********************************************************************************************************************
 *  GLOBAL VARIABLES
 **********************************************************************************************************************/

/***********************************************************************************************************************
 *  LOCAL FUNCTION PROTOTYPES
 **********************************************************************************************************************/

/***********************************************************************************************************************
 *  LOCAL FUNCTION
 **********************************************************************************************************************/

/***********************************************************************************************************************
 *  GLOBAL FUNCTION
 **********************************************************************************************************************/

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
)
{
    /* Check the status of transimitting */
    uint32_t BuffNum_u32 = 0x00U;
    Eth_Ipr_TxBufferInfoType *TxBufferInfo_ptr;

    BuffNum_u32 = Eth_Ipr_ChGetTxNum(CtrlIdx_u8, Channel_u8);
    for (uint8_t BuffIdx_u8 = 0U; BuffIdx_u8 < BuffNum_u32; BuffIdx_u8++)
    {
        Eth_Ipr_ChGetTxBufferInfo(CtrlIdx_u8, Channel_u8, &TxBufferInfo_ptr);
        if (NULL_PTR == TxBufferInfo_ptr)
        {
            break;
        }
        if (TRUE == TxBufferInfo_ptr->TxConfirmation_bool)
        {
            EthIf_TxConfirmation(CtrlIdx_u8, TxBufferInfo_ptr->BufferIdx_en, TxBufferInfo_ptr->TxResult_en);
        }
        Eth_Ipr_ChReleaseTxBuffer(CtrlIdx_u8, Channel_u8);
    }
}
#endif /*!< (STD_ON == ETH_TRANSMIT_IRQ_ENABLED)*/

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
)
{
    Eth_RxStatusType RxStatus_en = ETH_NOT_RECEIVED;
    Eth_FrameType EthType_en;
    const uint8_t *MacAddr_ptr;
    boolean IsBroadcast_bool;
    uint8_t *Buffer_ptr;
    uint16_t Length_u16;

    do
    {
        Eth_Ipr_Receive(CtrlIdx_u8, Channel_u8, &RxStatus_en, &Buffer_ptr, &Length_u16);
        if (ETH_NOT_RECEIVED != RxStatus_en)
        {
            /* Retrieve the Ethernet type. */
            EthType_en  = Buffer_ptr[ETH_FRAME_ETHERNET_TYPE_OFFSET + 1U];
            EthType_en |= ((uint16_t)Buffer_ptr[ETH_FRAME_ETHERNET_TYPE_OFFSET] << 8U);
            /* Check MAC Destination Address type */
            MacAddr_ptr      = &Buffer_ptr[ETH_FRAME_MAC_DESTINATION_OFFSET_0];
            IsBroadcast_bool = TRUE;
            for (uint8_t Index_u8 = 0; Index_u8 < ETH_FRAME_MAC_BYTES; Index_u8++)
            {
                if (0xFFU != MacAddr_ptr[Index_u8])
                {
                    IsBroadcast_bool = FALSE;
                    break;
                }
            }
            /* Retrieve the source MAC address. */
            MacAddr_ptr = &Buffer_ptr[ETH_FRAME_MAC_SOURCE_OFFSET];
            /* Retrieve the payload and invoke the callback function. */
            EthIf_RxIndication(CtrlIdx_u8, EthType_en, IsBroadcast_bool, MacAddr_ptr,
                               (Eth_DataType *)&Buffer_ptr[ETH_FRAME_ETHERNET_PAYLOAD_OFFSET],
                               Length_u16);
            /* Release the received buffer. */
            Eth_Ipr_ReleaseRxBuffer(CtrlIdx_u8, Channel_u8);
        }
    }
    while (ETH_NOT_RECEIVED != RxStatus_en);
}
#endif /*!< (STD_ON == ETH_RECEIVED_IRQ_ENABLED)*/

#ifdef __cplusplus
}
#endif

/**
 * @}
 */
