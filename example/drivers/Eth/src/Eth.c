/***********************************************************************************************************************
 * Project Name: XHSC AUTOSAR MCAL
 * File Name: Eth.c
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
 *   @file       Eth.c
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
#include "Eth.h"
#include "Eth_Ipr.h"
#include "EthIf.h"
#include "EthTrcv.h"
#if (STD_ON == ETH_DEV_ERROR_DETECT)
#include "Det.h"
#endif /*!< (STD_ON == ETH_DEV_ERROR_DETECT) */
#if (STD_ON == ETH_MULTICORE_ENABLED)
#include "CDD_System.h"
#endif /*!< (STD_ON == ETH_MULTICORE_ENABLED) */
#if (STD_ON == ETH_DEM_EVENT_ENABLE)
#include "Dem.h"
#endif /*!< (STD_ON == ETH_DEM_EVENT_ENABLE) */
#include "SchM_Eth.h"

/***********************************************************************************************************************
 *  VERSION CHECK
 **********************************************************************************************************************/
/**
 *  @brief   VersionInfo check.
 *  @details SRS ID:     SWS_Eth_00007
 */
#ifndef DISABLE_EXTERNAL_VERSION_CHECK
#if (STD_ON == ETH_DEV_ERROR_DETECT)
/**
 * @brief  Check if Eth.c and Det.h are of the same version
 */
#if ((DET_AR_RELEASE_MAJOR_VERSION != ETH_AR_RELEASE_MAJOR_VERSION) || \
     (DET_AR_RELEASE_MINOR_VERSION != ETH_AR_RELEASE_MINOR_VERSION))
#error "Inconsistent AutoSar Version Numbers of Eth.c and Det.h ."
#endif
#endif /*!< (STD_ON == ETH_DEV_ERROR_DETECT) */
#if (STD_ON == ETH_MULTICORE_ENABLED)
/**
 * @brief  Check if Eth.c and Osr_Internal.h are of the same version
 */
#if ((SYSTEM_AR_RELEASE_MAJOR_VERSION != ETH_AR_RELEASE_MAJOR_VERSION) || \
     (SYSTEM_AR_RELEASE_MINOR_VERSION != ETH_AR_RELEASE_MINOR_VERSION))
#error "Inconsistent AutoSar Version Numbers of Eth.c and Osr_Internal.h ."
#endif
#endif /*!< (STD_ON == ETH_MULTICORE_ENABLED) */
#if (STD_ON == ETH_DEM_EVENT_ENABLE)
/**
 * @brief Check if Eth.c and Dem.h are of the same version
 */
#if ((DEM_AR_RELEASE_MAJOR_VERSION != ETH_AR_RELEASE_MAJOR_VERSION) || \
     (DEM_AR_RELEASE_MINOR_VERSION != ETH_AR_RELEASE_MINOR_VERSION))
#error "Inconsistent AutoSar Version Numbers of Eth.c and Dem.h ."
#endif
#endif /*!< (STD_ON == ETH_DEM_EVENT_ENABLE) */
#endif
/**
 * @brief Check if Eth.c and Eth_Cfg.h are of the same version
 */
#if ((ETH_SW_MAJOR_VERSION != ETH_CFG_SW_MAJOR_VERSION) || \
     (ETH_SW_MINOR_VERSION != ETH_CFG_SW_MINOR_VERSION) || \
     (ETH_SW_PATCH_VERSION != ETH_CFG_SW_PATCH_VERSION))
#error "Inconsistent Software Version Numbers of Eth.c and Eth_Cfg.h ."
#endif
/***********************************************************************************************************************
 *  DEFINES & MACROS
 **********************************************************************************************************************/
#if (STD_ON == ETH_MULTICORE_ENABLED)
#define Eth_GetCoreID() ((uint32_t)Osr_GetCoreID())
#else
#define Eth_GetCoreID() ((uint32_t)0UL)
#endif /*!< (STD_ON == ETH_MULTICORE_ENABLED) */

#define ETH_MAX_ERROR_VALUE (255U)
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
#define ETH_START_SEC_VAR_CLEARED_UNSPECIFIED
#include "Eth_MemMap.h"

static Eth_ModeType s_EthCtrlMode_aa[ETH_MAX_CONTROLLER_COUNT];

#if (STD_ON == ETH_DEV_ERROR_DETECT)

static Eth_StateType s_EthCtrlState_aa[ETH_MAX_CONTROLLER_COUNT];

#endif /*!< (STD_ON == ETH_DEV_ERROR_DETECT) */

static const Eth_ConfigType *s_EthConfig_aa[ETH_MAX_PARTITIONS];

#define ETH_STOP_SEC_VAR_CLEARED_UNSPECIFIED
#include "Eth_MemMap.h"

#if (STD_ON == ETH_DEM_EVENT_ENABLE)
#define ETH_START_SEC_VAR_CLEARED_32
#include "Eth_MemMap.h"

static uint32_t s_EthDemErrorCounterOld_aa[ETH_MAX_CONTROLLER_COUNT][ETH_NUMBER_OF_DEM_ERRORS];

static uint32_t s_EthDemErrorCounterNew_aa[ETH_MAX_CONTROLLER_COUNT][ETH_NUMBER_OF_DEM_ERRORS];

#define ETH_STOP_SEC_VAR_CLEARED_32
#include "Eth_MemMap.h"
#endif /*!< (STD_ON == ETH_DEM_EVENT_ENABLE) */

/***********************************************************************************************************************
 *  GLOBAL VARIABLES
 **********************************************************************************************************************/

/***********************************************************************************************************************
 *  LOCAL FUNCTION PROTOTYPES
 **********************************************************************************************************************/
#define ETH_START_SEC_CODE
#include "Eth_MemMap.h"

static void Eth_LocalInit(
    uint32_t para_CoreId_u32
);

#if (STD_ON == ETH_DEV_ERROR_DETECT)

static boolean Eth_LocalCheckCommonDet(
    uint8_t para_CtrlIdx_u8,
    uint8_t para_ServiceIdx_u8
);
#endif /*!< (STD_ON == ETH_DEV_ERROR_DETECT) */

#if (STD_ON == ETH_DEM_EVENT_ENABLE)

static void Eth_LocalCheckDems(
    uint8_t para_CtrlIdx_u8,
    const Eth_DemEventType *para_Event_ptr
);
#endif /*!< (STD_ON == ETH_DEM_EVENT_ENABLE) */

/***********************************************************************************************************************
 *  LOCAL FUNCTION
 **********************************************************************************************************************/
/**
 *  @brief          Local Init.
 *
 *  @details        Service ID:        None
 *                  SRS ID:            None
 *                  Design ID:         Eth_LocalInit_Activity
 *                  Sync/Async:        None
 *                  Reentrancy:        None
 *
 *  @param[in]      para_CoreId_u32      None
 *
 *  @return         void
 *  @retval         None
 *
 *  @note           None
 *
 *  @pre            None
 *
 */

