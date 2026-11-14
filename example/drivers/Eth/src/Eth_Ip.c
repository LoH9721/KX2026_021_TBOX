/***********************************************************************************************************************
 * Project Name: XHSC AUTOSAR MCAL
 * File Name: Eth_Ip.c
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
 *   @file       Eth_Ip.c
 *   @version    1.0.3
 *   @brief      ETH IP Layer Driver
 *   @addtogroup eth_ip
 *   @{
 */
#ifdef __cplusplus
extern "C" {
#endif

/***********************************************************************************************************************
 *  INCLUDES
 **********************************************************************************************************************/
#include "Eth_Ip.h"
#include "Osr.h"

/***********************************************************************************************************************
 *  DEFINES & MACROS
 **********************************************************************************************************************/
/**
 *  @brief   Defines the ETH IP internal macro.
 */
#define ETH_IP_64_DIV_32                        (2U)
#define ETH_IP_NANOSECS_ONESECOND               (1000000000U)
#define ETH_IP_TDES0_TER_MASK                   (0x00200000U)
#define ETH_IP_TDES0_OWN_MASK                   (0x80000000U)
#define ETH_IP_TDES0_TFS_MASK                   (0x20000000U)
#define ETH_IP_TDES0_TLS_MASK                   (0x10000000U)
#define ETH_IP_TDES0_ERR_MASK                   (0x0001FF07U)
#if (STD_ON == ETH_IP_ENABLE_ENHANCED_DESC)
#define ETH_IP_TDES0_TTSS_MASK                  (0x00020000U)
#endif /*!< (STD_ON == ETH_IP_ENABLE_ENHANCED_DESC) */
#define ETH_IP_TDES1_TBS1_MASK                  (0x00001FFFU)
#define ETH_IP_RDES0_FRAL_MASK                  (0x3FFF0000U)
#define ETH_IP_RDES0_FRAL_POS                   (16U)
#define ETH_IP_RDES0_ERRSUM_MASK                (0x00008000U)
#define ETH_IP_RDES0_RLS_MASK                   (0x00000100U)
#define ETH_IP_RDES0_RFS_MASK                   (0x00000200U)
#define ETH_IP_RDES1_RBS1_MASK                  (0x00001FFFU)
#define ETH_IP_RDES1_RER_MASK                   (0x00008000U)
#define ETH_IP_FRAME_CHECK_SEQUENCE_LEN         (4U)
#define ETH_IP_UPPER_4_BITS_MASK                (0xF0000000U)
#define ETH_IP_CRC32_BITS_31_28_POS             (28U)
#define ETH_IP_MASK_CRC_COMPUTER                (0xEDB88320U)
#define ETH_IP_BYTE_5_IN_ADDRESS_POS            (8U)
#define ETH_IP_BYTE_4_IN_ADDRESS_POS            (0U)
#define ETH_IP_BYTE_3_IN_ADDRESS_POS            (24U)
#define ETH_IP_BYTE_2_IN_ADDRESS_POS            (16U)
#define ETH_IP_BYTE_1_IN_ADDRESS_POS            (8U)
#define ETH_IP_BYTE_0_IN_ADDRESS_POS            (0U)
#define ETH_IP_BYTE_IN_ADDRESS_MASK             (0xFFU)
#define ETH_IP_IPG_DEFAULT_VALUE                (103U)
#define ETH_IP_32_BIT_NUM                       (32UL)
#define ETH_IP_8_TIMES                          (8UL)
#define ASM_DMB()                               ASM_KEYWORD("DMB")
#define ASM_DSB()                               ASM_KEYWORD("DSB")
#define ETH_IP_CHECK_TIMEOUT(Start, Elapsed, Timeout, Return)           \
    if (TRUE == Eth_Ip_IsTimeout((Start), (Elapsed), (Timeout), ETH_IP_TIMEOUT_METHOD_SUPPORT)) \
    {   \
        (Return) = ETH_IP_STATUS_TIMEOUT;  \
        break;  \
    }
#define ETH_BASE_PTRS                           {CM_ETH}
#define ETH_INSTANCE_COUNT                      (1U)
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
#define ETH_START_SEC_CONST_UNSPECIFIED
#include "Eth_MemMap.h"
/**
 * @brief Register base address of ETH IP
 */
static CM_ETH_TypeDef *const s_EthBases_aa[ETH_INSTANCE_COUNT] = ETH_BASE_PTRS;
#define ETH_STOP_SEC_CONST_UNSPECIFIED
#include "Eth_MemMap.h"

#define ETH_START_SEC_VAR_CLEARED_UNSPECIFIED
#include "Eth_MemMap.h"
/**
 * @brief State of ETH IP
 */
static Eth_Ip_StateType *s_EthState_aa[ETH_INSTANCE_COUNT];
#define ETH_STOP_SEC_VAR_CLEARED_UNSPECIFIED
#include "Eth_MemMap.h"
/***********************************************************************************************************************
 *  GLOBAL VARIABLES
 **********************************************************************************************************************/

/***********************************************************************************************************************
 *  LOCAL FUNCTION PROTOTYPES
 **********************************************************************************************************************/
#define ETH_START_SEC_CODE
#include "Eth_MemMap.h"
static uint32_t Eth_Ip_ReverseBits(
    uint32_t para_Value_u32
);

static uint32_t Eth_Ip_CalculateCrc32(
    const uint8_t *para_MacAddr_ptr,
    uint8_t para_Size_u8
);

static uint16_t Eth_Ip_IncreaseIndex(
    uint16_t para_Index_u16,
    uint16_t para_Max_u16
);

static void Eth_Ip_InitTxBufferDescriptor(
    const Eth_Ip_TxChannelConfigType *para_TxChCfg_ptr
);

static void Eth_Ip_InitRxBufferDescriptor(
    const Eth_Ip_RxChannelConfigType *para_RxChCfg_ptr
);

static Eth_Ip_StatusType Eth_Ip_InitDMA(
    CM_ETH_TypeDef *para_Base_ptr,
    const Eth_Ip_CtrlConfigType *para_Config_ptr
);

static void Eth_Ip_Layer3Layer4FilterConfig(
    CM_ETH_TypeDef *para_Base_ptr,
    const Eth_Ip_Layer3Layer4FilterConfigType *para_L3L4FilterCfg_ptr
);

static Eth_Ip_StatusType Eth_Ip_Ptp1588Config(
    CM_ETH_TypeDef *para_Base_ptr,
    const Eth_Ip_PtpConfigType *para_PtpCfg_ptr
);

static void Eth_Ip_InitMAC(
    CM_ETH_TypeDef *para_Base_ptr,
    const Eth_Ip_NormalConfigType *para_Config_ptr
);

static void Eth_Ip_StartTimer(
    uint32_t *para_EthIpStartTimeOut_ptr,
    uint32_t *para_EthIpTimeoutTicksOut_ptr,
    uint32_t para_EthIpTimeoutUs_u32,
    Osr_CounterType para_EthIpOsrCounter_en
);

static boolean Eth_Ip_IsTimeout(
    uint32_t *para_EthIpStartTime_ptr,
    uint32_t *para_EthIpElapsedTicks_ptr,
    uint32_t para_EthIpTimeoutTicks_u32,
    Osr_CounterType para_EthIpOsrCounter_en
);

static void Eth_Ip_InitCtrlStateStruct(
    uint8_t para_Instance_u8,
    const Eth_Ip_CtrlConfigType *para_Config_ptr
);
/***********************************************************************************************************************
 *  LOCAL FUNCTION
 **********************************************************************************************************************/
/**
 *  @brief          Reverse 32 bit of value.
 *
 *  @details        SRS ID:            None
 *                  Sync/Async:        None
 *                  Reentrancy:        None
 *
 *  @param[in]      para_Value_u32    The value need reversal.
 *
 *  @return         uint32_t    value reversalled
 *  @retval         None
 *
 *  @note           None
 *
 *  @pre            None
 *
 */
static uint32_t Eth_Ip_ReverseBits(
    uint32_t para_Value_u32
)
{
    uint32_t ReverseValue_u32 = 0UL;

    /* Bit high-low swap */
    for (uint32_t Index_u32 = 0UL; Index_u32 < ETH_IP_32_BIT_NUM; Index_u32++)
    {
        if (0UL != (para_Value_u32 & ((uint32_t)(1UL << Index_u32))))
        {
            ReverseValue_u32 |= (uint32_t)(1UL << ((31UL) - Index_u32));
        }
    }

    return ReverseValue_u32;
}

/**
 *  @brief          Caculation the CRC32 value for MAC address.
 *
 *  @details        SRS ID:            None
 *                  Sync/Async:        None
 *                  Reentrancy:        None
 *
 *  @param[in]      para_MacAddr_ptr    The MAC address pointer.
 *  @param[in]      para_Size_u8        The size of MAC.
 *
 *  @return         uint32_t             CRC32 value.
 *  @retval         None
 *
 *  @note           None
 *
 *  @pre            None
 *
 */
static uint32_t Eth_Ip_CalculateCrc32(
    const uint8_t *para_MacAddr_ptr,
    uint8_t para_Size_u8
)
{
    uint32_t CrcValue_u32 = 0xFFFFFFFFUL;
    uint8_t ByteIdx_u8    = 0U;
    uint8_t BitIdx_u8     = 8U;
    /* CRC32 calculate */
    for (ByteIdx_u8 = 0U; ByteIdx_u8 < para_Size_u8; ByteIdx_u8++)
    {
        CrcValue_u32 ^= para_MacAddr_ptr[ByteIdx_u8];
        while (0U < BitIdx_u8)
        {
            if (0UL == (CrcValue_u32 & 0x01UL))
            {
                CrcValue_u32 = (CrcValue_u32 >> 1UL);
            }
            else
            {
                CrcValue_u32 = (CrcValue_u32 >> 1UL) ^ ETH_IP_MASK_CRC_COMPUTER;
            }
            BitIdx_u8--;
        }
        BitIdx_u8 = 8U;
    }

    return ~CrcValue_u32;
}

/**
 *  @brief          Increase index.
 *
 *  @details        SRS ID:            None
 *                  Sync/Async:        None
 *                  Reentrancy:        None
 *
 *  @param[in]      para_Index_u16    The index need increase
 *  @param[in]      para_Max_u16      The max of index
 *
 *  @return         uint16_t            Increased index.
 *  @retval         None
 *
 *  @note           None
 *
 *  @pre            None
 *
 */
static uint16_t Eth_Ip_IncreaseIndex(
    uint16_t para_Index_u16,
    uint16_t para_Max_u16
)
{
    uint16_t Index_u16 = para_Index_u16 + 1U;

    /* Reset the counter when the limit is reached */
    if(Index_u16 >= para_Max_u16)
    {
        Index_u16 = 0U;
    }
    return Index_u16;
}

/**
 *  @brief          Initializes Tx Buffer Descriptor.
 *
 *  @details        SRS ID:            None
 *                  Sync/Async:        None
 *                  Reentrancy:        None
 *
 *  @param[in]      para_TxChCfg_ptr        The Tx channel configuration.
 *
 *  @return         void
 *
 *  @note           None
 *
 *  @pre            None
 *
 */
static void Eth_Ip_InitTxBufferDescriptor(
    const Eth_Ip_TxChannelConfigType *para_TxChCfg_ptr
)
{
    uint16_t Index_u16                                = 0U;
#if (STD_OFF == ETH_IP_ENABLE_ENHANCED_DESC)
    Eth_Ip_TxBufferDescriptorType *TxDescAddr_ptr   = para_TxChCfg_ptr->TxDescAddr_ptr;
#else
    Eth_Ip_TxBufferEnDescriptorType *TxDescAddr_ptr = para_TxChCfg_ptr->TxEnDescAddr_ptr;
#endif /*!< (STD_OFF == ETH_IP_ENABLE_ENHANCED_DESC) */ 

    /* Initial transmit descriptor */
    for (Index_u16 = 0U; Index_u16 < para_TxChCfg_ptr->DescNum_16; Index_u16++)
    {
        para_TxChCfg_ptr->TxBuffer_pp[Index_u16]      = NULL_PTR;
        TxDescAddr_ptr[Index_u16].TxControlStat_u32 = 0U;
        TxDescAddr_ptr[Index_u16].TxBuffLen_u32     = 0U;
        TxDescAddr_ptr[Index_u16].TxBuff1Addr_u32   = 0U;
        TxDescAddr_ptr[Index_u16].TxBuff2Addr_u32   = 0U;
#if (STD_ON == ETH_IP_ENABLE_ENHANCED_DESC)
        TxDescAddr_ptr[Index_u16].Reserved1_u32     = 0U;
        TxDescAddr_ptr[Index_u16].Reserved2_u32     = 0U;
        TxDescAddr_ptr[Index_u16].TimeStampLow_u32  = 0U;
        TxDescAddr_ptr[Index_u16].TimeStampHigh_u32 = 0U;
#endif /*!< (STD_ON == ETH_IP_ENABLE_ENHANCED_DESC) */
        /* Record the list end */
        if (Index_u16 == (para_TxChCfg_ptr->DescNum_16 - 1U))
        {
            TxDescAddr_ptr[Index_u16].TxControlStat_u32 = ETH_IP_TDES0_TER_MASK;
        }
    }
}

/**
 *  @brief          Initializes Rx Buffer Descriptor.
 *
 *  @details        SRS ID:            None
 *                  Sync/Async:        None
 *                  Reentrancy:        None
 *
 *  @param[in]      para_TxChCfg_ptr   The Rx channel configuration.
 *
 *  @return         void
 *
 *  @note           None
 *
 *  @pre            None
 *
 */
static void Eth_Ip_InitRxBufferDescriptor(
    const Eth_Ip_RxChannelConfigType *para_RxChCfg_ptr
)
{
    uint16_t Index_u16                                = 0U;
#if (STD_OFF == ETH_IP_ENABLE_ENHANCED_DESC)
    Eth_Ip_RxBufferDescriptorType *RxDescAddr_ptr   = para_RxChCfg_ptr->RxDescAddr_ptr;
#else
    Eth_Ip_RxBufferEnDescriptorType *RxDescAddr_ptr = para_RxChCfg_ptr->RxEnDescAddr_ptr;
#endif /*!< (STD_OFF == ETH_IP_ENABLE_ENHANCED_DESC) */ 

    /* Initial receive descriptor */
    for (Index_u16 = 0U; Index_u16 < para_RxChCfg_ptr->DescNum_16; Index_u16++)
    {
        RxDescAddr_ptr[Index_u16].RxControl_u32     = ETH_IP_RDES0_OWN_MASK;
        RxDescAddr_ptr[Index_u16].RxBuffLen_u32     = ((uint32_t)para_RxChCfg_ptr->BuffLen_u16 & ETH_IP_RDES1_RBS1_MASK);
        RxDescAddr_ptr[Index_u16].RxBuff1Addr_u32   = (uint32_t)(&para_RxChCfg_ptr->Buffer_ptr[para_RxChCfg_ptr->BuffLen_u16 * Index_u16]);
        RxDescAddr_ptr[Index_u16].RxBuff2Addr_u32   = 0U;
#if (STD_ON == ETH_IP_ENABLE_ENHANCED_DESC)
        RxDescAddr_ptr[Index_u16].RxExtStatus_u32   = 0U;
        RxDescAddr_ptr[Index_u16].Reserved2_u32     = 0U;
        RxDescAddr_ptr[Index_u16].TimeStampLow_u32  = 0U;
        RxDescAddr_ptr[Index_u16].TimeStampHigh_u32 = 0U;
#endif /*!< (STD_ON == ETH_IP_ENABLE_ENHANCED_DESC) */
        /* Record the list end */
        if (Index_u16 == (para_RxChCfg_ptr->DescNum_16 - 1U))
        {
            RxDescAddr_ptr[Index_u16].RxBuffLen_u32 |= ETH_IP_RDES1_RER_MASK;
        }
    }
}

/**
 *  @brief          Initializes the DMA subsystem.
 *
 *  @details        SRS ID:            None
 *                  Sync/Async:        None
 *                  Reentrancy:        None
 *
 *  @param[in]      para_Base_ptr       Ethernet base address.
 *  @param[in]      para_Config_ptr     The structure contains the hardware controller configuration type.
 *
 *  @return         Eth_Ip_StatusType    (ETH_IP_STATUS_SUCCESS)
 *  @retval         None
 *
 *  @note           None
 *
 *  @pre            None
 *
 */
static Eth_Ip_StatusType Eth_Ip_InitDMA(
    CM_ETH_TypeDef *para_Base_ptr,
    const Eth_Ip_CtrlConfigType *para_Config_ptr
)
{
    uint32_t EthIpStartTime_u32;
    uint32_t EthIpTimeoutTicks_u32;
    uint32_t EthIpElapsedTicks_u32                  = 0U;
    uint32_t RegValue_u32                           = 0U;
    uint32_t OprRegValue_u32                        = 0U;
    const Eth_Ip_TxChannelConfigType *TxChCfg_ptr = para_Config_ptr->TxChannelCfg_ptr;
    const Eth_Ip_RxChannelConfigType *RxChCfg_ptr = para_Config_ptr->RxChannelCfg_ptr;
    Eth_Ip_StatusType Status_en                   = ETH_IP_STATUS_SUCCESS;

    /* Reset the MAC and DMA controller */
    SET_REG32_BIT(para_Base_ptr->DMA_BUSMODR, ETH_DMA_BUSMODR_SWR);
    /* Get the current counter to calculate the timeout value */
    Eth_Ip_StartTimer(&EthIpStartTime_u32, &EthIpTimeoutTicks_u32, (uint32_t)ETH_IP_LOOP_TIMEOUT_LENGTH,
                      ETH_IP_TIMEOUT_METHOD_SUPPORT);
    /* Check if the reset operation is complete */
    while (0U != (READ_REG32(para_Base_ptr->DMA_BUSMODR) & ETH_DMA_BUSMODR_SWR))
    {
        ETH_IP_CHECK_TIMEOUT(&EthIpStartTime_u32, &EthIpElapsedTicks_u32, EthIpTimeoutTicks_u32, Status_en)
    }

    if (ETH_IP_STATUS_SUCCESS == Status_en)
    {
#if (STD_ON == ETH_IP_ENABLE_ENHANCED_DESC)
        RegValue_u32 = ETH_DMA_BUSMODR_AAL | ETH_DMA_BUSMODR_TPBL_4 | ETH_DMA_BUSMODR_FBST | ETH_DMA_BUSMODR_DSEN;
#else
        RegValue_u32 = ETH_DMA_BUSMODR_AAL | ETH_DMA_BUSMODR_TPBL_4 | ETH_DMA_BUSMODR_FBST;
#endif /*!< (STD_ON == ETH_IP_ENABLE_ENHANCED_DESC) */
        /* Configure DMA transfers part */
        OprRegValue_u32 |= (uint32_t)TxChCfg_ptr->ThresholdLevel_en << ETH_DMA_OPRMODR_TTC_POS;
        OprRegValue_u32 |= (uint32_t)TxChCfg_ptr->EnStoreForward_bool << ETH_DMA_OPRMODR_TSF_POS;
        if ((boolean)TRUE == TxChCfg_ptr->EnTxInterrupt_bool)
        {
            /* Enable Tx interrupt. */
            SET_REG32_BIT(para_Base_ptr->DMA_INTENAR, ETH_DMA_INTENAR_TIE | ETH_DMA_INTENAR_NIE);
        }
        /* Configure transmit descriptor list base address */
#if (STD_OFF == ETH_IP_ENABLE_ENHANCED_DESC)
        WRITE_REG32(para_Base_ptr->DMA_TXDLADR, TxChCfg_ptr->TxDescAddr_ptr);
#else
        WRITE_REG32(para_Base_ptr->DMA_TXDLADR, TxChCfg_ptr->TxEnDescAddr_ptr);
#endif /*!< (STD_OFF == ETH_IP_ENABLE_ENHANCED_DESC) */ 
        /* Initial transmit descriptor */
        Eth_Ip_InitTxBufferDescriptor(TxChCfg_ptr);
        OprRegValue_u32 |= ETH_DMA_OPRMODR_STT;

        /* Configure DMA receive part */
        OprRegValue_u32 |= (uint32_t)RxChCfg_ptr->ThresholdLevel_en << ETH_DMA_OPRMODR_RTC_POS;
        OprRegValue_u32 |= (uint32_t)RxChCfg_ptr->EnFwUndersizeGoodPackets_bool << ETH_DMA_OPRMODR_DGF_POS;
        OprRegValue_u32 |= (uint32_t)RxChCfg_ptr->EnStoreForward_bool << ETH_DMA_OPRMODR_RSF_POS;
        OprRegValue_u32 |= (uint32_t)RxChCfg_ptr->DisDropChecksumErrPackets_bool << ETH_DMA_OPRMODR_DTCOE_POS;
        if ((boolean)TRUE == RxChCfg_ptr->EnRxInterrupt_bool)
        {
            /* Enable Rx interrupt. */
            SET_REG32_BIT(para_Base_ptr->DMA_INTENAR, ETH_DMA_INTENAR_RIE | ETH_DMA_INTENAR_NIE);
        }
        /* Configure receive descriptor list base address */
#if (STD_OFF == ETH_IP_ENABLE_ENHANCED_DESC)
        WRITE_REG32(para_Base_ptr->DMA_RXDLADR, RxChCfg_ptr->RxDescAddr_ptr);
#else
        WRITE_REG32(para_Base_ptr->DMA_RXDLADR, RxChCfg_ptr->RxEnDescAddr_ptr);
#endif /*!< (STD_OFF == ETH_IP_ENABLE_ENHANCED_DESC) */ 
        /* Initial receive descriptor */
        Eth_Ip_InitRxBufferDescriptor(RxChCfg_ptr);
        OprRegValue_u32 |= ETH_DMA_OPRMODR_STR;

        /* Configure DMA_BUSMODR register */
        WRITE_REG32(para_Base_ptr->DMA_BUSMODR, RegValue_u32);
        /* Configure DMA_OPRMODR register */
        WRITE_REG32(para_Base_ptr->DMA_OPRMODR, OprRegValue_u32);
    }

    return Status_en;
}

/**
 *  @brief          Configure Layer 3 Layer 4 filter feature.
 *
 *  @details        SRS ID:            None
 *                  Sync/Async:        None
 *                  Reentrancy:        None
 *
 *  @param[in]      para_Instance_u8          Ethernet instance number.
 *  @param[in]      para_L3L4FilterCfg_ptr    Point to configure parameter of the Layer 3 Layer 4 filter.
 *
 *  @return         void
 *
 *  @note           None
 *
 *  @pre            None
 *
 */
static void Eth_Ip_Layer3Layer4FilterConfig(
    CM_ETH_TypeDef *para_Base_ptr,
    const Eth_Ip_Layer3Layer4FilterConfigType *para_L3L4FilterCfg_ptr
)
{
    uint32_t RegValue_u32                                = 0x00U;
    const Eth_Ip_Layer3FilterConfigType *Layer3Cfg_ptr = para_L3L4FilterCfg_ptr->Layer3FilterConfigType_ptr;
    const Eth_Ip_Layer4FilterConfigType *Layer4Cfg_ptr = para_L3L4FilterCfg_ptr->Layer4FilterConfigType_ptr;

    /* L3 filter config */
    if (NULL_PTR != Layer3Cfg_ptr)
    {
        if (ETH_IP_LAYER3_IPV4 == Layer3Cfg_ptr->L3Type_en)
        {
            /* Configuration Layer 3 IPv4 bitmask and SA/DA match */
            RegValue_u32 |= ((uint32_t)Layer3Cfg_ptr->BitMask.L3Ipv4HigherBitsMatch_aa[0U] << ETH_MAC_L34CTLR_L3HSBM_POS) |
                           ((uint32_t)Layer3Cfg_ptr->BitMask.L3Ipv4HigherBitsMatch_aa[1U] << ETH_MAC_L34CTLR_L3HDBM_POS) |
                           ((uint32_t)Layer3Cfg_ptr->EnL3SrcAddrFilter_bool << ETH_MAC_L34CTLR_L3SAM_POS) |
                           ((uint32_t)Layer3Cfg_ptr->EnL3SrcAddrInvMatch_bool << ETH_MAC_L34CTLR_L3SAIM_POS) |
                           ((uint32_t)Layer3Cfg_ptr->EnL3DesAddrFilter_bool << ETH_MAC_L34CTLR_L3DAM_POS) |
                           ((uint32_t)Layer3Cfg_ptr->EnL3DesAddrInvMatch_bool << ETH_MAC_L34CTLR_L3DAIM_POS);
            /* Configuration Layer 3 IPv4 source address */
            WRITE_REG32(para_Base_ptr->MAC_L3ADDRR0, Layer3Cfg_ptr->FilterValue.Ipv4Address_aa[0U]);
            /* Configuration Layer 3 IPv4 destination address */
            WRITE_REG32(para_Base_ptr->MAC_L3ADDRR1, Layer3Cfg_ptr->FilterValue.Ipv4Address_aa[1U]);
        }
        else
        {
            /* Enable IPV6 filtering, which bit mask range is 0 ~ 127 */
            RegValue_u32 |= ETH_MAC_L34CTLR_L3PEN | (((uint32_t)Layer3Cfg_ptr->BitMask.L3Ipv6HigherBitsMatch_u16 & 0x7FU)  << ETH_MAC_L34CTLR_L3HSBM_POS);
            /* In IPV6 mode, it supports to check only one of the DA or SA */
            if (TRUE == Layer3Cfg_ptr->EnL3SrcAddrFilter_bool)
            {
                RegValue_u32 |= ETH_MAC_L34CTLR_L3SAM | ((uint32_t)Layer3Cfg_ptr->EnL3SrcAddrInvMatch_bool << ETH_MAC_L34CTLR_L3SAIM_POS);
            }
            else if (TRUE == Layer3Cfg_ptr->EnL3DesAddrFilter_bool)
            {
                RegValue_u32 |= ETH_MAC_L34CTLR_L3DAM | ((uint32_t)Layer3Cfg_ptr->EnL3DesAddrInvMatch_bool << ETH_MAC_L34CTLR_L3DAIM_POS);
            }
            else
            {
                /* Fulfill the remaining conditions, do nothing */
            }
            /* Configuration Layer 3 IPv6 address */
            WRITE_REG32(para_Base_ptr->MAC_L3ADDRR0, Layer3Cfg_ptr->FilterValue.Ipv6Address_aa[0U]);
            WRITE_REG32(para_Base_ptr->MAC_L3ADDRR1, Layer3Cfg_ptr->FilterValue.Ipv6Address_aa[1U]);
            WRITE_REG32(para_Base_ptr->MAC_L3ADDRR2, Layer3Cfg_ptr->FilterValue.Ipv6Address_aa[2U]);
            WRITE_REG32(para_Base_ptr->MAC_L3ADDRR3, Layer3Cfg_ptr->FilterValue.Ipv6Address_aa[3U]);
        }
    }

    /* L4 filter config */
    if (NULL_PTR != Layer4Cfg_ptr)
    {
        if (ETH_IP_LAYER4_UDP == Layer4Cfg_ptr->L4Type_en)
        {
            RegValue_u32 |= ETH_MAC_L34CTLR_L4PEN;
        }
        RegValue_u32 |= ((uint32_t)Layer4Cfg_ptr->EnL4SrcPortFilter_bool << ETH_MAC_L34CTLR_L4SPM_POS) |
            ((uint32_t)Layer4Cfg_ptr->EnL4SrcPortInvMatch_bool << ETH_MAC_L34CTLR_L4SPIM_POS) |
            ((uint32_t)Layer4Cfg_ptr->EnL4DesPortFilter_bool << ETH_MAC_L34CTLR_L4DPM_POS) |
            ((uint32_t)Layer4Cfg_ptr->EnL4DesPortInvMatch_bool << ETH_MAC_L34CTLR_L4DPIM_POS);
        /* Configuration Layer 4 Port address */
        WRITE_REG32(para_Base_ptr->MAC_L4PORTR, ((uint32_t)Layer4Cfg_ptr->L4DesPortNum_u16 << ETH_MAC_L4PORTR_L4DPVAL_POS) | (uint32_t)Layer4Cfg_ptr->L4SrcPortNum_u16);
    }

    /* Set Layer 3 and Layer 4 filtering register */
    WRITE_REG32(para_Base_ptr->MAC_L34CTLR, RegValue_u32);
    /* Set Drop Non-TCP/UDP over IP packet mode */
    if ((boolean)TRUE == para_L3L4FilterCfg_ptr->DropNonTcpUdpIpPacket_bool)
    {
        SET_REG32_BIT(para_Base_ptr->MAC_FLTCTLR, ETH_MAC_FLTCTLR_DNTU);
    }
    /* Enable MAC Layer 3 and Layer 4 filtering */
    SET_REG32_BIT(para_Base_ptr->MAC_FLTCTLR, ETH_MAC_FLTCTLR_IPFE);
}

/**
 *  @brief          Initialize Precsion Time Protocol (PTP) IEEE 1588.
 *
 *  @details        SRS ID:            None
 *                  Sync/Async:        None
 *                  Reentrancy:        None
 *
 *  @param[in]      para_Instance_u8    Ethernet instance number.
 *  @param[in]      para_PtpCfg_ptr     The PTP configuration.
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
static Eth_Ip_StatusType Eth_Ip_Ptp1588Config(
    CM_ETH_TypeDef *para_Base_ptr,
    const Eth_Ip_PtpConfigType *para_PtpCfg_ptr
)
{
    Eth_Ip_StatusType Status_en  = ETH_IP_STATUS_SUCCESS;
    uint32_t RegValue_u32          = 0x00U;
    uint32_t Control_u32           = 0x00U;
    uint32_t NanoSeconds_u32       = 0x00U;
    uint32_t EthIpStartTime_u32    = 0x00U;
    uint32_t EthIpTimeoutTicks_u32 = 0x00U;
    uint32_t EthIpElapsedTicks_u32 = (uint32_t)0U;

    RegValue_u32 = ETH_PTP_TSPCTLR_TSPEN | ETH_PTP_TSPCTLR_TSPINI | 
                    ((uint32_t)para_PtpCfg_ptr->EnSnapshotAllPackets_bool << ETH_PTP_TSPCTLR_TSPEALL_POS);
    /* Update Sub-seconds increment based on the PTP clock frequency */
    RegValue_u32 |= ETH_PTP_TSPCTLR_TSPSSR;
    Control_u32 = (uint32_t)(ETH_IP_NANOSECS_ONESECOND / para_PtpCfg_ptr->RefClockHz_u32);
    NanoSeconds_u32 = para_PtpCfg_ptr->NanoSecondsUpdate_u32;
    /* Enable Timestamp */
    WRITE_REG32(para_Base_ptr->PTP_TSPNSAR, Control_u32);
    WRITE_REG32(para_Base_ptr->PTP_TMUSECR, para_PtpCfg_ptr->SecondsUpdate_u32);
    WRITE_REG32(para_Base_ptr->PTP_TMUNSER, NanoSeconds_u32);
    WRITE_REG32(para_Base_ptr->PTP_TSPCTLR, RegValue_u32);
    /* Waiting Timestamp enable success */
    Eth_Ip_StartTimer(&EthIpStartTime_u32, &EthIpTimeoutTicks_u32, (uint32_t)ETH_IP_LOOP_TIMEOUT_LENGTH,
                      ETH_IP_TIMEOUT_METHOD_SUPPORT);
    while (0U != (READ_REG32(para_Base_ptr->PTP_TSPCTLR) & ETH_PTP_TSPCTLR_TSPINI))
    {
        ETH_IP_CHECK_TIMEOUT(&EthIpStartTime_u32, &EthIpElapsedTicks_u32, EthIpTimeoutTicks_u32, Status_en)
    }

    return Status_en;
}

