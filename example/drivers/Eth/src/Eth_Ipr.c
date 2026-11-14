/***********************************************************************************************************************
 * Project Name: XHSC AUTOSAR MCAL
 * File Name: Eth_Ipr.c
 *
 * Description: Implementation of ETH IP Router Layer Driver
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
 *   @file       Eth_Ipr.c
 *   @version    1.0.3
 *   @brief      Eth IP Router Layer Driver
 *   @addtogroup eth_ipr_driver
 *   @{
 */
#ifdef __cplusplus
extern "C" {
#endif

/***********************************************************************************************************************
 *  INCLUDES
 **********************************************************************************************************************/
#include "Eth_Ipr.h"
#include "Eth_Types.h"
#include "Eth_Ip.h"
#include "SchM_Eth.h"

/***********************************************************************************************************************
 *  DEFINES & MACROS
 **********************************************************************************************************************/

/***********************************************************************************************************************
 *  EXTERN
 **********************************************************************************************************************/

/***********************************************************************************************************************
 *  LOCAL TYPEDEFS
 **********************************************************************************************************************/
/**
 * @brief Define the Phy Address.
 */
typedef enum
{
    ETH_PHY_ADDR_OPEN_FILTER_TYPE   = 0x00U, /*!< Phy Address Open Filter. */
    ETH_PHY_ADDR_REDUCE_FILTER_TYPE = 0x01U, /*!< Phy Address Reduce Filter. */
    ETH_PHY_ADDR_MULTICAST_TYPE     = 0x02U, /*!< Phy Address Multicast. */
    ETH_PHY_ADDR_UNICAST_TYPE       = 0x03U  /*!< Phy Address Unicast. */
} Eth_PhyAddrType;

/***********************************************************************************************************************
 *  LOCAL CONSTANTS
 **********************************************************************************************************************/

/***********************************************************************************************************************
 *  LOCAL VARIABLES
 **********************************************************************************************************************/
#define ETH_START_SEC_VAR_CLEARED_UNSPECIFIED
#include "Eth_MemMap.h"
/**
 * @brief Priorty to FIFO index mapping info
 */
static uint8_t s_EthPriorityFifoIdMap_aa[ETH_MAX_CONTROLLER_COUNT][ETH_MAX_PRIORITY];
/**
 * @brief Pointer to state of ETH IP
 */
static Eth_Ip_StateType *s_EthIpState_aa[ETH_MAX_CONTROLLER_COUNT];
/**
 * @brief Tx buffers info
 */
static Eth_Ipr_TxBufferInfoType s_EthTxBufferInfo_aa[ETH_MAX_CONTROLLER_COUNT]
                                                            [ETH_MAX_TX_BUFFER_NUM * ETH_MAX_TX_FIFO_NUM];
/**
 * @brief Tx buffers list info
 */
static Eth_Ipr_TxBufferListType s_EthTxBufferList_aa[ETH_MAX_CONTROLLER_COUNT]
                                                            [ETH_MAX_TX_FIFO_NUM];
/**
 * @brief FIFO first buffer index info
 */
static Eth_BufIdxType s_EthFifoFirstBufferIdx_aa[ETH_MAX_CONTROLLER_COUNT][ETH_MAX_TX_FIFO_NUM];
/**
 * @brief Rx buffer info
 */
static Eth_Ipr_RxBufferInfoType s_EthRxBufferInfo_aa[ETH_MAX_CONTROLLER_COUNT]
                                                            [ETH_MAX_RX_FIFO_NUM];
/**
 * @brief ETH_IP MAC address filter completely open status
 */
static boolean s_EthIprOpenMacAddrFilter_aa[ETH_MAX_CONTROLLER_COUNT];
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

static Eth_PhyAddrType Eth_Ipr_CheckPhyAddressType(
    const uint8_t *para_PhyAddr_ptr
);

static void Eth_Ipr_Memset(
    uint8_t *para_DestAdrr_ptr,
    uint8_t para_Value_u8,
    uint32_t para_Bytes_u32
);
/***********************************************************************************************************************
 *  LOCAL FUNCTION
 **********************************************************************************************************************/
/**
 *  @brief          Check Phy Address Type.
 *
 *  @param[in]      para_PhyAddr_ptr    None
 *
 *  @return         Eth_PhyAddrType
 *  @retval         None
 *
 *  @note           None
 *
 *  @pre            None
 *
 */
static Eth_PhyAddrType Eth_Ipr_CheckPhyAddressType(
    const uint8_t *para_PhyAddr_ptr
)
{
    uint8_t IsBroadcast_u8    = 0x00U;
    uint8_t IsZero_u8         = 0x00U;
    Eth_PhyAddrType Type_en = ETH_PHY_ADDR_UNICAST_TYPE;

    /* Check the different type of MAC address */
    for (uint8_t i = 0; i < ETH_FRAME_MAC_BYTES; i++)
    {
        if (0xFFU == para_PhyAddr_ptr[i])
        {
            IsBroadcast_u8++;
        }
        else if (0x00U == para_PhyAddr_ptr[i])
        {
            IsZero_u8++;
        }
        else
        {
            break;
        }
    }
    /* Check MAC address to identify the address type */
    if (ETH_FRAME_MAC_BYTES == IsBroadcast_u8)
    {
        Type_en = ETH_PHY_ADDR_OPEN_FILTER_TYPE;
    }
    else if (ETH_FRAME_MAC_BYTES == IsZero_u8)
    {
        Type_en = ETH_PHY_ADDR_REDUCE_FILTER_TYPE;
    }
    else
    {
        if (0x1U == (para_PhyAddr_ptr[0] & 0x1U))
        {
            Type_en = ETH_PHY_ADDR_MULTICAST_TYPE;
        }
        else
        {
            Type_en = ETH_PHY_ADDR_UNICAST_TYPE;
        }
    }
    return Type_en;
}

/**
 *  @brief          Memset.
 *
 *  @param[in]      para_DestAdrr_ptr    None
 *  @param[in]      para_Value_u8        None
 *  @param[in]      para_Bytes_u32       None
 *
 *  @return         void
 *
 *  @note           None
 *
 *  @pre            None
 *
 */
static void Eth_Ipr_Memset(
    uint8_t *para_DestAdrr_ptr,
    uint8_t para_Value_u8,
    uint32_t para_Bytes_u32
)
{
    for (uint32_t Index_u32 = 0U; Index_u32 < para_Bytes_u32; Index_u32++)
    {
        para_DestAdrr_ptr[Index_u32] = para_Value_u8;
    }
}
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
)
{
    boolean Access_bool = TRUE;

    if (ETH_IP_CTRL_MODE_NOT_ACCESS == Eth_Ip_GetControllerMode(para_CtrlIdx_u8))
    {
        /* IP cannot be access due to missing clock */
        Access_bool = FALSE;
    }

    return Access_bool;
}

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
)
{
    boolean IdxValid_bool = TRUE;

    if ((ETH_MAX_TX_BUFFER_NUM * ETH_MAX_TX_FIFO_NUM) <= para_BufIdx_en)
    {
        IdxValid_bool = FALSE;
    }
    else if (FALSE == s_EthTxBufferInfo_aa[para_CtrlIdx_u8][para_BufIdx_en].Lock_bool)
    {
        /* Check that if buffer is locked (allocated but not transfered). */
        IdxValid_bool = FALSE;
    }
    else
    {
        /* Fulfill the remaining conditions, do nothing. */
    }

    return IdxValid_bool;
}

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
)
{
    Eth_Ip_RxCounterType RxCounters_st;
    Eth_Ip_TxCounterType TxCounters_st;

    /* Get the different types of receive packet counter */
    Eth_Ip_GetRxCounters(para_CtrlIdx_u8, &RxCounters_st);
    /* Get the different types of transmit packet counter */
    Eth_Ip_GetTxCounters(para_CtrlIdx_u8, &TxCounters_st);
    para_ErrorCounter_ptr[ETH_E_RX_FRAMES_LOST]    = 0U;
    para_ErrorCounter_ptr[ETH_E_CRC]               = RxCounters_st.RxCrcErrorPackets_u32;
    para_ErrorCounter_ptr[ETH_E_UNDERSIZEFRAME]    = 0U;
    para_ErrorCounter_ptr[ETH_E_OVERSIZEFRAME]     = 0U;
    para_ErrorCounter_ptr[ETH_E_ALIGNMENT]         = RxCounters_st.RxAlignmentErrorPackets_u32;
    para_ErrorCounter_ptr[ETH_E_SINGLECOLLISION]   = 0U;
    para_ErrorCounter_ptr[ETH_E_MULTIPLECOLLISION] = 0U;
    para_ErrorCounter_ptr[ETH_E_LATECOLLISION]     = TxCounters_st.TxLateCollisionFrame_u32;
}
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
)
{
    /* Initialize the controller IPR state structure pointer. */
    s_EthIpState_aa[para_CtrlIdx_u8] = NULL_PTR;
    /* Initialize the transmit buffer. */
    Eth_Ipr_Memset((uint8_t *)&s_EthTxBufferInfo_aa[para_CtrlIdx_u8][0], 0U,
                           sizeof(Eth_Ipr_TxBufferInfoType) * ETH_MAX_TX_BUFFER_NUM * ETH_MAX_TX_FIFO_NUM);
    /* Initialize the transmit list buffer. */
    Eth_Ipr_Memset((uint8_t *)&s_EthTxBufferList_aa[para_CtrlIdx_u8][0], 0U,
                           sizeof(Eth_Ipr_TxBufferListType) * ETH_MAX_TX_FIFO_NUM);
    /* Initialize the FIFO buffer. */
    Eth_Ipr_Memset((uint8_t *)&s_EthFifoFirstBufferIdx_aa[para_CtrlIdx_u8][0], 0U,
                           sizeof(Eth_BufIdxType) * ETH_MAX_TX_FIFO_NUM);
    /* Initialize the receive buffer. */
    Eth_Ipr_Memset((uint8_t *)&s_EthRxBufferInfo_aa[para_CtrlIdx_u8][0], 0U,
                           sizeof(Eth_Ipr_RxBufferInfoType) * ETH_MAX_RX_FIFO_NUM);
    /* Initialize the MAC address filter completely open status. */
    s_EthIprOpenMacAddrFilter_aa[para_CtrlIdx_u8] = FALSE;
    /* Initialize the FIFO priority to FIFO index map. */
    for (uint32_t Index_u32 = 0U; Index_u32 < ETH_MAX_PRIORITY; Index_u32++)
    {
        s_EthPriorityFifoIdMap_aa[para_CtrlIdx_u8][Index_u32] = para_IprCfg_ptr->PriorityToEgressFifoIdx_aa[Index_u32];
    }
}

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
)
{
    Std_ReturnType Status_en       = E_OK;
    Eth_Ip_StatusType IpStatus_en  = ETH_IP_STATUS_ERROR;

    if (NULL_PTR == s_EthIpState_aa[para_CtrlIdx_u8])
    {
        /* Invoke IPL function to complete controller initialization. */
        IpStatus_en = Eth_Ip_Init(para_CtrlIdx_u8, para_Config_ptr->CtrlCfg_ptr);
        /* Check the IP initialization status. */
        if (ETH_IP_STATUS_SUCCESS == IpStatus_en)
        {
            s_EthIpState_aa[para_CtrlIdx_u8]                      = para_Config_ptr->CtrlCfg_ptr->CtrlState_ptr;
            s_EthTxBufferList_aa[para_CtrlIdx_u8][0U].Buffer_ptr  = para_Config_ptr->TxPool_ptr->TxBuffer_ptr;
            s_EthTxBufferList_aa[para_CtrlIdx_u8][0U].BuffLen_u16 = para_Config_ptr->TxPool_ptr->TxBuffLen_u16;
        }
    }
    /* Change controller mode only when the controller completes initialization. */
    if (NULL_PTR != s_EthIpState_aa[para_CtrlIdx_u8])
    {
        if (ETH_MODE_DOWN == para_CtrlMode_en)
        {
            /* Disable the DMA transmission and receive */
            IpStatus_en = Eth_Ip_EnableController(para_CtrlIdx_u8, FALSE);
            if (ETH_IP_STATUS_SUCCESS == IpStatus_en)
            {
                /* Reset the related transmit and receive buffer */
                Eth_Ipr_Memset((uint8_t *)&s_EthTxBufferInfo_aa[para_CtrlIdx_u8][0], 0U,
                                       sizeof(Eth_Ipr_TxBufferInfoType) * ETH_MAX_TX_BUFFER_NUM * ETH_MAX_TX_FIFO_NUM);
                Eth_Ipr_Memset((uint8_t *)&s_EthRxBufferInfo_aa[para_CtrlIdx_u8][0], 0U,
                                       sizeof(Eth_Ipr_RxBufferInfoType) * ETH_MAX_RX_FIFO_NUM);
                s_EthTxBufferList_aa[para_CtrlIdx_u8][0U].BufferNum_u16    = 0U;
                s_EthTxBufferList_aa[para_CtrlIdx_u8][0U].TxBufferHead_ptr = NULL_PTR;
                s_EthTxBufferList_aa[para_CtrlIdx_u8][0U].TxBufferTail_ptr = NULL_PTR;
            }
        }
        else
        {
            /* Enable DMA transmission and receive */
            IpStatus_en = Eth_Ip_EnableController(para_CtrlIdx_u8, TRUE);
        }
    }

    if (ETH_IP_STATUS_SUCCESS != IpStatus_en)
    {
        Status_en = E_NOT_OK;
    }

    return Status_en;
}

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
)
{
    Eth_ModeType CtrlMode_en;

    if (ETH_IP_CTRL_MODE_ACTIVE == Eth_Ip_GetControllerMode(para_CtrlIdx_u8))
    {
        CtrlMode_en = ETH_MODE_ACTIVE;
    }
    else
    {
        CtrlMode_en = ETH_MODE_DOWN;
    }

    return CtrlMode_en;
}

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
)
{
    Eth_Ip_ReadMacAddr(para_CtrlIdx_u8, para_PhysAdrr_ptr);
}

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
)
{
    Eth_Ip_UpdateMacAddr(para_CtrlIdx_u8, para_PhysAdrr_ptr);
}

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
)
{
    Std_ReturnType Status_en = E_OK;
    Eth_PhyAddrType PhyAddrType_en;

    /* Check the current address type for MAC filter */
    PhyAddrType_en = Eth_Ipr_CheckPhyAddressType(para_PhysAdrr_ptr);

    if (ETH_PHY_ADDR_OPEN_FILTER_TYPE == PhyAddrType_en)
    {
        /* Enable Promiscuous Mode */
        Eth_Ip_EnableReceivePromiscuousMode(para_CtrlIdx_u8, TRUE);
        s_EthIprOpenMacAddrFilter_aa[para_CtrlIdx_u8] = TRUE;
    }
    else if (ETH_PHY_ADDR_REDUCE_FILTER_TYPE == PhyAddrType_en)
    {
        /* Reduce the filter to the controllers unique unicast MAC address */
        Eth_Ip_EnableReceivePromiscuousMode(para_CtrlIdx_u8, FALSE);
        Eth_Ip_ClearAllMacAddrFromHashTable(para_CtrlIdx_u8);
        s_EthIprOpenMacAddrFilter_aa[para_CtrlIdx_u8] = FALSE;
    }
    else if ((ETH_PHY_ADDR_MULTICAST_TYPE == PhyAddrType_en) && (TRUE != s_EthIprOpenMacAddrFilter_aa[para_CtrlIdx_u8]))
    {
        /* Enable do hash filter for multicast MAC address first */
        Eth_Ip_EnableMulticastMacAddrHashFilter(para_CtrlIdx_u8);
        if (ETH_ADD_TO_FILTER == para_Action_en)
        {
            Eth_Ip_AddMacAddrToHashTable(para_CtrlIdx_u8, para_PhysAdrr_ptr);
        }
        else
        {
            Eth_Ip_RemoveMacAddrFromHashTable(para_CtrlIdx_u8, para_PhysAdrr_ptr);
        }
    }
    else
    {
        /* Can't add new address when filter is completely open or try add a unicast address. */
        Status_en = E_NOT_OK;
    }

    return Status_en;
}

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
)
{
    Std_ReturnType Status_en = E_OK;

    if (ETH_IP_STATUS_SUCCESS != Eth_Ip_WriteMdioWithC22(para_CtrlIdx_u8, para_TrcvIdx_u8, para_RegIdx_u8, para_RegVal_u16))
    {
        Status_en = E_NOT_OK;
    }

    return Status_en;
}

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
)
{
    Std_ReturnType Status_en = E_OK;

    if (ETH_IP_STATUS_SUCCESS != Eth_Ip_ReadMdioWithC22(para_CtrlIdx_u8, para_TrcvIdx_u8, para_RegIdx_u8, para_RegVal_ptr))
    {
        Status_en = E_NOT_OK;
    }

    return Status_en;
}
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
)
{
    Eth_Ip_RxCounterType RxCountValue_st;
    Eth_Ip_TxCounterType TxCountValue_st;
    /* Get the different types of receive packet counter */
    Eth_Ip_GetRxCounters(para_CtrlIdx_u8, &RxCountValue_st);
    /* Get the different types of transmit packet counter */
    Eth_Ip_GetTxCounters(para_CtrlIdx_u8, &TxCountValue_st);

    para_Counter_ptr->DropPktBufOverrun = 0xFFFFFFFFU;
    para_Counter_ptr->DropPktCrc        = RxCountValue_st.RxCrcErrorPackets_u32;
    para_Counter_ptr->UndersizePkt      = 0xFFFFFFFFU;
    para_Counter_ptr->OversizePkt       = 0xFFFFFFFFU;
    para_Counter_ptr->AlgnmtErr         = RxCountValue_st.RxAlignmentErrorPackets_u32;
    para_Counter_ptr->SqeTestErr        = 0xFFFFFFFFU;
    para_Counter_ptr->DiscInbdPkt       = 0xFFFFFFFFU;
    para_Counter_ptr->ErrInbdPkt        = 0xFFFFFFFFU;
    para_Counter_ptr->DiscOtbdPkt       = 0xFFFFFFFFU;
    para_Counter_ptr->ErrOtbdPkt        = 0xFFFFFFFFU;
    para_Counter_ptr->SnglCollPkt       = 0xFFFFFFFFU;
    para_Counter_ptr->MultCollPkt       = 0xFFFFFFFFU;
    para_Counter_ptr->DfrdPkt           = TxCountValue_st.TxDeferredErrorFrame_u32;
    para_Counter_ptr->LatCollPkt        = TxCountValue_st.TxLateCollisionFrame_u32;
    para_Counter_ptr->HwDepCtr0        = 0xFFFFFFFFU;
    para_Counter_ptr->HwDepCtr1        = 0xFFFFFFFFU;
    para_Counter_ptr->HwDepCtr2        = 0xFFFFFFFFU;
    para_Counter_ptr->HwDepCtr3        = 0xFFFFFFFFU;

    return E_OK;
}
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
)
{
    Eth_Ip_RxCounterType RxCounters_st;
    Eth_Ip_GetRxCounters(para_CtrlIdx_u8, &RxCounters_st);

    para_RxStats_ptr->RxStatsDropEvents           = 0xFFFFFFFFU;
    para_RxStats_ptr->RxStatsOctets               = 0xFFFFFFFFU;
    para_RxStats_ptr->RxStatsPkts                 = 0xFFFFFFFFU;
    para_RxStats_ptr->RxStatsBroadcastPkts        = RxCounters_st.RxBroadcastPacketsGood_u32;
    para_RxStats_ptr->RxStatsMulticastPkts        = RxCounters_st.RxMulticastPacketsGood_u32;
    para_RxStats_ptr->RxStatsCrcAlignErrors       = RxCounters_st.RxCrcErrorPackets_u32 + RxCounters_st.RxAlignmentErrorPackets_u32;
    para_RxStats_ptr->RxStatsUndersizePkts        = 0xFFFFFFFFU;
    para_RxStats_ptr->RxStatsOversizePkts         = 0xFFFFFFFFU;
    para_RxStats_ptr->RxStatsFragments            = RxCounters_st.RxRuntErrorPackets_u32;
    para_RxStats_ptr->RxStatsJabbers              = 0xFFFFFFFFU;
    para_RxStats_ptr->RxStatsCollisions           = 0xFFFFFFFFU;
    para_RxStats_ptr->RxStatsPkts64Octets         = 0xFFFFFFFFU;
    para_RxStats_ptr->RxStatsPkts65to127Octets    = 0xFFFFFFFFU;
    para_RxStats_ptr->RxStatsPkts128to255Octets   = 0xFFFFFFFFU;
    para_RxStats_ptr->RxStatsPkts256to511Octets   = 0xFFFFFFFFU;
    para_RxStats_ptr->RxStatsPkts512to1023Octets  = 0xFFFFFFFFU;
    para_RxStats_ptr->RxStatsPkts1024to1518Octets = 0xFFFFFFFFU;
    para_RxStats_ptr->RxUnicastFrames             = RxCounters_st.RxUnicastPacketsGood_u32;

    return E_OK;
}
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
)
{
    Eth_Ip_TxCounterType TxCounters_st;
    Eth_Ip_GetTxCounters(para_CtrlIdx_u8, &TxCounters_st);

    para_TxStats_ptr->TxNumberOfOctets = 0xFFFFFFFFU;
    para_TxStats_ptr->TxNUcastPkts     = TxCounters_st.TxMulticastPacketsGood_u32 + TxCounters_st.TxBroadcastPacketsGood_u32;
    para_TxStats_ptr->TxUniCastPkts    = TxCounters_st.TxUnicastPacketsGood_u32 - 
                                        (TxCounters_st.TxMulticastPacketsGood_u32 + TxCounters_st.TxBroadcastPacketsGood_u32);

    return E_OK;
}
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
)
{
    Eth_Ip_TxCounterType TxCountValue_st;
    Eth_Ip_GetTxCounters(para_CtrlIdx_u8, &TxCountValue_st);

    para_TxErrorCounterValues_ptr->TxDroppedNoErrorPkts = 0xFFFFFFFFU;
    para_TxErrorCounterValues_ptr->TxDroppedErrorPkts   = 0xFFFFFFFFU;
    para_TxErrorCounterValues_ptr->TxDeferredTrans      = TxCountValue_st.TxDeferredErrorFrame_u32;
    para_TxErrorCounterValues_ptr->TxSingleCollision    = 0xFFFFFFFFU;
    para_TxErrorCounterValues_ptr->TxMultipleCollision  = 0xFFFFFFFFU;
    para_TxErrorCounterValues_ptr->TxLateCollision      = TxCountValue_st.TxLateCollisionFrame_u32;
    para_TxErrorCounterValues_ptr->TxExcessiveCollison  = TxCountValue_st.TxExcessiveCollisionFrame_u32;

    return E_OK;
}
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
)
{
    uint32_t Second_u32             = 0x00U;
    uint32_t NanoSecond_u32         = 0x00U;
    Std_ReturnType Status_en      = E_NOT_OK;
    Eth_Ip_StatusType IpStatus_en = ETH_IP_STATUS_SUCCESS;

    IpStatus_en                   = Eth_Ip_GetSystemTimeStamp(para_CtrlIdx_u8, &Second_u32, &NanoSecond_u32);

    if (ETH_IP_STATUS_SUCCESS == IpStatus_en)
    {
        para_TimeStamp_ptr->secondsHi   = 0U;
        para_TimeStamp_ptr->seconds     = Second_u32;
        para_TimeStamp_ptr->nanoseconds = NanoSecond_u32;
        *para_TimeQual_ptr              = ETH_VALID;
        Status_en                       = E_OK;
    }

    return Status_en;
}

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
)
{
    s_EthTxBufferInfo_aa[para_CtrlIdx_u8][para_BufIdx_en].TimeStamp_bool = TRUE;
}