static void Eth_LocalInit(
    uint32_t para_CoreId_u32
)
{
    uint8_t CtrlIdx_u8;
    const Eth_CtrlConfigType *Config_ptr;
    /* Initialize all the hardware controllers based on the configuration data */
    for (CtrlIdx_u8 = 0U; CtrlIdx_u8 < ETH_MAX_CONTROLLER_COUNT; CtrlIdx_u8++)
    {
        /* Initialize all hardware controllers belonging to this core as idle */
        Config_ptr = s_EthConfig_aa[para_CoreId_u32]->EthCtrConfigList_pp[CtrlIdx_u8];
        if (NULL_PTR != Config_ptr)
        {
#if (STD_ON == ETH_DEV_ERROR_DETECT)
            s_EthCtrlState_aa[CtrlIdx_u8] = ETH_STATE_UNINIT;
#endif /*!< (STD_ON == ETH_DEV_ERROR_DETECT) */

            s_EthCtrlMode_aa[CtrlIdx_u8] = ETH_MODE_DOWN;
            /* Check whether the controller can be initialized currently */
            if (TRUE == Eth_Ipr_CheckAccessToController(CtrlIdx_u8))
            {
                /* Invoke IP Route initialization function */
                Eth_Ipr_Init(CtrlIdx_u8, Config_ptr->EthIprCfg_ptr);

#if (STD_ON == ETH_DEV_ERROR_DETECT)
                /* Indicate that the current driver is now in an initialized state */
                s_EthCtrlState_aa[CtrlIdx_u8] = ETH_STATE_INIT;
#endif /*!< (STD_ON == ETH_DEV_ERROR_DETECT) */

#if (STD_ON == ETH_DEM_EVENT_ENABLE)
                for (uint32_t ErrorIdx_u32 = 0U; ErrorIdx_u32 < (uint32_t)ETH_NUMBER_OF_DEM_ERRORS; ErrorIdx_u32++)
                {
                    s_EthDemErrorCounterOld_aa[CtrlIdx_u8][ErrorIdx_u32] = 0U;
                }
                if (TRUE == Config_ptr->EthDemError_aa[ETH_E_ACCESS].Config_bool)
                {
                    (void)Dem_SetEventStatus((Dem_EventIdType)Config_ptr->EthDemError_aa[ETH_E_ACCESS].ErrorId_u32,
                                             DEM_EVENT_STATUS_PREPASSED);
                }
#endif /*!< (STD_ON == ETH_DEM_EVENT_ENABLE) */
            }
#if (STD_ON == ETH_DEM_EVENT_ENABLE)
            else
            {

                if (TRUE == Config_ptr->EthDemError_aa[ETH_E_ACCESS].Config_bool)
                {
                    (void)Dem_SetEventStatus((Dem_EventIdType)Config_ptr->EthDemError_aa[ETH_E_ACCESS].ErrorId_u32,
                                             DEM_EVENT_STATUS_PREFAILED);
                }
            }
#endif /*!< (STD_ON == ETH_DEM_EVENT_ENABLE) */
        }
    }
}

#if (STD_ON == ETH_DEV_ERROR_DETECT)

/**
 *  @brief          Local Check Common Det.
 *
 *  @details        Service ID:        None
 *                  SRS ID:            None
 *                  Design ID:         Eth_LocalCheckCommonDet_Activity
 *                  Sync/Async:        None
 *                  Reentrancy:        None
 *
 *  @param[in]      para_CtrlIdx_u8      None
 *  @param[in]      para_ServiceID_u8    None
 *
 *  @return         boolean
 *  @retval         None
 *
 *  @note           None
 *
 *  @pre            None
 *
 */
static boolean Eth_LocalCheckCommonDet(
    uint8_t para_CtrlIdx_u8,
    uint8_t para_ServiceIdx_u8
)
{
    boolean CheckPass_bool = FALSE;
    uint32_t CoreId_u32      = Eth_GetCoreID();
    uint8_t ErrorId_u8       = ETH_MAX_ERROR_VALUE;

#if (STD_ON == ETH_MULTICORE_ENABLED)
    if ((CoreId_u32 >= ETH_MAX_PARTITIONS) || (NULL_PTR == s_EthConfig_aa[CoreId_u32]))
#else
    if (NULL_PTR == s_EthConfig_aa[CoreId_u32])
#endif /*!< (STD_ON == ETH_MULTICORE_ENABLED) */
    {
        ErrorId_u8 = ETH_E_UNINIT;
    }
    else if ((para_CtrlIdx_u8 >= ETH_MAX_CONTROLLER_COUNT) ||
             (NULL_PTR == s_EthConfig_aa[CoreId_u32]->EthCtrConfigList_pp[para_CtrlIdx_u8]))
    {
        ErrorId_u8 = ETH_E_INV_CTRL_IDX;
    }
    else if (ETH_STATE_INIT != s_EthCtrlState_aa[para_CtrlIdx_u8])
    {
        ErrorId_u8 = ETH_E_UNINIT;
    }
    else
    {
        CheckPass_bool = TRUE;
    }

    if (ETH_MAX_ERROR_VALUE != ErrorId_u8)
    {
        (void)Det_ReportError((uint16_t)ETH_MODULE_ID, ETH_INSTANCE_ID, para_ServiceIdx_u8,
                              ErrorId_u8);
    }
    return CheckPass_bool;
}
#endif /*!< (STD_ON == ETH_DEV_ERROR_DETECT) */

#if (STD_ON == ETH_DEM_EVENT_ENABLE)

/**
 *  @brief          Local Check Dems.
 *
 *  @details        Service ID:        None
 *                  SRS ID:            None
 *                  Design ID:         Eth_LocalCheckDems_Activity
 *                  Sync/Async:        None
 *                  Reentrancy:        None
 *
 *  @param[in]      para_CtrlIdx_u8    None
 *  @param[in]      para_Event_ptr     None
 *
 *  @return         void
 *
 *  @note           None
 *
 *  @pre            None
 *
 */
static void Eth_LocalCheckDems(
    uint8_t para_CtrlIdx_u8,
    const Eth_DemEventType *para_Event_ptr
)
{
    uint32_t ErrorIdx_u32 = (uint32_t)ETH_E_RX_FRAMES_LOST;

    Eth_Ipr_GetDemErrorCounter(para_CtrlIdx_u8, &s_EthDemErrorCounterNew_aa[para_CtrlIdx_u8][0]);

    for (; ErrorIdx_u32 < (uint32_t)ETH_NUMBER_OF_DEM_ERRORS; ErrorIdx_u32++)
    {
        if (TRUE == para_Event_ptr[ErrorIdx_u32].Config_bool)
        {
            if (s_EthDemErrorCounterNew_aa[para_CtrlIdx_u8][ErrorIdx_u32] != s_EthDemErrorCounterOld_aa[para_CtrlIdx_u8][ErrorIdx_u32])
            {
                s_EthDemErrorCounterOld_aa[para_CtrlIdx_u8][ErrorIdx_u32] = s_EthDemErrorCounterNew_aa[para_CtrlIdx_u8][ErrorIdx_u32];
                (void)Dem_SetEventStatus((Dem_EventIdType)para_Event_ptr[ErrorIdx_u32].ErrorId_u32, DEM_EVENT_STATUS_PREFAILED);
            }
            else
            {
                (void)Dem_SetEventStatus((Dem_EventIdType)para_Event_ptr[ErrorIdx_u32].ErrorId_u32, DEM_EVENT_STATUS_PREPASSED);
            }
        }
    }
}
#endif /*!< (STD_ON == ETH_DEM_EVENT_ENABLE) */

