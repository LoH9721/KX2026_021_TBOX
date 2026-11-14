/***********************************************************************************************************************
 * Project Name: XHSC AUTOSAR MCAL
 * File Name: SchM_Fls.c
 *
 * Description: Implementation of SchM_Fls
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
 *   @file       SchM_Fls.c
 *   @version    1.0.3
 *   @brief      Exclusive area driver for Fls
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
#include "SchM_Fls.h"
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
static volatile uint32_t ExclusiveAreaNestedCounter_FLS_EXCLUSIVE_AREA_00;
static volatile uint32_t InterruptNestedStatus_FLS_EXCLUSIVE_AREA_00;
static volatile uint32_t ExclusiveAreaNestedCounter_FLS_EXCLUSIVE_AREA_01;
static volatile uint32_t InterruptNestedStatus_FLS_EXCLUSIVE_AREA_01;
static volatile uint32_t ExclusiveAreaNestedCounter_FLS_EXCLUSIVE_AREA_02;
static volatile uint32_t InterruptNestedStatus_FLS_EXCLUSIVE_AREA_02;
static volatile uint32_t ExclusiveAreaNestedCounter_FLS_EXCLUSIVE_AREA_03;
static volatile uint32_t InterruptNestedStatus_FLS_EXCLUSIVE_AREA_03;
static volatile uint32_t ExclusiveAreaNestedCounter_FLS_EXCLUSIVE_AREA_04;
static volatile uint32_t InterruptNestedStatus_FLS_EXCLUSIVE_AREA_04;
static volatile uint32_t ExclusiveAreaNestedCounter_FLS_EXCLUSIVE_AREA_05;
static volatile uint32_t InterruptNestedStatus_FLS_EXCLUSIVE_AREA_05;
static volatile uint32_t ExclusiveAreaNestedCounter_FLS_EXCLUSIVE_AREA_06;
static volatile uint32_t InterruptNestedStatus_FLS_EXCLUSIVE_AREA_06;
static volatile uint32_t ExclusiveAreaNestedCounter_FLS_EXCLUSIVE_AREA_07;
static volatile uint32_t InterruptNestedStatus_FLS_EXCLUSIVE_AREA_07;
static volatile uint32_t ExclusiveAreaNestedCounter_FLS_EXCLUSIVE_AREA_08;
static volatile uint32_t InterruptNestedStatus_FLS_EXCLUSIVE_AREA_08;
static volatile uint32_t ExclusiveAreaNestedCounter_FLS_EXCLUSIVE_AREA_09;
static volatile uint32_t InterruptNestedStatus_FLS_EXCLUSIVE_AREA_09;
static volatile uint32_t ExclusiveAreaNestedCounter_FLS_EXCLUSIVE_AREA_10;
static volatile uint32_t InterruptNestedStatus_FLS_EXCLUSIVE_AREA_10;
static volatile uint32_t ExclusiveAreaNestedCounter_FLS_EXCLUSIVE_AREA_11;
static volatile uint32_t InterruptNestedStatus_FLS_EXCLUSIVE_AREA_11;
static volatile uint32_t ExclusiveAreaNestedCounter_FLS_EXCLUSIVE_AREA_12;
static volatile uint32_t InterruptNestedStatus_FLS_EXCLUSIVE_AREA_12;
static volatile uint32_t ExclusiveAreaNestedCounter_FLS_EXCLUSIVE_AREA_13;
static volatile uint32_t InterruptNestedStatus_FLS_EXCLUSIVE_AREA_13;
static volatile uint32_t ExclusiveAreaNestedCounter_FLS_EXCLUSIVE_AREA_14;
static volatile uint32_t InterruptNestedStatus_FLS_EXCLUSIVE_AREA_14;
static volatile uint32_t ExclusiveAreaNestedCounter_FLS_EXCLUSIVE_AREA_15;
static volatile uint32_t InterruptNestedStatus_FLS_EXCLUSIVE_AREA_15;
static volatile uint32_t ExclusiveAreaNestedCounter_FLS_EXCLUSIVE_AREA_16;
static volatile uint32_t InterruptNestedStatus_FLS_EXCLUSIVE_AREA_16;
static volatile uint32_t ExclusiveAreaNestedCounter_FLS_EXCLUSIVE_AREA_17;
static volatile uint32_t InterruptNestedStatus_FLS_EXCLUSIVE_AREA_17;
static volatile uint32_t ExclusiveAreaNestedCounter_FLS_EXCLUSIVE_AREA_18;
static volatile uint32_t InterruptNestedStatus_FLS_EXCLUSIVE_AREA_18;
static volatile uint32_t ExclusiveAreaNestedCounter_FLS_EXCLUSIVE_AREA_19;
static volatile uint32_t InterruptNestedStatus_FLS_EXCLUSIVE_AREA_19;
static volatile uint32_t ExclusiveAreaNestedCounter_FLS_EXCLUSIVE_AREA_20;
static volatile uint32_t InterruptNestedStatus_FLS_EXCLUSIVE_AREA_20;
static volatile uint32_t ExclusiveAreaNestedCounter_FLS_EXCLUSIVE_AREA_21;
static volatile uint32_t InterruptNestedStatus_FLS_EXCLUSIVE_AREA_21;
static volatile uint32_t ExclusiveAreaNestedCounter_FLS_EXCLUSIVE_AREA_22;
static volatile uint32_t InterruptNestedStatus_FLS_EXCLUSIVE_AREA_22;
static volatile uint32_t ExclusiveAreaNestedCounter_FLS_EXCLUSIVE_AREA_23;
static volatile uint32_t InterruptNestedStatus_FLS_EXCLUSIVE_AREA_23;
static volatile uint32_t ExclusiveAreaNestedCounter_FLS_EXCLUSIVE_AREA_24;
static volatile uint32_t InterruptNestedStatus_FLS_EXCLUSIVE_AREA_24;
static volatile uint32_t ExclusiveAreaNestedCounter_FLS_EXCLUSIVE_AREA_25;
static volatile uint32_t InterruptNestedStatus_FLS_EXCLUSIVE_AREA_25;
static volatile uint32_t ExclusiveAreaNestedCounter_FLS_EXCLUSIVE_AREA_26;
static volatile uint32_t InterruptNestedStatus_FLS_EXCLUSIVE_AREA_26;
static volatile uint32_t ExclusiveAreaNestedCounter_FLS_EXCLUSIVE_AREA_27;
static volatile uint32_t InterruptNestedStatus_FLS_EXCLUSIVE_AREA_27;
static volatile uint32_t ExclusiveAreaNestedCounter_FLS_EXCLUSIVE_AREA_28;
static volatile uint32_t InterruptNestedStatus_FLS_EXCLUSIVE_AREA_28;
static volatile uint32_t ExclusiveAreaNestedCounter_FLS_EXCLUSIVE_AREA_29;
static volatile uint32_t InterruptNestedStatus_FLS_EXCLUSIVE_AREA_29;
static volatile uint32_t ExclusiveAreaNestedCounter_FLS_EXCLUSIVE_AREA_30;
static volatile uint32_t InterruptNestedStatus_FLS_EXCLUSIVE_AREA_30;
static volatile uint32_t ExclusiveAreaNestedCounter_FLS_EXCLUSIVE_AREA_31;
static volatile uint32_t InterruptNestedStatus_FLS_EXCLUSIVE_AREA_31;
static volatile uint32_t ExclusiveAreaNestedCounter_FLS_EXCLUSIVE_AREA_32;
static volatile uint32_t InterruptNestedStatus_FLS_EXCLUSIVE_AREA_32;
static volatile uint32_t ExclusiveAreaNestedCounter_FLS_EXCLUSIVE_AREA_33;
static volatile uint32_t InterruptNestedStatus_FLS_EXCLUSIVE_AREA_33;
static volatile uint32_t ExclusiveAreaNestedCounter_FLS_EXCLUSIVE_AREA_34;
static volatile uint32_t InterruptNestedStatus_FLS_EXCLUSIVE_AREA_34;
static volatile uint32_t ExclusiveAreaNestedCounter_FLS_EXCLUSIVE_AREA_35;
static volatile uint32_t InterruptNestedStatus_FLS_EXCLUSIVE_AREA_35;
static volatile uint32_t ExclusiveAreaNestedCounter_FLS_EXCLUSIVE_AREA_36;
static volatile uint32_t InterruptNestedStatus_FLS_EXCLUSIVE_AREA_36;
static volatile uint32_t ExclusiveAreaNestedCounter_FLS_EXCLUSIVE_AREA_37;
static volatile uint32_t InterruptNestedStatus_FLS_EXCLUSIVE_AREA_37;
static volatile uint32_t ExclusiveAreaNestedCounter_FLS_EXCLUSIVE_AREA_38;
static volatile uint32_t InterruptNestedStatus_FLS_EXCLUSIVE_AREA_38;
static volatile uint32_t ExclusiveAreaNestedCounter_FLS_EXCLUSIVE_AREA_39;
static volatile uint32_t InterruptNestedStatus_FLS_EXCLUSIVE_AREA_39;
#define RTE_STOP_SEC_VAR_CLEARED_32
#include "Rte_MemMap.h"

/***********************************************************************************************************************
 *  GLOBAL VARIABLES
 **********************************************************************************************************************/

