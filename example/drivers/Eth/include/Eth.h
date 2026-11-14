/***********************************************************************************************************************
 * Project Name: XHSC AUTOSAR MCAL
 * File Name: Eth.h
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
 *   @file       Eth.h
 *   @version    1.0.3
 *   @brief      Eth High Layer Driver
 *   @addtogroup eth_driver
 *   @{
 */

#ifndef ETH_H
#define ETH_H

#ifdef __cplusplus
extern "C" {
#endif

/***********************************************************************************************************************
 *  INCLUDES
 **********************************************************************************************************************/
#include "Eth_Types.h"
#include "Eth_Cfg.h"

/***********************************************************************************************************************
 *  DEFINES & MACROS
 **********************************************************************************************************************/
/**
 *  @brief   VersionInfo of Eth Module.
 *  @details SRS ID:     SWS_Eth_00006
 */
#define ETH_VENDOR_ID                   168UL
#define ETH_MODULE_ID                   88UL
#define ETH_AR_RELEASE_MAJOR_VERSION    4UL
#define ETH_AR_RELEASE_MINOR_VERSION    4UL
#define ETH_AR_RELEASE_REVISION_VERSION 0UL
#define ETH_SW_MAJOR_VERSION            1UL
#define ETH_SW_MINOR_VERSION            0UL
#define ETH_SW_PATCH_VERSION            3UL

#if (STD_ON == ETH_DEV_ERROR_DETECT)
/**
 * @brief   Eth Development Errors.
 * @details SRS ID:    SWS_Eth_00016
 */
/**
 *  @brief   Invalid controller index.
 */
#define ETH_E_INV_CTRL_IDX (0x01U)

/**
 *  @brief   Eth module or controller was not initialized.
 */
#define ETH_E_UNINIT (0x02U)

/**
 *  @brief   Invalid pointer in parameter list.
 */
#define ETH_E_PARAM_POINTER (0x03U)

/**
 *  @brief   Invalid parameter.
 */
#define ETH_E_INV_PARAM (0x04U)

/**
 *  @brief   Invalid mode.
 */
#define ETH_E_INV_MODE (0x05U)

/**
 *  @brief   API service ID for Eth_Init function.
 */
#define ETH_SID_INIT (0x01U)

/**
 *  @brief   API service ID for Eth_SetControllerMode function.
 */
#define ETH_SID_SETCONTROLLERMODE (0x03U)

/**
 *  @brief   API service ID for Eth_GetControllerMode function.
 */
#define ETH_SID_GETCONTROLLERMODE (0x04U)

#if (STD_ON == ETH_ENABLE_MII)
/**
 *  @brief   API service ID for Eth_WriteMii function.
 */
#define ETH_SID_WRITEMII (0x05U)

/**
 *  @brief   API service ID for Eth_ReadMii function.
 */
#define ETH_SID_READMII (0x06U)
#endif /*!< (STD_ON == ETH_ENABLE_MII) */

/**
 *  @brief   API service ID for Eth_GetPhysAddr function.
 */
#define ETH_SID_GETPHYSADDR (0x08U)

/**
 *  @brief   API service ID for Eth_ProvideTxBuffer function.
 */
#define ETH_SID_PROVIDETXBUFFER (0x09U)

/**
 *  @brief   API service ID for Eth_Transmit function.
 */
#define ETH_SID_TRANSMIT (0x0AU)

/**
 *  @brief   API service ID for Eth_Receive function.
 */
#define ETH_SID_RECEIVE (0x0BU)

/**
 *  @brief   API service ID for Eth_TxConfirmation function.
 */
#define ETH_SID_TXCONFIRMATION (0x0CU)

/**
 *  @brief   API service ID for Eth_GetVersionInfo function.
 */
#define ETH_SID_GETVERSIONINFO (0x0DU)

/**
 *  @brief   API service ID for Eth_UpdatePhysAddrFilter function.
 */
#define ETH_SID_UPDATEPHYSADDRFILTER (0x12U)

/**
 *  @brief   API service ID for Eth_SetPhysAddr function.
 */
#define ETH_SID_SETPHYSADDR (0x13U)

#if (STD_ON == ETH_GET_COUNTER_VALUES_API)
/**
 *  @brief   API service ID for Eth_GetCounterValues function.
 */
#define ETH_SID_GETCOUNTERVALUES (0x14U)
#endif /*!< (STD_ON == ETH_GET_COUNTER_VALUES_API) */

#if (STD_ON == ETH_GET_RECEIVED_STATS_API)
/**
 *  @brief   API service ID for Eth_GetRxStats function.
 */
#define ETH_SID_GETRXSTATS (0x15U)
#endif /*!< (STD_ON == ETH_GET_RECEIVED_STATS_API) */

#if (STD_ON == ETH_GLOBAL_TIME_SUPPORT_API)
/**
 *  @brief   API service ID for Eth_GetCurrentTime function.
 */
#define ETH_SID_GETCURRENTTIME (0x16U)

/**
 *  @brief   API service ID for Eth_EnableEgressTimeStamp function.
 */
#define ETH_SID_ENABLEEGRESSTIMESTAMP (0x17U)

/**
 *  @brief   API service ID for Eth_GetEgressTimeStamp function.
 */
#define ETH_SID_GETEGRESSTIMESTAMP (0x18U)

/**
 *  @brief   API service ID for Eth_GetIngressTimeStamp function.
 */
#define ETH_SID_GETINGRESSTIMESTAMP (0x19U)
#endif /*!< (STD_ON == ETH_GLOBAL_TIME_SUPPORT_API) */

#if (STD_ON == ETH_GET_TRANSMIT_STATS_API)
/**
 *  @brief   API service ID for Eth_GetTxStats function.
 */
#define ETH_SID_GETTXSTATS (0x1CU)
#endif /*!< (STD_ON == ETH_GET_TRANSMIT_STATS_API) */

#if (STD_ON == ETH_GET_TX_ERROR_COUNTER_VALUES_API)
/**
 *  @brief   API service ID for Eth_GetTxErrorCounterValues function.
 */
#define ETH_SID_GETTXERRORCOUNTERVALUES (0x1DU)
#endif /*!< (STD_ON == ETH_GET_TX_ERROR_COUNTER_VALUES_API) */
#endif /*!< (STD_ON == ETH_DEV_ERROR_DETECT) */

/***********************************************************************************************************************
 *  TYPEDEFS
 **********************************************************************************************************************/

/***********************************************************************************************************************
 *  CONSTANTS
 **********************************************************************************************************************/

/***********************************************************************************************************************
 *  GLOBAL VARIABLES
 **********************************************************************************************************************/
#define ETH_START_SEC_CONFIG_DATA_UNSPECIFIED
#include "Eth_MemMap.h"

#if (STD_ON == ETH_PRECOMPILE_CFG_ENABLED)
#if (STD_ON == ETH_MULTICORE_ENABLED)
extern const Eth_ConfigType *const Eth_g_Config_aa[ETH_MAX_PARTITIONS];
#else
extern const Eth_ConfigType Eth_Config;
#endif /*!< (STD_ON == ETH_MULTICORE_ENABLED) */
#else
ETH_USE_EXT_CFG
#endif /*!< (STD_ON == ETH_PRECOMPILE_CFG_ENABLED) */

#define ETH_STOP_SEC_CONFIG_DATA_UNSPECIFIED
#include "Eth_MemMap.h"

/***********************************************************************************************************************
 *  GLOBAL FUNCTION
 **********************************************************************************************************************/
#define ETH_START_SEC_CODE
#include "Eth_MemMap.h"

/**
 *  @brief          Initializes the Ethernet Driver.
 *
 *  @details        Service ID:        0x01
 *                  SRS ID:            SWS_Eth_00005,SWS_Eth_00008,SWS_Eth_00014,
 *                                     SWS_Eth_00027,SWS_Eth_00028,SWS_Eth_00029,
 *                                     SWS_Eth_00034,SWS_Eth_00039,SWS_Eth_00217,
 *                                     SWS_Eth_00173
 *                  Design ID:         SDD_ETH_1
 *                  Sync/Async:        Synchronous
 *                  Reentrancy:        Non Reentrant
 *
 *  @param[in]      CfgPtr    Points to the implementation specific structure.
 *
 *  @return         void
 *
 *  @note           This API has to be called during initialization.
 *
 *  @pre            None
 *
 */
void Eth_Init(
    const Eth_ConfigType *CfgPtr
);

/**
 *  @brief          Enables  disables the indexed controller.
 *
 *  @details        Service ID:        0x03
 *                  SRS ID:            SWS_Eth_00041,SWS_Eth_00042,SWS_Eth_00043
 *                                     SWS_Eth_00044,SWS_Eth_00045,SWS_Eth_00168
 *                                     SWS_Eth_00003,SWS_Eth_00008,SWS_Eth_00173
 *                  Design ID:         SDD_ETH_2
 *                  Sync/Async:        Asynchronous
 *                  Reentrancy:        Non Reentrant
 *
 *  @param[in]      CtrlIdx            Index of the controller within the context of the Ethernet Driver
 *  @param[in]      CtrlMode           Specified the controller mode which want to entered
 *  @arg            ETH_MODE_DOWN      Disable the controller
 *  @arg            ETH_MODE_ACTIVE    Enable the controller
 *
 *  @return         Std_ReturnType    Returns the run result
 *  @retval         E_OK              Success
 *  @retval         E_NOT_OK          Controller mode could not be changed
 *
 *  @note           This function requires previous controller initialization (Eth_Init).
 *
 *  @pre            None
 *
 */
Std_ReturnType Eth_SetControllerMode(
    uint8_t CtrlIdx,
    Eth_ModeType CtrlMode
);

/**
 *  @brief          Obtains the state of the indexed controller.
 *
 *  @details        Service ID:        0x04
 *                  SRS ID:            SWS_Eth_00046,SWS_Eth_00047,SWS_Eth_00048
 *                                     SWS_Eth_00049,SWS_Eth_00050,SWS_Eth_00051
 *                                     SWS_Eth_00003,SWS_Eth_00008
 *                  Design ID:         SDD_ETH_3
 *                  Sync/Async:        Synchronous
 *                  Reentrancy:        Non Reentrant
 *
 *  @param[in]      CtrlIdx        Index of the controller within the context of the Ethernet Driver
 *  @param[out]     CtrlModePtr    Pointer to the buffer which store the current controller mode.
 *
 *  @return         Std_ReturnType    Returns the run result
 *  @retval         E_OK              Success
 *  @retval         E_NOT_OK          Controller mode could not be obtained
 *
 *  @note           This function requires previous controller initialization (Eth_Init).
 *
 *  @pre            None
 *
 */
Std_ReturnType Eth_GetControllerMode(
    uint8_t CtrlIdx,
    Eth_ModeType *CtrlModePtr
);

/**
 *  @brief          Obtains the physical source address used by the indexed controller.
 *
 *  @details        Service ID:        0x08
 *                  SRS ID:            SWS_Eth_00052,SWS_Eth_00053,SWS_Eth_00054
 *                                     SWS_Eth_00055,SWS_Eth_00056,SWS_Eth_00057
 *                                     SWS_Eth_00003,SWS_Eth_00008
 *                  Design ID:         SDD_ETH_4
 *                  Sync/Async:        Synchronous
 *                  Reentrancy:        Non Reentrant
 *
 *  @param[in]      CtrlIdx        Index of the controller within the context of the Ethernet Driver
 *  @param[out]     PhysAddrPtr    Pointer to the buffer which store the Physical source address (MAC address) in
 *                                 network byte order.
 *
 *  @return         void
 *
 *  @note           This function requires previous controller initialization (Eth_Init).
 *
 *  @pre            None
 *
 */
void Eth_GetPhysAddr(
    uint8_t CtrlIdx,
    uint8_t *PhysAddrPtr
);

/**
 *  @brief          Sets the physical source address used by the indexed controller.
 *
 *  @details        Service ID:        0x13
 *                  SRS ID:            SWS_Eth_00139,SWS_Eth_00140,SWS_Eth_00141
 *                                     SWS_Eth_00142,SWS_Eth_00143,SWS_Eth_00151
 *                                     SWS_Eth_00003,SWS_Eth_00008
 *                  Design ID:         SDD_ETH_5
 *                  Sync/Async:        Synchronous
 *                  Reentrancy:        Non Reentrant for the same CtrlIdx, reentrant for different
 *
 *  @param[in]      CtrlIdx        Index of the controller within the context of the Ethernet Driver
 *  @param[in]      PhysAddrPtr    Pointer to memory containing the physical source address (MAC address) in
 *                                 network byte order.
 *
 *  @return         void
 *
 *  @note           This function requires previous controller initialization (Eth_Init).
 *
 *  @pre            None
 *
 */
void Eth_SetPhysAddr(
    uint8_t CtrlIdx,
    const uint8_t *PhysAddrPtr
);

/**
 *  @brief          Update the physical source address tofrom the indexed controller filter. If the Ethernet Controller
 *                  is not capable to do the filtering, the software has to do this.
 *
 *  @details        Service ID:        0x12
 *                  SRS ID:            SWS_Eth_00150,SWS_Eth_00152,SWS_Eth_00164
 *                                     SWS_Eth_00165,SWS_Eth_00245,SWS_Eth_00246
 *                                     SWS_Eth_00003,SWS_Eth_00008,SWS_Eth_00166
 *                                     SWS_Eth_00167,SWS_Eth_00144,SWS_Eth_00147
 *                  Design ID:         SDD_ETH_6
 *                  Sync/Async:        Synchronous
 *                  Reentrancy:        Non Reentrant for the same CtrlIdx, reentrant for different
 *
 *  @param[in]      CtrlIdx        Index of the controller within the context of the Ethernet Driver
 *  @param[in]      PhysAddrPtr    Pointer to memory containing the physical destination address (MAC address) in
 *                                 network byte order. This is the multicast destination address of the layer 2
 *                                 Ethernet packet.
 *  @param[in]      Action         Add or remove the address from the Ethernet controllers filter.
 *
 *  @return         Std_ReturnType    Returns the run result
 *  @retval         E_OK              Service accepted
 *  @retval         E_NOT_OK          Service denied
 *
 *  @note           This function requires previous controller initialization (Eth_Init).
 *
 *  @pre            None
 *
 */
Std_ReturnType Eth_UpdatePhysAddrFilter(
    uint8_t CtrlIdx,
    const uint8_t *PhysAddrPtr,
    Eth_FilterActionType Action
);

#if (STD_ON == ETH_ENABLE_MII)
/**
 *  @brief          Configures a transceiver register or triggers a function offered by the receiver.
 *
 *  @details        Service ID:        0x05
 *                  SRS ID:            SWS_Eth_00058,SWS_Eth_00059,SWS_Eth_00060
 *                                     SWS_Eth_00061,SWS_Eth_00062,SWS_Eth_00241
 *                                     SWS_Eth_00003,SWS_Eth_00008,SWS_Eth_00063
 *                                     SWS_Eth_00069
 *                  Design ID:         SDD_ETH_7
 *                  Sync/Async:        Synchronous
 *                  Reentrancy:        Non Reentrant
 *
 *  @param[in]      CtrlIdx    Index of the controller within the context of the Ethernet Driver
 *  @param[in]      TrcvIdx    Index of the transceiver on the MII
 *  @param[in]      RegIdx     Index of the transceiver register on the MII
 *  @param[in]      RegVal     Value to be written into the indexed register
 *
 *  @return         Std_ReturnType    Returns the run result
 *  @retval         E_OK              Service accepted
 *  @retval         E_NOT_OK          Service denied
 *
 *  @note           This function requires previous controller initialization (Eth_Init).
 *
 *  @pre            None
 *
 */
Std_ReturnType Eth_WriteMii(
    uint8_t CtrlIdx,
    uint8_t TrcvIdx,
    uint8_t RegIdx,
    uint16_t RegVal
);

/**
 *  @brief          Reads a transceiver register.
 *
 *  @details        Service ID:        0x06
 *                  SRS ID:            SWS_Eth_00064,SWS_Eth_00065,SWS_Eth_00066
 *                                     SWS_Eth_00067,SWS_Eth_00068,SWS_Eth_00242
 *                                     SWS_Eth_00003,SWS_Eth_00008,SWS_Eth_00070
 *                  Design ID:         SDD_ETH_8
 *                  Sync/Async:        Synchronous
 *                  Reentrancy:        Non Reentrant
 *
 *  @param[in]      CtrlIdx      Index of the controller within the context of the Ethernet Driver
 *  @param[in]      TrcvIdx      Index of the transceiver on the MII
 *  @param[in]      RegIdx       Index of the transceiver register on the MII
 *  @param[out]     RegValPtr    Pointer to the buffer which filled with the register content of the indexed register
 *
 *  @return         Std_ReturnType    Returns the run result
 *  @retval         E_OK              Service accepted
 *  @retval         E_NOT_OK          Service denied
 *
 *  @note           This function requires previous controller initialization (Eth_Init).
 *
 *  @pre            None
 *
 */
Std_ReturnType Eth_ReadMii(
    uint8_t CtrlIdx,
    uint8_t TrcvIdx,
    uint8_t RegIdx,
    uint16_t *RegValPtr
);
#endif /*!< (STD_ON == ETH_ENABLE_MII) */

#if (STD_ON == ETH_GET_COUNTER_VALUES_API)
/**
 *  @brief          Reads a list with drop counter values of the corresponding controller. The meaning
 *                  of these values is described at Eth_CounterType.
 *
 *  @details        Service ID:        0x14
 *                  SRS ID:            SWS_Eth_00226,SWS_Eth_00228,SWS_Eth_00229
 *                                     SWS_Eth_00230,SWS_Eth_00231,SWS_Eth_00003
 *                                     SWS_Eth_00008,SWS_Eth_00227,SWS_Eth_00232
 *                  Design ID:         SDD_ETH_9
 *                  Sync/Async:        Synchronous
 *                  Reentrancy:        Non Reentrant
 *
 *  @param[in]      CtrlIdx       Index of the controller within the context of the Ethernet Driver
 *  @param[out]     CounterPtr    Pointer to the buffer which filled with counter values according to IETF RFC
 *                                1757, RFC 1643 and RFC 2233.
 *
 *  @return         Std_ReturnType    Returns the run result
 *  @retval         E_OK              Success
 *  @retval         E_NOT_OK          Counter values read failure
 *
 *  @note           This function requires previous controller initialization (Eth_Init).
 *
 *  @pre            None
 *
 */
Std_ReturnType Eth_GetCounterValues(
    uint8_t CtrlIdx,
    Eth_CounterType *CounterPtr
);
#endif /*!< (STD_ON == ETH_GET_COUNTER_VALUES_API) */

#if (STD_ON == ETH_GET_RECEIVED_STATS_API)
/**
 *  @brief          Returns the following list according to IETF RFC2819, where the maximal possible value shall denote
 *                  an invalid value, e.g. if this counter is not available:
 *                  1. etherStatsDropEvents
 *                  2. etherStatsOctets
 *                  3. etherStatsPkts
 *                  4. etherStatsBroadcastPkts
 *                  5. etherStatsMulticastPkts
 *                  6. etherStatsCrcAlignErrors
 *                  7. etherStatsUndersizePkts
 *                  8. etherStatsOversizePkts
 *                  9. etherStatsFragments
 *                  10. etherStatsJabbers
 *                  11. etherStatsCollisions
 *                  12. etherStatsPkts64Octets
 *                  13. etherStatsPkts65to127Octets
 *                  14. etherStatsPkts128to255Octets
 *                  15. etherStatsPkts256to511Octets
 *                  16. etherStatsPkts512to1023Octets
 *                  17. etherStatsPkts1024to1518Octets
 *
 *  @details        Service ID:        0x15
 *                  SRS ID:            SWS_Eth_00233,SWS_Eth_00235,SWS_Eth_00236
 *                                     SWS_Eth_00237,SWS_Eth_00238,SWS_Eth_00003
 *                                     SWS_Eth_00008,SWS_Eth_00234
 *                  Design ID:         SDD_ETH_10
 *                  Sync/Async:        Synchronous
 *                  Reentrancy:        Non Reentrant
 *
 *  @param[in]      CtrlIdx     Index of the controller within the context of the Ethernet Driver
 *  @param[out]     RxStats     Pointer to the buffer which filled with list of values according to IETF RFC 2819
 *                              (Remote Network Monitoring Management Information Base)
 *
 *  @return         Std_ReturnType    Returns the run result
 *  @retval         E_OK              Success
 *  @retval         E_NOT_OK          Drop counter could not be obtained
 *
 *  @note           This function requires previous controller initialization (Eth_Init).
 *
 *  @pre            None
 *
 */
Std_ReturnType Eth_GetRxStats(
    uint8_t CtrlIdx,
    Eth_RxStatsType *RxStats
);
#endif /*!< (STD_ON == ETH_GET_RECEIVED_STATS_API) */

#if (STD_ON == ETH_GET_TRANSMIT_STATS_API)
/**
 *  @brief          Returns the list of Transmission Statistics out of IETF RFC1213 defined with Eth_TxStatsType, where
 *                  the maximal possible value shall denote an invalid value, e.g. this counter is not available.
 *
 *  @details        Service ID:        0x1c
 *                  SRS ID:            SWS_Eth_00248,SWS_Eth_00249,SWS_Eth_00250
 *                                     SWS_Eth_00251,SWS_Eth_91005,SWS_Eth_00003
 *                                     SWS_Eth_00008
 *                  Design ID:         SDD_ETH_11
 *                  Sync/Async:        Synchronous
 *                  Reentrancy:        Non Reentrant
 *
 *  @param[in]      CtrlIdx     Index of the controller within the context of the Ethernet Driver
 *  @param[out]     TxStats     Pointer to the buffer which filled with list of values to read statistic values for
 *                              transmission.
 *
 *  @return         Std_ReturnType    Returns the run result
 *  @retval         E_OK              Success
 *  @retval         E_NOT_OK          Tx-statistics could not be obtained
 *
 *  @note           This function requires previous controller initialization (Eth_Init).
 *
 *  @pre            None
 *
 */
Std_ReturnType Eth_GetTxStats(
    uint8_t CtrlIdx,
    Eth_TxStatsType *TxStats
);
#endif /*!< (STD_ON == ETH_GET_TRANSMIT_STATS_API) */

#if (STD_ON == ETH_GET_TX_ERROR_COUNTER_VALUES_API)
/**
 *  @brief          Returns the list of Transmission Error Counters out of IETF RFC1213 and RFC1643 defined with
 *                  Eth_TxErrorCounterValuesType, where the maximal possible value shall denote an invalid value, e.g.
 *                  this counter is not available.
 *
 *  @details        Service ID:        0x1d
 *                  SRS ID:            SWS_Eth_00252,SWS_Eth_00253,SWS_Eth_00254
 *                                     SWS_Eth_00255,SWS_Eth_91006,SWS_Eth_00003
 *                                     SWS_Eth_00008
 *                  Design ID:         SDD_ETH_12
 *                  Sync/Async:        Synchronous
 *                  Reentrancy:        Non Reentrant
 *
 *  @param[in]      CtrlIdx                 Index of the controller within the context of the Ethernet Driver
 *  @param[out]     TxErrorCounterValues    Pointer to the buffer which filled with list of values to read error
 *                                          statistic values for transmission.
 *
 *  @return         Std_ReturnType    Returns the run result
 *  @retval         E_OK              Success
 *  @retval         E_NOT_OK          Tx-statistics could not be obtained
 *
 *  @note           This function requires previous controller initialization (Eth_Init).
 *
 *  @pre            None
 *
 */
Std_ReturnType Eth_GetTxErrorCounterValues(
    uint8_t CtrlIdx,
    Eth_TxErrorCounterValuesType *TxErrorCounterValues
);
#endif /*!< (STD_ON == ETH_GET_TX_ERROR_COUNTER_VALUES_API) */

#if (STD_ON == ETH_GLOBAL_TIME_SUPPORT_API)
/**
 *  @brief          Returns a time value out of the HW registers according to the capability of the HW. Is the HW
 *                  resolution is lower than the Eth_TimeStampType resolution resp. range, than an the remaining bits
 *                  will be filled with 0.
 *
 *  @details        Service ID:        0x16
 *                  SRS ID:            SWS_Eth_00181,SWS_Eth_00182,SWS_Eth_00183
 *                                     SWS_Eth_00184,SWS_Eth_00185,SWS_Eth_00210
 *                                     SWS_Eth_00003,SWS_Eth_00008,SWS_Eth_00176
 *                  Design ID:         SDD_ETH_13
 *                  Sync/Async:        Synchronous
 *                  Reentrancy:        Non Reentrant
 *
 *  @param[in]      CtrlIdx         Index of the controller within the context of the Ethernet Driver
 *  @param[out]     timeQualPtr     Quality of HW time stamp, e.g. based on current drift
 *  @param[out]     timeStampPtr    Current time stamp
 *
 *  @return         Std_ReturnType    Returns the run result
 *  @retval         E_OK              Successful
 *  @retval         E_NOT_OK          Failed
 *
 *  @note           This function requires previous controller initialization (Eth_Init).
 *                  This function may be called within an exclusive area.
 *
 *  @pre            None
 *
 */
Std_ReturnType Eth_GetCurrentTime(
    uint8_t CtrlIdx,
    Eth_TimeStampQualType *timeQualPtr,
    Eth_TimeStampType *timeStampPtr
);

/**
 *  @brief          Activates egress time stamping on a dedicated message object.
 *                  Some HW does store once the egress time stamp marker and some HW needs it always before
 *                  transmission. There will be no "disable" functionality, due to the fact, that the message type is
 *                  always "time stamped" by network design.
 *
 *  @details        Service ID:        0x17
 *                  SRS ID:            SWS_Eth_00186,SWS_Eth_00187,SWS_Eth_00188
 *                                     SWS_Eth_00189,SWS_Eth_00211,SWS_Eth_00003
 *                                     SWS_Eth_00004,SWS_Eth_00008,SWS_Eth_00176
 *                  Design ID:         SDD_ETH_14
 *                  Sync/Async:        Synchronous
 *                  Reentrancy:        Non Reentrant
 *
 *  @param[in]      CtrlIdx    Index of the controller within the context of the Ethernet Driver
 *  @param[in]      BufIdx     Index of the message buffer, where Application expects egress time stamping
 *
 *  @return         void
 *
 *  @note           This function requires previous controller initialization (Eth_Init).
 *
 *  @pre            None
 *
 */
void Eth_EnableEgressTimeStamp(
    uint8_t CtrlIdx,
    Eth_BufIdxType BufIdx
);

/**
 *  @brief          Reads back the egress time stamp on a dedicated message object.
 *
 *  @details        Service ID:        0x18
 *                  SRS ID:            SWS_Eth_00190,SWS_Eth_00191,SWS_Eth_00192
 *                                     SWS_Eth_00193,SWS_Eth_00194,SWS_Eth_00212
 *                                     SWS_Eth_00003,SWS_Eth_00004,SWS_Eth_00008
 *                                     SWS_Eth_00176
 *                  Design ID:         SDD_ETH_15
 *                  Sync/Async:        Synchronous
 *                  Reentrancy:        Non Reentrant
 *
 *  @param[in]      CtrlIdx         Index of the controller within the context of the Ethernet Driver
 *  @param[in]      BufIdx          Index of the message buffer, where Application expects egress time stamping
 *  @param[out]     timeQualPtr     Pointer to the buffer which store quality of HW time stamp, e.g. based on
 *                                  current drift
 *  @param[out]     timeStampPtr    Pointer to the buffer which store current time stamp
 *
 *  @return         Std_ReturnType    Returns the run result
 *  @retval         E_OK              Success
 *  @retval         E_NOT_OK          Failed to read time stamp.
 *
 *  @note           This function requires previous controller initialization (Eth_Init).
 *                  This function must be called within the TxConfirmation() function.
 *
 *  @pre            None
 *
 */
Std_ReturnType Eth_GetEgressTimeStamp(
    uint8_t CtrlIdx,
    Eth_BufIdxType BufIdx,
    Eth_TimeStampQualType *timeQualPtr,
    Eth_TimeStampType *timeStampPtr
);

/**
 *  @brief          Reads back the ingress time stamp on a dedicated message object.
 *
 *  @details        Service ID:        0x19
 *                  SRS ID:            SWS_Eth_00195,SWS_Eth_00196,SWS_Eth_00197
 *                                     SWS_Eth_00198,SWS_Eth_00199,SWS_Eth_00213
 *                                     SWS_Eth_00003,SWS_Eth_00008,SWS_Eth_00176
 *                  Design ID:         SDD_ETH_16
 *                  Sync/Async:        Synchronous
 *                  Reentrancy:        Non Reentrant
 *
 *  @param[in]      CtrlIdx         Index of the controller within the context of the Ethernet Driver
 *  @param[in]      DataPtr         Pointer to the message buffer, where Application expects ingress time stamping
 *  @param[out]     timeQualPtr     Pointer to the buffer which store quality of HW time stamp, e.g. based on
 *                                  current drift
 *  @param[out]     timeStampPtr    Pointer to the buffer which store current time stamp
 *
 *  @return         Std_ReturnType    Returns the run result
 *  @retval         E_OK              Success
 *  @retval         E_NOT_OK          Failed to read time stamp.
 *
 *  @note           This function requires previous controller initialization (Eth_Init).
 *                  This function must be called within the RxIndication() function.
 *
 *  @pre            None
 *
 */
Std_ReturnType Eth_GetIngressTimeStamp(
    uint8_t CtrlIdx,
    const Eth_DataType *DataPtr,
    Eth_TimeStampQualType *timeQualPtr,
    Eth_TimeStampType *timeStampPtr
);
#endif /*!< (STD_ON == ETH_GLOBAL_TIME_SUPPORT_API) */

/**
 *  @brief          Provides access to a transmit buffer of the FIFO related to the specified priority.
 *
 *  @details        Service ID:        0x09
 *                  SRS ID:            SWS_Eth_00077,SWS_Eth_00078,SWS_Eth_00079
 *                                     SWS_Eth_00080,SWS_Eth_00081,SWS_Eth_00137
 *                                     SWS_Eth_00003,SWS_Eth_00004,SWS_Eth_00008
 *                                     SWS_Eth_00082,SWS_Eth_00083,SWS_Eth_00084
 *                                     SWS_Eth_00085,SWS_Eth_00086
 *                  Design ID:         SDD_ETH_17
 *                  Sync/Async:        Synchronous
 *                  Reentrancy:        Reentrant
 *
 *  @param[in]      CtrlIdx       Index of the controller within the context of the Ethernet Driver
 *  @param[in]      Priority      Frame priority for transmit buffer FIFO selection
 *  @param[out]     BufIdxPtr     Pointer to the buffer which store Index to the granted buffer resource. To be
 *                                used for subsequent requests
 *  @param[out]     BufPtr        Pointer to the granted buffer
 *  @param[inout]   LenBytePtr    In: desired length in bytes, out: granted length in bytes
 *
 *  @return         BufReq_ReturnType    Returns the run result
 *  @retval         BUFREQ_OK            Success
 *  @retval         BUFREQ_E_NOT_OK      Development error detected
 *  @retval         BUFREQ_E_BUSY        All buffers in use
 *  @retval         BUFREQ_E_OVFL        Requested buffer too large
 *
 *  @note           This function requires previous controller initialization (Eth_Init).
 *
 *  @pre            None
 *
 */
BufReq_ReturnType Eth_ProvideTxBuffer(
    uint8_t CtrlIdx,
    uint8_t Priority,
    Eth_BufIdxType *BufIdxPtr,
    uint8_t **BufPtr,
    uint16_t *LenBytePtr
);

/**
 *  @brief          Triggers transmission of a previously filled transmit buffer.
 *
 *  @details        Service ID:        0xA
 *                  SRS ID:            SWS_Eth_00087,SWS_Eth_00088,SWS_Eth_00090
 *                                     SWS_Eth_00091,SWS_Eth_00092,SWS_Eth_00138
 *                                     SWS_Eth_00003,SWS_Eth_00004,SWS_Eth_00008
 *                                     SWS_Eth_00216,SWS_Eth_00093,SWS_Eth_00129
 *                                     SWS_Eth_00094,SWS_Eth_00243
 *                  Design ID:         SDD_ETH_18
 *                  Sync/Async:        Synchronous
 *                  Reentrancy:        Reentrant for different buffer indexes and Ctrl indexes
 *
 *  @param[in]      CtrlIdx           Index of the controller within the context of the Ethernet Driver
 *  @param[in]      BufIdx            Index of the buffer resource
 *  @param[in]      FrameType         Ethernet frame type
 *  @param[in]      TxConfirmation    Activates transmission confirmation
 *  @param[in]      LenByte           Data length in byte
 *  @param[in]      PhysAddrPtr       Pointer to the buffer which store physical target address (MAC address) in
 *                                    network byte order
 *
 *  @return         Std_ReturnType    Returns the run result
 *  @retval         E_OK              Success
 *  @retval         E_NOT_OK          Transmission failed
 *
 *  @note           This function requires previous controller initialization (Eth_Init).
 *
 *  @pre            None
 *
 */
Std_ReturnType Eth_Transmit(
    uint8_t CtrlIdx,
    Eth_BufIdxType BufIdx,
    Eth_FrameType FrameType,
    boolean TxConfirmation,
    uint16_t LenByte,
    const uint8_t *PhysAddrPtr
);

/**
 *  @brief          Receive a frame from the related fifo.
 *
 *  @details        Service ID:        0xB
 *                  SRS ID:            SWS_Eth_00095,SWS_Eth_00096,SWS_Eth_00097
 *                                     SWS_Eth_00098,SWS_Eth_00132,SWS_Eth_00153
 *                                     SWS_Eth_00003,SWS_Eth_00008,SWS_Eth_00256
 *                                     SWS_Eth_00244,SWS_Eth_00099
 *                  Design ID:         SDD_ETH_19
 *                  Sync/Async:        Synchronous
 *                  Reentrancy:        Reentrant for different FIFOs. Non Reentrant for the same FIFO.
 *
 *  @param[in]      CtrlIdx        Index of the controller within the context of the Ethernet Driver
 *  @param[in]      FifoIdx        Specifies the related fifo
 *  @param[out]     RxStatusPtr    Pointer to the buffer which value indicates whether a frame has been
 *                                 received and if so, whether more frames are available for the related fifo.
 *
 *  @return         void
 *
 *  @note           This function requires previous controller initialization (Eth_Init).
 *
 *  @pre            None
 *
 */
void Eth_Receive(
    uint8_t CtrlIdx,
    uint8_t FifoIdx,
    Eth_RxStatusType *RxStatusPtr
);

/**
 *  @brief          Triggers frame transmission confirmation.
 *
 *  @details        Service ID:        0xC
 *                  SRS ID:            SWS_Eth_00100,SWS_Eth_00101,SWS_Eth_00102
 *                                     SWS_Eth_00103,SWS_Eth_00104,SWS_Eth_00134
 *                                     SWS_Eth_00003,SWS_Eth_00008,SWS_Eth_00105
 *                                     SWS_Eth_00243
 *                  Design ID:         SDD_ETH_20
 *                  Sync/Async:        Synchronous
 *                  Reentrancy:        Non Reentrant
 *
 *  @param[in]      CtrlIdx    Index of the controller within the context of the Ethernet Driver
 *
 *  @return         void
 *
 *  @note           This function requires previous controller initialization (Eth_Init).
 *
 *  @pre            None
 *
 */
void Eth_TxConfirmation(
    uint8_t CtrlIdx
);

#if (STD_ON == ETH_GET_VERSION_INFO_API)
/**
 *  @brief          Returns the version information of this module.
 *
 *  @details        Service ID:        0xD
 *                  SRS ID:            SWS_Eth_00106,SWS_Eth_00136,SWS_Eth_00003
 *                                     SWS_Eth_00003,SWS_Eth_00008
 *                  Design ID:         SDD_ETH_21
 *                  Sync/Async:        Synchronous
 *                  Reentrancy:        Reentrant
 *
 *  @param[out]     VersionInfoPtr    Pointer to the buffer which store version information of this module
 *
 *  @return         void
 *
 *  @note           None
 *
 *  @pre            None
 *
 */
void Eth_GetVersionInfo(
    Std_VersionInfoType *VersionInfoPtr
);
#endif /*!< (STD_ON == ETH_GET_VERSION_INFO_API) */

#define ETH_STOP_SEC_CODE
#include "Eth_MemMap.h"

#ifdef __cplusplus
}
#endif
/*! @}*/

#endif /*!< ETH_H */