/***********************************************************************************************************************
 *  GLOBAL FUNCTION
 **********************************************************************************************************************/
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
)
{
    uint32_t CoreId_u32 = Eth_GetCoreID();

#if (STD_ON == ETH_DEV_ERROR_DETECT)
    uint8_t ErrorId_u8 = ETH_MAX_ERROR_VALUE;
#endif /*!< (STD_ON == ETH_DEV_ERROR_DETECT) */

#if (STD_ON == ETH_PRECOMPILE_CFG_ENABLED)
#if (STD_ON == ETH_DEV_ERROR_DETECT)
    if (NULL_PTR != CfgPtr)
    {
        ErrorId_u8 = ETH_E_INV_PARAM;
    }
#if (STD_ON == ETH_MULTICORE_ENABLED)
    else if (NULL_PTR == Eth_g_Config_aa[CoreId_u32])
    {
        ErrorId_u8 = ETH_E_INV_PARAM;
    }
#endif /*!< (STD_ON == ETH_MULTICORE_ENABLED) */
    else
    {
#endif /*!< (STD_ON == ETH_DEV_ERROR_DETECT) */
#if (STD_ON == ETH_MULTICORE_ENABLED)
        /* Point the global configuration pointer ot user configuration pointer based on multicore */
        s_EthConfig_aa[CoreId_u32] = Eth_g_Config_aa[CoreId_u32];
#else  /*!< (STD_ON == ETH_MULTICORE_ENABLED) */
    /* Point the global configuration pointer ot user configuration pointer */
    s_EthConfig_aa[CoreId_u32] = &Eth_Config;
#endif /*!< (STD_ON == ETH_MULTICORE_ENABLED) */
    (void)CfgPtr;
#else  /*!< (STD_ON == ETH_PRECOMPILE_CFG_ENABLED) */
#if (STD_ON == ETH_DEV_ERROR_DETECT)
    if (NULL_PTR == CfgPtr)
    {
        ErrorId_u8 = ETH_E_PARAM_POINTER;
    }
#if (STD_ON == ETH_MULTICORE_ENABLED)
    else if (CoreId_u32 != CfgPtr->EthCoreID_u32)
    {
        ErrorId_u8 = ETH_E_INV_PARAM;
    }
#endif /*!< (STD_ON == ETH_MULTICORE_ENABLED) */
    else
    {
#endif /*!< (STD_ON == ETH_DEV_ERROR_DETECT) */
        /* Point the global configuration pointer ot user configuration pointer */
        s_EthConfig_aa[CoreId_u32] = CfgPtr;
#endif /*!< (STD_ON == ETH_PRECOMPILE_CFG_ENABLED) */
        /* Invoke local initialization function to prepare the initialization */
        Eth_LocalInit(CoreId_u32);
#if (STD_ON == ETH_DEV_ERROR_DETECT)
    }

    if (ETH_MAX_ERROR_VALUE != ErrorId_u8)
    {
        (void)Det_ReportError((uint16_t)ETH_MODULE_ID, ETH_INSTANCE_ID, ETH_SID_INIT,
                              ErrorId_u8);
    }
#endif /*!< (STD_ON == ETH_DEV_ERROR_DETECT) */
}

/**
 *  @brief          Enables  disables the indexed controller.
 *
 *  @details        Service ID:        0x03
 *                  SRS ID:            SWS_Eth_00041,SWS_Eth_00042,SWS_Eth_00043,
 *                                     SWS_Eth_00044,SWS_Eth_00045,SWS_Eth_00168,
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
)
{
    Std_ReturnType Status_en = E_NOT_OK;
    uint32_t CoreId_u32 = Eth_GetCoreID();
    const Eth_CtrlConfigType *CtrConfig_ptr;

#if (STD_ON == ETH_DEV_ERROR_DETECT)
    if (TRUE == Eth_LocalCheckCommonDet(CtrlIdx, ETH_SID_SETCONTROLLERMODE))
    {
#endif /*!< (STD_ON == ETH_DEV_ERROR_DETECT) */
        CtrConfig_ptr = s_EthConfig_aa[CoreId_u32]->EthCtrConfigList_pp[CtrlIdx];
        /* Check whether the controller can be initialized currently */
        if (TRUE == Eth_Ipr_CheckAccessToController(CtrlIdx))
        {
            /* Start or stop DMA transmission and receive */
            Status_en = Eth_Ipr_SetControllerMode(CtrlIdx, CtrConfig_ptr->EthIprCfg_ptr, CtrlMode);
#if (STD_ON == ETH_DEM_EVENT_ENABLE)
            if (TRUE == CtrConfig_ptr->EthDemError_aa[ETH_E_ACCESS].Config_bool)
            {
                (void)Dem_SetEventStatus((Dem_EventIdType)CtrConfig_ptr->EthDemError_aa[ETH_E_ACCESS].ErrorId_u32,
                                         DEM_EVENT_STATUS_PREPASSED);
            }
#endif /*!< (STD_ON == ETH_DEM_EVENT_ENABLE) */
        }
#if (STD_ON == ETH_DEM_EVENT_ENABLE)
        else
        {

            if (TRUE == CtrConfig_ptr->EthDemError_aa[ETH_E_ACCESS].Config_bool)
            {
                (void)Dem_SetEventStatus((Dem_EventIdType)CtrConfig_ptr->EthDemError_aa[ETH_E_ACCESS].ErrorId_u32,
                                         DEM_EVENT_STATUS_PREFAILED);
            }
        }
#endif /*!< (STD_ON == ETH_DEM_EVENT_ENABLE) */
#if (STD_ON == ETH_DEV_ERROR_DETECT)
    }
#endif /*!< (STD_ON == ETH_DEV_ERROR_DETECT) */

    return Status_en;
}

/**
 *  @brief          Obtains the state of the indexed controller.
 *
 *  @details        Service ID:        0x04
 *                  SRS ID:            SWS_Eth_00046,SWS_Eth_00047,SWS_Eth_00048,
 *                                     SWS_Eth_00049,SWS_Eth_00050,SWS_Eth_00051,
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
)
{
    Std_ReturnType Status_en = E_NOT_OK;

#if (STD_ON == ETH_DEV_ERROR_DETECT)
    if (NULL_PTR == CtrlModePtr)
    {
        (void)Det_ReportError((uint16_t)ETH_MODULE_ID, ETH_INSTANCE_ID, ETH_SID_GETCONTROLLERMODE,
                              ETH_E_PARAM_POINTER);
    }
    else if (TRUE == Eth_LocalCheckCommonDet(CtrlIdx, ETH_SID_GETCONTROLLERMODE))
    {
#endif /*!< (STD_ON == ETH_DEV_ERROR_DETECT) */
        /* Invoke the IPR route function to get the current controller mode */
        *CtrlModePtr = Eth_Ipr_GetControllerMode(CtrlIdx);
        Status_en    = E_OK;
#if (STD_ON == ETH_DEV_ERROR_DETECT)
    }
    else
    {
        /* Fulfill the remaining conditions, do nothing. */
    }
#endif /*!< (STD_ON == ETH_DEV_ERROR_DETECT) */

    return Status_en;
}