/***********************************************************************************************************************
 *  LOCAL FUNCTION PROTOTYPES
 **********************************************************************************************************************/
static uint32_t Fls_Schm_Read_Msr(void);

/***********************************************************************************************************************
 *  LOCAL FUNCTION
 **********************************************************************************************************************/
static uint32_t Fls_Schm_Read_Msr(void)
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
void SchM_Enter_Fls_FLS_EXCLUSIVE_AREA_00(void)
{
    uint32_t InterruptStatus_u32 = 0U;

    if(0UL == ExclusiveAreaNestedCounter_FLS_EXCLUSIVE_AREA_00)
    {
        InterruptStatus_u32 = Fls_Schm_Read_Msr();
        /* If the interrupt has been enabled, the interrupt will be disabled */
        if(INTERRUPT_IS_ENABLED(InterruptStatus_u32))
        {
            Osr_SuspendAllInterrupts();
        }
        InterruptNestedStatus_FLS_EXCLUSIVE_AREA_00 = InterruptStatus_u32;
    }
    ExclusiveAreaNestedCounter_FLS_EXCLUSIVE_AREA_00++;
}

void SchM_Exit_Fls_FLS_EXCLUSIVE_AREA_00(void)
{
    ExclusiveAreaNestedCounter_FLS_EXCLUSIVE_AREA_00--;
    /* If the interrupt has been enabled before disabling it when entering the exclusive area, the interrupt will be enabled */
    if((INTERRUPT_IS_ENABLED(InterruptNestedStatus_FLS_EXCLUSIVE_AREA_00)) && (0UL == ExclusiveAreaNestedCounter_FLS_EXCLUSIVE_AREA_00))
    {
        Osr_ResumeAllInterrupts();
    }
}

void SchM_Enter_Fls_FLS_EXCLUSIVE_AREA_01(void)
{
    uint32_t InterruptStatus_u32 = 0U;

    if(0UL == ExclusiveAreaNestedCounter_FLS_EXCLUSIVE_AREA_01)
    {
        InterruptStatus_u32 = Fls_Schm_Read_Msr();
        /* If the interrupt has been enabled, the interrupt will be disabled */
        if(INTERRUPT_IS_ENABLED(InterruptStatus_u32))
        {
            Osr_SuspendAllInterrupts();
        }
        InterruptNestedStatus_FLS_EXCLUSIVE_AREA_01 = InterruptStatus_u32;
    }
    ExclusiveAreaNestedCounter_FLS_EXCLUSIVE_AREA_01++;
}

void SchM_Exit_Fls_FLS_EXCLUSIVE_AREA_01(void)
{
    ExclusiveAreaNestedCounter_FLS_EXCLUSIVE_AREA_01--;
    /* If the interrupt has been enabled before disabling it when entering the exclusive area, the interrupt will be enabled */
    if((INTERRUPT_IS_ENABLED(InterruptNestedStatus_FLS_EXCLUSIVE_AREA_01)) && (0UL == ExclusiveAreaNestedCounter_FLS_EXCLUSIVE_AREA_01))
    {
        Osr_ResumeAllInterrupts();
    }
}

void SchM_Enter_Fls_FLS_EXCLUSIVE_AREA_02(void)
{
    uint32_t InterruptStatus_u32 = 0U;

    if(0UL == ExclusiveAreaNestedCounter_FLS_EXCLUSIVE_AREA_02)
    {
        InterruptStatus_u32 = Fls_Schm_Read_Msr();
        /* If the interrupt has been enabled, the interrupt will be disabled */
        if(INTERRUPT_IS_ENABLED(InterruptStatus_u32))
        {
            Osr_SuspendAllInterrupts();
        }
        InterruptNestedStatus_FLS_EXCLUSIVE_AREA_02 = InterruptStatus_u32;
    }
    ExclusiveAreaNestedCounter_FLS_EXCLUSIVE_AREA_02++;
}

void SchM_Exit_Fls_FLS_EXCLUSIVE_AREA_02(void)
{
    ExclusiveAreaNestedCounter_FLS_EXCLUSIVE_AREA_02--;
    /* If the interrupt has been enabled before disabling it when entering the exclusive area, the interrupt will be enabled */
    if((INTERRUPT_IS_ENABLED(InterruptNestedStatus_FLS_EXCLUSIVE_AREA_02)) && (0UL == ExclusiveAreaNestedCounter_FLS_EXCLUSIVE_AREA_02))
    {
        Osr_ResumeAllInterrupts();
    }
}

void SchM_Enter_Fls_FLS_EXCLUSIVE_AREA_03(void)
{
    uint32_t InterruptStatus_u32 = 0U;

    if(0UL == ExclusiveAreaNestedCounter_FLS_EXCLUSIVE_AREA_03)
    {
        InterruptStatus_u32 = Fls_Schm_Read_Msr();
        /* If the interrupt has been enabled, the interrupt will be disabled */
        if(INTERRUPT_IS_ENABLED(InterruptStatus_u32))
        {
            Osr_SuspendAllInterrupts();
        }
        InterruptNestedStatus_FLS_EXCLUSIVE_AREA_03 = InterruptStatus_u32;
    }
    ExclusiveAreaNestedCounter_FLS_EXCLUSIVE_AREA_03++;
}

void SchM_Exit_Fls_FLS_EXCLUSIVE_AREA_03(void)
{
    ExclusiveAreaNestedCounter_FLS_EXCLUSIVE_AREA_03--;
    /* If the interrupt has been enabled before disabling it when entering the exclusive area, the interrupt will be enabled */
    if((INTERRUPT_IS_ENABLED(InterruptNestedStatus_FLS_EXCLUSIVE_AREA_03)) && (0UL == ExclusiveAreaNestedCounter_FLS_EXCLUSIVE_AREA_03))
    {
        Osr_ResumeAllInterrupts();
    }
}

void SchM_Enter_Fls_FLS_EXCLUSIVE_AREA_04(void)
{
    uint32_t InterruptStatus_u32 = 0U;

    if(0UL == ExclusiveAreaNestedCounter_FLS_EXCLUSIVE_AREA_04)
    {
        InterruptStatus_u32 = Fls_Schm_Read_Msr();
        /* If the interrupt has been enabled, the interrupt will be disabled */
        if(INTERRUPT_IS_ENABLED(InterruptStatus_u32))
        {
            Osr_SuspendAllInterrupts();
        }
        InterruptNestedStatus_FLS_EXCLUSIVE_AREA_04 = InterruptStatus_u32;
    }
    ExclusiveAreaNestedCounter_FLS_EXCLUSIVE_AREA_04++;
}