/**
 *  @brief          Set MAC controller with the configuration.
 *
 *  @details        SRS ID:            None
 *                  Sync/Async:        None
 *                  Reentrancy:        None
 *
 *  @param[in]      para_Instance_u8    Ethernet instance number.
 *  @param[in]      para_Config_ptr     The structure contains the hardware controller configuration type.
 *
 *  @return         void
 *
 *  @note           None
 *
 *  @pre            None
 *
 */
static void Eth_Ip_InitMAC(
    CM_ETH_TypeDef *para_Base_ptr,
    const Eth_Ip_NormalConfigType *para_Config_ptr
)
{
    uint32_t RegValue_u32                    = 0x00U;
    const Eth_Ip_MacConfigType *MacCfg_ptr = para_Config_ptr->MacCfg_ptr;

    /* Configure MDIO interface */
    WRITE_REG32(para_Base_ptr->MAC_SMIADDR, (uint32_t)para_Config_ptr->CsrClockDiv_en << ETH_MAC_SMIADDR_SMIC_POS);
    /* Configure MAC address 0 as default MAC address */
    RegValue_u32 = ((uint32_t)para_Config_ptr->PhysAddr_ptr[5] << ETH_IP_BYTE_5_IN_ADDRESS_POS) |
                  ((uint32_t)para_Config_ptr->PhysAddr_ptr[4] << ETH_IP_BYTE_4_IN_ADDRESS_POS) |
                  ETH_MAC_MACADHR0_AE0;
    WRITE_REG32(para_Base_ptr->MAC_MACADHR0, RegValue_u32);
    RegValue_u32 = ((uint32_t)para_Config_ptr->PhysAddr_ptr[3] << ETH_IP_BYTE_3_IN_ADDRESS_POS) |
                  ((uint32_t)para_Config_ptr->PhysAddr_ptr[2] << ETH_IP_BYTE_2_IN_ADDRESS_POS) |
                  ((uint32_t)para_Config_ptr->PhysAddr_ptr[1] << ETH_IP_BYTE_1_IN_ADDRESS_POS) |
                  ((uint32_t)para_Config_ptr->PhysAddr_ptr[0] << ETH_IP_BYTE_0_IN_ADDRESS_POS);
    WRITE_REG32(para_Base_ptr->MAC_MACADLR0, RegValue_u32);
    /* Enable hash or perfect mac address filter */
    SET_REG32_BIT(para_Base_ptr->MAC_FLTCTLR, ETH_MAC_FLTCTLR_HPF);
    /* Configure MAC config register */
    RegValue_u32 = (uint32_t)para_Config_ptr->Speed_en | ETH_MAC_CONFIGR_CST | ETH_MAC_CONFIGR_ACS |
                    ((uint32_t)MacCfg_ptr->DisWatchdog_bool << ETH_MAC_CONFIGR_MWD_POS) |
                    ((uint32_t)MacCfg_ptr->DisJabber_bool << ETH_MAC_CONFIGR_MJB_POS) | 
                    ((uint32_t)MacCfg_ptr->EnLoopbackMode_bool << ETH_MAC_CONFIGR_LM_POS) |
                    ((uint32_t)MacCfg_ptr->EnFullDuplexMode_bool << ETH_MAC_CONFIGR_DM_POS) |
                    ((uint32_t)MacCfg_ptr->EnChecksumOffload_bool << ETH_MAC_CONFIGR_IPCO_POS);
    if ((boolean)TRUE == MacCfg_ptr->DisBroadcastPacket_bool)
    {
        /* Disable broadcast packet */
        SET_REG32_BIT(para_Base_ptr->MAC_FLTCTLR, ETH_MAC_FLTCTLR_DBF);
    }
    RegValue_u32 |= (uint32_t)((ETH_IP_IPG_DEFAULT_VALUE - (uint32_t)MacCfg_ptr->InterPacketGap_u16) / ETH_IP_8_TIMES) << ETH_MAC_CONFIGR_IFG_POS;
    WRITE_REG32(para_Base_ptr->MAC_CONFIGR, RegValue_u32);
}