/**
 *  @brief          Obtains the physical source address used by the indexed controller.
 *
 *  @details        Service ID:        0x08
 *                  SRS ID:            SWS_Eth_00052,SWS_Eth_00053,SWS_Eth_00054,
 *                                     SWS_Eth_00055,SWS_Eth_00056,SWS_Eth_00057,
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
)
{
#if (STD_ON == ETH_DEV_ERROR_DETECT)
    if (NULL_PTR == PhysAddrPtr)
    {
        (void)Det_ReportError((uint16_t)ETH_MODULE_ID, ETH_INSTANCE_ID, ETH_SID_GETPHYSADDR,
                              ETH_E_PARAM_POINTER);
    }
    else if (TRUE == Eth_LocalCheckCommonDet(CtrlIdx, ETH_SID_GETPHYSADDR))
    {
#endif /*!< (STD_ON == ETH_DEV_ERROR_DETECT) */
        /* Invoke IPR route to get the current MAC address */
        Eth_Ipr_GetPhysAddr(CtrlIdx, PhysAddrPtr);
#if (STD_ON == ETH_DEV_ERROR_DETECT)
    }
    else
    {
        /* Fulfill the remaining conditions, do nothing. */
    }
#endif /*!< (STD_ON == ETH_DEV_ERROR_DETECT) */
}

/**
 *  @brief          Sets the physical source address used by the indexed controller.
 *
 *  @details        Service ID:        0x13
 *                  SRS ID:            SWS_Eth_00139,SWS_Eth_00140,SWS_Eth_00141,
 *                                     SWS_Eth_00142,SWS_Eth_00143,SWS_Eth_00151,
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
)
{
#if (STD_ON == ETH_DEV_ERROR_DETECT)
    if (NULL_PTR == PhysAddrPtr)
    {
        (void)Det_ReportError((uint16_t)ETH_MODULE_ID, ETH_INSTANCE_ID, ETH_SID_SETPHYSADDR,
                              ETH_E_PARAM_POINTER);
    }
    else if (TRUE == Eth_LocalCheckCommonDet(CtrlIdx, ETH_SID_SETPHYSADDR))
    {
#endif /*!< (STD_ON == ETH_DEV_ERROR_DETECT) */
        /* Invoke IPR route to set MAC address */
        Eth_Ipr_SetPhysAddr(CtrlIdx, PhysAddrPtr);
#if (STD_ON == ETH_DEV_ERROR_DETECT)
    }
    else
    {
        /* Fulfill the remaining conditions, do nothing. */
    }
#endif /*!< (STD_ON == ETH_DEV_ERROR_DETECT) */
}

/**
 *  @brief          Update the physical source address tofrom the indexed controller filter. If the Ethernet Controller
 *                  is not capable to do the filtering, the software has to do this.
 *
 *  @details        Service ID:        0x12
 *                  SRS ID:            SWS_Eth_00150,SWS_Eth_00152,SWS_Eth_00164,
 *                                     SWS_Eth_00165,SWS_Eth_00245,SWS_Eth_00246,
 *                                     SWS_Eth_00003,SWS_Eth_00008,SWS_Eth_00166,
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
)
{
    Std_ReturnType Status_en = E_NOT_OK;
#if (STD_ON == ETH_DEV_ERROR_DETECT)
    if (NULL_PTR == PhysAddrPtr)
    {
        (void)Det_ReportError((uint16_t)ETH_MODULE_ID, ETH_INSTANCE_ID,
                              ETH_SID_UPDATEPHYSADDRFILTER, ETH_E_PARAM_POINTER);
    }
    else if (TRUE == Eth_LocalCheckCommonDet(CtrlIdx, ETH_SID_UPDATEPHYSADDRFILTER))
    {
#endif /*!< (STD_ON == ETH_DEV_ERROR_DETECT) */
        /* Invoke IPR route to update the current MAC filter address */
        Status_en = Eth_Ipr_UpdatePhysAddrFilter(CtrlIdx, PhysAddrPtr, Action);
#if (STD_ON == ETH_DEV_ERROR_DETECT)
    }
    else
    {
        /* Fulfill the remaining conditions, do nothing. */
    }
#endif /*!< (STD_ON == ETH_DEV_ERROR_DETECT) */
    return Status_en;
}

#if (STD_ON == ETH_ENABLE_MII)
/**
 *  @brief          Configures a transceiver register or triggers a function offered by the receiver.
 *
 *  @details        Service ID:        0x05
 *                  SRS ID:            SWS_Eth_00058,SWS_Eth_00059,SWS_Eth_00060,
 *                                     SWS_Eth_00061,SWS_Eth_00062,SWS_Eth_00241,
 *                                     SWS_Eth_00003,SWS_Eth_00008,SWS_Eth_00063,
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
)
{
    Std_ReturnType Status_en = E_NOT_OK;

#if (STD_ON == ETH_DEV_ERROR_DETECT)
    if (TRUE == Eth_LocalCheckCommonDet(CtrlIdx, ETH_SID_WRITEMII))
    {
#endif /*!< (STD_ON == ETH_DEV_ERROR_DETECT) */
        /* Invoke the IPR function to configure PHY with MII interface */
        Status_en = Eth_Ipr_WriteMii(CtrlIdx, TrcvIdx, RegIdx, RegVal);
        if (E_OK == Status_en)
        {
            /* Provide the PHY driver checking interface */
            EthTrcv_WriteMiiIndication(CtrlIdx, TrcvIdx, RegIdx);
        }
#if (STD_ON == ETH_DEV_ERROR_DETECT)
    }
#endif /*!< (STD_ON == ETH_DEV_ERROR_DETECT) */

    return Status_en;
}

/**
 *  @brief          Reads a transceiver register.
 *
 *  @details        Service ID:        0x06
 *                  SRS ID:            SWS_Eth_00064,SWS_Eth_00065,SWS_Eth_00066,
 *                                     SWS_Eth_00067,SWS_Eth_00068,SWS_Eth_00242,
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
)
{
    Std_ReturnType Status_en = E_NOT_OK;

#if (STD_ON == ETH_DEV_ERROR_DETECT)
    if (NULL_PTR == RegValPtr)
    {
        (void)Det_ReportError((uint16_t)ETH_MODULE_ID, ETH_INSTANCE_ID, ETH_SID_READMII,
                              ETH_E_PARAM_POINTER);
    }
    else if (TRUE == Eth_LocalCheckCommonDet(CtrlIdx, ETH_SID_READMII))
    {
#endif /*!< (STD_ON == ETH_DEV_ERROR_DETECT) */
        /* Invoke the IPR function to read data with MII interface */
        Status_en = Eth_Ipr_ReadMii(CtrlIdx, TrcvIdx, RegIdx, RegValPtr);
        if (E_OK == Status_en)
        {
            /* Provide the PHY driver checking interface */
            EthTrcv_ReadMiiIndication(CtrlIdx, TrcvIdx, RegIdx, (uint8_t)*RegValPtr);
        }
#if (STD_ON == ETH_DEV_ERROR_DETECT)
    }
    else
    {
        /* Fulfill the remaining conditions, do nothing. */
    }
#endif /*!< (STD_ON == ETH_DEV_ERROR_DETECT) */

    return Status_en;
}
#endif /*!< (STD_ON == ETH_ENABLE_MII) */