/**
 *  @brief          Get Egress TimeStamp.
 *  @details        Design ID:         SDD_ETH_40
 *
 *  @param[in]      para_CtrlIdx_u8       None
 *  @param[in]      para_BufIdx_en        None
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
Std_ReturnType Eth_Ipr_GetEgressTimeStamp(
    uint8_t para_CtrlIdx_u8,
    Eth_BufIdxType para_BufIdx_en,
    Eth_TimeStampQualType *para_TimeQual_ptr,
    Eth_TimeStampType *para_TimeStamp_ptr
)
{
    Std_ReturnType Status_en                   = E_NOT_OK;
    Eth_Ipr_TxBufferInfoType *TxBufferInfo_ptr = &s_EthTxBufferInfo_aa[para_CtrlIdx_u8][para_BufIdx_en];
    Eth_TimeStampType *Timestamp_ptr           = &s_EthTxBufferInfo_aa[para_CtrlIdx_u8][para_BufIdx_en].TxTimeStamp_st;

    if ((FALSE == TxBufferInfo_ptr->TimeStamp_bool) ||
        ((0U == Timestamp_ptr->nanoseconds) && (0U == Timestamp_ptr->seconds)))
    {
        *para_TimeQual_ptr = ETH_INVALID;
    }
    else
    {
        para_TimeStamp_ptr->nanoseconds = Timestamp_ptr->nanoseconds;
        para_TimeStamp_ptr->seconds     = Timestamp_ptr->seconds;
        para_TimeStamp_ptr->secondsHi   = Timestamp_ptr->secondsHi;

        *para_TimeQual_ptr              = ETH_VALID;
        Status_en                       = E_OK;
    }

    return Status_en;
}

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
)
{
    Std_ReturnType Status_en = E_NOT_OK;
    Eth_Ipr_RxBufferInfoType *RxBufferInfo_ptr;
    uint8_t *Buffer_ptr;

    RxBufferInfo_ptr = &s_EthRxBufferInfo_aa[para_CtrlIdx_u8][0U];
    Buffer_ptr       = &RxBufferInfo_ptr->Buffer_ptr[ETH_FRAME_ETHERNET_PAYLOAD_OFFSET];
    if (para_Data_ptr == Buffer_ptr)
    {
        if ((0U == RxBufferInfo_ptr->Timestamp_st.seconds) && (0U == RxBufferInfo_ptr->Timestamp_st.nanoseconds))
        {
            *para_TimeQual_ptr = ETH_INVALID;
        }
        else
        {
            para_TimeStamp_ptr->nanoseconds = RxBufferInfo_ptr->Timestamp_st.nanoseconds;
            para_TimeStamp_ptr->seconds     = RxBufferInfo_ptr->Timestamp_st.seconds;
            para_TimeStamp_ptr->secondsHi   = RxBufferInfo_ptr->Timestamp_st.secondsHi;
            *para_TimeQual_ptr              = ETH_VALID;
            Status_en                       = E_OK;
        }
    }

    return Status_en;
}
#endif /*!< (STD_ON == ETH_GLOBAL_TIME_SUPPORT_API) */