/**
 *  @brief          Start timing counter for Timeout.
 *
 *  @details        SRS ID:            None
 *                  Sync/Async:        None
 *                  Reentrancy:        None
 *
 *  @param[in]      para_EthIpStartTimeOut_ptr       Start TimeOut.
 *  @param[in]      para_EthIpTimeoutTicksOut_ptr    Timeout Ticks Out.
 *  @param[in]      para_EthIpTimeoutUs_u32          Timeout Us.
 *  @param[in]      para_EthIpOsrCounter_en          Osr Counter.
 *
 *  @return         void
 *
 *  @note           Init
 *
 *  @pre            None
 *
 */
static void Eth_Ip_StartTimer(
    uint32_t *para_EthIpStartTimeOut_ptr,
    uint32_t *para_EthIpTimeoutTicksOut_ptr,
    uint32_t para_EthIpTimeoutUs_u32,
    Osr_CounterType para_EthIpOsrCounter_en
)
{
    *para_EthIpStartTimeOut_ptr    = Osr_GetCounter(para_EthIpOsrCounter_en);
    *para_EthIpTimeoutTicksOut_ptr = Osr_UsToTicks(para_EthIpTimeoutUs_u32, para_EthIpOsrCounter_en);
}

/**
 *  @brief          Check Timeout value.
 *
 *  @details        SRS ID:            None
 *                  Sync/Async:        None
 *                  Reentrancy:        None
 *
 *  @param[in]      para_EthIpStartTime_ptr       Start Time.
 *  @param[in]      para_EthIpElapsedTicks_ptr    Elapsed Ticks.
 *  @param[in]      para_EthIpTimeoutTicks_u32    Timeout Ticks.
 *  @param[in]      para_EthIpOsrCounter_en       Osr Counter.
 *
 *  @return         boolean
 *  @retval         None
 *
 *  @note           Init
 *
 *  @pre            None
 *
 */