#if (STD_ON == ETH_GET_COUNTER_VALUES_API)
/**
 *  @brief          Reads a list with drop counter values of the corresponding controller. The meaning
 *                  of these values is described at Eth_CounterType.
 *
 *  @details        Service ID:        0x14
 *                  SRS ID:            SWS_Eth_00226,SWS_Eth_00228,SWS_Eth_00229,
 *                                     SWS_Eth_00230,SWS_Eth_00231,SWS_Eth_00003,
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
)
{
    Std_ReturnType Status_en = E_NOT_OK;

#if (STD_ON == ETH_DEV_ERROR_DETECT)
    if (NULL_PTR == CounterPtr)
    {
        (void)Det_ReportError((uint16_t)ETH_MODULE_ID, ETH_INSTANCE_ID, ETH_SID_GETCOUNTERVALUES,
                              ETH_E_PARAM_POINTER);
    }
    else if (TRUE == Eth_LocalCheckCommonDet(CtrlIdx, ETH_SID_GETCOUNTERVALUES))
    {
#endif /*!< (STD_ON == ETH_DEV_ERROR_DETECT) */
        Status_en = Eth_Ipr_GetCounterValues(CtrlIdx, CounterPtr);
#if (STD_ON == ETH_DEV_ERROR_DETECT)
    }
    else
    {
        /* Fulfill the remaining conditions, do nothing. */
    }
#endif /*!< (STD_ON == ETH_DEV_ERROR_DETECT) */

    return Status_en;
}
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
 *                  SRS ID:            SWS_Eth_00233,SWS_Eth_00235,SWS_Eth_00236,
 *                                     SWS_Eth_00237,SWS_Eth_00238,SWS_Eth_00003,
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
)
{
    Std_ReturnType Status_en = E_NOT_OK;

#if (STD_ON == ETH_DEV_ERROR_DETECT)
    if (NULL_PTR == RxStats)
    {
        (void)Det_ReportError((uint16_t)ETH_MODULE_ID, ETH_INSTANCE_ID, ETH_SID_GETRXSTATS,
                              ETH_E_PARAM_POINTER);
    }
    else if (TRUE == Eth_LocalCheckCommonDet(CtrlIdx, ETH_SID_GETRXSTATS))
    {
#endif /*!< (STD_ON == ETH_DEV_ERROR_DETECT) */
        Status_en = Eth_Ipr_GetRxStats(CtrlIdx, RxStats);
#if (STD_ON == ETH_DEV_ERROR_DETECT)
    }
    else
    {
        /* Fulfill the remaining conditions, do nothing. */
    }
#endif /*!< (STD_ON == ETH_DEV_ERROR_DETECT) */

    return Status_en;
}
#endif /*!< (STD_ON == ETH_GET_RECEIVED_STATS_API) */

#if (STD_ON == ETH_GET_TRANSMIT_STATS_API)
/**
 *  @brief          Returns the list of Transmission Statistics out of IETF RFC1213 defined with Eth_TxStatsType, where
 *                  the maximal possible value shall denote an invalid value, e.g. this counter is not available.
 *
 *  @details        Service ID:        0x1c
 *                  SRS ID:            SWS_Eth_00248,SWS_Eth_00249,SWS_Eth_00250,
 *                                     SWS_Eth_00251,SWS_Eth_91005,SWS_Eth_00003,
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
)
{
    Std_ReturnType Status_en = E_NOT_OK;

#if (STD_ON == ETH_DEV_ERROR_DETECT)
    if (NULL_PTR == TxStats)
    {
        (void)Det_ReportError((uint16_t)ETH_MODULE_ID, ETH_INSTANCE_ID, ETH_SID_GETTXSTATS,
                              ETH_E_PARAM_POINTER);
    }
    else if (TRUE == Eth_LocalCheckCommonDet(CtrlIdx, ETH_SID_GETTXSTATS))
    {
#endif /*!< (STD_ON == ETH_DEV_ERROR_DETECT) */
        Status_en = Eth_Ipr_GetTxStats(CtrlIdx, TxStats);
#if (STD_ON == ETH_DEV_ERROR_DETECT)
    }
    else
    {
        /* Fulfill the remaining conditions, do nothing. */
    }
#endif /*!< (STD_ON == ETH_DEV_ERROR_DETECT) */

    return Status_en;
}
#endif /*!< (STD_ON == ETH_GET_TRANSMIT_STATS_API) */

#if (STD_ON == ETH_GET_TX_ERROR_COUNTER_VALUES_API)
/**
 *  @brief          Returns the list of Transmission Error Counters out of IETF RFC1213 and RFC1643 defined with
 *                  Eth_TxErrorCounterValuesType, where the maximal possible value shall denote an invalid value, e.g.
 *                  this counter is not available.
 *
 *  @details        Service ID:        0x1d
 *                  SRS ID:            SWS_Eth_00252,SWS_Eth_00253,SWS_Eth_00254,
 *                                     SWS_Eth_00255,SWS_Eth_91006,SWS_Eth_00003,
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
)
{
    Std_ReturnType Status_en = E_NOT_OK;

#if (STD_ON == ETH_DEV_ERROR_DETECT)
    if (NULL_PTR == TxErrorCounterValues)
    {
        (void)Det_ReportError((uint16_t)ETH_MODULE_ID, ETH_INSTANCE_ID,
                              ETH_SID_GETTXERRORCOUNTERVALUES, ETH_E_PARAM_POINTER);
    }
    else if (TRUE == Eth_LocalCheckCommonDet(CtrlIdx, ETH_SID_GETTXERRORCOUNTERVALUES))
    {
#endif /*!< (STD_ON == ETH_DEV_ERROR_DETECT) */
        Status_en = Eth_Ipr_GetTxErrorCounterValues(CtrlIdx, TxErrorCounterValues);
#if (STD_ON == ETH_DEV_ERROR_DETECT)
    }
    else
    {
        /* Fulfill the remaining conditions, do nothing. */
    }
#endif /*!< (STD_ON == ETH_DEV_ERROR_DETECT) */

    return Status_en;
}
#endif /*!< (STD_ON == ETH_GET_TX_ERROR_COUNTER_VALUES_API) */

#if (STD_ON == ETH_GLOBAL_TIME_SUPPORT_API)
/**
 *  @brief          Returns a time value out of the HW registers according to the capability of the HW. Is the HW
 *                  resolution is lower than the Eth_TimeStampType resolution resp. range, than an the remaining bits
 *                  will be filled with 0.
 *
 *  @details        Service ID:        0x16
 *                  SRS ID:            SWS_Eth_00181,SWS_Eth_00182,SWS_Eth_00183,
 *                                     SWS_Eth_00184,SWS_Eth_00185,SWS_Eth_00210,
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
)
{
    Std_ReturnType Status_en = E_NOT_OK;

#if (STD_ON == ETH_DEV_ERROR_DETECT)
    if ((NULL_PTR == timeQualPtr) || (NULL_PTR == timeStampPtr))
    {
        (void)Det_ReportError((uint16_t)ETH_MODULE_ID, ETH_INSTANCE_ID, ETH_SID_GETCURRENTTIME,
                              ETH_E_PARAM_POINTER);
    }
    else if (TRUE == Eth_LocalCheckCommonDet(CtrlIdx, ETH_SID_GETCURRENTTIME))
    {
#endif /*!< (STD_ON == ETH_DEV_ERROR_DETECT) */
        Status_en = Eth_Ipr_GetCurrentTime(CtrlIdx, timeQualPtr, timeStampPtr);