void SchM_Exit_Fls_FLS_EXCLUSIVE_AREA_04(void)
{
    ExclusiveAreaNestedCounter_FLS_EXCLUSIVE_AREA_04--;
    /* If the interrupt has been enabled before disabling it when entering the exclusive area, the interrupt will be enabled */
    if((INTERRUPT_IS_ENABLED(InterruptNestedStatus_FLS_EXCLUSIVE_AREA_04)) && (0UL == ExclusiveAreaNestedCounter_FLS_EXCLUSIVE_AREA_04))
    {
        Osr_ResumeAllInterrupts();
    }
}

void SchM_Enter_Fls_FLS_EXCLUSIVE_AREA_05(void)
{
    uint32_t InterruptStatus_u32 = 0U;

    if(0UL == ExclusiveAreaNestedCounter_FLS_EXCLUSIVE_AREA_05)
    {
        InterruptStatus_u32 = Fls_Schm_Read_Msr();
        /* If the interrupt has been enabled, the interrupt will be disabled */
        if(INTERRUPT_IS_ENABLED(InterruptStatus_u32))
        {
            Osr_SuspendAllInterrupts();
        }
        InterruptNestedStatus_FLS_EXCLUSIVE_AREA_05 = InterruptStatus_u32;
    }
    ExclusiveAreaNestedCounter_FLS_EXCLUSIVE_AREA_05++;
}

void SchM_Exit_Fls_FLS_EXCLUSIVE_AREA_05(void)
{
    ExclusiveAreaNestedCounter_FLS_EXCLUSIVE_AREA_05--;
    /* If the interrupt has been enabled before disabling it when entering the exclusive area, the interrupt will be enabled */
    if((INTERRUPT_IS_ENABLED(InterruptNestedStatus_FLS_EXCLUSIVE_AREA_05)) && (0UL == ExclusiveAreaNestedCounter_FLS_EXCLUSIVE_AREA_05))
    {
        Osr_ResumeAllInterrupts();
    }
}

void SchM_Enter_Fls_FLS_EXCLUSIVE_AREA_06(void)
{
    uint32_t InterruptStatus_u32 = 0U;

    if(0UL == ExclusiveAreaNestedCounter_FLS_EXCLUSIVE_AREA_06)
    {
        InterruptStatus_u32 = Fls_Schm_Read_Msr();
        /* If the interrupt has been enabled, the interrupt will be disabled */
        if(INTERRUPT_IS_ENABLED(InterruptStatus_u32))
        {
            Osr_SuspendAllInterrupts();
        }
        InterruptNestedStatus_FLS_EXCLUSIVE_AREA_06 = InterruptStatus_u32;
    }
    ExclusiveAreaNestedCounter_FLS_EXCLUSIVE_AREA_06++;
}

void SchM_Exit_Fls_FLS_EXCLUSIVE_AREA_06(void)
{
    ExclusiveAreaNestedCounter_FLS_EXCLUSIVE_AREA_06--;
    /* If the interrupt has been enabled before disabling it when entering the exclusive area, the interrupt will be enabled */
    if((INTERRUPT_IS_ENABLED(InterruptNestedStatus_FLS_EXCLUSIVE_AREA_06)) && (0UL == ExclusiveAreaNestedCounter_FLS_EXCLUSIVE_AREA_06))
    {
        Osr_ResumeAllInterrupts();
    }
}

void SchM_Enter_Fls_FLS_EXCLUSIVE_AREA_07(void)
{
    uint32_t InterruptStatus_u32 = 0U;

    if(0UL == ExclusiveAreaNestedCounter_FLS_EXCLUSIVE_AREA_07)
    {
        InterruptStatus_u32 = Fls_Schm_Read_Msr();
        /* If the interrupt has been enabled, the interrupt will be disabled */
        if(INTERRUPT_IS_ENABLED(InterruptStatus_u32))
        {
            Osr_SuspendAllInterrupts();
        }
        InterruptNestedStatus_FLS_EXCLUSIVE_AREA_07 = InterruptStatus_u32;
    }
    ExclusiveAreaNestedCounter_FLS_EXCLUSIVE_AREA_07++;
}

void SchM_Exit_Fls_FLS_EXCLUSIVE_AREA_07(void)
{
    ExclusiveAreaNestedCounter_FLS_EXCLUSIVE_AREA_07--;
    /* If the interrupt has been enabled before disabling it when entering the exclusive area, the interrupt will be enabled */
    if((INTERRUPT_IS_ENABLED(InterruptNestedStatus_FLS_EXCLUSIVE_AREA_07)) && (0UL == ExclusiveAreaNestedCounter_FLS_EXCLUSIVE_AREA_07))
    {
        Osr_ResumeAllInterrupts();
    }
}

void SchM_Enter_Fls_FLS_EXCLUSIVE_AREA_08(void)
{
    uint32_t InterruptStatus_u32 = 0U;

    if(0UL == ExclusiveAreaNestedCounter_FLS_EXCLUSIVE_AREA_08)
    {
        InterruptStatus_u32 = Fls_Schm_Read_Msr();
        /* If the interrupt has been enabled, the interrupt will be disabled */
        if(INTERRUPT_IS_ENABLED(InterruptStatus_u32))
        {
            Osr_SuspendAllInterrupts();
        }
        InterruptNestedStatus_FLS_EXCLUSIVE_AREA_08 = InterruptStatus_u32;
    }
    ExclusiveAreaNestedCounter_FLS_EXCLUSIVE_AREA_08++;
}

void SchM_Exit_Fls_FLS_EXCLUSIVE_AREA_08(void)
{
    ExclusiveAreaNestedCounter_FLS_EXCLUSIVE_AREA_08--;
    /* If the interrupt has been enabled before disabling it when entering the exclusive area, the interrupt will be enabled */
    if((INTERRUPT_IS_ENABLED(InterruptNestedStatus_FLS_EXCLUSIVE_AREA_08)) && (0UL == ExclusiveAreaNestedCounter_FLS_EXCLUSIVE_AREA_08))
    {
        Osr_ResumeAllInterrupts();
    }
}

void SchM_Enter_Fls_FLS_EXCLUSIVE_AREA_09(void)
{
    uint32_t InterruptStatus_u32 = 0U;

    if(0UL == ExclusiveAreaNestedCounter_FLS_EXCLUSIVE_AREA_09)
    {
        InterruptStatus_u32 = Fls_Schm_Read_Msr();
        /* If the interrupt has been enabled, the interrupt will be disabled */
        if(INTERRUPT_IS_ENABLED(InterruptStatus_u32))
        {
            Osr_SuspendAllInterrupts();
        }
        InterruptNestedStatus_FLS_EXCLUSIVE_AREA_09 = InterruptStatus_u32;
    }
    ExclusiveAreaNestedCounter_FLS_EXCLUSIVE_AREA_09++;
}

void SchM_Exit_Fls_FLS_EXCLUSIVE_AREA_09(void)
{
    ExclusiveAreaNestedCounter_FLS_EXCLUSIVE_AREA_09--;
    /* If the interrupt has been enabled before disabling it when entering the exclusive area, the interrupt will be enabled */
    if((INTERRUPT_IS_ENABLED(InterruptNestedStatus_FLS_EXCLUSIVE_AREA_09)) && (0UL == ExclusiveAreaNestedCounter_FLS_EXCLUSIVE_AREA_09))
    {
        Osr_ResumeAllInterrupts();
    }
}

void SchM_Enter_Fls_FLS_EXCLUSIVE_AREA_10(void)
{
    uint32_t InterruptStatus_u32 = 0U;

    if(0UL == ExclusiveAreaNestedCounter_FLS_EXCLUSIVE_AREA_10)
    {
        InterruptStatus_u32 = Fls_Schm_Read_Msr();
        /* If the interrupt has been enabled, the interrupt will be disabled */
        if(INTERRUPT_IS_ENABLED(InterruptStatus_u32))
        {
            Osr_SuspendAllInterrupts();
        }
        InterruptNestedStatus_FLS_EXCLUSIVE_AREA_10 = InterruptStatus_u32;
    }
    ExclusiveAreaNestedCounter_FLS_EXCLUSIVE_AREA_10++;
}

