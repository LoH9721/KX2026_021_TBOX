/***********************************************************************************************************************
 * Project Name: XHSC AUTOSAR MCAL
 * File Name: SchM_Eth.c
 *
 * Description: Implementation of SchM_Eth
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
 *
 **********************************************************************************************************************/

/**
 *   @file       SchM_Eth.c
 *   @version    1.0.3
 *   @brief      Exclusive area driver for Eth
 *   @addtogroup rte_driver
 *   @{
 */

#ifdef __cplusplus
extern "C"{
#endif

/***********************************************************************************************************************
 *  INCLUDES
 **********************************************************************************************************************/
#include "Std_Types.h"
#include "SchM_Eth.h"
#include "Common.h"
#include "Osr_Internal.h"

/***********************************************************************************************************************
 *  DEFINES & MACROS
 **********************************************************************************************************************/
#define INTERRUPT_STATUS_MASK     ((uint32_t)0x00000001UL)

#define INTERRUPT_IS_ENABLED(interrupt_status)            ((uint32_t)0 == ((uint32_t)(interrupt_status) & (uint32_t)(INTERRUPT_STATUS_MASK)))

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
#define RTE_START_SEC_VAR_CLEARED_32
#include "Rte_MemMap.h"
static volatile uint32_t ExclusiveAreaNestedCounter_ETH_EXCLUSIVE_AREA_00;
static volatile uint32_t InterruptNestedStatus_ETH_EXCLUSIVE_AREA_00;
static volatile uint32_t ExclusiveAreaNestedCounter_ETH_EXCLUSIVE_AREA_01;
static volatile uint32_t InterruptNestedStatus_ETH_EXCLUSIVE_AREA_01;
static volatile uint32_t ExclusiveAreaNestedCounter_ETH_EXCLUSIVE_AREA_02;
static volatile uint32_t InterruptNestedStatus_ETH_EXCLUSIVE_AREA_02;
static volatile uint32_t ExclusiveAreaNestedCounter_ETH_EXCLUSIVE_AREA_03;
static volatile uint32_t InterruptNestedStatus_ETH_EXCLUSIVE_AREA_03;
static volatile uint32_t ExclusiveAreaNestedCounter_ETH_EXCLUSIVE_AREA_04;
static volatile uint32_t InterruptNestedStatus_ETH_EXCLUSIVE_AREA_04;
static volatile uint32_t ExclusiveAreaNestedCounter_ETH_EXCLUSIVE_AREA_05;
static volatile uint32_t InterruptNestedStatus_ETH_EXCLUSIVE_AREA_05;
static volatile uint32_t ExclusiveAreaNestedCounter_ETH_EXCLUSIVE_AREA_06;
static volatile uint32_t InterruptNestedStatus_ETH_EXCLUSIVE_AREA_06;
static volatile uint32_t ExclusiveAreaNestedCounter_ETH_EXCLUSIVE_AREA_07;
static volatile uint32_t InterruptNestedStatus_ETH_EXCLUSIVE_AREA_07;
static volatile uint32_t ExclusiveAreaNestedCounter_ETH_EXCLUSIVE_AREA_08;
static volatile uint32_t InterruptNestedStatus_ETH_EXCLUSIVE_AREA_08;
static volatile uint32_t ExclusiveAreaNestedCounter_ETH_EXCLUSIVE_AREA_09;
static volatile uint32_t InterruptNestedStatus_ETH_EXCLUSIVE_AREA_09;
#define RTE_STOP_SEC_VAR_CLEARED_32
#include "Rte_MemMap.h"

/***********************************************************************************************************************
 *  GLOBAL VARIABLES
 **********************************************************************************************************************/

/***********************************************************************************************************************
 *  LOCAL FUNCTION PROTOTYPES
 **********************************************************************************************************************/
static uint32_t Eth_Schm_Read_Msr(void);

/***********************************************************************************************************************
 *  LOCAL FUNCTION
 **********************************************************************************************************************/
static uint32_t Eth_Schm_Read_Msr(void)
{
    register uint32_t RegVal_u32 = 0U;

    ASM_KEYWORD volatile( " mrs %0, primask " : "=r" (RegVal_u32) );

    return (uint32_t)RegVal_u32;
}

/***********************************************************************************************************************
 *  GLOBAL FUNCTION
 **********************************************************************************************************************/
#define RTE_START_SEC_CODE
#include "Rte_MemMap.h"

/**
 *  Exclusive Area
 */
void SchM_Enter_Eth_ETH_EXCLUSIVE_AREA_00(void)
{
    uint32_t InterruptStatus_u32 = 0U;

    if(0UL == ExclusiveAreaNestedCounter_ETH_EXCLUSIVE_AREA_00)
    {
        InterruptStatus_u32 = Eth_Schm_Read_Msr();
        /* If the interrupt has been enabled, the interrupt will be disabled */
        if(INTERRUPT_IS_ENABLED(InterruptStatus_u32))
        {
            Osr_SuspendAllInterrupts();
        }
        InterruptNestedStatus_ETH_EXCLUSIVE_AREA_00 = InterruptStatus_u32;
    }
    ExclusiveAreaNestedCounter_ETH_EXCLUSIVE_AREA_00++;
}

void SchM_Exit_Eth_ETH_EXCLUSIVE_AREA_00(void)
{
    ExclusiveAreaNestedCounter_ETH_EXCLUSIVE_AREA_00--;
    /* If the interrupt has been enabled before disabling it when entering the exclusive area, the interrupt will be enabled */
    if((INTERRUPT_IS_ENABLED(InterruptNestedStatus_ETH_EXCLUSIVE_AREA_00)) && (0UL == ExclusiveAreaNestedCounter_ETH_EXCLUSIVE_AREA_00))
    {
        Osr_ResumeAllInterrupts();
    }
}

void SchM_Enter_Eth_ETH_EXCLUSIVE_AREA_01(void)
{
    uint32_t InterruptStatus_u32 = 0U;

    if(0UL == ExclusiveAreaNestedCounter_ETH_EXCLUSIVE_AREA_01)
    {
        InterruptStatus_u32 = Eth_Schm_Read_Msr();
        /* If the interrupt has been enabled, the interrupt will be disabled */
        if(INTERRUPT_IS_ENABLED(InterruptStatus_u32))
        {
            Osr_SuspendAllInterrupts();
        }
        InterruptNestedStatus_ETH_EXCLUSIVE_AREA_01 = InterruptStatus_u32;
    }
    ExclusiveAreaNestedCounter_ETH_EXCLUSIVE_AREA_01++;
}

void SchM_Exit_Eth_ETH_EXCLUSIVE_AREA_01(void)
{
    ExclusiveAreaNestedCounter_ETH_EXCLUSIVE_AREA_01--;
    /* If the interrupt has been enabled before disabling it when entering the exclusive area, the interrupt will be enabled */
    if((INTERRUPT_IS_ENABLED(InterruptNestedStatus_ETH_EXCLUSIVE_AREA_01)) && (0UL == ExclusiveAreaNestedCounter_ETH_EXCLUSIVE_AREA_01))
    {
        Osr_ResumeAllInterrupts();
    }
}

void SchM_Enter_Eth_ETH_EXCLUSIVE_AREA_02(void)
{
    uint32_t InterruptStatus_u32 = 0U;

    if(0UL == ExclusiveAreaNestedCounter_ETH_EXCLUSIVE_AREA_02)
    {
        InterruptStatus_u32 = Eth_Schm_Read_Msr();
        /* If the interrupt has been enabled, the interrupt will be disabled */
        if(INTERRUPT_IS_ENABLED(InterruptStatus_u32))
        {
            Osr_SuspendAllInterrupts();
        }
        InterruptNestedStatus_ETH_EXCLUSIVE_AREA_02 = InterruptStatus_u32;
    }
    ExclusiveAreaNestedCounter_ETH_EXCLUSIVE_AREA_02++;
}

void SchM_Exit_Eth_ETH_EXCLUSIVE_AREA_02(void)
{
    ExclusiveAreaNestedCounter_ETH_EXCLUSIVE_AREA_02--;
    /* If the interrupt has been enabled before disabling it when entering the exclusive area, the interrupt will be enabled */
    if((INTERRUPT_IS_ENABLED(InterruptNestedStatus_ETH_EXCLUSIVE_AREA_02)) && (0UL == ExclusiveAreaNestedCounter_ETH_EXCLUSIVE_AREA_02))
    {
        Osr_ResumeAllInterrupts();
    }
}

void SchM_Enter_Eth_ETH_EXCLUSIVE_AREA_03(void)
{
    uint32_t InterruptStatus_u32 = 0U;

    if(0UL == ExclusiveAreaNestedCounter_ETH_EXCLUSIVE_AREA_03)
    {
        InterruptStatus_u32 = Eth_Schm_Read_Msr();
        /* If the interrupt has been enabled, the interrupt will be disabled */
        if(INTERRUPT_IS_ENABLED(InterruptStatus_u32))
        {
            Osr_SuspendAllInterrupts();
        }
        InterruptNestedStatus_ETH_EXCLUSIVE_AREA_03 = InterruptStatus_u32;
    }
    ExclusiveAreaNestedCounter_ETH_EXCLUSIVE_AREA_03++;
}

void SchM_Exit_Eth_ETH_EXCLUSIVE_AREA_03(void)
{
    ExclusiveAreaNestedCounter_ETH_EXCLUSIVE_AREA_03--;
    /* If the interrupt has been enabled before disabling it when entering the exclusive area, the interrupt will be enabled */
    if((INTERRUPT_IS_ENABLED(InterruptNestedStatus_ETH_EXCLUSIVE_AREA_03)) && (0UL == ExclusiveAreaNestedCounter_ETH_EXCLUSIVE_AREA_03))
    {
        Osr_ResumeAllInterrupts();
    }
}

void SchM_Enter_Eth_ETH_EXCLUSIVE_AREA_04(void)
{
    uint32_t InterruptStatus_u32 = 0U;

    if(0UL == ExclusiveAreaNestedCounter_ETH_EXCLUSIVE_AREA_04)
    {
        InterruptStatus_u32 = Eth_Schm_Read_Msr();
        /* If the interrupt has been enabled, the interrupt will be disabled */
        if(INTERRUPT_IS_ENABLED(InterruptStatus_u32))
        {
            Osr_SuspendAllInterrupts();
        }
        InterruptNestedStatus_ETH_EXCLUSIVE_AREA_04 = InterruptStatus_u32;
    }
    ExclusiveAreaNestedCounter_ETH_EXCLUSIVE_AREA_04++;
}

void SchM_Exit_Eth_ETH_EXCLUSIVE_AREA_04(void)
{
    ExclusiveAreaNestedCounter_ETH_EXCLUSIVE_AREA_04--;
    /* If the interrupt has been enabled before disabling it when entering the exclusive area, the interrupt will be enabled */
    if((INTERRUPT_IS_ENABLED(InterruptNestedStatus_ETH_EXCLUSIVE_AREA_04)) && (0UL == ExclusiveAreaNestedCounter_ETH_EXCLUSIVE_AREA_04))
    {
        Osr_ResumeAllInterrupts();
    }
}

void SchM_Enter_Eth_ETH_EXCLUSIVE_AREA_05(void)
{
    uint32_t InterruptStatus_u32 = 0U;

    if(0UL == ExclusiveAreaNestedCounter_ETH_EXCLUSIVE_AREA_05)
    {
        InterruptStatus_u32 = Eth_Schm_Read_Msr();
        /* If the interrupt has been enabled, the interrupt will be disabled */
        if(INTERRUPT_IS_ENABLED(InterruptStatus_u32))
        {
            Osr_SuspendAllInterrupts();
        }
        InterruptNestedStatus_ETH_EXCLUSIVE_AREA_05 = InterruptStatus_u32;
    }
    ExclusiveAreaNestedCounter_ETH_EXCLUSIVE_AREA_05++;
}

void SchM_Exit_Eth_ETH_EXCLUSIVE_AREA_05(void)
{
    ExclusiveAreaNestedCounter_ETH_EXCLUSIVE_AREA_05--;
    /* If the interrupt has been enabled before disabling it when entering the exclusive area, the interrupt will be enabled */
    if((INTERRUPT_IS_ENABLED(InterruptNestedStatus_ETH_EXCLUSIVE_AREA_05)) && (0UL == ExclusiveAreaNestedCounter_ETH_EXCLUSIVE_AREA_05))
    {
        Osr_ResumeAllInterrupts();
    }
}

void SchM_Enter_Eth_ETH_EXCLUSIVE_AREA_06(void)
{
    uint32_t InterruptStatus_u32 = 0U;

    if(0UL == ExclusiveAreaNestedCounter_ETH_EXCLUSIVE_AREA_06)
    {
        InterruptStatus_u32 = Eth_Schm_Read_Msr();
        /* If the interrupt has been enabled, the interrupt will be disabled */
        if(INTERRUPT_IS_ENABLED(InterruptStatus_u32))
        {
            Osr_SuspendAllInterrupts();
        }
        InterruptNestedStatus_ETH_EXCLUSIVE_AREA_06 = InterruptStatus_u32;
    }
    ExclusiveAreaNestedCounter_ETH_EXCLUSIVE_AREA_06++;
}

void SchM_Exit_Eth_ETH_EXCLUSIVE_AREA_06(void)
{
    ExclusiveAreaNestedCounter_ETH_EXCLUSIVE_AREA_06--;
    /* If the interrupt has been enabled before disabling it when entering the exclusive area, the interrupt will be enabled */
    if((INTERRUPT_IS_ENABLED(InterruptNestedStatus_ETH_EXCLUSIVE_AREA_06)) && (0UL == ExclusiveAreaNestedCounter_ETH_EXCLUSIVE_AREA_06))
    {
        Osr_ResumeAllInterrupts();
    }
}

void SchM_Enter_Eth_ETH_EXCLUSIVE_AREA_07(void)
{
    uint32_t InterruptStatus_u32 = 0U;

    if(0UL == ExclusiveAreaNestedCounter_ETH_EXCLUSIVE_AREA_07)
    {
        InterruptStatus_u32 = Eth_Schm_Read_Msr();
        /* If the interrupt has been enabled, the interrupt will be disabled */
        if(INTERRUPT_IS_ENABLED(InterruptStatus_u32))
        {
            Osr_SuspendAllInterrupts();
        }
        InterruptNestedStatus_ETH_EXCLUSIVE_AREA_07 = InterruptStatus_u32;
    }
    ExclusiveAreaNestedCounter_ETH_EXCLUSIVE_AREA_07++;
}

void SchM_Exit_Eth_ETH_EXCLUSIVE_AREA_07(void)
{
    ExclusiveAreaNestedCounter_ETH_EXCLUSIVE_AREA_07--;
    /* If the interrupt has been enabled before disabling it when entering the exclusive area, the interrupt will be enabled */
    if((INTERRUPT_IS_ENABLED(InterruptNestedStatus_ETH_EXCLUSIVE_AREA_07)) && (0UL == ExclusiveAreaNestedCounter_ETH_EXCLUSIVE_AREA_07))
    {
        Osr_ResumeAllInterrupts();
    }
}

void SchM_Enter_Eth_ETH_EXCLUSIVE_AREA_08(void)
{
    uint32_t InterruptStatus_u32 = 0U;

    if(0UL == ExclusiveAreaNestedCounter_ETH_EXCLUSIVE_AREA_08)
    {
        InterruptStatus_u32 = Eth_Schm_Read_Msr();
        /* If the interrupt has been enabled, the interrupt will be disabled */
        if(INTERRUPT_IS_ENABLED(InterruptStatus_u32))
        {
            Osr_SuspendAllInterrupts();
        }
        InterruptNestedStatus_ETH_EXCLUSIVE_AREA_08 = InterruptStatus_u32;
    }
    ExclusiveAreaNestedCounter_ETH_EXCLUSIVE_AREA_08++;
}

void SchM_Exit_Eth_ETH_EXCLUSIVE_AREA_08(void)
{
    ExclusiveAreaNestedCounter_ETH_EXCLUSIVE_AREA_08--;
    /* If the interrupt has been enabled before disabling it when entering the exclusive area, the interrupt will be enabled */
    if((INTERRUPT_IS_ENABLED(InterruptNestedStatus_ETH_EXCLUSIVE_AREA_08)) && (0UL == ExclusiveAreaNestedCounter_ETH_EXCLUSIVE_AREA_08))
    {
        Osr_ResumeAllInterrupts();
    }
}

void SchM_Enter_Eth_ETH_EXCLUSIVE_AREA_09(void)
{
    uint32_t InterruptStatus_u32 = 0U;

    if(0UL == ExclusiveAreaNestedCounter_ETH_EXCLUSIVE_AREA_09)
    {
        InterruptStatus_u32 = Eth_Schm_Read_Msr();
        /* If the interrupt has been enabled, the interrupt will be disabled */
        if(INTERRUPT_IS_ENABLED(InterruptStatus_u32))
        {
            Osr_SuspendAllInterrupts();
        }
        InterruptNestedStatus_ETH_EXCLUSIVE_AREA_09 = InterruptStatus_u32;
    }
    ExclusiveAreaNestedCounter_ETH_EXCLUSIVE_AREA_09++;
}

void SchM_Exit_Eth_ETH_EXCLUSIVE_AREA_09(void)
{
    ExclusiveAreaNestedCounter_ETH_EXCLUSIVE_AREA_09--;
    /* If the interrupt has been enabled before disabling it when entering the exclusive area, the interrupt will be enabled */
    if((INTERRUPT_IS_ENABLED(InterruptNestedStatus_ETH_EXCLUSIVE_AREA_09)) && (0UL == ExclusiveAreaNestedCounter_ETH_EXCLUSIVE_AREA_09))
    {
        Osr_ResumeAllInterrupts();
    }
}


#define RTE_STOP_SEC_CODE
#include "Rte_MemMap.h"

#ifdef __cplusplus
}
#endif

/**
 * @}
 */