#if (STD_ON == ETH_DEV_ERROR_DETECT)
    }
    else
    {
        /* Fulfill the remaining conditions, do nothing. */
    }
#endif /*!< (STD_ON == ETH_DEV_ERROR_DETECT) */

    return Status_en;
}

/**
 *  @brief          Activates egress time stamping on a dedicated message object.
 *                  Some HW does store once the egress time stamp marker and some HW needs it always before
 *                  transmission. There will be no "disable" functionality, due to the fact, that the message type is
 *                  always "time stamped" by network design.
 *
 *  @details        Service ID:        0x17
 *                  SRS ID:            SWS_Eth_00186,SWS_Eth_00187,SWS_Eth_00188,
 *                                     SWS_Eth_00189,SWS_Eth_00211,SWS_Eth_00003,
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
)
{
#if (STD_ON == ETH_DEV_ERROR_DETECT)
    if ((ETH_MAX_TX_BUFFER_NUM * ETH_MAX_TX_FIFO_NUM) <= BufIdx)
    {
        (void)Det_ReportError((uint16_t)ETH_MODULE_ID, ETH_INSTANCE_ID, ETH_SID_ENABLEEGRESSTIMESTAMP,
                              ETH_E_INV_PARAM);
    }
    else if (TRUE == Eth_LocalCheckCommonDet(CtrlIdx, ETH_SID_ENABLEEGRESSTIMESTAMP))
    {
#endif /*!< (STD_ON == ETH_DEV_ERROR_DETECT) */
        Eth_Ipr_EnableEgressTimeStamp(CtrlIdx, BufIdx);
#if (STD_ON == ETH_DEV_ERROR_DETECT)
    }
    else
    {
        /* Fulfill the remaining conditions, do nothing. */
    }
#endif /*!< (STD_ON == ETH_DEV_ERROR_DETECT) */
}

/**
 *  @brief          Reads back the egress time stamp on a dedicated message object.
 *
 *  @details        Service ID:        0x18
 *                  SRS ID:            SWS_Eth_00190,SWS_Eth_00191,SWS_Eth_00192,
 *                                     SWS_Eth_00193,SWS_Eth_00194,SWS_Eth_00212,
 *                                     SWS_Eth_00003,SWS_Eth_00004,SWS_Eth_00008,
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
)
{
    Std_ReturnType Status_en = E_NOT_OK;

#if (STD_ON == ETH_DEV_ERROR_DETECT)
    if ((NULL_PTR == timeQualPtr) || (NULL_PTR == timeStampPtr))
    {
        (void)Det_ReportError((uint16_t)ETH_MODULE_ID, ETH_INSTANCE_ID, ETH_SID_GETEGRESSTIMESTAMP,
                              ETH_E_PARAM_POINTER);
    }
    else if ((ETH_MAX_TX_BUFFER_NUM * ETH_MAX_TX_FIFO_NUM) <= BufIdx)
    {
        (void)Det_ReportError((uint16_t)ETH_MODULE_ID, ETH_INSTANCE_ID, ETH_SID_GETEGRESSTIMESTAMP,
                              ETH_E_INV_PARAM);
    }
    else if (TRUE == Eth_LocalCheckCommonDet(CtrlIdx, ETH_SID_GETEGRESSTIMESTAMP))
    {
#endif /*!< (STD_ON == ETH_DEV_ERROR_DETECT) */
        Status_en = Eth_Ipr_GetEgressTimeStamp(CtrlIdx, BufIdx, timeQualPtr, timeStampPtr);
#if (STD_ON == ETH_DEV_ERROR_DETECT)
    }
    else
    {
        /* Fulfill the remaining conditions, do nothing. */
    }
#endif /*!< (STD_ON == ETH_DEV_ERROR_DETECT) */

    return Status_en;
}

/**
 *  @brief          Reads back the ingress time stamp on a dedicated message object.
 *
 *  @details        Service ID:        0x19
 *                  SRS ID:            SWS_Eth_00195,SWS_Eth_00196,SWS_Eth_00197,
 *                                     SWS_Eth_00198,SWS_Eth_00199,SWS_Eth_00213,
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
)
{
    Std_ReturnType Status_en = E_NOT_OK;

#if (STD_ON == ETH_DEV_ERROR_DETECT)
    if ((NULL_PTR == DataPtr) || (NULL_PTR == timeQualPtr) || (NULL_PTR == timeStampPtr))
    {
        (void)Det_ReportError((uint16_t)ETH_MODULE_ID, ETH_INSTANCE_ID,
                              ETH_SID_GETINGRESSTIMESTAMP, ETH_E_PARAM_POINTER);
    }
    else if (TRUE == Eth_LocalCheckCommonDet(CtrlIdx, ETH_SID_GETINGRESSTIMESTAMP))
    {
#endif /*!< (STD_ON == ETH_DEV_ERROR_DETECT) */
        Status_en = Eth_Ipr_GetIngressTimeStamp(CtrlIdx, DataPtr, timeQualPtr, timeStampPtr);
#if (STD_ON == ETH_DEV_ERROR_DETECT)
    }
    else
    {
        /* Fulfill the remaining conditions, do nothing. */
    }
#endif /*!< (STD_ON == ETH_DEV_ERROR_DETECT) */

    return Status_en;
}
#endif /*!< (STD_ON == ETH_GLOBAL_TIME_SUPPORT_API) */

/**
 *  @brief          Provides access to a transmit buffer of the FIFO related to the specified priority.
 *
 *  @details        Service ID:        0x09
 *                  SRS ID:            SWS_Eth_00077,SWS_Eth_00078,SWS_Eth_00079,
 *                                     SWS_Eth_00080,SWS_Eth_00081,SWS_Eth_00137,
 *                                     SWS_Eth_00003,SWS_Eth_00004,SWS_Eth_00008,
 *                                     SWS_Eth_00082,SWS_Eth_00083,SWS_Eth_00084,
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
)
{
    BufReq_ReturnType Status_en = BUFREQ_E_NOT_OK;

#if (STD_ON == ETH_DEV_ERROR_DETECT)
    if ((NULL_PTR == BufIdxPtr) || (NULL_PTR == BufPtr) || (NULL_PTR == LenBytePtr))
    {
        (void)Det_ReportError((uint16_t)ETH_MODULE_ID, ETH_INSTANCE_ID, ETH_SID_PROVIDETXBUFFER,
                              ETH_E_PARAM_POINTER);
    }
    else if (TRUE == Eth_LocalCheckCommonDet(CtrlIdx, ETH_SID_PROVIDETXBUFFER))
    {
#endif /*!< (STD_ON == ETH_DEV_ERROR_DETECT) */
        Status_en = Eth_Ipr_ProvideTxBuffer(CtrlIdx, Priority, BufIdxPtr, BufPtr, LenBytePtr);
#if (STD_ON == ETH_DEV_ERROR_DETECT)
    }
    else
    {
        /* Fulfill the remaining conditions, do nothing. */
    }
