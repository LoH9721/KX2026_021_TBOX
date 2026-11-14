/***********************************************************************************************************************
 * Project Name: XHSC AUTOSAR MCAL
 * File Name: Det.c
 *
 * Description: Implementation of DET High Level layer
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
 *   @file       Det.c
 *   @version    1.0.3
 *   @brief      Det High Layer Source File
 *   @addtogroup det_driver
 *   @{
 */

#ifdef __cplusplus
extern "C"{
#endif

/***********************************************************************************************************************
 *  INCLUDES
 **********************************************************************************************************************/
#include "StandardTypes.h"
#include "Det.h"
#include "Common.h"

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
/**
 * @brief   Det_DetErrBuffer   Det Error Buffer
 */
Det_ErrorType Det_DetErrBuffer[DET_MAX_BUFFER];

/**
 * @brief   Det_DetBuffIndex  Det Buffer Index
 */
uint32_t Det_DetBuffIndex = 0;

/***********************************************************************************************************************
 *  LOCAL FUNCTION PROTOTYPES
 **********************************************************************************************************************/

/***********************************************************************************************************************
 *  LOCAL FUNCTION
 **********************************************************************************************************************/

/***********************************************************************************************************************
 *  GLOBAL FUNCTION
 **********************************************************************************************************************/
/**
 *   @brief      Service to initialize the Default Error Tracer.
 *   @details    Service ID: 0x00
 *               Sync/Async: Synchronous
 *               Reentrancy: Non Reentrant
 *
 *   @param[in]  ConfigPtr        Pointer to the selected configuration set
 *
 *   @return     None
 */

void Det_Init(
    const Det_ConfigType* ConfigPtr
)
{
    (void)ConfigPtr;
}

/**
 *   @brief      Service to report development errors.
 *   @details    Service ID: 0x01
 *               Sync/Async: Not Applicable: The function never returns
 *               Reentrancy: Reentrant
 *
 *   @param[in]  ModuleId         Module ID of calling module.
 *   @param[in]  InstanceId       The identifier of the index based instance of a module, starting
 *                                from 0, If the module is a single instance module it shall pass 0 as
 *                                the InstanceId.
 *   @param[in]  ApiId            ID of API service in which error is detected(defined in SWS of calling module)
 *   @param[in]  ErrorId          ID of detected development error (defined in SWS of calling module).
 *
 *   @return     Std_ReturnType   Never returns a value, but has a return type for compatibility with
 *                                services and hooks
 *   @retval     E_OK             The requested job has been accepted by the module.
 *   @retval     E_NOT_OK         The requested job has not been accepted by the module.
 */

Std_ReturnType Det_ReportError(
    uint16_t ModuleId,
    uint8_t InstanceId,
    uint8_t ApiId,
    uint8_t ErrorId
)
{
    /* DET error is reported */
    Det_ErrorType *DetBuffer_ptr = &Det_DetErrBuffer[Det_DetBuffIndex % DET_MAX_BUFFER];
    DetBuffer_ptr->ModuleId = ModuleId;
    DetBuffer_ptr->InstanceId = InstanceId;
    DetBuffer_ptr->ApiId = ApiId;
    DetBuffer_ptr->ErrorId = ErrorId;
    Det_DetBuffIndex++;
    return(E_NOT_OK);
}

/**
 *  @brief          Service to report runtime errors. If a callout has been configured then this callout
 *                  shall be called.
 *
 *  @details        Service ID:        0x04
 *                  Sync/Async:        Synchronous
 *                  Reentrancy:        Reentrant
 *
 *  @param[in]      ModuleId      Module ID of calling module.
 *  @param[in]      InstanceId    The identifier of the index based instance of a module, starting
 *                                from 0, If the module is a single instance module it shall pass 0 as
 *                                the InstanceId.
 *  @param[in]      ApiId         ID of API service in which error is detected(defined in SWS of calling module)
 *  @param[in]      ErrorId       ID of detected development error (defined in SWS of calling module).
 *
 *  @return         Std_ReturnType    returns always E_OK (is required for services)
 *  @retval         E_OK              The requested job has been accepted by the module.
 *  @retval         E_NOT_OK          The requested job has not been accepted by the module.
 *
 *  @note           None
 *
 *  @pre            None
 *
 **/
Std_ReturnType Det_ReportRuntimeError(
    uint16_t ModuleId,
    uint8_t InstanceId,
    uint8_t ApiId,
    uint8_t ErrorId
)
{
    /* DET error is reported */
    Det_ErrorType *DetBuffer_ptr = &Det_DetErrBuffer[Det_DetBuffIndex % DET_MAX_BUFFER];
    DetBuffer_ptr->ModuleId = ModuleId;
    DetBuffer_ptr->InstanceId = InstanceId;
    DetBuffer_ptr->ApiId = ApiId;
    DetBuffer_ptr->ErrorId = ErrorId;
    Det_DetBuffIndex++;
    return(E_NOT_OK);
}

/**
 *  @brief          Service to report transient faults. If a callout has been configured than this callout
 *                  shall be called and the returned value of the callout shall be returned. Otherwise it
 *                  returns immediately with E_OK.
 *
 *  @details        Service ID:        0x05
 *                  Sync/Async:        Synchronous
 *                  Reentrancy:        Reentrant
 *
 *  @param[in]      ModuleId      Module ID of calling module.
 *  @param[in]      InstanceId    The identifier of the index based instance of a module, starting
 *                                from 0, If the module is a single instance module it shall pass 0 as
 *                                the InstanceId.
 *  @param[in]      ApiId         ID of API service in which error is detected(defined in SWS of calling module)
 *  @param[in]      FaultId       ID of detected transient fault (defined in SWS of calling module).
 *
 *  @return         Std_ReturnType    If no callout exists it shall return E_OK, otherwise it shall return
 *                                                                                        the value of the configured callout. In case several callouts are                                 configured the logical or (sum) of the callout return values shall                                 be returned. Rationale: since E_OK=0, E_OK will be only                                 returned if all are E_OK, and for multiple error codes there is a                                 good chance to detect several of them
 *  @retval         E_OK              The requested job has been accepted by the module.
 *  @retval         E_NOT_OK          The requested job has not been accepted by the module.
 *
 *  @note           None
 *
 *  @pre            None
 *
 **/
Std_ReturnType Det_ReportTransientFault(
    uint16_t ModuleId,
    uint8_t InstanceId,
    uint8_t ApiId,
    uint8_t FaultId
)
{
    /* DET error is reported */
    Det_ErrorType *DetBuffer_ptr = &Det_DetErrBuffer[Det_DetBuffIndex % DET_MAX_BUFFER];
    DetBuffer_ptr->ModuleId = ModuleId;
    DetBuffer_ptr->InstanceId = InstanceId;
    DetBuffer_ptr->ApiId = ApiId;
    DetBuffer_ptr->ErrorId = FaultId;
    Det_DetBuffIndex++;
    return(E_NOT_OK);
}

#ifdef __cplusplus
}
#endif

/**
 * @}
 */