static boolean Eth_Ip_IsTimeout(
    uint32_t *para_EthIpStartTime_ptr,
    uint32_t *para_EthIpElapsedTicks_ptr,
    uint32_t para_EthIpTimeoutTicks_u32,
    Osr_CounterType para_EthIpOsrCounter_en
)
{
    boolean IsTimeOut_bool            = FALSE;
    uint32_t EthCurrentElapsedTicks_u32 = Osr_GetElapsed(para_EthIpStartTime_ptr, para_EthIpOsrCounter_en);
    *para_EthIpElapsedTicks_ptr       = *para_EthIpElapsedTicks_ptr + EthCurrentElapsedTicks_u32;
    
    if (*para_EthIpElapsedTicks_ptr >= para_EthIpTimeoutTicks_u32)
    {
        IsTimeOut_bool = TRUE;
    }
    else
    {
        IsTimeOut_bool = FALSE;
    }

    return IsTimeOut_bool;
}

/**
 *  @brief          Initializes controller State Structure.
 *
 *  @details        SRS ID:            None    
 *                  Sync/Async:        None
 *                  Reentrancy:        None
 *
 *  @param[in]      para_Instance_u8        Ethernet instance number.
 *  @param[in]      para_Config_ptr         The structure contains the hardware controller configuration type.
 *
 *  @return         void
 *
 *  @note           None
 *
 *  @pre            None
 *
 */