/**
 *  @brief          Provide Tx Buffer.
 *  @details        Design ID:         SDD_ETH_42
 *
 *  @param[in]      para_CtrlIdx_u8     None
 *  @param[in]      para_Priority_u8    None
 *  @param[in]      para_BufIdx_ptr     None
 *  @param[in]      para_Buf_ptr        None
 *  @param[in]      para_LenByte_ptr    None
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
)
{
#if (STD_OFF == ETH_TRANSMIT_IRQ_ENABLED)
    Eth_Ip_TxFrameStatusType TxStatus_st;
#endif /*!< (STD_OFF == ETH_TRANSMIT_IRQ_ENABLED) */
    uint8_t FifoIdx_u8                           = 0U;
    uint16_t FrameLength_u16                     = 0U;
    uint32_t IdxTemp_u32                         = 0U;
    uint16_t TxFifoLength_u16                    = 0U;
    Eth_Ipr_TxBufferListType *TxBufferList_ptr = NULL_PTR;
    Eth_Ipr_TxBufferInfoType *TxBufferInfo_ptr = NULL_PTR;
    BufReq_ReturnType Status_en                = BUFREQ_E_BUSY;

    if (para_Priority_u8 >= ETH_MAX_PRIORITY)
    {
        FifoIdx_u8 = s_EthPriorityFifoIdMap_aa[para_CtrlIdx_u8][0U];
    }
    else
    {
        FifoIdx_u8 = s_EthPriorityFifoIdMap_aa[para_CtrlIdx_u8][para_Priority_u8];
    }

    FrameLength_u16  = *para_LenByte_ptr + ETH_FRAME_ETHERNET_HEADER_OF_LENGTH + ETH_FRAME_CHECK_SEQUENCE_LENGTH;
    TxBufferList_ptr = &s_EthTxBufferList_aa[para_CtrlIdx_u8][FifoIdx_u8];
    TxFifoLength_u16 = s_EthIpState_aa[para_CtrlIdx_u8]->TxBdRing_st.TxRingLen_u16;

    SchM_Enter_Eth_ETH_EXCLUSIVE_AREA_00();

    /* Check the length of transmit buffer is lower than the frame length */
    if (FrameLength_u16 > TxBufferList_ptr->BuffLen_u16)
    {
        /* Not lock buffer but return the available length and BUFREQ_E_OVFL. */
        *para_LenByte_ptr =
                TxBufferList_ptr->BuffLen_u16 - ETH_FRAME_ETHERNET_HEADER_OF_LENGTH - ETH_FRAME_CHECK_SEQUENCE_LENGTH;
        Status_en = BUFREQ_E_OVFL;
    }
    else
    {
        IdxTemp_u32 = s_EthFifoFirstBufferIdx_aa[para_CtrlIdx_u8][FifoIdx_u8];
        if (TxBufferList_ptr->BufferNum_u16 < TxFifoLength_u16)
        {
            for (uint16_t Index_u16 = 0U; Index_u16 < TxFifoLength_u16; Index_u16++)
            {
                TxBufferInfo_ptr = &s_EthTxBufferInfo_aa[para_CtrlIdx_u8][IdxTemp_u32];
                /* Check that if buffer is locked (allocated but not transfered). */
                if (FALSE == TxBufferInfo_ptr->Lock_bool)
                {
                    Status_en = BUFREQ_OK;
                    break;
                }
                IdxTemp_u32++;
            }
        }
#if (STD_OFF == ETH_TRANSMIT_IRQ_ENABLED)
        else
        {

            /* Release head buffer if it not eanble tx confirmation */
            TxBufferInfo_ptr = TxBufferList_ptr->TxBufferHead_ptr;
            if (NULL_PTR != TxBufferInfo_ptr)
            {
                if (FALSE == TxBufferInfo_ptr->TxConfirmation_bool)
                {
                    if (ETH_IP_STATUS_SUCCESS == Eth_Ip_GetTxFrameStatus(para_CtrlIdx_u8, TxBufferInfo_ptr->Buffer_ptr, &TxStatus_st))
                    {
                        Eth_Ipr_ChReleaseTxBuffer(para_CtrlIdx_u8, FifoIdx_u8);
                        IdxTemp_u32 = TxBufferInfo_ptr->BufferIdx_en;
                        Status_en   = BUFREQ_OK;
                    }
                }
            }
        }
#endif /*!< (STD_OFF == ETH_TRANSMIT_IRQ_ENABLED) */
    }

    if ((BUFREQ_OK == Status_en) && (NULL_PTR != TxBufferInfo_ptr))
    {
        TxBufferInfo_ptr->Lock_bool    = TRUE;
        TxBufferInfo_ptr->FifoIdx_u8   = FifoIdx_u8;
        TxBufferInfo_ptr->BufferIdx_en = IdxTemp_u32;
        IdxTemp_u32                    -= s_EthFifoFirstBufferIdx_aa[para_CtrlIdx_u8][FifoIdx_u8];
        TxBufferInfo_ptr->Buffer_ptr   = &TxBufferList_ptr->Buffer_ptr[IdxTemp_u32 * TxBufferList_ptr->BuffLen_u16];
        *para_Buf_ptr                  = &TxBufferInfo_ptr->Buffer_ptr[ETH_FRAME_ETHERNET_PAYLOAD_OFFSET];
        *para_BufIdx_ptr               = TxBufferInfo_ptr->BufferIdx_en;
    }

    SchM_Exit_Eth_ETH_EXCLUSIVE_AREA_00();

    return Status_en;
}

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
)
{
    uint8_t *FrameBuffer_ptr;
    uint16_t FrameLength_u16;
    Std_ReturnType Status_en                   = E_NOT_OK;
    Eth_Ipr_TxBufferInfoType *TxBufferTemp_ptr = NULL_PTR;
    Eth_Ipr_TxBufferInfoType *TxBufferInfo_ptr = &s_EthTxBufferInfo_aa[para_CtrlIdx_u8][para_BufIdx_en];
    Eth_Ipr_TxBufferListType *TxBufferList_ptr = &s_EthTxBufferList_aa[para_CtrlIdx_u8][TxBufferInfo_ptr->FifoIdx_u8];
    uint32_t TxOption_u32                        = 0U;

    if (TRUE == TxBufferInfo_ptr->Lock_bool)
    {
        TxBufferInfo_ptr->TxConfirmation_bool = para_TxConfirmation_bool;
        FrameBuffer_ptr                       = TxBufferInfo_ptr->Buffer_ptr;
        /* Copy destination  MAC address */
        FrameBuffer_ptr[ETH_FRAME_MAC_DESTINATION_OFFSET_0] = para_PhysAddr_ptr[0];
        FrameBuffer_ptr[ETH_FRAME_MAC_DESTINATION_OFFSET_1] = para_PhysAddr_ptr[1];
        FrameBuffer_ptr[ETH_FRAME_MAC_DESTINATION_OFFSET_2] = para_PhysAddr_ptr[2];
        FrameBuffer_ptr[ETH_FRAME_MAC_DESTINATION_OFFSET_3] = para_PhysAddr_ptr[3];
        FrameBuffer_ptr[ETH_FRAME_MAC_DESTINATION_OFFSET_4] = para_PhysAddr_ptr[4];
        FrameBuffer_ptr[ETH_FRAME_MAC_DESTINATION_OFFSET_5] = para_PhysAddr_ptr[5];
        /* Call function to copy source MAC address */
        Eth_Ipr_GetPhysAddr(para_CtrlIdx_u8, &FrameBuffer_ptr[ETH_FRAME_MAC_SOURCE_OFFSET]);
        /* Replicate the frame Buffer type. */
        if ((uint16_t)para_FrameType_en > 1500U)
        {
            FrameBuffer_ptr[ETH_FRAME_ETHERNET_TYPE_OFFSET]      = (uint8_t)((para_FrameType_en & 0xFF00U) >> 8U);
            FrameBuffer_ptr[ETH_FRAME_ETHERNET_TYPE_OFFSET + 1U] = (uint8_t)(para_FrameType_en & 0xFFU);
        }
        else
        {
            FrameBuffer_ptr[ETH_FRAME_ETHERNET_TYPE_OFFSET]      = (uint8_t)((para_LenByte_u16 & 0xFF00U) >> 8U);
            FrameBuffer_ptr[ETH_FRAME_ETHERNET_TYPE_OFFSET + 1U] = (uint8_t)(para_LenByte_u16 & 0xFFU);
        }
        /* Calculate frame length */
        FrameLength_u16 = para_LenByte_u16 + ETH_FRAME_ETHERNET_HEADER_OF_LENGTH;
        /* Set Transmit options */
#if (STD_ON == ETH_TRANSMIT_IRQ_ENABLED)
        TxOption_u32 |= ETH_IP_TDES0_IOC_MASK;
#endif /*!< (STD_ON == ETH_TRANSMIT_IRQ_ENABLED) */
#if (STD_ON == ETH_GLOBAL_TIME_SUPPORT_API)
        if (TRUE == TxBufferInfo_ptr->TimeStamp_bool)
        {
            TxOption_u32 |= ETH_IP_TDES0_TTSE_MASK;
        }
#endif /*!< (STD_ON == ETH_GLOBAL_TIME_SUPPORT_API) */
#if (STD_ON == ETH_ENABLE_L4_OFFLOAD)
        TxOption_u32 |= ETH_IP_TDES0_CIC_3_MASK;
#elif (STD_ON == ETH_ENABLE_L3_OFFLOAD)
        TxOption_u32 |= ETH_IP_TDES0_CIC_1_MASK;
#endif /*!< (STD_ON == ETH_ENABLE_L4_OFFLOAD) */
        SchM_Enter_Eth_ETH_EXCLUSIVE_AREA_01();
        /* Add transmited tx buffer to the buffer list tail */
        if (NULL_PTR == TxBufferList_ptr->TxBufferHead_ptr)
        {
            TxBufferList_ptr->TxBufferHead_ptr = TxBufferInfo_ptr;
            TxBufferList_ptr->TxBufferTail_ptr = TxBufferInfo_ptr;
        }
        else
        {
            TxBufferList_ptr->TxBufferTail_ptr->NextTxBuffer_ptr = TxBufferInfo_ptr;
            TxBufferList_ptr->TxBufferTail_ptr                   = TxBufferInfo_ptr;
        }
        TxBufferInfo_ptr->NextTxBuffer_ptr = NULL_PTR;
        /* Update the transmited buffer number */
        TxBufferList_ptr->BufferNum_u16++;
        SchM_Exit_Eth_ETH_EXCLUSIVE_AREA_01();
        if (ETH_IP_STATUS_SUCCESS == Eth_Ip_SendFrame(para_CtrlIdx_u8, FrameBuffer_ptr, FrameLength_u16, TxOption_u32))
        {
            Status_en = E_OK;
        }
        else
        {
            SchM_Enter_Eth_ETH_EXCLUSIVE_AREA_01();
            /* Remove buffer form tx buffer list */
            TxBufferList_ptr->BufferNum_u16--;
            if (TxBufferInfo_ptr == TxBufferList_ptr->TxBufferHead_ptr)
            {
                TxBufferList_ptr->TxBufferHead_ptr = NULL_PTR;
                TxBufferList_ptr->TxBufferTail_ptr = NULL_PTR;
            }
            else
            {
                TxBufferTemp_ptr = TxBufferList_ptr->TxBufferHead_ptr;
                while (NULL_PTR != (TxBufferTemp_ptr->NextTxBuffer_ptr))
                {
                    if ((TxBufferTemp_ptr->NextTxBuffer_ptr) == TxBufferInfo_ptr)
                    {
                        TxBufferList_ptr->TxBufferTail_ptr = TxBufferTemp_ptr;
                        TxBufferTemp_ptr->NextTxBuffer_ptr = NULL_PTR;
                        break;
                    }
                    else
                    {
                        TxBufferTemp_ptr = TxBufferTemp_ptr->NextTxBuffer_ptr;
                    }
                }
            }
            SchM_Exit_Eth_ETH_EXCLUSIVE_AREA_01();
        }
    }

    return Status_en;
}

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
)
{
    uint32_t FrameLength_u32;
    Eth_Ip_StatusType Status_en;
#if ((STD_ON == ETH_ENABLE_L3_OFFLOAD) || (STD_ON == ETH_ENABLE_L4_OFFLOAD) || (STD_ON == ETH_GLOBAL_TIME_SUPPORT_API))
    Eth_Ip_RxBufferEnDescriptorType RxDesc_st;
#else
    Eth_Ip_RxBufferDescriptorType RxDesc_st;
#endif /*!< ((STD_ON == ETH_ENABLE_L3_OFFLOAD) || (STD_ON == ETH_ENABLE_L4_OFFLOAD) || (STD_ON == ETH_GLOBAL_TIME_SUPPORT_API)) */
    Eth_Ip_RxBufferType RxBuffer_st            = {NULL_PTR};
    Eth_Ipr_RxBufferInfoType *RxBufferInfo_ptr = &s_EthRxBufferInfo_aa[para_CtrlIdx_u8][para_FifoIdx_u8];

    Status_en = Eth_Ip_GetRxBuffer(para_CtrlIdx_u8, &RxDesc_st, &RxBuffer_st);
    if (ETH_IP_STATUS_SUCCESS == Status_en)
    {
        *para_RxStatus_ptr = ETH_RECEIVED;
#if (STD_ON == ETH_GLOBAL_TIME_SUPPORT_API)
        /* Record receive information of buffer */
#if ((STD_ON == ETH_ENABLE_L3_OFFLOAD) || (STD_ON == ETH_ENABLE_L4_OFFLOAD))
        if (0U == (RxDesc_st.RxExtStatus_u32 & ETH_IP_RDES4_TSPD_MASK))
#else
        if ((0U != (RxDesc_st.RxControl_u32 & ETH_IP_RDES0_TSPA_MASK)) && (0U == (RxDesc_st.RxExtStatus_u32 & ETH_IP_RDES4_TSPD_MASK)))
#endif
        {
            RxBufferInfo_ptr->Timestamp_st.nanoseconds = RxDesc_st.TimeStampLow_u32;
            RxBufferInfo_ptr->Timestamp_st.seconds     = RxDesc_st.TimeStampHigh_u32;
            RxBufferInfo_ptr->Timestamp_st.secondsHi   = 0U;
        }
#endif /*!< (STD_ON == ETH_GLOBAL_TIME_SUPPORT_API) */
        RxBufferInfo_ptr->Buffer_ptr = RxBuffer_st.Buffer_ptr;
        *para_Buffer_ptr             = RxBuffer_st.Buffer_ptr;
        *para_Length_ptr             = RxBuffer_st.Length_u16 - (uint16_t)ETH_FRAME_ETHERNET_HEADER_OF_LENGTH;
        /* Determine if there has more frames */
        Status_en = Eth_Ip_GetRxFrameSize(para_CtrlIdx_u8, &FrameLength_u32);
        if ((ETH_IP_STATUS_SUCCESS == Status_en) && (0U != FrameLength_u32))
        {
            /* This means there are frames stored in one desc or multiple desc, but only the frame store in one desc will be received */
            *para_RxStatus_ptr = ETH_RECEIVED_MORE_DATA_AVAILABLE;
        }
    }
    else if (ETH_IP_STATUS_ERROR == Status_en)
    {
        /* Release the rx buffer if received frame stored in more than one Rx desc or has error */
        Eth_Ipr_ReleaseRxBuffer(para_CtrlIdx_u8, para_FifoIdx_u8);
        *para_RxStatus_ptr = ETH_NOT_RECEIVED;
    }
    else
    {
        *para_RxStatus_ptr = ETH_NOT_RECEIVED;
    }
}

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
)
{
#if ((STD_ON == ETH_ENABLE_L3_OFFLOAD) || (STD_ON == ETH_ENABLE_L4_OFFLOAD) || (STD_ON == ETH_GLOBAL_TIME_SUPPORT_API))
    Eth_Ip_RxBufferEnDescriptorType RxDesc_st;
#else
    Eth_Ip_RxBufferDescriptorType RxDesc_st;
#endif /*!< ((STD_ON == ETH_ENABLE_L3_OFFLOAD) || (STD_ON == ETH_ENABLE_L4_OFFLOAD) || (STD_ON == ETH_GLOBAL_TIME_SUPPORT_API)) */
    uint16_t RxClearDesc_u16                     = 0U;
    Eth_Ip_RxBdRingType *CurRxBd_ptr           = &s_EthIpState_aa[para_CtrlIdx_u8]->RxBdRing_st;
    Eth_Ipr_RxBufferInfoType *RxBufferInfo_ptr = &s_EthRxBufferInfo_aa[para_CtrlIdx_u8][para_FifoIdx_u8];

    /* Release the Rx buffer whether or not there is a valid frame that can be received  */
    RxDesc_st.RxControl_u32     = ETH_IP_RDES0_OWN_MASK;
    RxDesc_st.RxBuff2Addr_u32   = 0U;
    RxDesc_st.RxBuffLen_u32     = CurRxBd_ptr->RxBuffLen_u16;
#if ((STD_ON == ETH_ENABLE_L3_OFFLOAD) || (STD_ON == ETH_ENABLE_L4_OFFLOAD) || (STD_ON == ETH_GLOBAL_TIME_SUPPORT_API))
    RxDesc_st.RxExtStatus_u32   = 0U;
    RxDesc_st.Reserved2_u32     = 0U;
    RxDesc_st.TimeStampLow_u32  = 0U;
    RxDesc_st.TimeStampHigh_u32 = 0U;
#endif /*!< ((STD_ON == ETH_ENABLE_L3_OFFLOAD) || (STD_ON == ETH_ENABLE_L4_OFFLOAD) || (STD_ON == ETH_GLOBAL_TIME_SUPPORT_API)) */
    if (CurRxBd_ptr->RxGenIdx_u16 > CurRxBd_ptr->RxConsumIdx_u16)
    {
        RxClearDesc_u16 = CurRxBd_ptr->RxGenIdx_u16 - CurRxBd_ptr->RxConsumIdx_u16;
    }
    else
    {
        RxClearDesc_u16 = CurRxBd_ptr->RxRingLen_u16 - CurRxBd_ptr->RxConsumIdx_u16 + CurRxBd_ptr->RxGenIdx_u16;
    }

    for (uint16_t Index_u16 = 0U; Index_u16 < RxClearDesc_u16; Index_u16++)
    {
        RxDesc_st.RxBuff1Addr_u32 = (uint32_t)&CurRxBd_ptr->RxBuffer_ptr[CurRxBd_ptr->RxConsumIdx_u16 * CurRxBd_ptr->RxBuffLen_u16];
        Eth_Ip_SetupRxDescriptor(para_CtrlIdx_u8, &RxDesc_st);
    }
    RxBufferInfo_ptr->Buffer_ptr = NULL_PTR;
}

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
)
{
    (void)para_CtrlIdx_u8;
    return 1U;
}

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
)
{
    return s_EthTxBufferList_aa[para_CtrlIdx_u8][para_FifoIdx_u8].BufferNum_u16;
}

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
)
{
    Eth_Ip_TxFrameStatusType TxStatus_st;
    Eth_Ipr_TxBufferInfoType *TxBufferInfo_ptr;

    TxBufferInfo_ptr = s_EthTxBufferList_aa[para_CtrlIdx_u8][para_FifoIdx_u8].TxBufferHead_ptr;

    if(NULL_PTR != TxBufferInfo_ptr)
    {
        if (ETH_IP_STATUS_SUCCESS == Eth_Ip_GetTxFrameStatus(para_CtrlIdx_u8, TxBufferInfo_ptr->Buffer_ptr, &TxStatus_st))
        {
#if (STD_ON == ETH_GLOBAL_TIME_SUPPORT_API)
            if (TRUE == TxBufferInfo_ptr->TimeStamp_bool)
            {
                TxBufferInfo_ptr->TxTimeStamp_st.nanoseconds = TxStatus_st.TxTimeStamp_st.Nanosecond_u32;
                TxBufferInfo_ptr->TxTimeStamp_st.seconds     = (uint32_t)TxStatus_st.TxTimeStamp_st.Second_u64;
                TxBufferInfo_ptr->TxTimeStamp_st.secondsHi   = (uint16_t)(TxStatus_st.TxTimeStamp_st.Second_u64 >> 32U);
            }
#endif /*!< (STD_ON == ETH_GLOBAL_TIME_SUPPORT_API) */
            if (0U != TxStatus_st.ErrStatus_u32)
            {
                TxBufferInfo_ptr->TxResult_en = E_NOT_OK;
            }
            else
            {
                TxBufferInfo_ptr->TxResult_en = E_OK;
            }
            *para_TxBufferInfo_ptr = TxBufferInfo_ptr;
        }
        else
        {
            *para_TxBufferInfo_ptr = NULL_PTR;
        }
    }
}

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
)
{
    Eth_Ipr_TxBufferInfoType *TxBufferInfo_ptr;

    TxBufferInfo_ptr = s_EthTxBufferList_aa[para_CtrlIdx_u8][para_FifoIdx_u8].TxBufferHead_ptr;

    if(NULL_PTR != TxBufferInfo_ptr)
    {
        SchM_Enter_Eth_ETH_EXCLUSIVE_AREA_02();
        /* Release the buffer to application use */
        TxBufferInfo_ptr->Lock_bool      = FALSE;
        TxBufferInfo_ptr->Buffer_ptr     = NULL_PTR;
#if (STD_ON == ETH_GLOBAL_TIME_SUPPORT_API)
        TxBufferInfo_ptr->TimeStamp_bool = FALSE;
#endif /*!< (STD_ON == ETH_GLOBAL_TIME_SUPPORT_API) */
        /* Remove Tx buffer from the transmited list */
        s_EthTxBufferList_aa[para_CtrlIdx_u8][para_FifoIdx_u8].TxBufferHead_ptr = TxBufferInfo_ptr->NextTxBuffer_ptr;
        s_EthTxBufferList_aa[para_CtrlIdx_u8][para_FifoIdx_u8].BufferNum_u16--;
        SchM_Exit_Eth_ETH_EXCLUSIVE_AREA_02();
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