void SchM_Exit_Fls_FLS_EXCLUSIVE_AREA_10(void)
{
    ExclusiveAreaNestedCounter_FLS_EXCLUSIVE_AREA_10--;
    /* If the interrupt has been enabled before disabling it when entering the exclusive area, the interrupt will be enabled */
    if((INTERRUPT_IS_ENABLED(InterruptNestedStatus_FLS_EXCLUSIVE_AREA_10)) && (0UL == ExclusiveAreaNestedCounter_FLS_EXCLUSIVE_AREA_10))
    {
        Osr_ResumeAllInterrupts();
    }
}

void SchM_Enter_Fls_FLS_EXCLUSIVE_AREA_11(void)
{
    uint32_t InterruptStatus_u32 = 0U;

    if(0UL == ExclusiveAreaNestedCounter_FLS_EXCLUSIVE_AREA_11)
    {
        InterruptStatus_u32 = Fls_Schm_Read_Msr();
        /* If the interrupt has been enabled, the interrupt will be disabled */
        if(INTERRUPT_IS_ENABLED(InterruptStatus_u32))
        {
            Osr_SuspendAllInterrupts();
        }
        InterruptNestedStatus_FLS_EXCLUSIVE_AREA_11 = InterruptStatus_u32;
    }
    ExclusiveAreaNestedCounter_FLS_EXCLUSIVE_AREA_11++;
}

void SchM_Exit_Fls_FLS_EXCLUSIVE_AREA_11(void)
{
    ExclusiveAreaNestedCounter_FLS_EXCLUSIVE_AREA_11--;
    /* If the interrupt has been enabled before disabling it when entering the exclusive area, the interrupt will be enabled */
    if((INTERRUPT_IS_ENABLED(InterruptNestedStatus_FLS_EXCLUSIVE_AREA_11)) && (0UL == ExclusiveAreaNestedCounter_FLS_EXCLUSIVE_AREA_11))
    {
        Osr_ResumeAllInterrupts();
    }
}

void SchM_Enter_Fls_FLS_EXCLUSIVE_AREA_12(void)
{
    uint32_t InterruptStatus_u32 = 0U;

    if(0UL == ExclusiveAreaNestedCounter_FLS_EXCLUSIVE_AREA_12)
    {
        InterruptStatus_u32 = Fls_Schm_Read_Msr();
        /* If the interrupt has been enabled, the interrupt will be disabled */
        if(INTERRUPT_IS_ENABLED(InterruptStatus_u32))
        {
            Osr_SuspendAllInterrupts();
        }
        InterruptNestedStatus_FLS_EXCLUSIVE_AREA_12 = InterruptStatus_u32;
    }
    ExclusiveAreaNestedCounter_FLS_EXCLUSIVE_AREA_12++;
}

void SchM_Exit_Fls_FLS_EXCLUSIVE_AREA_12(void)
{
    ExclusiveAreaNestedCounter_FLS_EXCLUSIVE_AREA_12--;
    /* If the interrupt has been enabled before disabling it when entering the exclusive area, the interrupt will be enabled */
    if((INTERRUPT_IS_ENABLED(InterruptNestedStatus_FLS_EXCLUSIVE_AREA_12)) && (0UL == ExclusiveAreaNestedCounter_FLS_EXCLUSIVE_AREA_12))
    {
        Osr_ResumeAllInterrupts();
    }
}

void SchM_Enter_Fls_FLS_EXCLUSIVE_AREA_13(void)
{
    uint32_t InterruptStatus_u32 = 0U;

    if(0UL == ExclusiveAreaNestedCounter_FLS_EXCLUSIVE_AREA_13)
    {
        InterruptStatus_u32 = Fls_Schm_Read_Msr();
        /* If the interrupt has been enabled, the interrupt will be disabled */
        if(INTERRUPT_IS_ENABLED(InterruptStatus_u32))
        {
            Osr_SuspendAllInterrupts();
        }
        InterruptNestedStatus_FLS_EXCLUSIVE_AREA_13 = InterruptStatus_u32;
    }
    ExclusiveAreaNestedCounter_FLS_EXCLUSIVE_AREA_13++;
}

void SchM_Exit_Fls_FLS_EXCLUSIVE_AREA_13(void)
{
    ExclusiveAreaNestedCounter_FLS_EXCLUSIVE_AREA_13--;
    /* If the interrupt has been enabled before disabling it when entering the exclusive area, the interrupt will be enabled */
    if((INTERRUPT_IS_ENABLED(InterruptNestedStatus_FLS_EXCLUSIVE_AREA_13)) && (0UL == ExclusiveAreaNestedCounter_FLS_EXCLUSIVE_AREA_13))
    {
        Osr_ResumeAllInterrupts();
    }
}

void SchM_Enter_Fls_FLS_EXCLUSIVE_AREA_14(void)
{
    uint32_t InterruptStatus_u32 = 0U;

    if(0UL == ExclusiveAreaNestedCounter_FLS_EXCLUSIVE_AREA_14)
    {
        InterruptStatus_u32 = Fls_Schm_Read_Msr();
        /* If the interrupt has been enabled, the interrupt will be disabled */
        if(INTERRUPT_IS_ENABLED(InterruptStatus_u32))
        {
            Osr_SuspendAllInterrupts();
        }
        InterruptNestedStatus_FLS_EXCLUSIVE_AREA_14 = InterruptStatus_u32;
    }
    ExclusiveAreaNestedCounter_FLS_EXCLUSIVE_AREA_14++;
}

void SchM_Exit_Fls_FLS_EXCLUSIVE_AREA_14(void)
{
    ExclusiveAreaNestedCounter_FLS_EXCLUSIVE_AREA_14--;
    /* If the interrupt has been enabled before disabling it when entering the exclusive area, the interrupt will be enabled */
    if((INTERRUPT_IS_ENABLED(InterruptNestedStatus_FLS_EXCLUSIVE_AREA_14)) && (0UL == ExclusiveAreaNestedCounter_FLS_EXCLUSIVE_AREA_14))
    {
        Osr_ResumeAllInterrupts();
    }
}

void SchM_Enter_Fls_FLS_EXCLUSIVE_AREA_15(void)
{
    uint32_t InterruptStatus_u32 = 0U;

    if(0UL == ExclusiveAreaNestedCounter_FLS_EXCLUSIVE_AREA_15)
    {
        InterruptStatus_u32 = Fls_Schm_Read_Msr();
        /* If the interrupt has been enabled, the interrupt will be disabled */
        if(INTERRUPT_IS_ENABLED(InterruptStatus_u32))
        {
            Osr_SuspendAllInterrupts();
        }
        InterruptNestedStatus_FLS_EXCLUSIVE_AREA_15 = InterruptStatus_u32;
    }
    ExclusiveAreaNestedCounter_FLS_EXCLUSIVE_AREA_15++;
}

void SchM_Exit_Fls_FLS_EXCLUSIVE_AREA_15(void)
{
    ExclusiveAreaNestedCounter_FLS_EXCLUSIVE_AREA_15--;
    /* If the interrupt has been enabled before disabling it when entering the exclusive area, the interrupt will be enabled */
    if((INTERRUPT_IS_ENABLED(InterruptNestedStatus_FLS_EXCLUSIVE_AREA_15)) && (0UL == ExclusiveAreaNestedCounter_FLS_EXCLUSIVE_AREA_15))
    {
        Osr_ResumeAllInterrupts();
    }
}

void SchM_Enter_Fls_FLS_EXCLUSIVE_AREA_16(void)
{
    uint32_t InterruptStatus_u32 = 0U;

    if(0UL == ExclusiveAreaNestedCounter_FLS_EXCLUSIVE_AREA_16)
    {
        InterruptStatus_u32 = Fls_Schm_Read_Msr();
        /* If the interrupt has been enabled, the interrupt will be disabled */
        if(INTERRUPT_IS_ENABLED(InterruptStatus_u32))
        {
            Osr_SuspendAllInterrupts();
        }
        InterruptNestedStatus_FLS_EXCLUSIVE_AREA_16 = InterruptStatus_u32;
    }
    ExclusiveAreaNestedCounter_FLS_EXCLUSIVE_AREA_16++;
}