static void Eth_Ip_InitCtrlStateStruct(
    uint8_t para_Instance_u8,
    const Eth_Ip_CtrlConfigType *para_Config_ptr
)
{
    const Eth_Ip_RxChannelConfigType *RxChCfg_ptr = para_Config_ptr->RxChannelCfg_ptr;
    const Eth_Ip_TxChannelConfigType *TxChCfg_ptr = para_Config_ptr->TxChannelCfg_ptr;
    Eth_Ip_StateType *State_ptr                   = para_Config_ptr->CtrlState_ptr;

    /* Init Tx state */
#if (STD_OFF == ETH_IP_ENABLE_ENHANCED_DESC)
    State_ptr->TxBdRing_st.TxBdBase_ptr       = TxChCfg_ptr->TxDescAddr_ptr;
#else
    State_ptr->TxBdRing_st.TxEnBdBase_ptr     = TxChCfg_ptr->TxEnDescAddr_ptr;
#endif /*!< (STD_OFF == ETH_IP_ENABLE_ENHANCED_DESC) */
    State_ptr->TxBdRing_st.TxGenIdx_u16       = 0U;
    State_ptr->TxBdRing_st.TxBuffer_pp        = TxChCfg_ptr->TxBuffer_pp;
    State_ptr->TxBdRing_st.TxRingLen_u16      = TxChCfg_ptr->DescNum_16;
    State_ptr->TxBdRing_st.EnTxInterrupt_bool = TxChCfg_ptr->EnTxInterrupt_bool;
    State_ptr->TxCallback_pfn                 = TxChCfg_ptr->Callback_pfn;
    /* Init Rx state */
#if (STD_OFF == ETH_IP_ENABLE_ENHANCED_DESC)
    State_ptr->RxBdRing_st.RxBdBase_ptr       = RxChCfg_ptr->RxDescAddr_ptr;
#else
    State_ptr->RxBdRing_st.RxEnBdBase_ptr     = RxChCfg_ptr->RxEnDescAddr_ptr;
#endif /*!< (STD_OFF == ETH_IP_ENABLE_ENHANCED_DESC) */ 
    State_ptr->RxBdRing_st.RxGenIdx_u16       = 0U;
    State_ptr->RxBdRing_st.RxConsumIdx_u16    = 0U;
    State_ptr->RxBdRing_st.RxRingLen_u16      = RxChCfg_ptr->DescNum_16;
    State_ptr->RxBdRing_st.RxBuffer_ptr       = RxChCfg_ptr->Buffer_ptr;
    State_ptr->RxBdRing_st.RxBuffLen_u16      = RxChCfg_ptr->BuffLen_u16;
    State_ptr->RxBdRing_st.EnRxInterrupt_bool = RxChCfg_ptr->EnRxInterrupt_bool;
    State_ptr->RxCallback_pfn                 = RxChCfg_ptr->Callback_pfn;

    /* Record status Structure pointer in the IP driver */
    s_EthState_aa[para_Instance_u8] = State_ptr;
}
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
)
{
    uint32_t EthIpStartTime_u32;
    uint32_t EthIpTimeoutTicks_u32;
    uint32_t EthIpElapsedTicks_u32 = (uint32_t)0U;
    Eth_Ip_StatusType Status_en  = ETH_IP_STATUS_SUCCESS;
    CM_ETH_TypeDef *Base_ptr     = s_EthBases_aa[para_Instance_u8];

    if (TRUE == para_CtrlEn_bool)
    {
        SET_REG32_BIT(Base_ptr->DMA_OPRMODR, ETH_DMA_OPRMODR_STT | ETH_DMA_OPRMODR_STR);
        /* Activate both the transmitter configuration and receiver configuration in MAC */
        SET_REG32_BIT(Base_ptr->MAC_CONFIGR, ETH_MAC_CONFIGR_TE | ETH_MAC_CONFIGR_RE);
    }
    else
    {
        /* Deactivate the transmission/receive DMA channel control */
        CLR_REG32_BIT(Base_ptr->DMA_OPRMODR, ETH_DMA_OPRMODR_STT | ETH_DMA_OPRMODR_STR);
        /* Flush Tx Queue */
        SET_REG32_BIT(Base_ptr->DMA_OPRMODR, ETH_DMA_OPRMODR_FTF);
        Eth_Ip_StartTimer(&EthIpStartTime_u32, &EthIpTimeoutTicks_u32, (uint32_t)ETH_IP_LOOP_TIMEOUT_LENGTH,
                          ETH_IP_TIMEOUT_METHOD_SUPPORT);
        /* Wait until Tx Queue is empty */
        while (0U != (READ_REG32(Base_ptr->DMA_OPRMODR) & ETH_DMA_OPRMODR_FTF))
        {
            ETH_IP_CHECK_TIMEOUT(&EthIpStartTime_u32, &EthIpElapsedTicks_u32, EthIpTimeoutTicks_u32, Status_en)
        }
        /* Clear MAC configuration for both transmission and reception. */
        CLR_REG32_BIT(Base_ptr->MAC_CONFIGR, ETH_MAC_CONFIGR_TE | ETH_MAC_CONFIGR_RE);
    }

    return Status_en;
}

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
)
{
    Eth_Ip_CtrlModeType StateCtrMode_en = ETH_IP_CTRL_MODE_RESET;
    CM_ETH_TypeDef *Base_ptr            = s_EthBases_aa[para_Instance_u8];

    /* Check whether the current controller clock is not disabled */
    if (0U != (READ_REG32(CM_PWC->FCG1) & PWC_FCG1_ETHMAC))
    {
        StateCtrMode_en = ETH_IP_CTRL_MODE_NOT_ACCESS;
    }
    else if (0U != (READ_REG32(Base_ptr->DMA_BUSMODR) & ETH_DMA_BUSMODR_SWR))
    {
        StateCtrMode_en = ETH_IP_CTRL_MODE_RESET;
    }
    else if (0U !=
             (READ_REG32(Base_ptr->MAC_CONFIGR) & (ETH_MAC_CONFIGR_TE | ETH_MAC_CONFIGR_RE)))
    {
        StateCtrMode_en = ETH_IP_CTRL_MODE_ACTIVE;
    }
    else
    {
        StateCtrMode_en = ETH_IP_CTRL_MODE_INACTIVE;
    }

    return StateCtrMode_en;
}

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
)
{
    uint32_t EthIpStartTime_u32;
    uint32_t EthIpTimeoutTicks_u32;
    uint32_t EthIpElapsedTicks_u32 = 0U;
    uint32_t RegValue_u32          = 0U;
    Eth_Ip_StatusType Status_en  = ETH_IP_STATUS_SUCCESS;
    CM_ETH_TypeDef *Base_ptr     = s_EthBases_aa[para_Instance_u8];

    /* Clear MDIO address to prepare send a new address */
    RegValue_u32 = READ_REG32(Base_ptr->MAC_SMIADDR) & ETH_MAC_SMIADDR_SMIC;
    RegValue_u32 |= ((uint32_t)para_PhyAddr_u8 << ETH_MAC_SMIADDR_SMIA_POS) | ((uint32_t)para_RegAddr_u8 << ETH_MAC_SMIADDR_SMIR_POS) | ETH_MAC_SMIADDR_SMIB;
    /* Trigger read command */
    WRITE_REG32(Base_ptr->MAC_SMIADDR, RegValue_u32);
    /* Check the current read is complete */
    Eth_Ip_StartTimer(&EthIpStartTime_u32, &EthIpTimeoutTicks_u32, (uint32_t)ETH_IP_LOOP_TIMEOUT_LENGTH,
                      ETH_IP_TIMEOUT_METHOD_SUPPORT);
    while (0U != (READ_REG32(Base_ptr->MAC_SMIADDR) & ETH_MAC_SMIADDR_SMIB))
    {
        ETH_IP_CHECK_TIMEOUT(&EthIpStartTime_u32, &EthIpElapsedTicks_u32, EthIpTimeoutTicks_u32, Status_en)
    }
    if (ETH_IP_STATUS_SUCCESS == Status_en)
    {
        /* Copy the receive data to buffer */
        *para_Data_ptr = (uint16_t)READ_REG32(Base_ptr->MAC_SMIDATR);
    }

    return Status_en;
}

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
)
{
    uint32_t EthIpStartTime_u32;
    uint32_t EthIpTimeoutTicks_u32;
    uint32_t EthIpElapsedTicks_u32 = 0U;
    uint32_t RegValue_u32          = 0U;
    Eth_Ip_StatusType Status_en  = ETH_IP_STATUS_SUCCESS;
    CM_ETH_TypeDef *Base_ptr     = s_EthBases_aa[para_Instance_u8];

    /* Prepare the write data */
    WRITE_REG32(Base_ptr->MAC_SMIDATR, para_Data_u16);
    /* Clear MDIO address to prepare send a new address */
    RegValue_u32 = READ_REG32(Base_ptr->MAC_SMIADDR) & ETH_MAC_SMIADDR_SMIC;
    RegValue_u32 |= ((uint32_t)para_PhyAddr_u8 << ETH_MAC_SMIADDR_SMIA_POS) | ((uint32_t)para_RegAddr_u8 << ETH_MAC_SMIADDR_SMIR_POS) | 
                    ETH_MAC_SMIADDR_SMIW | ETH_MAC_SMIADDR_SMIB;
    /* Trigger write command */
    WRITE_REG32(Base_ptr->MAC_SMIADDR, RegValue_u32);
    /* Check the current transmission is complete */
    Eth_Ip_StartTimer(&EthIpStartTime_u32, &EthIpTimeoutTicks_u32, (uint32_t)ETH_IP_LOOP_TIMEOUT_LENGTH,
                      ETH_IP_TIMEOUT_METHOD_SUPPORT);
    while (0U != (READ_REG32(Base_ptr->MAC_SMIADDR) & ETH_MAC_SMIADDR_SMIB))
    {
        ETH_IP_CHECK_TIMEOUT(&EthIpStartTime_u32, &EthIpElapsedTicks_u32, EthIpTimeoutTicks_u32, Status_en)
    }

    return Status_en;
}

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
)
{
    CM_ETH_TypeDef *Base_ptr = s_EthBases_aa[para_Instance_u8];

    /* Clear receive all mode and enable hash multicast */
    CLR_REG32_BIT(Base_ptr->MAC_FLTCTLR, ETH_MAC_FLTCTLR_RA | ETH_MAC_FLTCTLR_PR);
    SET_REG32_BIT(Base_ptr->MAC_FLTCTLR, ETH_MAC_FLTCTLR_HMC);
}

/**
 *  @brief          Enables/Disables pass all incoming packets mode.
 *  @details        SRS ID:            None
 *                  Design ID:         SDD_ETH_57
 *                  Sync/Async:        None
 *                  Reentrancy:        None
 *
 *  @param[in]      para_Instance_u8    Ethernet instance number.
 *  @param[in]      para_Enable_bool    If TRUE, the application will receive all incomming packets regardless of
 *                                      each layer filter status.
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
)
{
    CM_ETH_TypeDef *Base_ptr = s_EthBases_aa[para_Instance_u8];

    if (TRUE == para_Enable_bool)
    {
        /* Set Promiscuous Mode bit */
        SET_REG32_BIT(Base_ptr->MAC_FLTCTLR, ETH_MAC_FLTCTLR_PR);
    }
    else
    {
        /* Clear Promiscuous Mode bit */
        CLR_REG32_BIT(Base_ptr->MAC_FLTCTLR, ETH_MAC_FLTCTLR_PR);
    }
}

/**
 *  @brief          Update MAC address filter of the controller.
 *
 *  @details        SRS ID:            None
 *                  Design ID:         SDD_ETH_58
 *                  Sync/Async:        None
 *                  Reentrancy:        None
 *
 *  @param[in]      para_Instance_u8   Ethernet instance number.
 *  @param[in]      para_PhysAdrr_ptr  Pointer to one MAC address filter entry. The address in network byte order stored
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
)
{
    uint32_t Address_u32;
    CM_ETH_TypeDef *Base_ptr = s_EthBases_aa[para_Instance_u8];

    /* Establish the value in the high register of the physical address */
    Address_u32 = ((uint32_t)para_PhysAdrr_ptr[5] << ETH_IP_BYTE_5_IN_ADDRESS_POS) |
                  ((uint32_t)para_PhysAdrr_ptr[4] << ETH_IP_BYTE_4_IN_ADDRESS_POS);
    Address_u32 |= ETH_MAC_MACADHR0_AE0;
    /* Set the upper 16 bits of the second 6-byte MAC address */
    WRITE_REG32(Base_ptr->MAC_MACADHR0, Address_u32);
    /* Establish the value in the low register of the physical address */
    Address_u32 = ((uint32_t)para_PhysAdrr_ptr[3] << ETH_IP_BYTE_3_IN_ADDRESS_POS) |
                  ((uint32_t)para_PhysAdrr_ptr[2] << ETH_IP_BYTE_2_IN_ADDRESS_POS) |
                  ((uint32_t)para_PhysAdrr_ptr[1] << ETH_IP_BYTE_1_IN_ADDRESS_POS) |
                  ((uint32_t)para_PhysAdrr_ptr[0] << ETH_IP_BYTE_0_IN_ADDRESS_POS);
    /* Set the lower 32 bits of the second 6-byte MAC address */
    WRITE_REG32(Base_ptr->MAC_MACADLR0, Address_u32);
}