#endif /*!< (STD_ON == ETH_DEV_ERROR_DETECT) */

    return Status_en;
}

/**
 *  @brief          Triggers transmission of a previously filled transmit buffer.
 *
 *  @details        Service ID:        0xA
 *                  SRS ID:            SWS_Eth_00087,SWS_Eth_00088,SWS_Eth_00090,
 *                                     SWS_Eth_00091,SWS_Eth_00092,SWS_Eth_00138,
 *                                     SWS_Eth_00003,SWS_Eth_00004,SWS_Eth_00008,
 *                                     SWS_Eth_00216,SWS_Eth_00093,SWS_Eth_00129,
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
)
{
    Std_ReturnType Status_en = E_NOT_OK;

#if (STD_ON == ETH_DEV_ERROR_DETECT)
    uint8_t ErrorId_u8 = ETH_MAX_ERROR_VALUE;

    if (NULL_PTR == PhysAddrPtr)
    {
        ErrorId_u8 = ETH_E_PARAM_POINTER;
    }
    else if (TRUE == Eth_LocalCheckCommonDet(CtrlIdx, ETH_SID_TRANSMIT))
    {
        if (TRUE == Eth_Ipr_CheckBufferIdxValid(CtrlIdx, BufIdx))
        {
            if (ETH_MODE_ACTIVE == Eth_Ipr_GetControllerMode(CtrlIdx))
            {
#endif /*!< (STD_ON == ETH_DEV_ERROR_DETECT) */
                Status_en = Eth_Ipr_Transmit(CtrlIdx, BufIdx, FrameType, TxConfirmation, LenByte, PhysAddrPtr);
#if (STD_ON == ETH_DEV_ERROR_DETECT)
            }
            else
            {
                ErrorId_u8 = ETH_E_INV_MODE;
            }
        }
        else
        {
            ErrorId_u8 = ETH_E_INV_PARAM;
        }
    }
    else
    {
        /* Fulfill the remaining conditions, do nothing. */
    }

    if (ETH_MAX_ERROR_VALUE != ErrorId_u8)
    {
        (void)Det_ReportError((uint16_t)ETH_MODULE_ID, ETH_INSTANCE_ID, ETH_SID_TRANSMIT,
                              ErrorId_u8);
    }
#endif /*!< (STD_ON == ETH_DEV_ERROR_DETECT) */

    return Status_en;
}

/**
 *  @brief          Receive a frame from the related fifo.
 *
 *  @details        Service ID:        0xB
 *                  SRS ID:            SWS_Eth_00095,SWS_Eth_00096,SWS_Eth_00097,
 *                                     SWS_Eth_00098,SWS_Eth_00132,SWS_Eth_00153,
 *                                     SWS_Eth_00003,SWS_Eth_00008,SWS_Eth_00256,
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
)
{
#if (STD_OFF == ETH_RECEIVED_IRQ_ENABLED)
    Eth_FrameType EthType_en;
    const uint8_t *MacAddr_ptr;
    uint8_t *Buffer_ptr;
    uint16_t Length_u16;
    boolean IsBroadcast_bool;
#endif /*!< (STD_OFF == ETH_RECEIVED_IRQ_ENABLED) */
#if (STD_ON == ETH_DEV_ERROR_DETECT)
    if (NULL_PTR == RxStatusPtr)
    {
        (void)Det_ReportError((uint16_t)ETH_MODULE_ID, ETH_INSTANCE_ID, ETH_SID_RECEIVE,
                              ETH_E_PARAM_POINTER);
    }
    else if (TRUE == Eth_LocalCheckCommonDet(CtrlIdx, ETH_SID_RECEIVE))
    {
        if (ETH_MODE_ACTIVE == Eth_Ipr_GetControllerMode(CtrlIdx))
        {
#endif /*!< (STD_ON == ETH_DEV_ERROR_DETECT) */
            /* If Rx interrupt enabled then polling mode will empty. */
#if (STD_OFF == ETH_RECEIVED_IRQ_ENABLED)
            Eth_Ipr_Receive(CtrlIdx, FifoIdx, RxStatusPtr, &Buffer_ptr, &Length_u16);
            if (ETH_NOT_RECEIVED != *RxStatusPtr)
            {
                /* Retrieve the Ethernet type. */
                EthType_en  = Buffer_ptr[ETH_FRAME_ETHERNET_TYPE_OFFSET + 1U];
                EthType_en |= ((uint16_t)Buffer_ptr[ETH_FRAME_ETHERNET_TYPE_OFFSET] << 8U);
                /* Check MAC Destination Address type */
                MacAddr_ptr      = &Buffer_ptr[ETH_FRAME_MAC_DESTINATION_OFFSET_0];
                IsBroadcast_bool = TRUE;
                for (uint8_t i = 0; i < ETH_FRAME_MAC_BYTES; i++)
                {
                    if (MacAddr_ptr[i] != 0xFFU)
                    {
                        IsBroadcast_bool = FALSE;
                        break;
                    }
                }
                /* Retrieve the source MAC address. */
                MacAddr_ptr = &Buffer_ptr[ETH_FRAME_MAC_SOURCE_OFFSET];
                /* Retrieve the payload and invoke the callback function. */
                EthIf_RxIndication(CtrlIdx, EthType_en, IsBroadcast_bool, MacAddr_ptr,
                                   (Eth_DataType *)&Buffer_ptr[ETH_FRAME_ETHERNET_PAYLOAD_OFFSET],
                                   Length_u16);
                /* Release the received buffer. */
                Eth_Ipr_ReleaseRxBuffer(CtrlIdx, FifoIdx);
            }
#else
            *RxStatusPtr = ETH_NOT_RECEIVED;
            (void)CtrlIdx;
            (void)FifoIdx;
#endif  /*!< (STD_OFF == ETH_RECEIVED_IRQ_ENABLED) */
#if (STD_ON == ETH_DEV_ERROR_DETECT)
        }
        else
        {
            (void)Det_ReportError((uint16_t)ETH_MODULE_ID, ETH_INSTANCE_ID, ETH_SID_RECEIVE,
                                  ETH_E_INV_MODE);
        }
    }
    else
    {
        /* Fulfill the remaining conditions, do nothing. */
    }
#endif /*!< (STD_ON == ETH_DEV_ERROR_DETECT) */
}

