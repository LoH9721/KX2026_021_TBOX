/***********************************************************************************************************************
 * Project Name: XHSC AUTOSAR MCAL
 * File Name: Eth_Ip.h
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
 *   @file       Eth_Ip.h
 *   @version    1.0.3
 *   @brief      ETH IP Layer Driver
 *   @addtogroup eth_ip
 *   @{
 */

#ifndef ETH_IP_H
#define ETH_IP_H

#ifdef __cplusplus
extern "C" {
#endif

/***********************************************************************************************************************
 *  INCLUDES
 **********************************************************************************************************************/
#include "Eth_Ip_Types.h"

/***********************************************************************************************************************
 *  DEFINES & MACROS
 **********************************************************************************************************************/
/**
 * @brief      Rx Desc status macros.
 */
#define ETH_IP_RDES4_TSPD_MASK        (0x00004000U)
#define ETH_IP_RDES0_OWN_MASK         (0x80000000U)
#define ETH_IP_RDES0_TSPA_MASK        (0x00000080U)
/**
 * @brief      Tx Desc status macros.
 */
#define ETH_IP_TDES0_IOC_MASK         (0x40000000U)
#define ETH_IP_TDES0_TTSE_MASK        (0x02000000U)
#define ETH_IP_TDES0_CIC_1_MASK       (0x00400000U)
#define ETH_IP_TDES0_CIC_3_MASK       (0x00C00000U)
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
 *  @brief          Enables/Disable controller by enables/disable all configured transmit and receive DMA channle.
 *
 *  @details        SRS ID:            None
 *                  Design ID:         SDD_ETH_52
 *                  Sync/Async:        None
 *                  Reentrancy:        None
 *
 *  @param[in]      para_Instance_u8    Ethernet instance number.
 *  @param[in]      para_CtrlEn_bool    Enable/Disable the controller.
 *
 *  @return         Eth_Ip_StatusType
 *  @retval         ETH_IP_STATUS_SUCCESS     Operation success.
 *  @retval         ETH_IP_STATUS_TIMEOUT     Operation timeout.
 *
 *  @note           None
 *
 *  @pre            None
 *
 */
Eth_Ip_StatusType Eth_Ip_EnableController(
    uint8_t para_Instance_u8,
    boolean para_CtrlEn_bool
);

/**
 *  @brief          Get state mode of Eth module.
 *
 *  @details        SRS ID:            None
 *                  Design ID:         SDD_ETH_53
 *                  Sync/Async:        None
 *                  Reentrancy:        None
 *
 *  @param[in]      para_Instance_u8    Ethernet instance number.
 *
 *  @return         Eth_Ip_CtrlModeType
 *  @retval         ETH_IP_CTRL_MODE_NOT_ACCESS   The Eth module cannot be access due to missing clock.
 *  @retval         ETH_IP_CTRL_MODE_RESET        The Eth module under reset.
 *  @retval         ETH_IP_CTRL_MODE_ACTIVE       The Transmitter/Receiver are active.
 *  @retval         ETH_IP_CTRL_MODE_INACTIVE     The Transmitter/Receiver are inactive.
 *
 *  @note           Get Power State
 *
 *  @pre            None
 *
 */
Eth_Ip_CtrlModeType Eth_Ip_GetControllerMode(
    uint8_t para_Instance_u8
);

/**
 *  @brief          Reads the selected register of the PHY with MDIO IEEE802.3 Clause 22 frame.
 *
 *  @details        SRS ID:            None
 *                  Design ID:         SDD_ETH_54
 *                  Sync/Async:        None
 *                  Reentrancy:        None
 *
 *  @param[in]      para_Instance_u8    Ethernet instance number.
 *  @param[in]      para_PhyAddr_u8     Physical layer address.
 *  @param[in]      para_RegAddr_u8     PHY register address.
 *  @param[out]     para_Data_ptr       Data read from the PHY.
 *
 *  @return         Eth_Ip_StatusType
 *  @retval         ETH_IP_STATUS_SUCCESS     Read success.
 *  @retval         ETH_IP_STATUS_TIMEOUT     Read timeout.
 *
 *  @note           Support PHY interfaces
 *
 *  @pre            None
 *
 */
Eth_Ip_StatusType Eth_Ip_ReadMdioWithC22(
    uint8_t para_Instance_u8,
    uint8_t para_PhyAddr_u8,
    uint8_t para_RegAddr_u8,
    uint16_t *para_Data_ptr
);

/**
 *  @brief          Writes the selected register of the PHY with MDIO IEEE802.3 Clause 22 frame.
 *
 *  @details        SRS ID:            None
 *                  Design ID:         SDD_ETH_55
 *                  Sync/Async:        None
 *                  Reentrancy:        None
 *
 *  @param[in]      para_Instance_u8    Ethernet instance number.
 *  @param[in]      para_PhyAddr_u8     Physical layer address.
 *  @param[in]      para_RegAddr_u8     PHY register address.
 *  @param[in]      para_Data_ptr       Data to be written in the specified register of the PHY.
 *
 *  @return         Eth_Ip_StatusType
 *  @retval         ETH_IP_STATUS_SUCCESS     Write success.
 *  @retval         ETH_IP_STATUS_TIMEOUT     Write timeout.
 *
 *  @note           Support PHY interfaces
 *
 *  @pre            None
 *
 */
Eth_Ip_StatusType Eth_Ip_WriteMdioWithC22(
    uint8_t para_Instance_u8,
    uint8_t para_PhyAddr_u8,
    uint8_t para_RegAddr_u8,
    uint16_t para_Data_u16
);

/**
 *  @brief          Enables/Disables do hash filter for incoming packets' destination MAC address.
 *  @details        SRS ID:            None
 *                  Design ID:         SDD_ETH_56
 *                  Sync/Async:        None
 *                  Reentrancy:        None
 *
 *  @param[in]      para_Instance_u8    Ethernet instance number.
 *
 *  @return         void
 *
 *  @note           L2 filtering and routing
 *
 *  @pre            None
 *
 */
void Eth_Ip_EnableMulticastMacAddrHashFilter(
    uint8_t para_Instance_u8
);

/**
 *  @brief          Enables/Disables pass all incoming packets mode.
 *  @details        SRS ID:            None
 *                  Design ID:         SDD_ETH_57
 *                  Sync/Async:        None
 *                  Reentrancy:        None
 *
 *  @param[in]      para_Instance_u8    Ethernet instance number.
 *  @param[in]      para_Enable_bool    If TRUE, the application will receive all incomming packets regardless of each layer filter status.
 *
 *  @return         void
 *
 *  @note           L2 filtering and routing
 *
 *  @pre            None
 *
 */
void Eth_Ip_EnableReceivePromiscuousMode(
    uint8_t para_Instance_u8,
    boolean para_Enable_bool
);

/**
 *  @brief          Update MAC address filter of the controller.
 *
 *  @details        SRS ID:            None
 *                  Design ID:         SDD_ETH_58
 *                  Sync/Async:        None
 *                  Reentrancy:        None
 *
 *  @param[in]      para_Instance_u8       Ethernet instance number.
 *  @param[in]      para_PhysAdrr_ptr      Pointer to one MAC address filter entry. The address in network byte order stored
 *                                         into 6 bytes of memory.
 *
 *  @return         void
 *
 *  @note           None
 *
 *  @pre            None
 *
 */
void Eth_Ip_UpdateMacAddr(
    uint8_t para_Instance_u8,
    const uint8_t *para_PhysAdrr_ptr
);

/**
 *  @brief          Read MAC address filter of the controller.
 *
 *  @details        SRS ID:            None
 *                  Design ID:         SDD_ETH_59
 *                  Sync/Async:        None
 *                  Reentrancy:        None
 *
 *  @param[in]      para_Instance_u8       Ethernet instance number.
 *  @param[out]     para_PhysAdrr_ptr      Point to the buffer which store the read back MAC address filter entry value.
 *
 *  @return         void
 *
 *  @note           L2 filtering and routing
 *
 *  @pre            None
 *
 */
void Eth_Ip_ReadMacAddr(
    uint8_t para_Instance_u8,
    uint8_t *para_PhysAdrr_ptr
);

/**
 *  @brief          Adds a hardware address to the hash filter. The destination address of
 *                  an incoming packet is passed through CRC logic and then compared to the
 *                  entries in the hash table.
 *
 *  @details        SRS ID:            None
 *                  Design ID:         SDD_ETH_60
 *                  Sync/Async:        None
 *                  Reentrancy:        None
 *
 *  @param[in]      para_Instance_u8    Ethernet instance number.
 *  @param[in]      para_MacAddr_ptr    The physical address of the MAC.
 *
 *  @return         void
 *
 *  @note           L2 filtering and routing
 *
 *  @pre            None
 *
 */
void Eth_Ip_AddMacAddrToHashTable(
    uint8_t para_Instance_u8,
    const uint8_t *para_MacAddr_ptr
);

/**
 *  @brief          Removes a hardware address from the hash filter. The destination
 *                  address of an incoming packet is passed through CRC logic and then compared
 *                  to the entries in the hash table.
 *
 *  @details        SRS ID:            None
 *                  Design ID:         SDD_ETH_61
 *                  Sync/Async:        None
 *                  Reentrancy:        None
 *
 *  @param[in]      para_Instance_u8    Ethernet instance number.
 *  @param[in]      para_MacAddr_ptr    The physical address of the MAC.
 *
 *  @return         void
 *
 *  @note           L2 filtering and routing
 *
 *  @pre            None
 *
 */
void Eth_Ip_RemoveMacAddrFromHashTable(
    uint8_t para_Instance_u8,
    const uint8_t *para_MacAddr_ptr
);

/**
 *  @brief          Clear all hardware address from the hash filter. The destination
 *                  address of an incoming packet is passed through CRC logic and then compared
 *                  to the entries in the hash table.
 *
 *  @details        SRS ID:            None
 *                  Design ID:         SDD_ETH_62
 *                  Sync/Async:        None
 *                  Reentrancy:        None
 *
 *  @param[in]      para_Instance_u8    Ethernet instance number.
 *
 *  @return         void
 *
 *  @note           L2 filtering and routing
 *
 *  @pre            None
 *
 */
void Eth_Ip_ClearAllMacAddrFromHashTable(
    uint8_t para_Instance_u8
);

/**
 *  @brief          Gets statistics from MMC Rx counters.
 *
 *  @details        SRS ID:            None
 *                  Design ID:         SDD_ETH_63
 *                  Sync/Async:        None
 *                  Reentrancy:        None
 *
 *  @param[in]      para_Instance_u8      Ethernet instance number.
 *  @param[out]     para_RxCounter_ptr    The pointer to struct which stored all Rx counters.
 *
 *  @return         void
 *
 *  @note           MMC Rx features
 *
 *  @pre            None
 *
 */
void Eth_Ip_GetRxCounters(
    uint8_t para_Instance_u8,
    Eth_Ip_RxCounterType *para_RxCounter_ptr
);

/**
 *  @brief          Gets statistics from MMC Tx counters.
 *
 *  @details        SRS ID:            None
 *                  Design ID:         SDD_ETH_64
 *                  Sync/Async:        None
 *                  Reentrancy:        None
 *
 *  @param[in]      para_Instance_u8      Ethernet instance number.
 *  @param[out]     para_TxCounter_ptr    The pointer to struct which stored all Tx counters.
 *
 *  @return         void
 *
 *  @note           MMC Tx features
 *
 *  @pre            None
 *
 */
void Eth_Ip_GetTxCounters(
    uint8_t para_Instance_u8,
    Eth_Ip_TxCounterType *para_TxCounter_ptr
);

/**
 *  @brief          Initializes the ETH_IP module.
 *
 *  @details        SRS ID:            None
 *                  Design ID:         SDD_ETH_65
 *                  Sync/Async:        None
 *                  Reentrancy:        None
 *
 *  @param[in]      para_Instance_u8    Ethernet instance number.
 *  @param[in]      para_Config_ptr     The structure contains the hardware controller configuration type.
 *
 *  @return         Eth_Ip_StatusType
 *  @retval         ETH_IP_STATUS_SUCCESS     Init success.
 *  @retval         ETH_IP_STATUS_TIMEOUT     Init timeout.
 *
 *  @note           None
 *
 *  @pre            None
 *
 */
Eth_Ip_StatusType Eth_Ip_Init(
    uint8_t para_Instance_u8,
    const Eth_Ip_CtrlConfigType *para_Config_ptr
);

/**
 *  @brief          Sends an Ethernet frame.
 *
 *  @details        SRS ID:            None
 *                  Design ID:         SDD_ETH_66
 *                  Sync/Async:        None
 *                  Reentrancy:        None
 *
 *  @param[in]      para_Instance_u8     Ethernet instance number.
 *  @param[in]      para_Buff_ptr        The buffer containing the frame.
 *  @param[in]      para_Length_u32      Length of frame/packet.
 *  @param[in]      para_TxOption_u32    The option when transmit packet.
 *
 *  @return         Eth_Ip_StatusType
 *  @retval         ETH_IP_STATUS_TX_QUEUE_FULL    Select Tx FIFO is full.
 *  @retval         ETH_IP_STATUS_SUCCESS          Send frame success.
 *
 *  @note           Transmit
 *
 *  @pre            None
 *
 */
Eth_Ip_StatusType Eth_Ip_SendFrame(
    uint8_t para_Instance_u8,
    uint8_t *para_Buff_ptr,
    uint32_t para_Length_u32,
    uint32_t para_TxOption_u32
);

/**
 *  @brief          Get the transmited Tx Frame status from special Tx ring.
 *
 *  @details        SRS ID:            None
 *                  Design ID:         SDD_ETH_67
 *                  Sync/Async:        None
 *                  Reentrancy:        None
 *
 *  @param[in]      para_Instance_u8     Ethernet instance number.
 *  @param[in]      para_Buff_ptr        The transmit buffer for which the status shall be checked.
 *  @param[out]     para_TxStatus_ptr    The Tx frame status.
 *
 *  @return         Eth_Ip_StatusType
 *  @retval         ETH_IP_STATUS_TX_NO_COMPLETE   Select Tx FIFO no completed Tx frames.
 *  @retval         ETH_IP_STATUS_SUCCESS          Select Tx FIFO has completed Tx frame.
 *
 *  @note           None
 *
 *  @pre            None
 *
 */
Eth_Ip_StatusType Eth_Ip_GetTxFrameStatus(
    uint8_t para_Instance_u8,
    const uint8_t *para_Buff_ptr,
    Eth_Ip_TxFrameStatusType *para_TxStatus_ptr
);

/**
 *  @brief          Gets the size of the read frame.
 *
 *  @details        SRS ID:            None
 *                  Design ID:         SDD_ETH_68
 *                  Sync/Async:        None
 *                  Reentrancy:        None
 *
 *  @param[in]      para_Instance_u8    Ethernet instance number.
 *  @param[out]     para_Length_ptr     Length of frame/packet.
 *
 *  @return         Eth_Ip_StatusType
 *  @retval         ETH_IP_STATUS_RX_QUEUE_EMPTY   Select Rx FIFO is empty.
 *  @retval         ETH_IP_STATUS_ERROR            There was an error in the latest Rx desc.
 *  @retval         ETH_IP_STATUS_SUCCESS          Select Rx FIFO has a valid Rx desc.
 *
 *  @note           Receive
 *
 *  @pre            None
 *
 */
Eth_Ip_StatusType Eth_Ip_GetRxFrameSize(
    uint8_t para_Instance_u8,
    uint32_t *para_Length_ptr
);

/**
 *  @brief          Get one received buffer in specified BD ring.
 *
 *  @details        SRS ID:            None
 *                  Design ID:         SDD_ETH_69
 *                  Sync/Async:        None
 *                  Reentrancy:        None
 *
 *  @param[in]      para_Instance_u8       Ethernet instance number.
 *  @param[out]     para_BufferDesc_ptr    Point to the Rx buffer desc structure.
 *  @param[out]     para_Buffer_ptr        Point to the Rx buffer structure.
 *
 *  @return         Eth_Ip_StatusType
 *  @retval         ETH_IP_STATUS_RX_QUEUE_EMPTY   Select Rx FIFO is empty.
 *  @retval         ETH_IP_STATUS_ERROR            There was an error in the latest Rx desc.
 *  @retval         ETH_IP_STATUS_SUCCESS          Select Rx FIFO has a valid Rx desc.
 *
 *  @note           The received frame must be store in one Rx desc
 *
 *  @pre            None
 *
 */
Eth_Ip_StatusType Eth_Ip_GetRxBuffer(
    uint8_t para_Instance_u8,
#if (STD_OFF == ETH_IP_ENABLE_ENHANCED_DESC)
    Eth_Ip_RxBufferDescriptorType *para_BufferDesc_ptr,
#else
    Eth_Ip_RxBufferEnDescriptorType *para_BufferDesc_ptr,
#endif /*!< (STD_OFF == ETH_IP_ENABLE_ENHANCED_DESC) */
    Eth_Ip_RxBufferType *para_Buffer_ptr
);

/**
 *  @brief          Setup an Rx Descriptor in specified BD ring.
 *
 *  @details        SRS ID:            None
 *                  Design ID:         SDD_ETH_70
 *                  Sync/Async:        None
 *                  Reentrancy:        None
 *
 *  @param[in]      para_Instance_u8       Ethernet instance number.
 *  @param[in]      para_Ring_u8           The ring number.
 *  @param[in]      para_BufferDesc_ptr    Point to the Rx buffer desc structure.
 *
 *  @return         void
 *
 *  @note           Receive
 *
 *  @pre            None
 *
 */
void Eth_Ip_SetupRxDescriptor(
    uint8_t para_Instance_u8,
#if (STD_OFF == ETH_IP_ENABLE_ENHANCED_DESC)
    const Eth_Ip_RxBufferDescriptorType *para_BufferDesc_ptr
#else
    const Eth_Ip_RxBufferEnDescriptorType *para_BufferDesc_ptr
#endif /*!< (STD_OFF == ETH_IP_ENABLE_ENHANCED_DESC) */
);

/**
 *  @brief          Gets the current Eth time from the PTP 1588 timer.
 *
 *  @details        SRS ID:            None
 *                  Design ID:         SDD_ETH_71
 *                  Sync/Async:        None
 *                  Reentrancy:        None
 *
 *  @param[in]      para_Instance_u8        Ethernet instance number.
 *  @param[out]     para_Second_ptr         Pointer to the buffer which stores the PTP time seconds.
 *  @param[out]     para_Nanoseconds_ptr    Pointer to the buffer which stores the PTP time nanoseconds.
 *
 *  @return         Eth_Ip_StatusType
 *  @retval         ETH_IP_STATUS_SUCCESS     Operation success.
 *  @retval         ETH_IP_STATUS_TIMEOUT     Operation timeout.
 *
 *  @note           IEEE 1588 timestamp features
 *
 *  @pre            None
 *
 */
Eth_Ip_StatusType Eth_Ip_GetSystemTimeStamp(
    uint8_t para_Instance_u8,
    uint32_t *para_Second_ptr,
    uint32_t *para_Nanoseconds_ptr
);

/**
 *  @brief          The function handle DMA Transmit interrupt.
 *
 *  @details        SRS ID:            None
 *                  Design ID:         SDD_ETH_72
 *                  Sync/Async:        None
 *                  Reentrancy:        None
 *
 *  @param[in]      para_Instance_u8    Ethernet instance number.
 *
 *  @return         void
 *
 *  @note           None
 *
 *  @pre            None
 *
 */
void Eth_Ip_TransIRQHandler(
    uint8_t para_Instance_u8
);

#ifdef __cplusplus
}
#endif
/*! @}*/

#endif /*!< ETH_IP_H */
