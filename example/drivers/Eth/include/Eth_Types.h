/***********************************************************************************************************************
 * Project Name: XHSC AUTOSAR MCAL
 * File Name: Eth_Types.h
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
 *   @file       Eth_Types.h
 *   @version    1.0.3
 *   @brief      Eth High Layer Driver
 *   @addtogroup eth_driver
 *   @{
 */

#ifndef ETH_TYPES_H
#define ETH_TYPES_H

#ifdef __cplusplus
extern "C" {
#endif

/***********************************************************************************************************************
 *  INCLUDES
 **********************************************************************************************************************/
#include "Eth_GeneralTypes.h"
#include "Eth_Ipr_Types.h"
#include "Eth_Cfg.h"

/***********************************************************************************************************************
 *  DEFINES & MACROS
 **********************************************************************************************************************/

/***********************************************************************************************************************
 *  TYPEDEFS
 **********************************************************************************************************************/
#if (STD_ON == ETH_DEM_EVENT_ENABLE)
/**
 *  @brief   This type is used to store ETH DEM errors configuration.
 *  @details SRS ID:     SWS_Eth_00173, SWS_Eth_00219, SWS_Eth_00222
 *           Design ID:
 */
typedef enum
{
    ETH_E_ACCESS             = 0U, /*!< Contains enablement of specific error report and value assigned by Dem as ETH_E_ACCESS */
    ETH_E_RX_FRAMES_LOST     = 1U, /*!< Contains enablement of specific error report and value assigned by Dem as ETH_E_RX_FRAMES_LOST */
    ETH_E_CRC                = 2U, /*!< Contains enablement of specific error report and value assigned by Dem as ETH_E_CRC */
    ETH_E_UNDERSIZEFRAME     = 3U, /*!< Contains enablement of specific error report and value assigned by Dem as ETH_E_UNDERSIZEFRAME */
    ETH_E_OVERSIZEFRAME      = 4U, /*!< Contains enablement of specific error report and value assigned by Dem as ETH_E_OVERSIZEFRAME */
    ETH_E_ALIGNMENT          = 5U, /*!< Contains enablement of specific error report and value assigned by Dem as ETH_E_ALIGNMENT */
    ETH_E_SINGLECOLLISION    = 6U, /*!< Contains enablement of specific error report and value assigned by Dem as ETH_E_SINGLECOLLISION */
    ETH_E_MULTIPLECOLLISION  = 7U, /*!< Contains enablement of specific error report and value assigned by Dem as ETH_E_MULTIPLECOLLISION */
    ETH_E_LATECOLLISION      = 8U, /*!< Contains enablement of specific error report and value assigned by Dem as ETH_E_LATECOLLISION */
    ETH_NUMBER_OF_DEM_ERRORS = 9U /*!< Contains enablement of specific error report and value assigned by Dem as ETH_E_NUMBER_OF_DEM_ERRORS */
} Eth_DemErrorIdxType;

/**
 *  @brief   This type is used to store ETH DEM events configuration.
 *  @details SRS ID:     SWS_Eth_00173, SWS_Eth_00219,SWS_Eth_00222
 *           Design ID:
 */
typedef struct
{
    boolean Config_bool; /*!< Enable/Disable Dem Event */
    uint32_t ErrorId_u32;   /*!< Dem Error Id */
} Eth_DemEventType;
#endif /*!< (STD_ON == ETH_DEM_EVENT_ENABLE) */

/**
 *  @brief   Define the Ctrl Config for Eth.
 *  @details SRS ID:     SWS_Eth_00156
 *           Design ID:
 */
typedef struct
{
    const Eth_Ipr_ConfigType *EthIprCfg_ptr; /*!< The pointer containing the configuration of the IP Router layer. */
#if (STD_ON == ETH_DEM_EVENT_ENABLE)
    const Eth_DemEventType EthDemError_aa[ETH_NUMBER_OF_DEM_ERRORS]; /*!< Dem Error Event Type */
#endif /*!< (STD_ON == ETH_DEM_EVENT_ENABLE) */
} Eth_CtrlConfigType;

/**
 *  @brief   Define the Eth config.
 *  @details SRS ID:     SWS_Eth_00156
 *           Design ID:  SDD_ETH_73
 */
typedef struct
{
#if (STD_ON == ETH_MULTICORE_ENABLED)
    const uint32_t EthCoreID_u32; /*!< Specified core ID. */
#endif /* (STD_ON == ETH_MULTICORE_ENABLED) */
    const Eth_CtrlConfigType *EthCtrConfigList_pp[ETH_MAX_CONTROLLER_COUNT]; /*!< The pointer containing the configuration of the Ctrl list. */
} Eth_ConfigType;

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

#endif /*!< ETH_TYPES_H */