/**
 *  @brief          Triggers frame transmission confirmation.
 *
 *  @details        Service ID:        0xC
 *                  SRS ID:            SWS_Eth_00100,SWS_Eth_00101,SWS_Eth_00102,
 *                                     SWS_Eth_00103,SWS_Eth_00104,SWS_Eth_00134,
 *                                     SWS_Eth_00003,SWS_Eth_00008,SWS_Eth_00105,
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
)
{
#if (STD_OFF == ETH_TRANSMIT_IRQ_ENABLED)
    uint8_t FifoNum_u8   = 0x00U;
    uint32_t BuffNum_u32 = 0x00U;
    Eth_Ipr_TxBufferInfoType *TxBufferInfo_ptr;
#endif  /*!< (STD_OFF == ETH_TRANSMIT_IRQ_ENABLED) */
#if (STD_ON == ETH_DEV_ERROR_DETECT)
    if (TRUE == Eth_LocalCheckCommonDet(CtrlIdx, ETH_SID_TXCONFIRMATION))
    {
        if (ETH_MODE_ACTIVE == Eth_Ipr_GetControllerMode(CtrlIdx))
        {
#endif /*!< (STD_ON == ETH_DEV_ERROR_DETECT) */
            /* If Tx interrupt enabled then polling mode will empty. */
#if (STD_OFF == ETH_TRANSMIT_IRQ_ENABLED)
            FifoNum_u8 = Eth_Ipr_GetTxChNum(CtrlIdx);
            for (uint8_t FifoIdx_u8 = 0U; FifoIdx_u8 < FifoNum_u8; FifoIdx_u8++)
            {
                BuffNum_u32 = Eth_Ipr_ChGetTxNum(CtrlIdx, FifoIdx_u8);
                for (uint8_t BuffIdx_u8 = 0U; BuffIdx_u8 < BuffNum_u32; BuffIdx_u8++)
                {
                    Eth_Ipr_ChGetTxBufferInfo(CtrlIdx, FifoIdx_u8, &TxBufferInfo_ptr);
                    if (NULL_PTR == TxBufferInfo_ptr)
                    {
                        break;
                    }
                    if (TRUE == TxBufferInfo_ptr->TxConfirmation_bool)
                    {
                        EthIf_TxConfirmation(CtrlIdx, TxBufferInfo_ptr->BufferIdx_en, TxBufferInfo_ptr->TxResult_en);
                    }
                    Eth_Ipr_ChReleaseTxBuffer(CtrlIdx, FifoIdx_u8);
                }
            }
#else
            (void)CtrlIdx;
#endif  /*!< (STD_OFF == ETH_TRANSMIT_IRQ_ENABLED) */
#if (STD_ON == ETH_DEV_ERROR_DETECT)
        }
        else
        {
            (void)Det_ReportError((uint16_t)ETH_MODULE_ID, ETH_INSTANCE_ID, ETH_SID_TXCONFIRMATION,
                                  ETH_E_INV_MODE);
        }
    }
#endif /*!< (STD_ON == ETH_DEV_ERROR_DETECT) */
}

#if (STD_ON == ETH_GET_VERSION_INFO_API)
/**
 *  @brief          Returns the version information of this module.
 *
 *  @details        Service ID:        0xD
 *                  SRS ID:            SWS_Eth_00106,SWS_Eth_00136,SWS_Eth_00008
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
)
{
#if (STD_ON == ETH_DEV_ERROR_DETECT)
    if (NULL_PTR == VersionInfoPtr)
    {
        (void)Det_ReportError((uint16_t)ETH_MODULE_ID, ETH_INSTANCE_ID, ETH_SID_GETVERSIONINFO,
                              ETH_E_PARAM_POINTER);
    }
    else
    {
#endif /*!< (STD_ON == ETH_DEV_ERROR_DETECT) */
        VersionInfoPtr->vendorID         = (uint16_t)ETH_VENDOR_ID;
        VersionInfoPtr->moduleID         = (uint16_t)ETH_MODULE_ID;
        VersionInfoPtr->sw_major_version = (uint8_t)ETH_SW_MAJOR_VERSION;
        VersionInfoPtr->sw_minor_version = (uint8_t)ETH_SW_MINOR_VERSION;
        VersionInfoPtr->sw_patch_version = (uint8_t)ETH_SW_PATCH_VERSION;
#if (STD_ON == ETH_DEV_ERROR_DETECT)
    }
#endif /*!< (STD_ON == ETH_DEV_ERROR_DETECT) */
}
#endif /*!< (STD_ON == ETH_GET_VERSION_INFO_API) */

/**
 *  @brief          The function checks for controller errors and lost frames. Used for polling state changes. Calls
 *                  EthIf_CtrlModeIndication when the controller mode changed.
 *
 *  @details        Service ID:        0x20
 *                  SRS ID:            SWS_Eth_00169,SWS_Eth_00171,SWS_Eth_00172,
 *                                     SWS_Eth_00008,SWS_Eth_00173,SWS_Eth_00219,
 *                                     SWS_Eth_00222,SWS_Eth_00240,
 *                  Design ID:         SDD_ETH_22
 *                  Sync/Async:        None
 *                  Reentrancy:        None
 *
 *  @return         void
 *
 *  @note           Calls EthIf_CtrlModeIndication when the controller mode changed.
 *
 *  @pre            None
 *
 */
void Eth_MainFunction(
    void
)
{
    uint8_t CtrlIdx_u8;
    Eth_ModeType CurrentMode_en;
    uint32_t CoreId_u32 = Eth_GetCoreID();
#if (STD_ON == ETH_DEM_EVENT_ENABLE)
    const Eth_CtrlConfigType *CtrConfig_ptr;
#endif /*!< (STD_ON == ETH_DEM_EVENT_ENABLE) */

#if (STD_ON == ETH_MULTICORE_ENABLED)
    if ((CoreId_u32 < ETH_MAX_PARTITIONS) && (NULL_PTR != s_EthConfig_aa[CoreId_u32]))
#else
    if (NULL_PTR != s_EthConfig_aa[CoreId_u32])
#endif /*!< (STD_ON == ETH_MULTICORE_ENABLED) */
    {
        for (CtrlIdx_u8 = 0U; CtrlIdx_u8 < ETH_MAX_CONTROLLER_COUNT; CtrlIdx_u8++)
        {
            if (NULL_PTR != s_EthConfig_aa[CoreId_u32]->EthCtrConfigList_pp[CtrlIdx_u8])
            {
                /* Check the current controller mode */
                CurrentMode_en = Eth_Ipr_GetControllerMode(CtrlIdx_u8);
                if (CurrentMode_en != s_EthCtrlMode_aa[CtrlIdx_u8])
                {
                    s_EthCtrlMode_aa[CtrlIdx_u8] = CurrentMode_en;
                    /* Provide interface to indicate the controller mode */
                    EthIf_CtrlModeIndication(CtrlIdx_u8, CurrentMode_en);
                }
#if (STD_ON == ETH_DEM_EVENT_ENABLE)
                CtrConfig_ptr = s_EthConfig_aa[CoreId_u32]->EthCtrConfigList_pp[CtrlIdx_u8];
                if (TRUE == Eth_Ipr_CheckAccessToController(CtrlIdx_u8))
                {
                    if (TRUE == CtrConfig_ptr->EthDemError_aa[ETH_E_ACCESS].Config_bool)
                    {
                        (void)Dem_SetEventStatus((Dem_EventIdType)CtrConfig_ptr->EthDemError_aa[ETH_E_ACCESS].ErrorId_u32,
                                                DEM_EVENT_STATUS_PREPASSED);
                    }
                    Eth_LocalCheckDems(CtrlIdx_u8, &CtrConfig_ptr->EthDemError_aa[0U]);
                }
                else
                {
                    if (TRUE == CtrConfig_ptr->EthDemError_aa[ETH_E_ACCESS].Config_bool)
                    {
                        (void)Dem_SetEventStatus((Dem_EventIdType)CtrConfig_ptr->EthDemError_aa[ETH_E_ACCESS].ErrorId_u32,
                                                DEM_EVENT_STATUS_PREFAILED);
                    }
                }
#endif /*!< (STD_ON == ETH_DEM_EVENT_ENABLE) */
            }
        }
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