void SchM_Exit_Fls_FLS_EXCLUSIVE_AREA_16(void)
{
    ExclusiveAreaNestedCounter_FLS_EXCLUSIVE_AREA_16--;
    /* If the interrupt has been enabled before disabling it when entering the exclusive area, the interrupt will be enabled */
    if((INTERRUPT_IS_ENABLED(InterruptNestedStatus_FLS_EXCLUSIVE_AREA_16)) && (0UL == ExclusiveAreaNestedCounter_FLS_EXCLUSIVE_AREA_16))
    {
        Osr_ResumeAllInterrupts();
    }
}

void SchM_Enter_Fls_FLS_EXCLUSIVE_AREA_17(void)
{
    uint32_t InterruptStatus_u32 = 0U;

    if(0UL == ExclusiveAreaNestedCounter_FLS_EXCLUSIVE_AREA_17)
    {
        InterruptStatus_u32 = Fls_Schm_Read_Msr();
        /* If the interrupt has been enabled, the interrupt will be disabled */
        if(INTERRUPT_IS_ENABLED(InterruptStatus_u32))
        {
            Osr_SuspendAllInterrupts();
        }
        InterruptNestedStatus_FLS_EXCLUSIVE_AREA_17 = InterruptStatus_u32;
    }
    ExclusiveAreaNestedCounter_FLS_EXCLUSIVE_AREA_17++;
}

void SchM_Exit_Fls_FLS_EXCLUSIVE_AREA_17(void)
{
    ExclusiveAreaNestedCounter_FLS_EXCLUSIVE_AREA_17--;
    /* If the interrupt has been enabled before disabling it when entering the exclusive area, the interrupt will be enabled */
    if((INTERRUPT_IS_ENABLED(InterruptNestedStatus_FLS_EXCLUSIVE_AREA_17)) && (0UL == ExclusiveAreaNestedCounter_FLS_EXCLUSIVE_AREA_17))
    {
        Osr_ResumeAllInterrupts();
    }
}

void SchM_Enter_Fls_FLS_EXCLUSIVE_AREA_18(void)
{
    uint32_t InterruptStatus_u32 = 0U;

    if(0UL == ExclusiveAreaNestedCounter_FLS_EXCLUSIVE_AREA_18)
    {
        InterruptStatus_u32 = Fls_Schm_Read_Msr();
        /* If the interrupt has been enabled, the interrupt will be disabled */
        if(INTERRUPT_IS_ENABLED(InterruptStatus_u32))
        {
            Osr_SuspendAllInterrupts();
        }
        InterruptNestedStatus_FLS_EXCLUSIVE_AREA_18 = InterruptStatus_u32;
    }
    ExclusiveAreaNestedCounter_FLS_EXCLUSIVE_AREA_18++;
}

void SchM_Exit_Fls_FLS_EXCLUSIVE_AREA_18(void)
{
    ExclusiveAreaNestedCounter_FLS_EXCLUSIVE_AREA_18--;
    /* If the interrupt has been enabled before disabling it when entering the exclusive area, the interrupt will be enabled */
    if((INTERRUPT_IS_ENABLED(InterruptNestedStatus_FLS_EXCLUSIVE_AREA_18)) && (0UL == ExclusiveAreaNestedCounter_FLS_EXCLUSIVE_AREA_18))
    {
        Osr_ResumeAllInterrupts();
    }
}

void SchM_Enter_Fls_FLS_EXCLUSIVE_AREA_19(void)
{
    uint32_t InterruptStatus_u32 = 0U;

    if(0UL == ExclusiveAreaNestedCounter_FLS_EXCLUSIVE_AREA_19)
    {
        InterruptStatus_u32 = Fls_Schm_Read_Msr();
        /* If the interrupt has been enabled, the interrupt will be disabled */
        if(INTERRUPT_IS_ENABLED(InterruptStatus_u32))
        {
            Osr_SuspendAllInterrupts();
        }
        InterruptNestedStatus_FLS_EXCLUSIVE_AREA_19 = InterruptStatus_u32;
    }
    ExclusiveAreaNestedCounter_FLS_EXCLUSIVE_AREA_19++;
}

void SchM_Exit_Fls_FLS_EXCLUSIVE_AREA_19(void)
{
    ExclusiveAreaNestedCounter_FLS_EXCLUSIVE_AREA_19--;
    /* If the interrupt has been enabled before disabling it when entering the exclusive area, the interrupt will be enabled */
    if((INTERRUPT_IS_ENABLED(InterruptNestedStatus_FLS_EXCLUSIVE_AREA_19)) && (0UL == ExclusiveAreaNestedCounter_FLS_EXCLUSIVE_AREA_19))
    {
        Osr_ResumeAllInterrupts();
    }
}

void SchM_Enter_Fls_FLS_EXCLUSIVE_AREA_20(void)
{
    uint32_t InterruptStatus_u32 = 0U;

    if(0UL == ExclusiveAreaNestedCounter_FLS_EXCLUSIVE_AREA_20)
    {
        InterruptStatus_u32 = Fls_Schm_Read_Msr();
        /* If the interrupt has been enabled, the interrupt will be disabled */
        if(INTERRUPT_IS_ENABLED(InterruptStatus_u32))
        {
            Osr_SuspendAllInterrupts();
        }
        InterruptNestedStatus_FLS_EXCLUSIVE_AREA_20 = InterruptStatus_u32;
    }
    ExclusiveAreaNestedCounter_FLS_EXCLUSIVE_AREA_20++;
}

void SchM_Exit_Fls_FLS_EXCLUSIVE_AREA_20(void)
{
    ExclusiveAreaNestedCounter_FLS_EXCLUSIVE_AREA_20--;
    /* If the interrupt has been enabled before disabling it when entering the exclusive area, the interrupt will be enabled */
    if((INTERRUPT_IS_ENABLED(InterruptNestedStatus_FLS_EXCLUSIVE_AREA_20)) && (0UL == ExclusiveAreaNestedCounter_FLS_EXCLUSIVE_AREA_20))
    {
        Osr_ResumeAllInterrupts();
    }
}

void SchM_Enter_Fls_FLS_EXCLUSIVE_AREA_21(void)
{
    uint32_t InterruptStatus_u32 = 0U;

    if(0UL == ExclusiveAreaNestedCounter_FLS_EXCLUSIVE_AREA_21)
    {
        InterruptStatus_u32 = Fls_Schm_Read_Msr();
        /* If the interrupt has been enabled, the interrupt will be disabled */
        if(INTERRUPT_IS_ENABLED(InterruptStatus_u32))
        {
            Osr_SuspendAllInterrupts();
        }
        InterruptNestedStatus_FLS_EXCLUSIVE_AREA_21 = InterruptStatus_u32;
    }
    ExclusiveAreaNestedCounter_FLS_EXCLUSIVE_AREA_21++;
}

void SchM_Exit_Fls_FLS_EXCLUSIVE_AREA_21(void)
{
    ExclusiveAreaNestedCounter_FLS_EXCLUSIVE_AREA_21--;
    /* If the interrupt has been enabled before disabling it when entering the exclusive area, the interrupt will be enabled */
    if((INTERRUPT_IS_ENABLED(InterruptNestedStatus_FLS_EXCLUSIVE_AREA_21)) && (0UL == ExclusiveAreaNestedCounter_FLS_EXCLUSIVE_AREA_21))
    {
        Osr_ResumeAllInterrupts();
    }
}

void SchM_Enter_Fls_FLS_EXCLUSIVE_AREA_22(void)
{
    uint32_t InterruptStatus_u32 = 0U;

    if(0UL == ExclusiveAreaNestedCounter_FLS_EXCLUSIVE_AREA_22)
    {
        InterruptStatus_u32 = Fls_Schm_Read_Msr();
        /* If the interrupt has been enabled, the interrupt will be disabled */
        if(INTERRUPT_IS_ENABLED(InterruptStatus_u32))
        {
            Osr_SuspendAllInterrupts();
        }
        InterruptNestedStatus_FLS_EXCLUSIVE_AREA_22 = InterruptStatus_u32;
    }
    ExclusiveAreaNestedCounter_FLS_EXCLUSIVE_AREA_22++;
}

