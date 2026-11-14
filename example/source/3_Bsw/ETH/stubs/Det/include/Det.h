/***********************************************************************************************************************
 * Project Name: XHSC AUTOSAR MCAL
 * File Name: Det.h
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
 *   @file       Det.h
 *   @version    1.0.3
 *   @brief      Det High Layer Header File
 *   @addtogroup det_driver
 *   @{
 */

#ifndef DET_H
#define DET_H

#ifdef __cplusplus
extern "C"{
#endif

/***********************************************************************************************************************
 *  INCLUDES
 **********************************************************************************************************************/
#include "StandardTypes.h"

/***********************************************************************************************************************
 *  DEFINES & MACROS
 **********************************************************************************************************************/
#define DET_MODULE_ID                                       15
#define DET_VENDOR_ID                                       168
#define DET_AR_RELEASE_MAJOR_VERSION                        4
#define DET_AR_RELEASE_MINOR_VERSION                        4
#define DET_AR_RELEASE_REVISION_VERSION                     0
#define DET_SW_MAJOR_VERSION                                1
#define DET_SW_MINOR_VERSION                                0
#define DET_SW_REVISION_VERSION                             3

/**
 *  @brief   Size of Det Buffer.
 **/
#define DET_MAX_BUFFER                   100

/***********************************************************************************************************************
 *  TYPEDEFS
 **********************************************************************************************************************/
/**
 *  @brief   Defining the structure to store the parameters of Det Report Error function.
 **/
typedef struct
{
    uint16_t ModuleId;            /*!< It will store the ModuleId of the reporting module. */
    uint8_t InstanceId;           /*!< It will store the index based InstanceId of the reporting module. */
    uint8_t ApiId;                /*!< It will store the ApiId of the reporting function. */
    uint8_t ErrorId;              /*!< It will store the ErrorId of the reporting error. */
} Det_ErrorType;

typedef uint32_t Det_ConfigType;

/***********************************************************************************************************************
 *  CONSTANTS
 **********************************************************************************************************************/


/***********************************************************************************************************************
 *  GLOBAL VARIABLES
 **********************************************************************************************************************/
/**
 * @brief   Det_DetErrBuffer
 */
extern Det_ErrorType Det_DetErrBuffer[DET_MAX_BUFFER];

/**
 * @brief   Det_DetBuffIndex
 */
extern uint32_t Det_DetBuffIndex;


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
);

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
);

/**
 *   @brief      Service to report runtime errors. If a callout has been configured then this callout
 *               shall be called.
 *   @details    Service ID: 0x04
 *               Sync/Async: Synchronous
 *               Reentrancy: Reentrant
 *
 *   @param[in]  ModuleId         Module ID of calling module.
 *   @param[in]  InstanceId       The identifier of the index based instance of a module, starting
 *                                from 0, If the module is a single instance module it shall pass 0 as
 *                                the InstanceId.
 *   @param[in]  ApiId            ID of API service in which error is detected(defined in SWS of calling module)
 *   @param[in]  ErrorId          ID of detected development error (defined in SWS of calling module).
 *
 *   @return     Std_ReturnType   returns always E_OK (is required for services)
 *
 *   @retval     E_OK             The requested job has been accepted by the module.
 *   @retval     E_NOT_OK         The requested job has not been accepted by the module.
 */

Std_ReturnType Det_ReportRuntimeError(
    uint16_t ModuleId,
    uint8_t InstanceId,
    uint8_t ApiId,
    uint8_t ErrorId
);

/**
 *  @brief          Service to report transient faults. If a callout has been configured than this callout
 *                  shall be called and the returned value of the callout shall be returned. Otherwise it
 *                  returns immediately with E_OK.
 *
 *  @details        Service ID:        0x05
 *                  SRS ID:            None
 *                  Design ID:         Det_ReportTransientFault_Activity
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
);


#ifdef __cplusplus
}
#endif

/**
 * @}
 */

#endif /* DET_H */
