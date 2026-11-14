/***********************************************************************************************************************
 * Project Name: XHSC AUTOSAR MCAL
 * File Name: Eth_Ip_Types.h
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
 *   @file       Eth_Ip_Types.h
 *   @version    1.0.3
 *   @brief      ETH IP Layer Driver
 *   @addtogroup eth_ip
 *   @{
 */

#ifndef ETH_IP_TYPES_H
#define ETH_IP_TYPES_H

#ifdef __cplusplus
extern "C" {
#endif

/***********************************************************************************************************************
 *  INCLUDES
 **********************************************************************************************************************/
#include "Std_Types.h"
#include "Eth_Ip_Cfg.h"
#include "hc32_ll_eth.h"

/***********************************************************************************************************************
 *  DEFINES & MACROS
 **********************************************************************************************************************/
#define ETH_IP_MAC_ADDR_SIZE  (6U)
#define ETH_IP_IPV4_ADDR_SIZE (2U)
#define ETH_IP_LENGTH_4       (4U)
/***********************************************************************************************************************
 *  TYPEDEFS
 **********************************************************************************************************************/
/**
 *  @brief          The callback function of the DMA Tx or Rx channel transmission completion interrupt.
 *
 *  @details        SRS ID:            None
 *                  Design ID:
 *                  Sync/Async:        None
 *                  Reentrancy:        None
 *
 *  @param[in]      para_Instance_u8    Ethernet instance number.
 *  @param[in]      para_Channel_u8     The Channel index.
 *
 *  @return         void
 *
 *  @note           None
 *
 *  @pre            None
 *
 */
typedef void (*Eth_Ip_ChCallbackType)(
    uint8_t para_Instance_u8,
    uint8_t para_Channel_u8
);

/**
 *  @brief   Generic status return codes.
 *  @details SRS ID:     None
 *           Design ID:
 */
typedef enum
{
    ETH_IP_STATUS_SUCCESS             = 0x000U, /*!< Generic operation success status. */
    ETH_IP_STATUS_ERROR               = 0x001U, /*!< Generic operation failure status. */
    ETH_IP_STATUS_BUSY                = 0x002U, /*!< Generic operation busy status. */
    ETH_IP_STATUS_TIMEOUT             = 0x003U, /*!< Generic operation timeout status. */
    ETH_IP_STATUS_UNSUPPORTED         = 0x004U, /*!< Generic operation unsupported status. */
    ETH_IP_STATUS_TX_QUEUE_FULL       = 0x005U, /*!< Tx queue full */
    ETH_IP_STATUS_TX_NO_COMPLETE      = 0x006U, /*!< No complete frame has been sent yet. */
    ETH_IP_STATUS_TX_QUEUE_NOT_ENOUGH = 0x007U, /*!< Tx Queue not enough to send that many buffers. */
    ETH_IP_STATUS_RX_QUEUE_EMPTY      = 0x008U, /*!< Rx queue empty */
    ETH_IP_STATUS_RX_BUFFER_OVERFLOW  = 0x009U  /*!< Provide Rx buffer not enough to hold the frame data */
} Eth_Ip_StatusType;

/**
 *  @brief   Defines for State Ctrl Mode.
 *  @details SRS ID:     None
 *           Design ID:
 */
typedef enum
{
    ETH_IP_CTRL_MODE_INACTIVE   = 0x00U, /*!< The Transmitter/Receiver are inactive */
    ETH_IP_CTRL_MODE_ACTIVE     = 0x01U, /*!< The Transmitter/Receiver are active */
    ETH_IP_CTRL_MODE_RESET      = 0x02U, /*!< The Eth module is reset. */
    ETH_IP_CTRL_MODE_NOT_ACCESS = 0x03U  /*!< The Eth module cannot be access due to missing clock */
} Eth_Ip_CtrlModeType;

/**
 *  @brief   Available Tx threshold values.
 *  @details SRS ID:     None
 *           Design ID:
 */
typedef enum
{
    ETH_IP_TX_THRESHOLD_16_BYTES  = 0x07U, /*!< Tx Threshold 16 bytes. */
    ETH_IP_TX_THRESHOLD_24_BYTES  = 0x06U, /*!< Tx Threshold 24 bytes. */
    ETH_IP_TX_THRESHOLD_32_BYTES  = 0x05U, /*!< Tx Threshold 32 bytes. */
    ETH_IP_TX_THRESHOLD_40_BYTES  = 0x04U, /*!< Tx Threshold 40 bytes. */
    ETH_IP_TX_THRESHOLD_64_BYTES  = 0x00U, /*!< Tx Threshold 64 bytes. */
    ETH_IP_TX_THRESHOLD_128_BYTES = 0x01U, /*!< Tx Threshold 128 bytes. */
    ETH_IP_TX_THRESHOLD_192_BYTES = 0x02U, /*!< Tx Threshold 192 bytes. */
    ETH_IP_TX_THRESHOLD_256_BYTES = 0x03U  /*!< Tx Threshold 256 bytes. */
} Eth_Ip_TxThresholdType;

/**
 *  @brief   Defines the Rx Threshold.
 *  @details SRS ID:     None
 *           Design ID:
 */
typedef enum
{
    ETH_IP_RX_THRESHOLD_64_BYTES  = 0x00U, /*!< Rx Threshold 64 bytes. */
    ETH_IP_RX_THRESHOLD_32_BYTES  = 0x01U, /*!< Rx Threshold 32 bytes. */
    ETH_IP_RX_THRESHOLD_96_BYTES  = 0x02U, /*!< Rx Threshold 96 bytes. */
    ETH_IP_RX_THRESHOLD_128_BYTES = 0x03U  /*!< Rx Threshold 128 bytes. */
} Eth_Ip_RxThresholdType;

/**
 *  @brief   Media Independent Interface selection.
 *  @details SRS ID:     xxx
 *           Design ID:
 */
typedef enum
{
    ETH_IP_INTERFACE_MII  = 0X0, /*!< MII interface. */
    ETH_IP_INTERFACE_RMII        /*!< RMII interface. */
} Eth_Ip_InterfaceType;

/**
 *  @brief   Media Independent Interface speed selection.
 *  @details SRS ID:     None
 *           Design ID:
 */
typedef enum
{
    ETH_IP_SPEED_10M  = 0x0000U, /*!< Speed 10 Mbps. */
    ETH_IP_SPEED_100M = 0x4000U  /*!< Speed 100 Mbps. */
} Eth_Ip_SpeedType;

/**
 *  @brief   VLAN tag and type deletion, insertion, or replacement control for transmitted packets.
 *  @details SRS ID:     None
 *           Design ID:
 */
typedef enum
{
    ETH_IP_VLAN_TX_CONTROL_NO          = 0x00U, /*!< No Vlan tag deletion, insertion, or replacement. */
    ETH_IP_VLAN_TX_CONTROL_DELETION    = 0x01U, /*!< Vlan tag deletion */
    ETH_IP_VLAN_TX_CONTROL_INSERTION   = 0x02U, /*!< Vlan tag insertion */
    ETH_IP_VLAN_TX_CONTROL_REPLACEMENT = 0x03U  /*!< Vlan tag replacement. */
} Eth_Ip_VlanTxControlType;

/**
 *  @brief   VLAN PCP Enumeration.
 *  @details SRS ID:     None
 *           Design ID:
 */
typedef enum
{
    ETH_IP_VLAN_PRIORITY_0   = 0x01U, /*!< VLAN priority 0 */
    ETH_IP_VLAN_PRIORITY_1   = 0x02U, /*!< VLAN priority 1 */
    ETH_IP_VLAN_PRIORITY_2   = 0x04U, /*!< VLAN priority 2 */
    ETH_IP_VLAN_PRIORITY_3   = 0x08U, /*!< VLAN priority 3 */
    ETH_IP_VLAN_PRIORITY_4   = 0x10U, /*!< VLAN priority 4 */
    ETH_IP_VLAN_PRIORITY_5   = 0x20U, /*!< VLAN priority 5 */
    ETH_IP_VLAN_PRIORITY_6   = 0x40U, /*!< VLAN priority 6 */
    ETH_IP_VLAN_PRIORITY_7   = 0x80U, /*!< VLAN priority 7 */
    ETH_IP_VLAN_PRIORITY_ALL = 0xFFU  /*!< VLAN priority all */
} Eth_Ip_VlanPriorityType;

/**
 *  @brief   Defines the system time rollover control.
 *  @details SRS ID:     None
 *           Design ID:
 */
typedef enum
{
    ETH_IP_TS_ROLLOVER_BINARY  = 0x00U, /*!< System time binary rollover.*/
    ETH_IP_TS_ROLLOVER_DIGITAL = 0x01U  /*!< System time digital rollover.*/
} Eth_Ip_TsRolloverType;

/**
 *  @brief   Defines the MAC address mask byte.
 *  @details SRS ID:     None
 *           Design ID:
 */
typedef enum
{
    ETH_IP_MAC_ADDR_MASK_BYTE_NO = 0x00U, /*!< No MAC address byte mask */
    ETH_IP_MAC_ADDR_MASK_BYTE_0  = 0x01U, /*!< MAC address byte 0 mask */
    ETH_IP_MAC_ADDR_MASK_BYTE_1  = 0x02U, /*!< MAC address byte 1 mask */
    ETH_IP_MAC_ADDR_MASK_BYTE_2  = 0x04U, /*!< MAC address byte 2 mask */
    ETH_IP_MAC_ADDR_MASK_BYTE_3  = 0x08U, /*!< MAC address byte 3 mask */
    ETH_IP_MAC_ADDR_MASK_BYTE_4  = 0x10U, /*!< MAC address byte 4 mask */
    ETH_IP_MAC_ADDR_MASK_BYTE_5  = 0x20U  /*!< MAC address byte 5 mask */
} Eth_Ip_MacAddrMaskByteType;

/**
 *  @brief   Defines the types of MMC Rx Counter.
 *  @details SRS ID:     None
 *           Design ID:
 */
typedef struct
{
    uint32_t RxUnicastPacketsGood_u32;    /*!< Number of Receive Unicast Packets Good */
    uint32_t RxMulticastPacketsGood_u32;  /*!< Number of Receive Multicast Packets Good */
    uint32_t RxBroadcastPacketsGood_u32;  /*!< Number of Receive Broadcast Packets Good */
    uint32_t RxCrcErrorPackets_u32;       /*!< Number of Receive CRC Error Packets. */
    uint32_t RxAlignmentErrorPackets_u32; /*!< Number of Receive Alignment Error Packets. */
    uint32_t RxRuntErrorPackets_u32;      /*!< Number of Receive Runt Error Packets. */
    uint32_t RxLengthErrorPackets_u32;    /*!< Number of Receive Length Error Packets. */
    uint32_t RxOutOfRangeTypePackets_u32; /*!< Number of Receive Out Of Range Type Packets. */
} Eth_Ip_RxCounterType;

/**
 *  @brief   Defines the types of MMC Tx Counter.
 *  @details SRS ID:     None
 *           Design ID:
 */
typedef struct
{
    uint32_t TxUnicastPacketsGood_u32;      /*!< Number of Receive Unicast Packets Good */
    uint32_t TxMulticastPacketsGood_u32;    /*!< Transmit Multicast Packets Good */
    uint32_t TxBroadcastPacketsGood_u32;    /*!< Transmit Broadcast Packets Good */
    uint32_t TxDeferredErrorFrame_u32;      /*!< Transmit Multicast Packets Good Bad */
    uint32_t TxCarrierErrorFrame_u32;       /*!< Transmit Broadcast Packets Good Bad */
    uint32_t TxLateCollisionFrame_u32;      /*!< Transmit Broadcast Packets Good Bad */
    uint32_t TxExcessiveCollisionFrame_u32; /*!< Transmit Broadcast Packets Good Bad */
    uint32_t TxExcessiveDelayFrame_u32;     /*!< Transmit Broadcast Packets Good Bad */
} Eth_Ip_TxCounterType;

/**
 *  @brief   Defines Csr divide factor of MDC clock.
 *  @details SRS ID:     None
 *           Design ID:
 */
typedef enum
{
    ETH_IP_CSR_CLOCK_DIV_20_35M   = 0x02U, /*!< Used for CSR clock = 20-35 MHz; MDC clock = CSR clock/16. */
    ETH_IP_CSR_CLOCK_DIV_35_60M   = 0x03U, /*!< Used for CSR clock = 35-60 MHz; MDC clock = CSR clock/26. */
    ETH_IP_CSR_CLOCK_DIV_60_100M  = 0x00U, /*!< Used for CSR clock = 60-100 MHz; MDC clock = CSR clock/42. */
    ETH_IP_CSR_CLOCK_DIV_100_120M = 0x01U  /*!< Used for CSR clock = 100-120 MHz; MDC clock = CSR clock/62. */
} Eth_Ip_CsrClockDivType;

/**
 *  @brief   Defines for Layer 3.
 *  @details SRS ID:     None
 *           Design ID:
 */
typedef enum
{
    ETH_IP_LAYER3_IPV4 = 0x0U, /*!< Layer 3 IPv4 refers to the network layer protocol in the Internet Protocol version 4 (IPv4) suite. */
    ETH_IP_LAYER3_IPV6 = 0x1U  /*!< Layer 3 IPv6 refers to the network layer protocol in the Internet Protocol version 6 (IPv6) suite. */
} Eth_Ip_Layer3Type;

/**
 *  @brief   Defines for Layer 4.
 *  @details SRS ID:     None
 *           Design ID:
 */
typedef enum
{
    ETH_IP_LAYER4_TCP = 0x0U, /*!< Layer 4 TCP, also known as Transmission Control Protocol, is a transport layer protocol in the TCP/IP suite. */
    ETH_IP_LAYER4_UDP = 0x1U  /*!< Layer 4 UDP, or User Datagram Protocol, is a transport layer protocol in the TCP/IP suite. */
} Eth_Ip_Layer4Type;

/**
 *  @brief   Defines mode of the MAC Address receive all mode.
 *  @details SRS ID:     None
 *           Design ID:
 */
typedef enum
{
    ETH_IP_RECEIVE_ALL_DISABLE = 0x00U, /*!< Receive frames base on the each Layer filtering status. */
    ETH_IP_RECEIVE_ALL_WITH_STATUS =
        0x01U, /*!< Receive all frames with updat eeach Layer filtering status in Rx status word. */
    ETH_IP_RECEIVE_ALL_WITHOUT_STATUS =
        0x02U  /*!< Receive all frames without update each Layer filtering status in Rx status word (Promiscuous Mode). */
} Eth_Ip_ReceiveAllType;

/**
 *  @brief   Defines the maximum size for spliting the Header data.
 *  @details SRS ID:     None
 *           Design ID:
 */
typedef enum
{
    ETH_IP_SPLIT_HEADER_SIZE_64_BYTES   = 0x00U, /*!< The maximum header size for spliting is 64 bytes. */
    ETH_IP_SPLIT_HEADER_SIZE_128_BYTES  = 0x01U, /*!< The maximum header size for spliting is 128 bytes. */
    ETH_IP_SPLIT_HEADER_SIZE_256_BYTES  = 0x02U, /*!< The maximum header size for spliting is 256 bytes. */
    ETH_IP_SPLIT_HEADER_SIZE_512_BYTES  = 0x03U, /*!< The maximum header size for spliting is 512 bytes. */
    ETH_IP_SPLIT_HEADER_SIZE_1024_BYTES = 0x04U  /*!< The maximum header size for spliting is 1024 bytes. */
} Eth_Ip_SplitHeaderSizeType;

/**
 *  @brief   Defines the Receive interrupt Watchdog timer count units.
 *  @details SRS ID:     None
 *           Design ID:
 */
typedef enum
{
    ETH_IP_RX_INTR_WD_TIMER_COUNT_UNIT_256  = 0x00U, /*!< The Receive interrupt watchdog timeout count 256 units. */
    ETH_IP_RX_INTR_WD_TIMER_COUNT_UNIT_512  = 0x01U, /*!< The Receive interrupt watchdog timeout count 512 units. */
    ETH_IP_RX_INTR_WD_TIMER_COUNT_UNIT_1024 = 0x02U, /*!< The Receive interrupt watchdog timeout count 1024 units. */
    ETH_IP_RX_INTR_WD_TIMER_COUNT_UNIT_2048 = 0x03U  /*!< The Receive interrupt watchdog timeout count 2048 units. */
} Eth_Ip_RxIntrWdTimerCountUnitType;

/**
 *  @brief   Defines the Get status interrupt type.
 *  @details SRS ID:     None
 *           Design ID:
 */
typedef enum
{
    ETH_IP_INTERRUPT_STATUS_MAC_TYPE         = 0x00U, /*!< Get the status of MAC interrupt. */
    ETH_IP_INTERRUPT_STATUS_MMC_RX_TYPE      = 0x01U, /*!< Get the status of MMC RX interrupt. */
    ETH_IP_INTERRUPT_STATUS_MMC_TX_TYPE      = 0x02U, /*!< Get the status of MMC TX interrupt. */
    ETH_IP_INTERRUPT_STATUS_MMC_IPC_RX_TYPE  = 0x03U, /*!< Get the status of MMC IPC RX interrupt. */
    ETH_IP_INTERRUPT_STATUS_MMC_FPE_TX_TYPE  = 0x04U, /*!< Get the status of MMC FPE TX interrupt. */
    ETH_IP_INTERRUPT_STATUS_MMC_FPE_RX_TYPE  = 0x05U, /*!< Get the status of MMC FPE RX interrupt. */
    ETH_IP_INTERRUPT_STATUS_MTL_EST_TYPE     = 0x06U, /*!< Get the status of MTL EST interrupt. */
    ETH_IP_INTERRUPT_STATUS_MTL_RXP_TYPE     = 0x07U, /*!< Get the status of MTL RXP interrupt. */
    ETH_IP_INTERRUPT_STATUS_MTL_ECC_TYPE     = 0x08U, /*!< Get the status of MTL RCC interrupt. */
    ETH_IP_INTERRUPT_STATUS_MTL_QUEUE_TYPE   = 0x09U, /*!< Get the status of MTL Queue interrupt. */
    ETH_IP_INTERRUPT_STATUS_DMA_ECC_TYPE     = 0x0AU, /*!< Get the status of DMA ECC interrupt. */
    ETH_IP_INTERRUPT_STATUS_DMA_CHANNEL_TYPE = 0x0BU, /*!< Get the status of DMA Channel interrupt. */
    ETH_IP_INTERRUPT_STATUS_PCTH_TYPE        = 0x0CU, /*!< Get the status of policing counters threshold based interrupts. */
    ETH_IP_INTERRUPT_STATUS_PCTW_TYPE        = 0x0DU  /*!< Get the status of policing counters Time window based interrupt. */
} Eth_Ip_InterruptStatusType;

/**
 *  @brief   Defines the Tx error type in Tx desc.
 *  @details SRS ID:     None
 *           Design ID:
 */
typedef enum
{
    ETH_IP_TX_ERROR_IP_HEADER           = 0x000001U, /*!< IP header error. */
    ETH_IP_TX_ERROR_UNDERFLOW           = 0x000004U, /*!< Underflow Error. */
    ETH_IP_TX_ERROR_EXCESSIVE_DEFERRAL  = 0x000008U, /*!< Excessive Deferral error. */
    ETH_IP_TX_ERROR_EXCESSIVE_COLLISION = 0x000100U, /*!< Excessive Collision error. */
    ETH_IP_TX_ERROR_LATE_COLLISION      = 0x000200U, /*!< Late Collision error. */
    ETH_IP_TX_ERROR_NO_CARRIER          = 0x000400U, /*!< No Carrier error. */
    ETH_IP_TX_ERROR_LOSS_OF_CARRIER     = 0x000800U, /*!< Loss of Carrier error. */
    ETH_IP_TX_ERROR_PAYLOAD_CHECKSUM    = 0x001000U, /*!< Payload checksum error. */
    ETH_IP_TX_ERROR_PACKET_FLUSHED      = 0x002000U, /*!< Packet Flushed error. */
    ETH_IP_TX_ERROR_JABBER_TIMEOUT      = 0x004000U, /*!< Jabber Timeout error. */
    ETH_IP_TX_ERROR_ERROR_SUMMARY       = 0x008000U, /*!< Error Summary. */
    ETH_IP_TX_ERROR_DESCRIPTOR          = 0x800000U  /*!< Descriptor Error. */
} Eth_Ip_TxErrorType;

/**
 *  @brief   Defines the Rx error type in Rx desc.
 *  @details SRS ID:     None
 *           Design ID:
 */
typedef enum
{
    ETH_IP_RX_ERROR_ERROR_SUMMARY       = 0x0008000U, /*!< Error summary. */
    ETH_IP_RX_ERROR_DRIBBLE_BIT         = 0x0080000U, /*!< Dribble bit Error. */
    ETH_IP_RX_ERROR_RECEIVE             = 0x0100000U, /*!< Receive error. */
    ETH_IP_RX_ERROR_OVERFLOW            = 0x0200000U, /*!< Overflow error. */
    ETH_IP_RX_ERROR_RX_WATCHDOG_TIMEOUT = 0x0400000U, /*!< Receive watchdog timeout error. */
    ETH_IP_RX_ERROR_GIANT_PACKET        = 0x0800000U, /*!< Giant packet error. */
    ETH_IP_RX_ERROR_CRC                 = 0x1000000U  /*!< CRC error. */
} Eth_Ip_RxErrorType;

/**
 *  @brief   Defines the VLAN configuration structure.
 *  @details SRS ID:     None
 *           Design ID:
 */
typedef struct
{
    Eth_Ip_VlanTxControlType VlanTxControl_en; /*!< Selects if VLAN tag are deleted, inserted or replaced in transmitted packets. */
    uint16_t VlanTag_u16;                        /*!< Inserted/Replaced VLAN Tag value */
} Eth_Ip_VlanConfigType;

/**
 *  @brief   Defines the Eth PTP configuration structure.
 *  @details SRS ID:     None
 *           Design ID:
 */
typedef struct
{
    Eth_Ip_TsRolloverType TsRollover_en; /*!< Select 1588 time nanosecond rollover type. */
    boolean EnSnapshotAllPackets_bool;   /*!< Enable timestamp snapshot for all received packets. */
    uint32_t SecondsUpdate_u32;            /*!< The second to update system timestamp when initialize PTP. */
    uint32_t NanoSecondsUpdate_u32;        /*!< The nanosecond to update system timestamp when initialize PTP. The 31 bit set means subtract time,
                                             31 bit clear means add time. The 0 ~ 30 bit range in 0 ~ 999999999. */
    uint32_t RefClockHz_u32;               /*!< PTP External or Internal Reference clock value. */
} Eth_Ip_PtpConfigType;

/**
 *  @brief   Defines the Eth NAC configuration structure.
 *  @details SRS ID:     None
 *           Design ID:
 */
typedef struct
{
    boolean DisWatchdog_bool;        /*!< The MAC disables the watchdog timer on the receiver. The MAC can receive packets of up to 16383 bytes. */
    boolean DisJabber_bool;          /*!< The MAC disables the jabber timer on the transmitter. The MAC can transfer packets of up to 16383 bytes.*/
    uint16_t InterPacketGap_u16;       /*!< The minimum Inter-Packet Gap between transmission packets , rang is 40 ~ 96, and should be 8 bits aligned. */
    boolean EnLoopbackMode_bool;     /*!< The MAC operation in the loopback mode at MII. */
    boolean EnFullDuplexMode_bool;   /*!< The MAC operation in the full-duplex mode at MII. */
    boolean EnChecksumOffload_bool;  /*!< Enables the IPv4 header checksum checking and IPv4 or IPv6 TCP, UDP, or ICMP payload checksum checking. */
    boolean DisBroadcastPacket_bool; /*!< When true, blocks all incoming broadcast packest, otherwise pass all broadcast. */
} Eth_Ip_MacConfigType;

/**
 *  @brief   Defines the ETH IP PTP time stamp structure.
 *  @details SRS ID:     None
 *           Design ID:
 */
typedef struct
{
    uint64_t Second_u64;     /*!< Second */
    uint32_t Nanosecond_u32; /*!< Nanosecond */
} Eth_Ip_PtpTimeInfoType;

/**
 *  @brief   Send internal buffer descriptor.
 *  @details SRS ID:     None
 *           Design ID:
 */
typedef struct
{
    uint32_t TxControlStat_u32; /*!< TDES control and status word */
    uint32_t TxBuffLen_u32;     /*!< Buffer 1/2 byte counts. */
    uint32_t TxBuff1Addr_u32;   /*!< Buffer 1 address. */
    uint32_t TxBuff2Addr_u32;   /*!< Buffer 2 address or next buffer descriptor address. */
} Eth_Ip_TxBufferDescriptorType;

/**
 *  @brief   Enhanced Ring descriptor.
 *  @details SRS ID:     None
 *           Design ID:
 */
typedef struct
{
    uint32_t TxControlStat_u32; /*!< TDES control and status word */
    uint32_t TxBuffLen_u32;     /*!< Buffer 1/2 byte counts. */
    uint32_t TxBuff1Addr_u32;   /*!< Buffer 1 address. */
    uint32_t TxBuff2Addr_u32;   /*!< Buffer 2 address or next buffer descriptor address. */
    uint32_t Reserved1_u32;     /*!< Reserved 1 */
    uint32_t Reserved2_u32;     /*!< Reserved 2 */
    uint32_t TimeStampLow_u32;  /*!< TimeStamp Low */
    uint32_t TimeStampHigh_u32; /*!< TimeStamp High */
} Eth_Ip_TxBufferEnDescriptorType;

/**
 *  @brief   Send Tx frame status.
 *  @details SRS ID:     None
 *           Design ID:
 */
typedef struct
{
    Eth_Ip_PtpTimeInfoType TxTimeStamp_st; /*!< Tx timestamp value */
    uint32_t ErrStatus_u32;                  /*!< Tx error status, logical OR of "Eth_Ip_TxErrorType" */
} Eth_Ip_TxFrameStatusType;

/**
 *  @brief   Receive internal buffer descriptor.
 *  @details SRS ID:     None
 *           Design ID:
 */
typedef struct
{
    uint32_t RxControl_u32;   /*!< Buffer 1/2 byte counts and control */
    uint32_t RxBuffLen_u32;   /*!< Buffer 1/2 byte counts. */
    uint32_t RxBuff1Addr_u32; /*!< Buffer 1 address. */
    uint32_t RxBuff2Addr_u32; /*!< Buffer 2 or next descriptor address. */
} Eth_Ip_RxBufferDescriptorType;

/**
 *  @brief   Enhanced Ring descriptor.
 *  @details SRS ID:     None
 *           Design ID:
 */
typedef struct
{
    uint32_t RxControl_u32;     /*!< Buffer 1/2 byte counts and control */
    uint32_t RxBuffLen_u32;     /*!< Buffer 1/2 byte counts. */
    uint32_t RxBuff1Addr_u32;   /*!< Buffer 1 address. */
    uint32_t RxBuff2Addr_u32;   /*!< Buffer 2 or next descriptor address. */
    uint32_t RxExtStatus_u32;   /*!< Reserved 1 */
    uint32_t Reserved2_u32;     /*!< Reserved 2 */
    uint32_t TimeStampLow_u32;  /*!< TimeStamp Low */
    uint32_t TimeStampHigh_u32; /*!< TimeStamp High */
} Eth_Ip_RxBufferEnDescriptorType;

/**
 *  @brief   Defines the ETH IP Tx channel (DMA + MTL) configuration structure.
 *  @details SRS ID:     None
 *           Design ID:
 */
typedef struct
{
#if (STD_OFF == ETH_IP_ENABLE_ENHANCED_DESC)
    Eth_Ip_TxBufferDescriptorType *TxDescAddr_ptr;     /*!< Aligned transmit descriptor start address. */
#else
    Eth_Ip_TxBufferEnDescriptorType *TxEnDescAddr_ptr; /*!< Aligned Enhanced transmit descriptor start address. */
#endif /*!< (STD_OFF == ETH_IP_ENABLE_ENHANCED_DESC) */
    uint8_t **TxBuffer_pp;                               /*!< Record the Tx BD transmit buffer start address. */
    boolean EnTxInterrupt_bool;                        /*!< Enable Tx ring transmit Interrupt. */
    uint16_t DescNum_16;                                 /*!< Transmit descriptors number, and each descriptors points to buffer in buffer data pool. */
    boolean EnStoreForward_bool;                       /*!< Enable transmission starts when a full packet resides in the MTL Tx queue */
    Eth_Ip_TxThresholdType ThresholdLevel_en;          /*!< Threshold level of the MTL Tx Queue, used only when EnStoreForward_bool is FALSE. */
    Eth_Ip_ChCallbackType Callback_pfn;                    /*!< Callback function for current channel. */
} Eth_Ip_TxChannelConfigType;

/**
 *  @brief   Defines the ETH IP Rx channel (DMA + MTL) configuration structure.
 *  @details SRS ID:     None
 *           Design ID:
 */
typedef struct
{
#if (STD_OFF == ETH_IP_ENABLE_ENHANCED_DESC)
    Eth_Ip_RxBufferDescriptorType *RxDescAddr_ptr;     /*!< Aligned receive descriptor start address. */
#else
    Eth_Ip_RxBufferEnDescriptorType *RxEnDescAddr_ptr; /*!< Aligned Enhanced receive descriptor start address. */
#endif /*!< (STD_OFF == ETH_IP_ENABLE_ENHANCED_DESC) */
    boolean EnRxInterrupt_bool;                        /*!< Enable Rx ring receive interrupt. */
    uint16_t DescNum_16;                                 /*!< Receive descriptors number (each descriptors points to a ore/two buffer in buffer data pool. */
    uint8_t *Buffer_ptr;                                 /*!< Receive buffer data pool start address. */
    uint16_t BuffLen_u16;                                /*!< Length of each individual buffer in buffer data pool, and this length must be 4-byte aligned. */
    boolean EnStoreForward_bool;                       /*!< Enable reads a packet from the Rx queue only after the complete packet has been written. */
    Eth_Ip_RxThresholdType ThresholdLevel_en;          /*!< Threshold level of the MTL Rx Queue, used only when EnStoreForward_bool is FALSE. */
    boolean DisDropChecksumErrPackets_bool;            /*!< Disable dropping of TCP/IP Error checksum packets. */
    boolean EnFwUndersizeGoodPackets_bool;             /*!< The Rx queue forwards the undersized good packets (packets with no error and length less than 64 bytes),
                                                           including pad-bytes and CRC. */
    Eth_Ip_ChCallbackType Callback_pfn;                /*!< Callback function for current channel. */
} Eth_Ip_RxChannelConfigType;

/**
 *  @brief   Defines the ETH IP module normal configuration structure.
 *  @details SRS ID:     None
 *           Design ID:
 */
typedef struct
{
    Eth_Ip_InterfaceType Interface_en;      /*!< Configures the MII interface type. */
    Eth_Ip_SpeedType Speed_en;              /*!< Configures the controller Speed. */
    Eth_Ip_CsrClockDivType CsrClockDiv_en;  /*!< The divide between CSR clock and MDC clock */
    const uint8_t *PhysAddr_ptr;              /*!< The pointer containing the configuration of the controller Phy Address
                                                (hardware MAC address register 0). */
    const Eth_Ip_MacConfigType *MacCfg_ptr; /*!< The pointer containing the configuration of the MAC.*/
} Eth_Ip_NormalConfigType;

/**
 *  @brief   Defines the ETH IP transmit buffer descriptor ringqueue structure.
 *  @details SRS ID:     None
 *           Design ID:
 */
typedef struct
{
#if (STD_OFF == ETH_IP_ENABLE_ENHANCED_DESC)
    Eth_Ip_TxBufferDescriptorType *TxBdBase_ptr;     /*!< Buffer descriptor base address pointer. */
#else
    Eth_Ip_TxBufferEnDescriptorType *TxEnBdBase_ptr; /*!< Enhanced Buffer descriptor base address pointer. */
#endif /*!< (STD_OFF == ETH_IP_ENABLE_ENHANCED_DESC) */
    uint8_t **TxBuffer_pp;                             /*!< Record the Tx BD transmit buffer start address. */
    uint16_t TxGenIdx_u16;                             /*!< The current available Tx descriptor index. */
    uint16_t TxRingLen_u16;                            /*!< Tx ring length. */
    boolean EnTxInterrupt_bool;                      /*!< Tx descriptor interrupt enable/disable status. */
} Eth_Ip_TxBdRingType;

/**
 *  @brief   Defines the ETH IP receive buffer descriptor ringqueue structure.
 *  @details SRS ID:     None
 *           Design ID:
 */
typedef struct
{
#if (STD_OFF == ETH_IP_ENABLE_ENHANCED_DESC)
    Eth_Ip_RxBufferDescriptorType *RxBdBase_ptr;     /*!< Buffer descriptor base address pointer. */
#else
    Eth_Ip_RxBufferEnDescriptorType *RxEnBdBase_ptr; /*!< Enhanced Buffer descriptor base address pointer. */
#endif /*!< (STD_OFF == ETH_IP_ENABLE_ENHANCED_DESC) */
    uint16_t RxGenIdx_u16;                             /*!< The current available Rx descriptor index. */
    uint16_t RxConsumIdx_u16;                          /*!< Rx consume index. */
    uint16_t RxRingLen_u16;                            /*!< Rx ring length. */
    uint16_t RxBuffLen_u16;                            /*!< Length of each individual buffer in buffer data pool. */
    uint8_t *RxBuffer_ptr;                             /*!< Buffer data pool start address. */
    boolean EnRxInterrupt_bool;                      /*!< Rx descriptor interrupt enable/disable status. */
} Eth_Ip_RxBdRingType;

/**
 *  @brief   Defines an Rx buffer structure.
 *  @details SRS ID:     None
 *           Design ID:
 */
typedef struct
{
    uint8_t *Buffer_ptr; /*!< The buffer store the whole or partial frame. */
    uint16_t Length_u16; /*!< The byte length of this buffer. */
} Eth_Ip_RxBufferType;

/**
 *  @brief   Defines the Vlan Filter Config.
 *  @details SRS ID:     None
 *           Design ID:
 */
typedef struct
{
    boolean EnVlanTagInverseMatch_bool;   /*!< When true, enables VLAN tag inverse filter match. */
    boolean En12bitVlanComparison_bool;   /*!< When true, enable 12-bit VLAN Tag comparison, otherwise use 16-bit VLAN tag comparison. */
    uint16_t VlanTag_u16;                   /*!< VLAN Tag Id used for current VLAN Tag filter entry. */
    boolean EnVlanTagHashTableMatch_bool; /*!< When true, the most significant four bits of CRC of VLAN Tag are used index the context of
                                                the MAC_VLAN_HASH_TABLE register. When false, the VLAN Hash Match operation is not performed */
    uint32_t HashVlanNum_u32;               /*!< The number of the VLAN tag value which want to add to the hash filter table. */
    const uint16_t *HashVlan_ptr;           /*!< Point to the VLAN tag which want to add to the hash filter table. */
} Eth_Ip_VlanFilterConfigType;

/**
 *  @brief   Defines the Layer 3 filter configuration structure.
 *  @details SRS ID:     None
 *           Design ID:
 */
typedef struct
{
    Eth_Ip_Layer3Type L3Type_en; /*!< Type of L3 layer used for filtering */
    union
    {
        uint8_t L3Ipv4HigherBitsMatch_aa[ETH_IP_IPV4_ADDR_SIZE]; /*!< Layer 3 IPv4 Address Higher Bits Match, Index 0 is Source Address,
                                               index 1 isDestination Address (0x00 <= n <= 0x1F). */
        uint16_t L3Ipv6HigherBitsMatch_u16;   /*!< Layer 3 IPv6 Source or Destination Address Higher Bits Match, (0x00 <= n
                                               <= 0x3F). */
    } BitMask;
    boolean EnL3SrcAddrFilter_bool;   /*!< Layer 3 Source Address Match Enable */
    boolean EnL3DesAddrFilter_bool;   /*!< Layer 3 Destination Address Match Enable */
    boolean EnL3SrcAddrInvMatch_bool; /*!< Layer 3 Source Address Inverse Match Enable, only valid when
                                         EnL3SrcAddrFilter_bool is TRUE */
    boolean EnL3DesAddrInvMatch_bool; /*!< Layer 3 Destination Address Inverse Match Enable, only valid when
                                         EnL3DesAddrFilter_bool is TRUE */
    union
    {
        uint32_t Ipv4Address_aa[ETH_IP_IPV4_ADDR_SIZE]; /*!< IPV4 filtering value, Index 0 is used for 32-bit source address, index 1 is used
                                      for destination address. */
        uint32_t Ipv6Address_aa[ETH_IP_LENGTH_4]; /*!< IPV6 filtering value, 128-bit source address or 128-bit destination address. */
    } FilterValue;
} Eth_Ip_Layer3FilterConfigType;

/**
 *  @brief   Defines the Layer 4 filter configuration structure.
 *  @details SRS ID:     None
 *           Design ID:
 */
typedef struct
{
    Eth_Ip_Layer4Type L4Type_en;      /*!< Type of L4 layer used for filtering */
    boolean EnL4SrcPortFilter_bool;   /*!< Layer 4 Source Port Match Enable */
    boolean EnL4DesPortFilter_bool;   /*!< Layer 4 Destination Port Match Enable */
    boolean EnL4SrcPortInvMatch_bool; /*!< Layer 4 Source Port Inverse Match Enable, only valid when
                                         EnL4SrcPortFilter_bool is TRUE */
    boolean EnL4DesPortInvMatch_bool; /*!< Layer 4 Destination Port Inverse Match Enable, only valid when
                                         EnL4DesPortFilter_bool is TRUE */
    uint16_t L4SrcPortNum_u16;          /*!< 16-bit Source Port number which used for Layer 4 Source Port Match */
    uint16_t L4DesPortNum_u16;          /*!< 16-bit Destination Port number which used for Layer 4 Destination Port Match */
} Eth_Ip_Layer4FilterConfigType;

/**
 *  @brief   Defines the Layer 3 and Layer 4 packet filtering.
 *  @details SRS ID:     None
 *           Design ID:
 */
typedef struct
{
    boolean DropNonTcpUdpIpPacket_bool;                              /*!< The MAC drops non the TCP or UDP over IP packet. */
    const Eth_Ip_Layer3FilterConfigType *Layer3FilterConfigType_ptr; /*!< The configure parameter for Layer 3 filter. */
    const Eth_Ip_Layer4FilterConfigType *Layer4FilterConfigType_ptr; /*!< The configure parameter for Layer 4 filter. */
} Eth_Ip_Layer3Layer4FilterConfigType;

/**
 *  @brief   Defines the ETH IP state structure.
 *  @details SRS ID:     None
 *           Design ID:
 */
typedef struct
{
    Eth_Ip_RxBdRingType RxBdRing_st;     /*!< Receive buffer descriptor.  */
    Eth_Ip_TxBdRingType TxBdRing_st;     /*!< Transmit buffer descriptor.  */
    Eth_Ip_ChCallbackType RxCallback_pfn; /*!< Callback function for Receive channel transmission completion. */
    Eth_Ip_ChCallbackType TxCallback_pfn; /*!< Callback function for Transmit channel transmission completion. */
} Eth_Ip_StateType;

/**
 *  @brief   Define the ETH hardware controller configuration type.
 *  @details SRS ID:     None
 *           Design ID:
 */
typedef struct
{
    const Eth_Ip_NormalConfigType *NormalCfg_ptr;                 /*!< The pointer containing the configuration of the Ctrl normal part. */
    const Eth_Ip_RxChannelConfigType *RxChannelCfg_ptr;           /*!< The pointer containing the configuration of the Ctrl Rx channel (DMA + MTL). */
    const Eth_Ip_TxChannelConfigType *TxChannelCfg_ptr;           /*!< The pointer containing the configuration of the Ctrl Tx channel (DMA + MTL). */
    const Eth_Ip_PtpConfigType *PtpCfg_ptr;                       /*!< The pointer containing the configuration of the Ctrl Ptp . */
    const Eth_Ip_VlanConfigType *VlanCfg_ptr;                     /*!< The pointer containing the configuration of the Ctrl VLan. */
    const Eth_Ip_VlanFilterConfigType *VlanFilterCfg_ptr;         /*!< The pointer containing the configuration of the Ctrl Vlan Filter. */
    const Eth_Ip_Layer3Layer4FilterConfigType *L3L4FilterCfg_ptr; /*!< The pointer containing the configuration of the Ctrl Layer 3 Layer 4 Filter. */
    Eth_Ip_StateType *CtrlState_ptr;                              /*!< The pointer containing the the Ctrl state. */
} Eth_Ip_CtrlConfigType;

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
/*!< @}*/

#endif /*!< ETH_IP_TYPES_H */