void SchM_Exit_Fls_FLS_EXCLUSIVE_AREA_22(void)
{
    ExclusiveAreaNestedCounter_FLS_EXCLUSIVE_AREA_22--;
    /* If the interrupt has been enabled before disabling it when entering the exclusive area, the interrupt will be enabled */
    if((INTERRUPT_IS_ENABLED(InterruptNestedStatus_FLS_EXCLUSIVE_AREA_22)) && (0UL == ExclusiveAreaNestedCounter_FLS_EXCLUSIVE_AREA_22))
    {
        Osr_ResumeAllInterrupts();
    }
}

void SchM_Enter_Fls_FLS_EXCLUSIVE_AREA_23(void)
{
    uint32_t InterruptStatus_u32 = 0U;

    if(0UL == ExclusiveAreaNestedCounter_FLS_EXCLUSIVE_AREA_23)
    {
        InterruptStatus_u32 = Fls_Schm_Read_Msr();
        /* If the interrupt has been enabled, the interrupt will be disabled */
        if(INTERRUPT_IS_ENABLED(InterruptStatus_u32))
        {
            Osr_SuspendAllInterrupts();
        }
        InterruptNestedStatus_FLS_EXCLUSIVE_AREA_23 = InterruptStatus_u32;
    }
    ExclusiveAreaNestedCounter_FLS_EXCLUSIVE_AREA_23++;
}

void SchM_Exit_Fls_FLS_EXCLUSIVE_AREA_23(void)
{
    ExclusiveAreaNestedCounter_FLS_EXCLUSIVE_AREA_23--;
    /* If the interrupt has been enabled before disabling it when entering the exclusive area, the interrupt will be enabled */
    if((INTERRUPT_IS_ENABLED(InterruptNestedStatus_FLS_EXCLUSIVE_AREA_23)) && (0UL == ExclusiveAreaNestedCounter_FLS_EXCLUSIVE_AREA_23))
    {
        Osr_ResumeAllInterrupts();
    }
}

void SchM_Enter_Fls_FLS_EXCLUSIVE_AREA_24(void)
{
    uint32_t InterruptStatus_u32 = 0U;

    if(0UL == ExclusiveAreaNestedCounter_FLS_EXCLUSIVE_AREA_24)
    {
        InterruptStatus_u32 = Fls_Schm_Read_Msr();
        /* If the interrupt has been enabled, the interrupt will be disabled */
        if(INTERRUPT_IS_ENABLED(InterruptStatus_u32))
        {
            Osr_SuspendAllInterrupts();
        }
        InterruptNestedStatus_FLS_EXCLUSIVE_AREA_24 = InterruptStatus_u32;
    }
    ExclusiveAreaNestedCounter_FLS_EXCLUSIVE_AREA_24++;
}

void SchM_Exit_Fls_FLS_EXCLUSIVE_AREA_24(void)
{
    ExclusiveAreaNestedCounter_FLS_EXCLUSIVE_AREA_24--;
    /* If the interrupt has been enabled before disabling it when entering the exclusive area, the interrupt will be enabled */
    if((INTERRUPT_IS_ENABLED(InterruptNestedStatus_FLS_EXCLUSIVE_AREA_24)) && (0UL == ExclusiveAreaNestedCounter_FLS_EXCLUSIVE_AREA_24))
    {
        Osr_ResumeAllInterrupts();
    }
}

void SchM_Enter_Fls_FLS_EXCLUSIVE_AREA_25(void)
{
    uint32_t InterruptStatus_u32 = 0U;

    if(0UL == ExclusiveAreaNestedCounter_FLS_EXCLUSIVE_AREA_25)
    {
        InterruptStatus_u32 = Fls_Schm_Read_Msr();
        /* If the interrupt has been enabled, the interrupt will be disabled */
        if(INTERRUPT_IS_ENABLED(InterruptStatus_u32))
        {
            Osr_SuspendAllInterrupts();
        }
        InterruptNestedStatus_FLS_EXCLUSIVE_AREA_25 = InterruptStatus_u32;
    }
    ExclusiveAreaNestedCounter_FLS_EXCLUSIVE_AREA_25++;
}

void SchM_Exit_Fls_FLS_EXCLUSIVE_AREA_25(void)
{
    ExclusiveAreaNestedCounter_FLS_EXCLUSIVE_AREA_25--;
    /* If the interrupt has been enabled before disabling it when entering the exclusive area, the interrupt will be enabled */
    if((INTERRUPT_IS_ENABLED(InterruptNestedStatus_FLS_EXCLUSIVE_AREA_25)) && (0UL == ExclusiveAreaNestedCounter_FLS_EXCLUSIVE_AREA_25))
    {
        Osr_ResumeAllInterrupts();
    }
}

void SchM_Enter_Fls_FLS_EXCLUSIVE_AREA_26(void)
{
    uint32_t InterruptStatus_u32 = 0U;

    if(0UL == ExclusiveAreaNestedCounter_FLS_EXCLUSIVE_AREA_26)
    {
        InterruptStatus_u32 = Fls_Schm_Read_Msr();
        /* If the interrupt has been enabled, the interrupt will be disabled */
        if(INTERRUPT_IS_ENABLED(InterruptStatus_u32))
        {
            Osr_SuspendAllInterrupts();
        }
        InterruptNestedStatus_FLS_EXCLUSIVE_AREA_26 = InterruptStatus_u32;
    }
    ExclusiveAreaNestedCounter_FLS_EXCLUSIVE_AREA_26++;
}

void SchM_Exit_Fls_FLS_EXCLUSIVE_AREA_26(void)
{
    ExclusiveAreaNestedCounter_FLS_EXCLUSIVE_AREA_26--;
    /* If the interrupt has been enabled before disabling it when entering the exclusive area, the interrupt will be enabled */
    if((INTERRUPT_IS_ENABLED(InterruptNestedStatus_FLS_EXCLUSIVE_AREA_26)) && (0UL == ExclusiveAreaNestedCounter_FLS_EXCLUSIVE_AREA_26))
    {
        Osr_ResumeAllInterrupts();
    }
}

void SchM_Enter_Fls_FLS_EXCLUSIVE_AREA_27(void)
{
    uint32_t InterruptStatus_u32 = 0U;

    if(0UL == ExclusiveAreaNestedCounter_FLS_EXCLUSIVE_AREA_27)
    {
        InterruptStatus_u32 = Fls_Schm_Read_Msr();
        /* If the interrupt has been enabled, the interrupt will be disabled */
        if(INTERRUPT_IS_ENABLED(InterruptStatus_u32))
        {
            Osr_SuspendAllInterrupts();
        }
        InterruptNestedStatus_FLS_EXCLUSIVE_AREA_27 = InterruptStatus_u32;
    }
    ExclusiveAreaNestedCounter_FLS_EXCLUSIVE_AREA_27++;
}

void SchM_Exit_Fls_FLS_EXCLUSIVE_AREA_27(void)
{
    ExclusiveAreaNestedCounter_FLS_EXCLUSIVE_AREA_27--;
    /* If the interrupt has been enabled before disabling it when entering the exclusive area, the interrupt will be enabled */
    if((INTERRUPT_IS_ENABLED(InterruptNestedStatus_FLS_EXCLUSIVE_AREA_27)) && (0UL == ExclusiveAreaNestedCounter_FLS_EXCLUSIVE_AREA_27))
    {
        Osr_ResumeAllInterrupts();
    }
}