/**
 *  @brief          Read MAC address filter of the controller.
 *
 *  @details        SRS ID:            None
 *                  Design ID:         SDD_ETH_59
 *                  Sync/Async:        None
 *                  Reentrancy:        None
 *
 *  @param[in]      para_Instance_u8   Ethernet instance number.
 *  @param[out]     para_PhysAdrr_ptr  Point to the buffer which store the read back MAC address filter entry value.
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
)
{
    uint32_t Address_u32;
    CM_ETH_TypeDef *Base_ptr = s_EthBases_aa[para_Instance_u8];

    /* Retrieve the value from the lower register of the physical address */
    Address_u32         = READ_REG32(Base_ptr->MAC_MACADLR0);
    para_PhysAdrr_ptr[0] = (uint8_t)(ETH_IP_BYTE_IN_ADDRESS_MASK & (Address_u32 >> ETH_IP_BYTE_0_IN_ADDRESS_POS));
    para_PhysAdrr_ptr[1] = (uint8_t)(ETH_IP_BYTE_IN_ADDRESS_MASK & (Address_u32 >> ETH_IP_BYTE_1_IN_ADDRESS_POS));
    para_PhysAdrr_ptr[2] = (uint8_t)(ETH_IP_BYTE_IN_ADDRESS_MASK & (Address_u32 >> ETH_IP_BYTE_2_IN_ADDRESS_POS));
    para_PhysAdrr_ptr[3] = (uint8_t)(ETH_IP_BYTE_IN_ADDRESS_MASK & (Address_u32 >> ETH_IP_BYTE_3_IN_ADDRESS_POS));
    /* Retrieve the value from the high register of the physical address */
    Address_u32         = READ_REG32(Base_ptr->MAC_MACADHR0);
    para_PhysAdrr_ptr[4] = (uint8_t)(ETH_IP_BYTE_IN_ADDRESS_MASK & (Address_u32 >> ETH_IP_BYTE_4_IN_ADDRESS_POS));
    para_PhysAdrr_ptr[5] = (uint8_t)(ETH_IP_BYTE_IN_ADDRESS_MASK & (Address_u32 >> ETH_IP_BYTE_5_IN_ADDRESS_POS));
}

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
)
{
    uint32_t Crc_u32           = 0x00U;
    uint32_t RegValue_u32      = 0x00U;
    CM_ETH_TypeDef *Base_ptr = s_EthBases_aa[para_Instance_u8];

    Crc_u32             = Eth_Ip_CalculateCrc32(para_MacAddr_ptr, 6U);
    Crc_u32             = Eth_Ip_ReverseBits(Crc_u32);
    RegValue_u32        = (uint32_t)(1UL << ((Crc_u32 >> 26UL) & 0x1FUL));
    /* Set the hash table according to the different crc value */
    if (0U != (Crc_u32 & 0x80000000U))
    {
        SET_REG32_BIT(Base_ptr->MAC_HASHTHR, RegValue_u32);
    }
    else
    {
        SET_REG32_BIT(Base_ptr->MAC_HASHTLR, RegValue_u32);
    }
}

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
)
{
    uint32_t Crc_u32      = 0x0UL;
    uint32_t RegValue_u32 = 0x00UL;
    CM_ETH_TypeDef *Base_ptr  = s_EthBases_aa[para_Instance_u8];

    Crc_u32             = Eth_Ip_CalculateCrc32(para_MacAddr_ptr, 6U);
    Crc_u32             = Eth_Ip_ReverseBits(Crc_u32);
    RegValue_u32        = (uint32_t)(1UL << ((Crc_u32 >> 26UL) & 0x1FUL));
    if (0U != (Crc_u32 & 0x80000000U))
    {
        CLR_REG32_BIT(Base_ptr->MAC_HASHTHR, RegValue_u32);
    }
    else
    {
        CLR_REG32_BIT(Base_ptr->MAC_HASHTLR, RegValue_u32);
    }
}

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
)
{
    CM_ETH_TypeDef *Base_ptr = s_EthBases_aa[para_Instance_u8];

    CLR_REG32(Base_ptr->MAC_HASHTHR);
    CLR_REG32(Base_ptr->MAC_HASHTLR);
}

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
)
{
    CM_ETH_TypeDef *Base_ptr = s_EthBases_aa[para_Instance_u8];

    para_RxCounter_ptr->RxUnicastPacketsGood_u32    = READ_REG32(Base_ptr->MMC_RXUNGFR);
    para_RxCounter_ptr->RxMulticastPacketsGood_u32  = READ_REG32(Base_ptr->MMC_RXMUGFR);
    para_RxCounter_ptr->RxBroadcastPacketsGood_u32  = READ_REG32(Base_ptr->MMC_RXBRGFR);
    para_RxCounter_ptr->RxCrcErrorPackets_u32       = READ_REG32(Base_ptr->MMC_RXCREFR);
    para_RxCounter_ptr->RxAlignmentErrorPackets_u32 = READ_REG32(Base_ptr->MMC_RXALEFR);
    para_RxCounter_ptr->RxRuntErrorPackets_u32      = READ_REG32(Base_ptr->MMC_RXRUEFR);
    para_RxCounter_ptr->RxLengthErrorPackets_u32    = READ_REG32(Base_ptr->MMC_RXLEEFR);
    para_RxCounter_ptr->RxOutOfRangeTypePackets_u32 = READ_REG32(Base_ptr->MMC_RXOREFR);
}

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
)
{
    CM_ETH_TypeDef *Base_ptr = s_EthBases_aa[para_Instance_u8];

    para_TxCounter_ptr->TxUnicastPacketsGood_u32      = READ_REG32(Base_ptr->MMC_TXUNGFR);
    para_TxCounter_ptr->TxMulticastPacketsGood_u32    = READ_REG32(Base_ptr->MMC_TXMUGFR);
    para_TxCounter_ptr->TxBroadcastPacketsGood_u32    = READ_REG32(Base_ptr->MMC_TXBRGFR);
    para_TxCounter_ptr->TxDeferredErrorFrame_u32      = READ_REG32(Base_ptr->MMC_TXDEEFR);
    para_TxCounter_ptr->TxCarrierErrorFrame_u32       = READ_REG32(Base_ptr->MMC_TXCAEFR);
    para_TxCounter_ptr->TxLateCollisionFrame_u32      = READ_REG32(Base_ptr->MMC_TXLCEFR);
    para_TxCounter_ptr->TxExcessiveCollisionFrame_u32 = READ_REG32(Base_ptr->MMC_TXECEFR);
    para_TxCounter_ptr->TxExcessiveDelayFrame_u32     = READ_REG32(Base_ptr->MMC_TXEDEFR);
}

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
)
{
    uint32_t RegValue_u32                                  = 0x00U;
    uint32_t Index_u32                                     = 0x00U;
    uint32_t Crc_u32                                       = 0x00U;
    Eth_Ip_StatusType Status_en                          = ETH_IP_STATUS_SUCCESS;
    CM_ETH_TypeDef *Base_ptr                             = s_EthBases_aa[para_Instance_u8];
    const Eth_Ip_VlanFilterConfigType *VlanFilterCfg_ptr = NULL_PTR;

    /* Configure MAC interface */
    if (ETH_IP_INTERFACE_RMII == para_Config_ptr->NormalCfg_ptr->Interface_en)
    {
        WRITE_REG32(CM_PERIC->ETH_MAC_IFCONFR, PERIC_ETH_MAC_IFCONFR_IFSEL);
    }
    else
    {
        WRITE_REG32(CM_PERIC->ETH_MAC_IFCONFR, 0U);
    }

    /* Initialize DMA controller */
    Status_en = Eth_Ip_InitDMA(Base_ptr, para_Config_ptr);

    if (ETH_IP_STATUS_SUCCESS == Status_en)
    {
        /* Configure the L3/L4 filtering */
        if (NULL_PTR != para_Config_ptr->L3L4FilterCfg_ptr)
        {
            Eth_Ip_Layer3Layer4FilterConfig(Base_ptr, para_Config_ptr->L3L4FilterCfg_ptr);
        }
        /* Configure the VLAN filtering */
        if (NULL_PTR != para_Config_ptr->VlanFilterCfg_ptr)
        {
            VlanFilterCfg_ptr = para_Config_ptr->VlanFilterCfg_ptr;
            RegValue_u32      = ((uint32_t)VlanFilterCfg_ptr->EnVlanTagHashTableMatch_bool << ETH_MAC_VTAFLTR_VTHM_POS) |
                                ((uint32_t)VlanFilterCfg_ptr->EnVlanTagInverseMatch_bool << ETH_MAC_VTAFLTR_VTIM_POS) |
                                ((uint32_t)VlanFilterCfg_ptr->En12bitVlanComparison_bool << ETH_MAC_VTAFLTR_VTAL_POS) |
                                ((uint32_t)VlanFilterCfg_ptr->VlanTag_u16 & ETH_MAC_VTAFLTR_VLFLT);
            WRITE_REG32(Base_ptr->MAC_VTAFLTR, RegValue_u32);
            /* Configure the hash table of VLAN tag filtering */
            if ((boolean)TRUE == VlanFilterCfg_ptr->EnVlanTagHashTableMatch_bool)
            {
                for (Index_u32 = 0U; Index_u32 < VlanFilterCfg_ptr->HashVlanNum_u32; Index_u32++)
                {
                    Crc_u32             = Eth_Ip_CalculateCrc32((const uint8_t *)&VlanFilterCfg_ptr->HashVlan_ptr[Index_u32], 2U);
                    Crc_u32             = Eth_Ip_ReverseBits(Crc_u32);
                    RegValue_u32        = (uint32_t)(1UL << ((Crc_u32 & ETH_IP_UPPER_4_BITS_MASK) >> ETH_IP_CRC32_BITS_31_28_POS));
                    /* Set the VLAN hash table */
                    SET_REG32_BIT(Base_ptr->MAC_VLAHTBR, RegValue_u32);
                }
            }
            /* Enable the VLAN tag filtering */
            SET_REG32_BIT(Base_ptr->MAC_FLTCTLR, ETH_MAC_FLTCTLR_VTFE);
        }
        /* Configure the outer VLAN and inner VLAN */
        if (NULL_PTR != para_Config_ptr->VlanCfg_ptr)
        {
            RegValue_u32 = ETH_MAC_VTACTLR_VLANS | ((uint32_t)para_Config_ptr->VlanCfg_ptr->VlanTxControl_en << ETH_MAC_VTACTLR_VLANC_POS) | 
                           ((uint32_t)para_Config_ptr->VlanCfg_ptr->VlanTag_u16 & ETH_MAC_VTACTLR_VLANV);
            WRITE_REG32(Base_ptr->MAC_VTACTLR, RegValue_u32);
        }
        /* Configure the 1588 PTP */
        if (NULL_PTR != para_Config_ptr->PtpCfg_ptr)
        {
            Status_en = Eth_Ip_Ptp1588Config(Base_ptr, para_Config_ptr->PtpCfg_ptr);
        }
        if (ETH_IP_STATUS_SUCCESS == Status_en)
        {
            /* Disable all MMC interrupt */
            WRITE_REG32(Base_ptr->MMC_RITCTLR, 0xFFFFFFFFU);
            WRITE_REG32(Base_ptr->MMC_TITCTLR, 0xFFFFFFFFU);
            /* Initialize MAC controller */
            Eth_Ip_InitMAC(Base_ptr, para_Config_ptr->NormalCfg_ptr);
            /* Initialize IP status structure */
            Eth_Ip_InitCtrlStateStruct(para_Instance_u8, para_Config_ptr);
        }
    }

    return Status_en;
}

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
)
{
    uint16_t TxDescId_u16                         = 0x00U;
    uint32_t Value_u32                            = 0x00U;
    CM_ETH_TypeDef *Base_ptr                    = s_EthBases_aa[para_Instance_u8];
    Eth_Ip_StatusType Status_en                 = ETH_IP_STATUS_SUCCESS;
    Eth_Ip_TxBdRingType *TxBdRing_ptr           = &s_EthState_aa[para_Instance_u8]->TxBdRing_st;
#if (STD_OFF == ETH_IP_ENABLE_ENHANCED_DESC)
    Eth_Ip_TxBufferDescriptorType *ListBd_ptr   = TxBdRing_ptr->TxBdBase_ptr;
    Eth_Ip_TxBufferDescriptorType *Bd_ptr       = &ListBd_ptr[TxBdRing_ptr->TxGenIdx_u16];

#else
    Eth_Ip_TxBufferEnDescriptorType *ListBd_ptr = TxBdRing_ptr->TxEnBdBase_ptr;
    Eth_Ip_TxBufferEnDescriptorType *Bd_ptr     = &ListBd_ptr[TxBdRing_ptr->TxGenIdx_u16];
#endif /*!< (STD_OFF == ETH_IP_ENABLE_ENHANCED_DESC) */

    /* Calculate the index of the currently available BD */
    TxDescId_u16 = (uint16_t)((sint32)(Bd_ptr - ListBd_ptr));
    /* Check whether BD own by hardware */
    if (0U != (Bd_ptr->TxControlStat_u32 & ETH_IP_TDES0_OWN_MASK))
    {
        Status_en = ETH_IP_STATUS_TX_QUEUE_FULL;
    }
    else
    {
        /* Record the transmit buffer address.*/
        TxBdRing_ptr->TxBuffer_pp[TxDescId_u16] = para_Buff_ptr;
        /* Prepare Tx BD.*/
        Bd_ptr->TxBuff1Addr_u32   = (uint32_t)para_Buff_ptr;
        Bd_ptr->TxBuff2Addr_u32   = 0U;
        Bd_ptr->TxBuffLen_u32     = para_Length_u32 & ETH_IP_TDES1_TBS1_MASK;
        Value_u32                 = (Bd_ptr->TxControlStat_u32 & ETH_IP_TDES0_TER_MASK) | para_TxOption_u32 | 
                                        ETH_IP_TDES0_TFS_MASK | ETH_IP_TDES0_TLS_MASK | ETH_IP_TDES0_OWN_MASK;
        Bd_ptr->TxControlStat_u32 = Value_u32;
        ASM_DMB();
        WRITE_REG32(Base_ptr->DMA_TXPOLLR, 1U);
        ASM_DSB();
        TxBdRing_ptr->TxGenIdx_u16 = Eth_Ip_IncreaseIndex(TxBdRing_ptr->TxGenIdx_u16, TxBdRing_ptr->TxRingLen_u16);
    }

    return Status_en;
}

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
)
{
    uint32_t Control_u32;
    uint8_t **TxBuffer_pp;
    uint8_t **TxBufferTemp_ptr;
    uint16_t TxRingLen_u16;
#if (STD_OFF == ETH_IP_ENABLE_ENHANCED_DESC)
    Eth_Ip_TxBufferDescriptorType *Bd_ptr       = NULL_PTR;
    Eth_Ip_TxBufferDescriptorType *ListBd_ptr   = s_EthState_aa[para_Instance_u8]->TxBdRing_st.TxBdBase_ptr;
#else
    Eth_Ip_TxBufferEnDescriptorType *Bd_ptr     = NULL_PTR;
    Eth_Ip_TxBufferEnDescriptorType *ListBd_ptr = s_EthState_aa[para_Instance_u8]->TxBdRing_st.TxEnBdBase_ptr;
#endif /*!< (STD_OFF == ETH_IP_ENABLE_ENHANCED_DESC) */
    Eth_Ip_StatusType Status_en                 = ETH_IP_STATUS_TX_NO_COMPLETE;

    /* Get Tx BD length and Tx buffer start address */
    TxBuffer_pp   = s_EthState_aa[para_Instance_u8]->TxBdRing_st.TxBuffer_pp;
    TxRingLen_u16 = s_EthState_aa[para_Instance_u8]->TxBdRing_st.TxRingLen_u16;
    /* Find the special tx buffer in the BD list */
    for (uint16_t Index_u16 = 0U; Index_u16 < TxRingLen_u16; Index_u16++)
    {
        if (para_Buff_ptr == TxBuffer_pp[Index_u16])
        {
            TxBufferTemp_ptr = &TxBuffer_pp[Index_u16];
            Bd_ptr           = &ListBd_ptr[Index_u16];
            Control_u32      = Bd_ptr->TxControlStat_u32;
            while ((0U == (Control_u32 & (uint32_t)ETH_IP_TDES0_OWN_MASK)) && (0U == (Control_u32 & (uint32_t)ETH_IP_TDES0_TLS_MASK)))
            {
                *TxBufferTemp_ptr = NULL_PTR;
                Bd_ptr++;
                TxBufferTemp_ptr++;
                if ((uint32_t)Bd_ptr >= (uint32_t)&ListBd_ptr[TxRingLen_u16])
                {
                    Bd_ptr = ListBd_ptr;
                }
                Control_u32 = Bd_ptr->TxControlStat_u32;
                if (Bd_ptr == &(ListBd_ptr[Index_u16]))
                {
                    /* Avoid endless loop */
                    break;
                }
            }
            if ((0U == (Control_u32 & (uint32_t)ETH_IP_TDES0_OWN_MASK)) && (0U != (Control_u32 & (uint32_t)ETH_IP_TDES0_TLS_MASK)))
            {
#if (STD_ON == ETH_IP_ENABLE_ENHANCED_DESC)
                /* Check whether has Tx timestamp */
                if (0U != (Control_u32 & (uint32_t)ETH_IP_TDES0_TTSS_MASK))
                {
                    /* Get transmit timestamp nanoseconds. */
                    para_TxStatus_ptr->TxTimeStamp_st.Nanosecond_u32 = Bd_ptr->TimeStampLow_u32;
                    para_TxStatus_ptr->TxTimeStamp_st.Second_u64     = Bd_ptr->TimeStampHigh_u32;
                }
                else
                {
                    para_TxStatus_ptr->TxTimeStamp_st.Nanosecond_u32 = 0U;
                    para_TxStatus_ptr->TxTimeStamp_st.Second_u64     = 0U;
                }
#else
                para_TxStatus_ptr->TxTimeStamp_st.Nanosecond_u32 = 0U;
                para_TxStatus_ptr->TxTimeStamp_st.Second_u64     = 0U;
#endif /*!< (STD_ON == ETH_IP_ENABLE_ENHANCED_DESC) */
                para_TxStatus_ptr->ErrStatus_u32                 = Control_u32 & ETH_IP_TDES0_ERR_MASK;
                *TxBufferTemp_ptr                                = NULL_PTR;
                Status_en                                        = ETH_IP_STATUS_SUCCESS;
            }
            break;
        }
    }

    return Status_en;
}

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
)
{
    uint32_t Control_u32;
    boolean ResultFlag_bool                     = FALSE;
    Eth_Ip_StatusType Status_en                 = ETH_IP_STATUS_ERROR;
    Eth_Ip_RxBdRingType *RxBdRing_ptr           = (Eth_Ip_RxBdRingType *)&s_EthState_aa[para_Instance_u8]->RxBdRing_st;
    uint16_t Index_u16                            = RxBdRing_ptr->RxGenIdx_u16;
#if (STD_OFF == ETH_IP_ENABLE_ENHANCED_DESC)
    Eth_Ip_RxBufferDescriptorType *RxDesc_ptr   = NULL_PTR;
#else
    Eth_Ip_RxBufferEnDescriptorType *RxDesc_ptr = NULL_PTR;
#endif /*!< (STD_OFF == ETH_IP_ENABLE_ENHANCED_DESC) */

    /* Reset the length to zero */
    *para_Length_ptr = 0;

    for (uint16_t TimeReadDesc_u16 = 0U; TimeReadDesc_u16 < RxBdRing_ptr->RxRingLen_u16; TimeReadDesc_u16++)
    {
#if (STD_OFF == ETH_IP_ENABLE_ENHANCED_DESC)
        RxDesc_ptr  = &RxBdRing_ptr->RxBdBase_ptr[Index_u16];
#else
        RxDesc_ptr  = &RxBdRing_ptr->RxEnBdBase_ptr[Index_u16];
#endif /*!< (STD_OFF == ETH_IP_ENABLE_ENHANCED_DESC) */
        Control_u32 = RxDesc_ptr->RxControl_u32;
        if (0U != (Control_u32 & ETH_IP_RDES0_OWN_MASK))
        {
            Status_en       = ETH_IP_STATUS_RX_QUEUE_EMPTY;
            ResultFlag_bool = TRUE;
        }
        else
        {
            /* Application owns the buffer descriptor, get the length */
            if (0U != (Control_u32 & ETH_IP_RDES0_RLS_MASK))
            {
                if (0U != (Control_u32 & ETH_IP_RDES0_ERRSUM_MASK))
                {
                    Status_en = ETH_IP_STATUS_ERROR;
                }
                else
                {
                    *para_Length_ptr = ((RxDesc_ptr->RxControl_u32 & ETH_IP_RDES0_FRAL_MASK) >> ETH_IP_RDES0_FRAL_POS) - ETH_IP_FRAME_CHECK_SEQUENCE_LEN;
                    Status_en        = ETH_IP_STATUS_SUCCESS;
                }
                ResultFlag_bool = TRUE;
            }
            else
            {
                Index_u16 = Eth_Ip_IncreaseIndex(Index_u16, RxBdRing_ptr->RxRingLen_u16);
            }
        }
        if (TRUE == ResultFlag_bool)
        {
            break;
        }
    }

    return Status_en;
}

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
)
{
    uint32_t Control_u32;
    boolean ResultFlag_bool                     = FALSE;
    Eth_Ip_RxBdRingType *RxBdRing_ptr           = &s_EthState_aa[para_Instance_u8]->RxBdRing_st;
    uint16_t Index_u16                            = RxBdRing_ptr->RxGenIdx_u16;
#if (STD_OFF == ETH_IP_ENABLE_ENHANCED_DESC)
    Eth_Ip_RxBufferDescriptorType *RxDesc_ptr   = NULL_PTR;
#else
    Eth_Ip_RxBufferEnDescriptorType *RxDesc_ptr = NULL_PTR;
#endif /*!< (STD_OFF == ETH_IP_ENABLE_ENHANCED_DESC) */
    Eth_Ip_StatusType Status_en                 = ETH_IP_STATUS_RX_QUEUE_EMPTY;

    for (uint32_t RxDesc_u32 = 0U; RxDesc_u32 < RxBdRing_ptr->RxRingLen_u16; RxDesc_u32++)
    {
        /* Record buffer start address */
        para_Buffer_ptr->Buffer_ptr = &RxBdRing_ptr->RxBuffer_ptr[Index_u16 * RxBdRing_ptr->RxBuffLen_u16];
        /* Retrieve the receive descriptor */
#if (STD_OFF == ETH_IP_ENABLE_ENHANCED_DESC)
        RxDesc_ptr  = &RxBdRing_ptr->RxBdBase_ptr[Index_u16];
#else
        RxDesc_ptr  = &RxBdRing_ptr->RxEnBdBase_ptr[Index_u16];
#endif /*!< (STD_OFF == ETH_IP_ENABLE_ENHANCED_DESC) */
        Control_u32 = RxDesc_ptr->RxControl_u32;
        if (0U != (Control_u32 & ETH_IP_RDES0_OWN_MASK))
        {
            Status_en       = ETH_IP_STATUS_RX_QUEUE_EMPTY;
            ResultFlag_bool = TRUE;
        }
        else
        {
            Index_u16 = Eth_Ip_IncreaseIndex(Index_u16, RxBdRing_ptr->RxRingLen_u16);
            /* Check whether received frame stored in one Rx desc (both set LD and FD) and no error*/
            if (0U != (Control_u32 & ETH_IP_RDES0_RLS_MASK))
            {
                RxBdRing_ptr->RxGenIdx_u16 = Index_u16;
                if ((0U != (Control_u32 & ETH_IP_RDES0_RFS_MASK)) && (0U == (Control_u32 & ETH_IP_RDES0_ERRSUM_MASK)))
                {
                    Status_en = ETH_IP_STATUS_SUCCESS;
                }
                else
                {
                    Status_en = ETH_IP_STATUS_ERROR;
                }
                ResultFlag_bool = TRUE;
            }
        }
        if (TRUE == ResultFlag_bool)
        {
            break;
        }
    }

    if (ETH_IP_STATUS_SUCCESS == Status_en)
    {
        para_BufferDesc_ptr->RxControl_u32     = RxDesc_ptr->RxControl_u32;
        para_BufferDesc_ptr->RxBuffLen_u32     = RxDesc_ptr->RxBuffLen_u32;
        para_BufferDesc_ptr->RxBuff1Addr_u32   = RxDesc_ptr->RxBuff1Addr_u32;
        para_BufferDesc_ptr->RxBuff2Addr_u32   = RxDesc_ptr->RxBuff2Addr_u32;
#if (STD_ON == ETH_IP_ENABLE_ENHANCED_DESC)
        para_BufferDesc_ptr->RxExtStatus_u32   = RxDesc_ptr->RxExtStatus_u32;
        para_BufferDesc_ptr->Reserved2_u32     = RxDesc_ptr->Reserved2_u32;
        para_BufferDesc_ptr->TimeStampLow_u32  = RxDesc_ptr->TimeStampLow_u32;
        para_BufferDesc_ptr->TimeStampHigh_u32 = RxDesc_ptr->TimeStampHigh_u32;
#endif /*!< (STD_ON == ETH_IP_ENABLE_ENHANCED_DESC) */
        para_Buffer_ptr->Length_u16            = (uint16_t)((RxDesc_ptr->RxControl_u32 & ETH_IP_RDES0_FRAL_MASK) >> ETH_IP_RDES0_FRAL_POS);
    }
    else
    {
        para_BufferDesc_ptr->RxControl_u32     = 0U;
        para_BufferDesc_ptr->RxBuffLen_u32     = 0U;
        para_BufferDesc_ptr->RxBuff1Addr_u32   = 0U;
        para_BufferDesc_ptr->RxBuff2Addr_u32   = 0U;
#if (STD_ON == ETH_IP_ENABLE_ENHANCED_DESC)
        para_BufferDesc_ptr->RxExtStatus_u32   = 0U;
        para_BufferDesc_ptr->Reserved2_u32     = 0U;
        para_BufferDesc_ptr->TimeStampLow_u32  = 0U;
        para_BufferDesc_ptr->TimeStampHigh_u32 = 0U;
#endif /*!< (STD_ON == ETH_IP_ENABLE_ENHANCED_DESC) */
        para_Buffer_ptr->Length_u16            = 0U;
    }

    return Status_en;
}

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
)
{
    CM_ETH_TypeDef *Base_ptr                    = s_EthBases_aa[para_Instance_u8];
    Eth_Ip_RxBdRingType *RxBdRing_ptr           = &s_EthState_aa[para_Instance_u8]->RxBdRing_st;
#if (STD_OFF == ETH_IP_ENABLE_ENHANCED_DESC)
    Eth_Ip_RxBufferDescriptorType *RxDesc_ptr   = &RxBdRing_ptr->RxBdBase_ptr[RxBdRing_ptr->RxConsumIdx_u16];
#else
    Eth_Ip_RxBufferEnDescriptorType *RxDesc_ptr = &RxBdRing_ptr->RxEnBdBase_ptr[RxBdRing_ptr->RxConsumIdx_u16];
#endif /*!< (STD_OFF == ETH_IP_ENABLE_ENHANCED_DESC) */

    /* Update the Rx Descriptor */
    RxDesc_ptr->RxControl_u32     = para_BufferDesc_ptr->RxControl_u32;
    RxDesc_ptr->RxBuffLen_u32     = (RxDesc_ptr->RxBuffLen_u32 & ETH_IP_RDES1_RER_MASK) | para_BufferDesc_ptr->RxBuffLen_u32;
    RxDesc_ptr->RxBuff1Addr_u32   = para_BufferDesc_ptr->RxBuff1Addr_u32;
    RxDesc_ptr->RxBuff2Addr_u32   = para_BufferDesc_ptr->RxBuff2Addr_u32;
#if (STD_ON == ETH_IP_ENABLE_ENHANCED_DESC)
    RxDesc_ptr->RxExtStatus_u32   = para_BufferDesc_ptr->RxExtStatus_u32;
    RxDesc_ptr->Reserved2_u32     = para_BufferDesc_ptr->Reserved2_u32;
    RxDesc_ptr->TimeStampLow_u32  = para_BufferDesc_ptr->TimeStampLow_u32;
    RxDesc_ptr->TimeStampHigh_u32 = para_BufferDesc_ptr->TimeStampHigh_u32;
#endif /*!< (STD_ON == ETH_IP_ENABLE_ENHANCED_DESC) */
    /* Update the consum index */
    RxBdRing_ptr->RxConsumIdx_u16 = Eth_Ip_IncreaseIndex(RxBdRing_ptr->RxConsumIdx_u16, RxBdRing_ptr->RxRingLen_u16);
    /* Try to start receive, in case it had stopped */
    WRITE_REG32(Base_ptr->DMA_RXPOLLR, 1U);
}

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
)
{
    uint32_t EthIpStartTime_u32;
    uint32_t EthIpTimeoutTicks_u32;
    uint32_t EthIpElapsedTicks_u32    = 0U;
    uint32_t Sec_aa[ETH_IP_64_DIV_32] = {0U, 0U};
    Eth_Ip_StatusType Status_en     = ETH_IP_STATUS_SUCCESS;
    CM_ETH_TypeDef *Base_ptr        = s_EthBases_aa[para_Instance_u8];

    Sec_aa[1] = READ_REG32(Base_ptr->PTP_TMSSECR);
    do
    {
        Eth_Ip_StartTimer(&EthIpStartTime_u32, &EthIpTimeoutTicks_u32, (uint32_t)ETH_IP_LOOP_TIMEOUT_LENGTH,
                          ETH_IP_TIMEOUT_METHOD_SUPPORT);
        Sec_aa[0]             = Sec_aa[1];
        *para_Nanoseconds_ptr = (READ_REG32(Base_ptr->PTP_TMSNSER) & ETH_PTP_TMSNSER_TSPSYSNSEC);
        Sec_aa[1]             = READ_REG32(Base_ptr->PTP_TMSSECR);

        ETH_IP_CHECK_TIMEOUT(&EthIpStartTime_u32, &EthIpElapsedTicks_u32, EthIpTimeoutTicks_u32, Status_en)
    } while (Sec_aa[0] != Sec_aa[1]);

    if (ETH_IP_STATUS_SUCCESS == Status_en)
    {
        *para_Second_ptr   = Sec_aa[1];
    }

    return Status_en;
}

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
)
{
    CM_ETH_TypeDef *Base_ptr = s_EthBases_aa[para_Instance_u8];
    uint32_t StatusFlags_u32   = READ_REG32(Base_ptr->DMA_DMASTSR);

    StatusFlags_u32          &= READ_REG32(Base_ptr->DMA_INTENAR) & (ETH_DMA_INTENAR_TIE | ETH_DMA_INTENAR_RIE);

    if (NULL_PTR != s_EthState_aa[para_Instance_u8])
    {
        if (0U != (StatusFlags_u32 & ETH_DMA_INTENAR_RIE))
        {
            /* Clear the interrupt event Dma Channel */
            SET_REG32_BIT(Base_ptr->DMA_DMASTSR, ETH_DMA_INTENAR_RIE);
            /* Call the channel received callback function  */
            s_EthState_aa[para_Instance_u8]->RxCallback_pfn(para_Instance_u8, 0U);
        }
        if (0U != (StatusFlags_u32 & ETH_DMA_INTENAR_TIE))
        {
            /* Clear the interrupt event Dma Channel. */
            SET_REG32_BIT(Base_ptr->DMA_DMASTSR, ETH_DMA_INTENAR_TIE);
            /* Call the channel Transmit callback function */
            s_EthState_aa[para_Instance_u8]->TxCallback_pfn(para_Instance_u8, 0U);
        }
    }
    else
    {
        /* If interrupt no be enabled, only clear the interrupt status without call callback. */
        SET_REG32_BIT(Base_ptr->DMA_DMASTSR, StatusFlags_u32);
    }
}

#define ETH_STOP_SEC_CODE
#include "Eth_MemMap.h"

#ifdef __cplusplus
}
#endif

/**
 * @}
 */
