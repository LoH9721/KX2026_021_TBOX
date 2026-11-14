/***********************************************************************************************************************
 * Project Name: XHSC AUTOSAR MCAL
 * File Name: Terminal.h
 *
 * Description: Implementation of SYSTEM
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

#ifndef TERMINAL_H
#define TERMINAL_H
/**
 *   @file       Terminal.h
 *   @version    1.0.2
 *   @brief      System Driver
 *   @addtogroup system_driver
 *   @{
 */

#ifdef __cplusplus
extern "C"{
#endif


/***********************************************************************************************************************
 *  INCLUDES
 **********************************************************************************************************************/
#include "Std_Types.h"
  
/***********************************************************************************************************************
 *  DEFINES & MACROS
 **********************************************************************************************************************/

/***********************************************************************************************************************
 *  TYPEDEFS
 **********************************************************************************************************************/

/***********************************************************************************************************************
 *  GLOBAL VARIABLES
 **********************************************************************************************************************/

/***********************************************************************************************************************
 *  GLOBAL CONSTANTS
 **********************************************************************************************************************/

/***********************************************************************************************************************
 *  GLOBAL FUNCTION
 **********************************************************************************************************************/
/**
 *  @brief          ADL input function
 *
 *  @param[in]      * para_Buffer_ptr    Pointer for input value store
 *  @param[in]      para_Size_u32       Size for expected input value
 *  @param[in]      para_Timeout_u32    Timeout for input monitor
 *
 *  @return         uint16_t    Returns the number of char written on the stdout
 *
 *  @note           None
 *
 *  @pre            None
 *
 */
uint32_t Terminal_Input(
    uint8_t* para_Buffer_ptr, 
    uint32_t para_Size_u32, 
    uint32_t para_Timeout_u32
);

/**
 *  @brief          Adl output a buffer with a size value
 *
 *  @return         uint32_t   how many byte had be sent out
 *
 *  @note           None
 *
 *  @pre            ADL_PrintIpInit() should be call first
 *
 */
uint32_t Terminal_PutBuffer(
    uint8_t * para_Buffer_ptr, 
    uint32_t para_Size_u32
);

/**
 *  @brief          ADL ADL_Printf function
 *
 *  @param[in]      para_Format_ptr   print format
 *
 *  @return         void
 *
 *  @note           Only support %d,%s,%c,%x,%X
 *
 *  @pre            None
 *
 */
void Terminal_Printf(
    const char * para_Format_ptr, 
    ...
);
#ifdef __cplusplus
}
#endif

#endif /*!< TERMINAL_H */
/**
 * @}
 */