void SchM_Enter_Fls_FLS_EXCLUSIVE_AREA_28(void)
{
    uint32_t InterruptStatus_u32 = 0U;

    if(0UL == ExclusiveAreaNestedCounter_FLS_EXCLUSIVE_AREA_28)
    {
        InterruptStatus_u32 = Fls_Schm_Read_Msr();
        /* If the interrupt has been enabled, the interrupt will be disabled */
        if(INTERRUPT_IS_ENABLED(InterruptStatus_u32))
        {
            Osr_SuspendAllInterrupts();
        }
        InterruptNestedStatus_FLS_EXCLUSIVE_AREA_28 = InterruptStatus_u32;
    }
    ExclusiveAreaNestedCounter_FLS_EXCLUSIVE_AREA_28++;
}

void SchM_Exit_Fls_FLS_EXCLUSIVE_AREA_28(void)
{
    ExclusiveAreaNestedCounter_FLS_EXCLUSIVE_AREA_28--;
    /* If the interrupt has been enabled before disabling it when entering the exclusive area, the interrupt will be enabled */
    if((INTERRUPT_IS_ENABLED(InterruptNestedStatus_FLS_EXCLUSIVE_AREA_28)) && (0UL == ExclusiveAreaNestedCounter_FLS_EXCLUSIVE_AREA_28))
    {
        Osr_ResumeAllInterrupts();
    }
}

void SchM_Enter_Fls_FLS_EXCLUSIVE_AREA_29(void)
{
    uint32_t InterruptStatus_u32 = 0U;

    if(0UL == ExclusiveAreaNestedCounter_FLS_EXCLUSIVE_AREA_29)
    {
        InterruptStatus_u32 = Fls_Schm_Read_Msr();
        /* If the interrupt has been enabled, the interrupt will be disabled */
        if(INTERRUPT_IS_ENABLED(InterruptStatus_u32))
        {
            Osr_SuspendAllInterrupts();
        }
        InterruptNestedStatus_FLS_EXCLUSIVE_AREA_29 = InterruptStatus_u32;
    }
    ExclusiveAreaNestedCounter_FLS_EXCLUSIVE_AREA_29++;
}

void SchM_Exit_Fls_FLS_EXCLUSIVE_AREA_29(void)
{
    ExclusiveAreaNestedCounter_FLS_EXCLUSIVE_AREA_29--;
    /* If the interrupt has been enabled before disabling it when entering the exclusive area, the interrupt will be enabled */
    if((INTERRUPT_IS_ENABLED(InterruptNestedStatus_FLS_EXCLUSIVE_AREA_29)) && (0UL == ExclusiveAreaNestedCounter_FLS_EXCLUSIVE_AREA_29))
    {
        Osr_ResumeAllInterrupts();
    }
}

void SchM_Enter_Fls_FLS_EXCLUSIVE_AREA_30(void)
{
    uint32_t InterruptStatus_u32 = 0U;

    if(0UL == ExclusiveAreaNestedCounter_FLS_EXCLUSIVE_AREA_30)
    {
        InterruptStatus_u32 = Fls_Schm_Read_Msr();
        /* If the interrupt has been enabled, the interrupt will be disabled */
        if(INTERRUPT_IS_ENABLED(InterruptStatus_u32))
        {
            Osr_SuspendAllInterrupts();
        }
        InterruptNestedStatus_FLS_EXCLUSIVE_AREA_30 = InterruptStatus_u32;
    }
    ExclusiveAreaNestedCounter_FLS_EXCLUSIVE_AREA_30++;
}

void SchM_Exit_Fls_FLS_EXCLUSIVE_AREA_30(void)
{
    ExclusiveAreaNestedCounter_FLS_EXCLUSIVE_AREA_30--;
    /* If the interrupt has been enabled before disabling it when entering the exclusive area, the interrupt will be enabled */
    if((INTERRUPT_IS_ENABLED(InterruptNestedStatus_FLS_EXCLUSIVE_AREA_30)) && (0UL == ExclusiveAreaNestedCounter_FLS_EXCLUSIVE_AREA_30))
    {
        Osr_ResumeAllInterrupts();
    }
}

void SchM_Enter_Fls_FLS_EXCLUSIVE_AREA_31(void)
{
    uint32_t InterruptStatus_u32 = 0U;

    if(0UL == ExclusiveAreaNestedCounter_FLS_EXCLUSIVE_AREA_31)
    {
        InterruptStatus_u32 = Fls_Schm_Read_Msr();
        /* If the interrupt has been enabled, the interrupt will be disabled */
        if(INTERRUPT_IS_ENABLED(InterruptStatus_u32))
        {
            Osr_SuspendAllInterrupts();
        }
        InterruptNestedStatus_FLS_EXCLUSIVE_AREA_31 = InterruptStatus_u32;
    }
    ExclusiveAreaNestedCounter_FLS_EXCLUSIVE_AREA_31++;
}

void SchM_Exit_Fls_FLS_EXCLUSIVE_AREA_31(void)
{
    ExclusiveAreaNestedCounter_FLS_EXCLUSIVE_AREA_31--;
    /* If the interrupt has been enabled before disabling it when entering the exclusive area, the interrupt will be enabled */
    if((INTERRUPT_IS_ENABLED(InterruptNestedStatus_FLS_EXCLUSIVE_AREA_31)) && (0UL == ExclusiveAreaNestedCounter_FLS_EXCLUSIVE_AREA_31))
    {
        Osr_ResumeAllInterrupts();
    }
}

void SchM_Enter_Fls_FLS_EXCLUSIVE_AREA_32(void)
{
    uint32_t InterruptStatus_u32 = 0U;

    if(0UL == ExclusiveAreaNestedCounter_FLS_EXCLUSIVE_AREA_32)
    {
        InterruptStatus_u32 = Fls_Schm_Read_Msr();
        /* If the interrupt has been enabled, the interrupt will be disabled */
        if(INTERRUPT_IS_ENABLED(InterruptStatus_u32))
        {
            Osr_SuspendAllInterrupts();
        }
        InterruptNestedStatus_FLS_EXCLUSIVE_AREA_32 = InterruptStatus_u32;
    }
    ExclusiveAreaNestedCounter_FLS_EXCLUSIVE_AREA_32++;
}

void SchM_Exit_Fls_FLS_EXCLUSIVE_AREA_32(void)
{
    ExclusiveAreaNestedCounter_FLS_EXCLUSIVE_AREA_32--;
    /* If the interrupt has been enabled before disabling it when entering the exclusive area, the interrupt will be enabled */
    if((INTERRUPT_IS_ENABLED(InterruptNestedStatus_FLS_EXCLUSIVE_AREA_32)) && (0UL == ExclusiveAreaNestedCounter_FLS_EXCLUSIVE_AREA_32))
    {
        Osr_ResumeAllInterrupts();
    }
}

void SchM_Enter_Fls_FLS_EXCLUSIVE_AREA_33(void)
{
    uint32_t InterruptStatus_u32 = 0U;

    if(0UL == ExclusiveAreaNestedCounter_FLS_EXCLUSIVE_AREA_33)
    {
        InterruptStatus_u32 = Fls_Schm_Read_Msr();
        /* If the interrupt has been enabled, the interrupt will be disabled */
        if(INTERRUPT_IS_ENABLED(InterruptStatus_u32))
        {
            Osr_SuspendAllInterrupts();
        }
        InterruptNestedStatus_FLS_EXCLUSIVE_AREA_33 = InterruptStatus_u32;
    }
    ExclusiveAreaNestedCounter_FLS_EXCLUSIVE_AREA_33++;
}

void SchM_Exit_Fls_FLS_EXCLUSIVE_AREA_33(void)
{
    ExclusiveAreaNestedCounter_FLS_EXCLUSIVE_AREA_33--;
    /* If the interrupt has been enabled before disabling it when entering the exclusive area, the interrupt will be enabled */
    if((INTERRUPT_IS_ENABLED(InterruptNestedStatus_FLS_EXCLUSIVE_AREA_33)) && (0UL == ExclusiveAreaNestedCounter_FLS_EXCLUSIVE_AREA_33))
    {
        Osr_ResumeAllInterrupts();
    }
}

