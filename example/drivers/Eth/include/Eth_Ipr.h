/***********************************************************************************************************************
 * Project Name: XHSC AUTOSAR MCAL
 * File Name: Eth_Ipr.h
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
 *   @file       Eth_Ipr.h
 *   @version    1.0.3
 *   @brief      Eth IP Router Layer Driver
 *   @addtogroup eth_ipr_driver
 *   @{
 */

#ifndef ETH_IPR_H
#define ETH_IPR_H

#ifdef __cplusplus
extern "C" {
#endif

/***********************************************************************************************************************
 *  INCLUDES
 **********************************************************************************************************************/
#include "Eth_Ipr_Types.h"
#include "Eth_Cfg.h"

/***********************************************************************************************************************
 *  DEFINES & MACROS
 **********************************************************************************************************************/
#define ETH_FRAME_MAC_DESTINATION_OFFSET_0 (0U)
#define ETH_FRAME_MAC_DESTINATION_OFFSET_1 (1U)
#define ETH_FRAME_MAC_DESTINATION_OFFSET_2 (2U)
#define ETH_FRAME_MAC_DESTINATION_OFFSET_3 (3U)
#define ETH_FRAME_MAC_DESTINATION_OFFSET_4 (4U)
#define ETH_FRAME_MAC_DESTINATION_OFFSET_5 (5U)
#define ETH_FRAME_MAC_SOURCE_OFFSET (6U)
#define ETH_FRAME_ETHERNET_TYPE_OFFSET (12U)
#define ETH_FRAME_ETHERNET_PAYLOAD_OFFSET (14U)
#define ETH_FRAME_ETHERNET_HEADER_OF_LENGTH (0x6U + 0x6U + 0x2U)
#define ETH_FRAME_CHECK_SEQUENCE_LENGTH (0x4U)
#define ETH_FRAME_MAC_BYTES (6U)
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
 *  @brief          Check whether Eth controller can be access.
 *  @details        Design ID:         SDD_ETH_23
 *
 *  @param[in]      para_CtrlIdx_u8    Index of the controller within the context of the Ethernet Driver
 *
 *  @return         boolean
 *  @retval         None
 *
 *  @note           None
 *
 *  @pre            None
 *
 */
boolean Eth_Ipr_CheckAccessToController(
    uint8_t para_CtrlIdx_u8
);

/**
 *  @brief          Check whether buffer id is valid.
 *  @details        Design ID:         SDD_ETH_24
 *
 *  @param[in]      para_CtrlIdx_u8    Index of the controller within the context of the Ethernet Driver
 *  @param[in]      para_BufIdx_en     Index of the buffer resource
 *
 *  @return         boolean
 *  @retval         None
 *
 *  @note           None
 *
 *  @pre            None
 *
 */
boolean Eth_Ipr_CheckBufferIdxValid(
    uint8_t para_CtrlIdx_u8,
    Eth_BufIdxType para_BufIdx_en
);

#if (STD_ON == ETH_DEM_EVENT_ENABLE)
/**
 *  @brief          Get Dem Error Counter.
 *  @details        Design ID:         SDD_ETH_25
 *
 *  @param[in]      para_CtrlIdx_u8          Index of the controller within the context of the Ethernet Driver.
 *  @param[in]      para_ErrorCounter_ptr    The pointer to hold read back error counter.
 *
 *  @return         void
 *
 *  @note           None
 *
 *  @pre            None
 *
 */
void Eth_Ipr_GetDemErrorCounter(
    uint8_t para_CtrlIdx_u8,
    uint32_t *para_ErrorCounter_ptr
);
#endif /*!< (STD_ON == ETH_DEM_EVENT_ENABLE) */

/**
 *  @brief          Initializes the Ethernet Driver.
 *  @details        Design ID:         SDD_ETH_26
 *
 *  @param[in]      para_CtrlIdx_u8    Index of the controller within the context of the Ethernet Driver.
 *  @param[in]      para_IprCfg_ptr    The pointer to hold IPR layer Eth configuration.
 *
 *  @return         void
 *
 *  @note           None
 *
 *  @pre            None
 *
 */
void Eth_Ipr_Init(
    uint8_t para_CtrlIdx_u8,
    const Eth_Ipr_ConfigType *para_IprCfg_ptr
);

/**
 *  @brief          Set Controller Mode.
 *  @details        Design ID:         SDD_ETH_27
 *
 *  @param[in]      para_CtrlIdx_u8     None
 *  @param[in]      para_Config_ptr     None
 *  @param[in]      para_CtrlMode_en    None
 *
 *  @return         Std_ReturnType
 *  @retval         None
 *
 *  @note           None
 *
 *  @pre            None
 *
 */
Std_ReturnType Eth_Ipr_SetControllerMode(
    uint8_t para_CtrlIdx_u8,
    const Eth_Ipr_ConfigType *para_Config_ptr,
    Eth_ModeType para_CtrlMode_en
);

/**
 *  @brief          Get Controller Mode.
 *  @details        Design ID:         SDD_ETH_28
 *
 *  @param[in]      para_CtrlIdx_u8    None
 *
 *  @return         Eth_ModeType
 *  @retval         None
 *
 *  @note           None
 *
 *  @pre            None
 *
 */
Eth_ModeType Eth_Ipr_GetControllerMode(
    uint8_t para_CtrlIdx_u8
);

/**
 *  @brief          Get Phys Address.
 *  @details        Design ID:         SDD_ETH_29
 *
 *  @param[in]      para_CtrlIdx_u8      None
 *  @param[in]      para_PhysAdrr_ptr    None
 *
 *  @return         void
 *
 *  @note           None
 *
 *  @pre            None
 *
 */
void Eth_Ipr_GetPhysAddr(
    uint8_t para_CtrlIdx_u8,
    uint8_t *para_PhysAdrr_ptr
);

/**
 *  @brief          Set Phys Address.
 *  @details        Design ID:         SDD_ETH_30
 *
 *  @param[in]      para_CtrlIdx_u8      None
 *  @param[in]      para_PhysAdrr_ptr    None
 *
 *  @return         void
 *
 *  @note           None
 *
 *  @pre            None
 *
 */
void Eth_Ipr_SetPhysAddr(
    uint8_t para_CtrlIdx_u8,
    const uint8_t *para_PhysAdrr_ptr
);

/**
 *  @brief          Update Phys Address Filter.
 *  @details        Design ID:         SDD_ETH_31
 *
 *  @param[in]      para_CtrlIdx_u8      None
 *  @param[in]      para_PhysAdrr_ptr    None
 *  @param[in]      para_Action_en       None
 *
 *  @return         Std_ReturnType
 *  @retval         None
 *
 *  @note           None
 *
 *  @pre            None
 *
 */
Std_ReturnType Eth_Ipr_UpdatePhysAddrFilter(
    uint8_t para_CtrlIdx_u8,
    const uint8_t *para_PhysAdrr_ptr,
    Eth_FilterActionType para_Action_en
);

#if (STD_ON == ETH_ENABLE_MII)
/**
 *  @brief          Write Mii.
 *  @details        Design ID:         SDD_ETH_32
 *
 *  @param[in]      para_CtrlIdx_u8    None
 *  @param[in]      para_TrcvIdx_u8    None
 *  @param[in]      para_RegIdx_u8     None
 *  @param[in]      para_RegVal_u16    None
 *
 *  @return         Std_ReturnType
 *  @retval         None
 *
 *  @note           None
 *
 *  @pre            None
 *
 */
Std_ReturnType Eth_Ipr_WriteMii(
    uint8_t para_CtrlIdx_u8,
    uint8_t para_TrcvIdx_u8,
    uint8_t para_RegIdx_u8,
    uint16_t para_RegVal_u16
);

/**
 *  @brief          Read Mii.
 *  @details        Design ID:         SDD_ETH_33
 *
 *  @param[in]      para_CtrlIdx_u8    None
 *  @param[in]      para_TrcvIdx_u8    None
 *  @param[in]      para_RegIdx_u8     None
 *  @param[in]      para_RegVal_ptr    None
 *
 *  @return         Std_ReturnType
 *  @retval         None
 *
 *  @note           None
 *
 *  @pre            None
 *
 */
Std_ReturnType Eth_Ipr_ReadMii(
    uint8_t para_CtrlIdx_u8,
    uint8_t para_TrcvIdx_u8,
    uint8_t para_RegIdx_u8,
    uint16_t *para_RegVal_ptr
);
#endif /*!< (STD_ON == ETH_ENABLE_MII) */

#if (STD_ON == ETH_GET_COUNTER_VALUES_API)
/**
 *  @brief          Get Counter Values.
 *  @details        Design ID:         SDD_ETH_34
 *
 *  @param[in]      para_CtrlIdx_u8     None
 *  @param[in]      para_Counter_ptr    None
 *
 *  @return         Std_ReturnType
 *  @retval         None
 *
 *  @note           None
 *
 *  @pre            None
 *
 */
Std_ReturnType Eth_Ipr_GetCounterValues(
    uint8_t para_CtrlIdx_u8,
    Eth_CounterType *para_Counter_ptr
);
#endif /*!< (STD_ON == ETH_GET_COUNTER_VALUES_API) */

#if (STD_ON == ETH_GET_RECEIVED_STATS_API)
/**
 *  @brief          Get Rx Stats.
 *  @details        Design ID:         SDD_ETH_35
 *
 *  @param[in]      para_CtrlIdx_u8     None
 *  @param[out]     para_RxStats_ptr    None
 *
 *  @return         Std_ReturnType
 *  @retval         None
 *
 *  @note           None
 *
 *  @pre            None
 *
 */
Std_ReturnType Eth_Ipr_GetRxStats(
    uint8_t para_CtrlIdx_u8,
    Eth_RxStatsType *para_RxStats_ptr
);
#endif /*!< (STD_ON == ETH_GET_RECEIVED_STATS_API) */

#if (STD_ON == ETH_GET_TRANSMIT_STATS_API)
/**
 *  @brief          Get Tx Stats.
 *  @details        Design ID:         SDD_ETH_36
 *
 *  @param[in]      para_CtrlIdx_u8     None
 *  @param[out]     para_TxStats_ptr    None
 *
 *  @return         Std_ReturnType
 *  @retval         None
 *
 *  @note           None
 *
 *  @pre            None
 *
 */
Std_ReturnType Eth_Ipr_GetTxStats(
    uint8_t para_CtrlIdx_u8,
    Eth_TxStatsType *para_TxStats_ptr
);
#endif /*!< (STD_ON == ETH_GET_TRANSMIT_STATS_API) */

#if (STD_ON == ETH_GET_TX_ERROR_COUNTER_VALUES_API)
/**
 *  @brief          Get Tx Error Counter Values.
 *  @details        Design ID:         SDD_ETH_37
 *
 *  @param[in]      para_CtrlIdx_u8                  None
 *  @param[out]     para_TxErrorCounterValues_ptr    None
 *
 *  @return         Std_ReturnType
 *  @retval         None
 *
 *  @note           None
 *
 *  @pre            None
 *
 */
Std_ReturnType Eth_Ipr_GetTxErrorCounterValues(
    uint8_t para_CtrlIdx_u8,
    Eth_TxErrorCounterValuesType *para_TxErrorCounterValues_ptr
);
#endif /*!< (STD_ON == ETH_GET_TX_ERROR_COUNTER_VALUES_API) */

#if (STD_ON == ETH_GLOBAL_TIME_SUPPORT_API)
/**
 *  @brief          Get Current Time.
 *  @details        Design ID:         SDD_ETH_38
 *
 *  @param[in]      para_CtrlIdx_u8       None
 *  @param[out]     para_TimeQual_ptr     None
 *  @param[out]     para_TimeStamp_ptr    None
 *
 *  @return         Std_ReturnType
 *  @retval         None
 *
 *  @note           None
 *
 *  @pre            None
 *
 */
Std_ReturnType Eth_Ipr_GetCurrentTime(
    uint8_t para_CtrlIdx_u8,
    Eth_TimeStampQualType *para_TimeQual_ptr,
    Eth_TimeStampType *para_TimeStamp_ptr
);

/**
 *  @brief          Enable Egress TimeStamp.
 *  @details        Design ID:         SDD_ETH_39
 *
 *  @param[in]      para_CtrlIdx_u8    None
 *  @param[in]      para_BufIdx_en     None
 *
 *  @return         void
 *
 *  @note           None
 *
 *  @pre            None
 *
 */
void Eth_Ipr_EnableEgressTimeStamp(
    uint8_t para_CtrlIdx_u8,
    Eth_BufIdxType para_BufIdx_en
);

/**
 *  @brief          Get Egress TimeStamp.
 *  @details        Design ID:         SDD_ETH_40
 *
 *  @param[in]      para_CtrlIdx_u8       None
 *  @param[in]      para_BufIdx_en        None
 *  @param[in]      para_TimeQual_ptr     None
 *  @param[in]      para_TimeStamp_ptr    None
 *
 *  @return         Std_ReturnType
 *  @retval         None
 *
 *  @note           None
 *
 *  @pre            None
 *
 */
Std_ReturnType Eth_Ipr_GetEgressTimeStamp(
    uint8_t para_CtrlIdx_u8,
    Eth_BufIdxType para_BufIdx_en,
    Eth_TimeStampQualType *para_TimeQual_ptr,
    Eth_TimeStampType *para_TimeStamp_ptr
);

/**
 *  @brief          Get Ingress TimeStamp.
 *  @details        Design ID:         SDD_ETH_41
 *
 *  @param[in]      para_CtrlIdx_u8       None
 *  @param[in]      para_Data_ptr         None
 *  @param[out]     para_TimeQual_ptr     None
 *  @param[out]     para_TimeStamp_ptr    None
 *
 *  @return         Std_ReturnType
 *  @retval         None
 *
 *  @note           None
 *
 *  @pre            None
 *
 */
Std_ReturnType Eth_Ipr_GetIngressTimeStamp(
    uint8_t para_CtrlIdx_u8,
    const Eth_DataType *para_Data_ptr,
    Eth_TimeStampQualType *para_TimeQual_ptr,
    Eth_TimeStampType *para_TimeStamp_ptr
);
#endif /*!< (STD_ON == ETH_GLOBAL_TIME_SUPPORT_API) */

/**
 *  @brief          Provide Tx Buffer.
 *  @details        Design ID:         SDD_ETH_42
 *
 *  @param[in]      para_CtrlIdx_u8     None
 *  @param[in]      para_Priority_u8    None
 *  @param[out]     para_BufIdx_ptr     None
 *  @param[out]     para_Buf_ptr        None
 *  @param[inout]   para_LenByte_ptr    None
 *
 *  @return         BufReq_ReturnType
 *  @retval         None
 *
 *  @note           None
 *
 *  @pre            None
 *
 */
BufReq_ReturnType Eth_Ipr_ProvideTxBuffer(
    uint8_t para_CtrlIdx_u8,
    uint8_t para_Priority_u8,
    Eth_BufIdxType *para_BufIdx_ptr,
    uint8_t **para_Buf_ptr,
    uint16_t *para_LenByte_ptr
);

/**
 *  @brief          Eth Transmit.
 *  @details        Design ID:         SDD_ETH_43
 *
 *  @param[in]      para_CtrlIdx_u8             None
 *  @param[in]      para_BufIdx_en              None
 *  @param[in]      para_FrameType_en           None
 *  @param[in]      para_TxConfirmation_bool    None
 *  @param[in]      para_LenByte_u16            None
 *  @param[in]      para_PhysAddr_ptr           None
 *
 *  @return         Std_ReturnType
 *  @retval         None
 *
 *  @note           None
 *
 *  @pre            None
 *
 */
Std_ReturnType Eth_Ipr_Transmit(
    uint8_t para_CtrlIdx_u8,
    Eth_BufIdxType para_BufIdx_en,
    Eth_FrameType para_FrameType_en,
    boolean para_TxConfirmation_bool,
    uint16_t para_LenByte_u16,
    const uint8_t *para_PhysAddr_ptr
);

/**
 *  @brief          Eth Receive.
 *  @details        Design ID:         SDD_ETH_44
 *
 *  @param[in]      para_CtrlIdx_u8      None
 *  @param[in]      para_FifoIdx_u8      None
 *  @param[out]     para_RxStatus_ptr    None
 *  @param[out]     para_Buffer_ptr      None
 *  @param[out]     para_Length_ptr      None
 *
 *  @return         void
 *
 *  @note           None
 *
 *  @pre            None
 *
 */
void Eth_Ipr_Receive(
    uint8_t para_CtrlIdx_u8,
    uint8_t para_FifoIdx_u8,
    Eth_RxStatusType *para_RxStatus_ptr,
    uint8_t **para_Buffer_ptr,
    uint16_t *para_Length_ptr
);

/**
 *  @brief          Eth release received buffer.
 *  @details        Design ID:         SDD_ETH_45
 *
 *  @param[in]      para_CtrlIdx_u8      None
 *  @param[in]      para_FifoIdx_u8      None
 *
 *  @return         void
 *
 *  @note           None
 *
 *  @pre            None
 *
 */
void Eth_Ipr_ReleaseRxBuffer(
    uint8_t para_CtrlIdx_u8,
    uint8_t para_FifoIdx_u8
);

/**
 *  @brief          Get Tx channel number.
 *  @details        Design ID:         SDD_ETH_46
 *
 *  @param[in]      para_CtrlIdx_u8    None
 *
 *  @return         uint8_t
 *  @retval         None
 *
 *  @note           None
 *
 *  @pre            None
 *
 */
uint8_t Eth_Ipr_GetTxChNum(
    uint8_t para_CtrlIdx_u8
);

/**
 *  @brief          Get channel transfer buffer number.
 *  @details        Design ID:         SDD_ETH_47
 *
 *  @param[in]      para_CtrlIdx_u8    None
 *  @param[in]      para_FifoIdx_u8    None
 *
 *  @return         uint32_t
 *  @retval         None
 *
 *  @note           None
 *
 *  @pre            None
 *
 */
uint32_t Eth_Ipr_ChGetTxNum(
    uint8_t para_CtrlIdx_u8,
    uint8_t para_FifoIdx_u8
);

/**
 *  @brief          Get Channel Tx buffer info.
 *  @details        Design ID:         SDD_ETH_48
 *
 *  @param[in]      para_CtrlIdx_u8          None
 *  @param[in]      para_FifoIdx_u8          None
 *  @param[out]     para_TxBufferInfo_ptr    None
 *
 *  @return         void
 *
 *  @note           None
 *
 *  @pre            None
 *
 */
void Eth_Ipr_ChGetTxBufferInfo(
    uint8_t para_CtrlIdx_u8,
    uint8_t para_FifoIdx_u8,
    Eth_Ipr_TxBufferInfoType **para_TxBufferInfo_ptr
);

/**
 *  @brief          Release the head buffer in the FIFO transmited list.
 *  @details        Design ID:         SDD_ETH_49
 *
 *  @param[in]      para_CtrlIdx_u8    None
 *  @param[in]      para_FifoIdx_u8    None
 *
 *  @return         void
 *
 *  @note           None
 *
 *  @pre            None
 *
 */
void Eth_Ipr_ChReleaseTxBuffer(
    uint8_t para_CtrlIdx_u8,
    uint8_t para_FifoIdx_u8
);

#ifdef __cplusplus
}
#endif
/*! @}*/

#endif /*!< ETH_IPR_H */