void SchM_Enter_Fls_FLS_EXCLUSIVE_AREA_34(void)
{
    uint32_t InterruptStatus_u32 = 0U;

    if(0UL == ExclusiveAreaNestedCounter_FLS_EXCLUSIVE_AREA_34)
    {
        InterruptStatus_u32 = Fls_Schm_Read_Msr();
        /* If the interrupt has been enabled, the interrupt will be disabled */
        if(INTERRUPT_IS_ENABLED(InterruptStatus_u32))
        {
            Osr_SuspendAllInterrupts();
        }
        InterruptNestedStatus_FLS_EXCLUSIVE_AREA_34 = InterruptStatus_u32;
    }
    ExclusiveAreaNestedCounter_FLS_EXCLUSIVE_AREA_34++;
}

void SchM_Exit_Fls_FLS_EXCLUSIVE_AREA_34(void)
{
    ExclusiveAreaNestedCounter_FLS_EXCLUSIVE_AREA_34--;
    /* If the interrupt has been enabled before disabling it when entering the exclusive area, the interrupt will be enabled */
    if((INTERRUPT_IS_ENABLED(InterruptNestedStatus_FLS_EXCLUSIVE_AREA_34)) && (0UL == ExclusiveAreaNestedCounter_FLS_EXCLUSIVE_AREA_34))
    {
        Osr_ResumeAllInterrupts();
    }
}

void SchM_Enter_Fls_FLS_EXCLUSIVE_AREA_35(void)
{
    uint32_t InterruptStatus_u32 = 0U;

    if(0UL == ExclusiveAreaNestedCounter_FLS_EXCLUSIVE_AREA_35)
    {
        InterruptStatus_u32 = Fls_Schm_Read_Msr();
        /* If the interrupt has been enabled, the interrupt will be disabled */
        if(INTERRUPT_IS_ENABLED(InterruptStatus_u32))
        {
            Osr_SuspendAllInterrupts();
        }
        InterruptNestedStatus_FLS_EXCLUSIVE_AREA_35 = InterruptStatus_u32;
    }
    ExclusiveAreaNestedCounter_FLS_EXCLUSIVE_AREA_35++;
}

void SchM_Exit_Fls_FLS_EXCLUSIVE_AREA_35(void)
{
    ExclusiveAreaNestedCounter_FLS_EXCLUSIVE_AREA_35--;
    /* If the interrupt has been enabled before disabling it when entering the exclusive area, the interrupt will be enabled */
    if((INTERRUPT_IS_ENABLED(InterruptNestedStatus_FLS_EXCLUSIVE_AREA_35)) && (0UL == ExclusiveAreaNestedCounter_FLS_EXCLUSIVE_AREA_35))
    {
        Osr_ResumeAllInterrupts();
    }
}

void SchM_Enter_Fls_FLS_EXCLUSIVE_AREA_36(void)
{
    uint32_t InterruptStatus_u32 = 0U;

    if(0UL == ExclusiveAreaNestedCounter_FLS_EXCLUSIVE_AREA_36)
    {
        InterruptStatus_u32 = Fls_Schm_Read_Msr();
        /* If the interrupt has been enabled, the interrupt will be disabled */
        if(INTERRUPT_IS_ENABLED(InterruptStatus_u32))
        {
            Osr_SuspendAllInterrupts();
        }
        InterruptNestedStatus_FLS_EXCLUSIVE_AREA_36 = InterruptStatus_u32;
    }
    ExclusiveAreaNestedCounter_FLS_EXCLUSIVE_AREA_36++;
}

void SchM_Exit_Fls_FLS_EXCLUSIVE_AREA_36(void)
{
    ExclusiveAreaNestedCounter_FLS_EXCLUSIVE_AREA_36--;
    /* If the interrupt has been enabled before disabling it when entering the exclusive area, the interrupt will be enabled */
    if((INTERRUPT_IS_ENABLED(InterruptNestedStatus_FLS_EXCLUSIVE_AREA_36)) && (0UL == ExclusiveAreaNestedCounter_FLS_EXCLUSIVE_AREA_36))
    {
        Osr_ResumeAllInterrupts();
    }
}

void SchM_Enter_Fls_FLS_EXCLUSIVE_AREA_37(void)
{
    uint32_t InterruptStatus_u32 = 0U;

    if(0UL == ExclusiveAreaNestedCounter_FLS_EXCLUSIVE_AREA_37)
    {
        InterruptStatus_u32 = Fls_Schm_Read_Msr();
        /* If the interrupt has been enabled, the interrupt will be disabled */
        if(INTERRUPT_IS_ENABLED(InterruptStatus_u32))
        {
            Osr_SuspendAllInterrupts();
        }
        InterruptNestedStatus_FLS_EXCLUSIVE_AREA_37 = InterruptStatus_u32;
    }
    ExclusiveAreaNestedCounter_FLS_EXCLUSIVE_AREA_37++;
}

void SchM_Exit_Fls_FLS_EXCLUSIVE_AREA_37(void)
{
    ExclusiveAreaNestedCounter_FLS_EXCLUSIVE_AREA_37--;
    /* If the interrupt has been enabled before disabling it when entering the exclusive area, the interrupt will be enabled */
    if((INTERRUPT_IS_ENABLED(InterruptNestedStatus_FLS_EXCLUSIVE_AREA_37)) && (0UL == ExclusiveAreaNestedCounter_FLS_EXCLUSIVE_AREA_37))
    {
        Osr_ResumeAllInterrupts();
    }
}

void SchM_Enter_Fls_FLS_EXCLUSIVE_AREA_38(void)
{
    uint32_t InterruptStatus_u32 = 0U;

    if(0UL == ExclusiveAreaNestedCounter_FLS_EXCLUSIVE_AREA_38)
    {
        InterruptStatus_u32 = Fls_Schm_Read_Msr();
        /* If the interrupt has been enabled, the interrupt will be disabled */
        if(INTERRUPT_IS_ENABLED(InterruptStatus_u32))
        {
            Osr_SuspendAllInterrupts();
        }
        InterruptNestedStatus_FLS_EXCLUSIVE_AREA_38 = InterruptStatus_u32;
    }
    ExclusiveAreaNestedCounter_FLS_EXCLUSIVE_AREA_38++;
}

void SchM_Exit_Fls_FLS_EXCLUSIVE_AREA_38(void)
{
    ExclusiveAreaNestedCounter_FLS_EXCLUSIVE_AREA_38--;
    /* If the interrupt has been enabled before disabling it when entering the exclusive area, the interrupt will be enabled */
    if((INTERRUPT_IS_ENABLED(InterruptNestedStatus_FLS_EXCLUSIVE_AREA_38)) && (0UL == ExclusiveAreaNestedCounter_FLS_EXCLUSIVE_AREA_38))
    {
        Osr_ResumeAllInterrupts();
    }
}

void SchM_Enter_Fls_FLS_EXCLUSIVE_AREA_39(void)
{
    uint32_t InterruptStatus_u32 = 0U;

    if(0UL == ExclusiveAreaNestedCounter_FLS_EXCLUSIVE_AREA_39)
    {
        InterruptStatus_u32 = Fls_Schm_Read_Msr();
        /* If the interrupt has been enabled, the interrupt will be disabled */
        if(INTERRUPT_IS_ENABLED(InterruptStatus_u32))
        {
            Osr_SuspendAllInterrupts();
        }
        InterruptNestedStatus_FLS_EXCLUSIVE_AREA_39 = InterruptStatus_u32;
    }
    ExclusiveAreaNestedCounter_FLS_EXCLUSIVE_AREA_39++;
}

void SchM_Exit_Fls_FLS_EXCLUSIVE_AREA_39(void)
{
    ExclusiveAreaNestedCounter_FLS_EXCLUSIVE_AREA_39--;
    /* If the interrupt has been enabled before disabling it when entering the exclusive area, the interrupt will be enabled */
    if((INTERRUPT_IS_ENABLED(InterruptNestedStatus_FLS_EXCLUSIVE_AREA_39)) && (0UL == ExclusiveAreaNestedCounter_FLS_EXCLUSIVE_AREA_39))
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

