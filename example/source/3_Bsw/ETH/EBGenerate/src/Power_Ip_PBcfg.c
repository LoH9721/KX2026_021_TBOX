/***********************************************************************************************************************
 * Project Name: XHSC AUTOSAR MCAL
 * File Name: Power_Ip_PBcfg.c
 *
 * Description: Configuration file for MCU module
 *              Compiler:                IAR GCC
 *              AutoSAR Version:         4.4.0
 *
 *
 * Copyright (c) 2022 - 2025 Xiaohua Semiconductor Co., Ltd.
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
 *   @file       Power_Ip_PBcfg.c
 *   @version    V1.0.3
 *   @brief      Autosar 4.4.0 MCU Driver
 *   @addtogroup mcu_driver
 *   @{
 */

#ifdef __cplusplus
extern "C"{
#endif

/***********************************************************************************************************************
 *  INCLUDES
 **********************************************************************************************************************/
#include "Power_Ip_PBcfg.h"
#include "Power_Ip.h"
#include "Clock_Ip_Types.h"
#include "StandardTypes.h"

/***********************************************************************************************************************
 *   VERSION CHECK
 **********************************************************************************************************************/

/***********************************************************************************************************************
 *  DEFINES & MACROS
 **********************************************************************************************************************/

/***********************************************************************************************************************
 *  EXTERN
 **********************************************************************************************************************/

/***********************************************************************************************************************
 *  TYPEDEFS
 **********************************************************************************************************************/

/***********************************************************************************************************************
 *  CONSTANTS
 **********************************************************************************************************************/

/***********************************************************************************************************************
 *  GLOBAL DATA TYPES
 **********************************************************************************************************************/
#define MCU_START_SEC_CONFIG_DATA_UNSPECIFIED
#include "Mcu_MemMap.h"




/* --------------------- PostBuild for Sleep Mode -------------------------- */
/* Power_Ip_TurnOffClockSourceType */
static const Power_Ip_TurnOffClockSourceType l_Power_Ip_SleepModeClockSource_0_st = {
            .Xtal32Control = PWC_WORKING,
            .HRCControl = PWC_WORKING,
            .MRCControl = PWC_WORKING,
            .XtalControl = PWC_WORKING,
            .LRCControl = PWC_WORKING,
            .PLLHControl = PWC_WORKING,
            .PLLAControl = PWC_WORKING,
            .Reserved = 0U
};

static const Power_Ip_FCG0Type l_Power_Ip_SleepModeFuncClockFCG0_0_st = {
            /* DCU8 */
            .DCU8 = 0U,
            /* DCU7 */
            .DCU7 = 0U,
            /* DCU6 */
            .DCU6 = 0U,
            /* DCU4 */
            .DCU4 = 0U,
            /* DCU5 */
            .DCU5 = 0U,
            /* DCU2 */
            .DCU2 = 0U,
            /* DCU3 */
            .DCU3 = 0U,
            /* DCU1 */
            .DCU1 = 0U,
            /* CRC */
            .CRC = 0U,
            /* TRNG */
            .TRNG = 0U,
            /* HASH */
            .HASH = 0U,
            /* SKE_AES */
            .SKE_AES = 0U,
            /* MAU */
            .MAU = 0U,
            /* CTC */
            .CTC = 0U,
            /* AOS */
            .AOS = 0U,
            /* FCM */
            .FCM = 0U,
            /* DMA2 */
            .DMA2 = 0U,
            /* DMA1 */
            .DMA1 = 0U,
            /* KEY */
            .KEY = 0U,
            /* PKE */
            .PKE = 0U,
            /* Reserved1 */
            .Reserved1 = 0U,
            /* SRAMB */
            .SRAMB = 0U,
            /* Reserved2 */
            .Reserved2 = 0U,
            /* ERMU */
            .ERMU = 0U,
            /* SRAM4 */
            .SRAM4 = 0U,
            /* SRAM3 */
            .SRAM3 = 0U,
            /* SRAM2 */
            .SRAM2 = 0U,
            /* SRAM1 */
            .SRAM1 = 0U,
            /* Reserved3 */
            .Reserved3 = 0U,
            /* SRAMH */
            .SRAMH = 0U
};

static const Power_Ip_FCG1Type l_Power_Ip_SleepModeFuncClockFCG1_0_st = 
{
            /* Reserved */
            .Reserved = 0U,
            /* MCAN2 */
            .MCAN2 = 0U,
            /* MCAN1 */
            .MCAN1 = 0U,
            /* FMAC4 */
            .FMAC4 = 0U,
            /* FMAC3 */
            .FMAC3 = 0U,
            /* FMAC2 */
            .FMAC2 = 0U,
            /* FMAC1 */
            .FMAC1 = 0U,
            /* USBHS */
            .USBHS = 1U,
            /* USBFS */
            .USBFS = 1U,
            /* SPI6 */
            .SPI6 = 0U,
            /* SPI5 */
            .SPI5 = 0U,
            /* SPI4 */
            .SPI4 = 0U,
            /* SPI3 */
            .SPI3 = 0U,
            /* SPI2 */
            .SPI2 = 0U,
            /* SPI1 */
            .SPI1 = 0U,
            /* I2S4 */
            .I2S4 = 0U,
            /* I2S3 */
            .I2S3 = 0U,
            /* I2S2 */
            .I2S2 = 0U,
            /* I2S1 */
            .I2S1 = 0U,
            /* SDIOC2 */
            .SDIOC2 = 0U,
            /* SDIOC1 */
            .SDIOC1 = 0U,
            /* I2C6 */
            .I2C6 = 0U,
            /* I2C5 */
            .I2C5 = 0U,
            /* I2C4 */
            .I2C4 = 0U,
            /* I2C3 */
            .I2C3 = 0U,
            /* I2C2 */
            .I2C2 = 0U,
            /* I2C1 */
            .I2C1 = 0U,
            /* QSPI */
            .QSPI = 0U,
            /* ETHMAC */
            .ETHMAC = 0U,
            /* CAN2 */
            .CAN2 = 0U,
            /* CAN1 */
            .CAN1 = 0U
};

static const Power_Ip_FCG2Type l_Power_Ip_SleepModeFuncClockFCG2_0_st = {

            /* TMRA_12 */
            .TMRA_12 = 0U,
            /* TMRA_11 */
            .TMRA_11 = 0U,
            /* TMRA_10 */
            .TMRA_10 = 0U,
            /* TMRA_9 */
            .TMRA_9 = 0U,
            /* TMRA_8 */
            .TMRA_8 = 0U,
            /* TMRA_7 */
            .TMRA_7 = 0U,
            /* TMRA_6 */
            .TMRA_6 = 0U,
            /* TMRA_5 */
            .TMRA_5 = 0U,
            /* TMRA_4 */
            .TMRA_4 = 0U,
            /* TMRA_3 */
            .TMRA_3 = 0U,
            /* TMRA_2 */
            .TMRA_2 = 0U,
            /* TMRA_1 */
            .TMRA_1 = 0U,
            /* TMR2_4 */
            .TMR2_4 = 0U,
            /* TMR2_3 */
            .TMR2_3 = 0U,
            /* TMR2_2 */
            .TMR2_2 = 0U,
            /* TMR2_1 */
            .TMR2_1 = 0U,
            /* EMB */
            .EMB = 0U,
            /* TMR0_3 */
            .TMR0_3 = 0U,
            /* TMR0_2 */
            .TMR0_2 = 0U,
            /* TMR0_1 */
            .TMR0_1 = 0U,
            /* HRPWM */
            .HRPWM = 0U,
            /* TMR4_3 */
            .TMR4_3 = 0U,
            /* TMR4_2 */
            .TMR4_2 = 0U,
            /* TMR4_1 */
            .TMR4_1 = 0U, 
            /* TMR6_8 */
            .TMR6_8 = 0U,
            /* TMR6_7 */
            .TMR6_7 = 0U,
            /* TMR6_6 */
            .TMR6_6 = 0U,
            /* TMR6_5 */
            .TMR6_5 = 0U,
            /* TMR6_4 */
            .TMR6_4 = 0U,
            /* TMR6_3 */
            .TMR6_3 = 0U,
            /* TMR6_2 */
            .TMR6_2 = 0U,
            /* TMR6_1 */
            .TMR6_1 = 0U
};

static const Power_Ip_FCG3Type l_Power_Ip_SleepModeFuncClockFCG3_0_st = {

            /* Reserved1 */
            .Reserved1 = 0U,
            /* USART10 */
            .USART10 = 0U,
            /* USART9 */
            .USART9 = 0U,
            /* USART8 */
            .USART8 = 0U,
            /* USART7 */
            .USART7 = 0U,
            /* USART6 */
            .USART6 = 0U,
            /* USART5 */
            .USART5 = 0U,
            /* USART4 */
            .USART4 = 0U,
            /* USART3 */
            .USART3 = 0U,
            /* USART2 */
            .USART2 = 0U,
            /* USART1 */
            .USART1 = 0U,
            /* Reserved2 */
            .Reserved2 = 0U,
            /* NFC */
            .NFC = 0U,
            /* DMC */
            .DMC = 0U,
            /* SMC */
            .SMC = 0U,
            /* DVP */
            .DVP = 0U,
            /* Reserved3 */
            .Reserved3 = 0U,
            /* OTS */
            .OTS = 0U,
            /* TMR0_5 */
            .TMR0_5 = 0U,
            /* TMR0_4 */
            .TMR0_4 = 0U,
            /* CMP34 */
            .CMP34 = 0U,
            /* CMP12 */
            .CMP12 = 0U,
            /* Reserved4 */
            .Reserved4 = 0U,
            /* DAC2 */
            .DAC2 = 0U,
            /* DAC1 */
            .DAC1 = 0U,
            /* CMBIAS */
            .CMBIAS = 0U,
            /* ADC3 */
            .ADC3 = 0U,
            /* ADC2 */
            .ADC2 = 0U,
            /* ADC1 */
            .ADC1 = 0U
};

/* TurnOffUnusedRam */
static const Power_Ip_TurnoffRamType l_Power_Ip_SleepModeRamTurnoff_0_st = {

            /* RAMPDC10 */
            .RAMPDC10 = 0U,
            /* RAMPDC9 */
            .RAMPDC9 = 0U,
            /* RAMPDC8 */
            .RAMPDC8 = 0U,
            /* RAMPDC7 */
            .RAMPDC7 = 0U,
            /* RAMPDC6 */
            .RAMPDC6 = 0U,
            /* RAMPDC5 */
            .RAMPDC5 = 0U,
            /* RAMPDC4 */
            .RAMPDC4 = 0U,
            /* RAMPDC3 */
            .RAMPDC3 = 0U,
            /* RAMPDC2 */
            .RAMPDC2 = 0U,
            /* RAMPDC1 */
            .RAMPDC1 = 0U,
            /* RAMPDC0 */
            .RAMPDC0 = 0U,
            /* PRAMPDC11 */
            .PRAMPDC11 = 0U,
            /* PRAMPDC10 */
            .PRAMPDC10 = 0U,
            /* PRAMPDC9 */
            .PRAMPDC9 = 0U,
            /* PRAMPDC8 */
            .PRAMPDC8 = 0U,
            /* PRAMPDC7 */
            .PRAMPDC7 = 0U,
            /* PRAMPDC6 */
            .PRAMPDC6 = 0U,
            /* PRAMPDC5 */
            .PRAMPDC5 = 0U,
            /* PRAMPDC4 */
            .PRAMPDC4 = 0U,
            /* PRAMPDC3 */
            .PRAMPDC3 = 0U,
            /* PRAMPDC2 */
            .PRAMPDC2 = 0U,
            /* PRAMPDC1 */
            .PRAMPDC1 = 0U,
            /* PRAMPDC0 */
            .PRAMPDC0 = 0U,
            /* SRAMB */
            .SRAMB = 0U,
            /* Reserved */
            .Reserved = 0U
};

static const Power_Ip_ConfigPeripheralType l_Power_Ip_SleepModePeripheral_0_st = {
            .ClockSource_ptr = &l_Power_Ip_SleepModeClockSource_0_st,
            .FuncClockFCG0_ptr = &l_Power_Ip_SleepModeFuncClockFCG0_0_st,
            .FuncClockFCG1_ptr = &l_Power_Ip_SleepModeFuncClockFCG1_0_st,
            .FuncClockFCG2_ptr = &l_Power_Ip_SleepModeFuncClockFCG2_0_st,
            .FuncClockFCG3_ptr = &l_Power_Ip_SleepModeFuncClockFCG3_0_st,
            .RamTurnoff_ptr = &l_Power_Ip_SleepModeRamTurnoff_0_st,
};

/* Power_Ip_SleepModeCfgType */
static const Power_Ip_SleepModeCfgType l_Power_Ip_SleepModeCfg_0_st = {
            /* .Peripheral_ptr = */                       
            .Peripheral_ptr = &l_Power_Ip_SleepModePeripheral_0_st
};

/* --------------------- PostBuild for Stop Mode -------------------------- */
/* Power_Ip_StopModeWKUSrcType */
static const Power_Ip_StopModeWKUSrcType l_Power_Ip_StopModeWKUSrc_1_st = {
            /* EIRQ0  */
            .EIRQ0 = 1U,
            /* EIRQ1  */
            .EIRQ1 = 0U,
            /* EIRQ2  */
            .EIRQ2 = 0U,
            /* EIRQ3  */
            .EIRQ3 = 0U,
            /** EIRQ4  */
            .EIRQ4 = 0U,
            /** EIRQ5  */
            .EIRQ5 = 0U,
            /** EIRQ6  */
            .EIRQ6 = 0U,
            /** EIRQ7  */
            .EIRQ7 = 0U,
            /** EIRQ8  */
            .EIRQ8 = 0U,
            /** EIRQ9  */
            .EIRQ9 = 0U,
            /** EIRQ10  */
            .EIRQ10 = 0U,
            /** EIRQ11 */
            .EIRQ11 = 0U,
            /** EIRQ12 */
            .EIRQ12 = 0U,
            /** EIRQ13 */
            .EIRQ13 = 0U,
            /** EIRQ14 */
            .EIRQ14 = 0U,
            /** EIRQ15 */
            .EIRQ15 = 0U,
            /** SWDT */
            .SWDT = 0U,
            /** PVD1 */
            .PVD1 = 0U,
            /** PVD2_Enable */
            .PVD2 = 0U,
            /** CMP_Enable */
            .CMP = 0U,
            /** WKTM_Enable */
            .WKTM = 0U,
            /** RTCALM_Enable */
            .RTCALM = 0U,
            /** RTCPRD_Enable */
            .RTCPRD = 0U,
            /** TMR0CMP_Enable */
            .TMR0CMP = 0U,
            /** TMR2GCM_Enable */
            .TMR2GCM = 1U,
            /** TMR2OVF_Enable */
            .TMR2OVF = 1U,
            /** USART1RX_Enable */
            .USART1RX = 1U,
            /** USBHS_Enable */
            .USBHS = 0U,
            /** USBFS_Enable */
            .USBFS = 0U,
            /** ETHPMT_Enable */
            .ETHPMT = 0U,
            /* Reserved */
            .Reserved = 0U
};

/* Power_Ip_TurnOffClockSourceType */
static const Power_Ip_TurnOffClockSourceType l_Power_Ip_StopModeClockSource_1_st = {
            /* Xtal32Control */
            .Xtal32Control = PWC_WORKING,
            /* HRCControl */
            .HRCControl = PWC_STOP,
            /* MRCControl */
            .MRCControl = PWC_STOP,
            /* XtalControl */
            .XtalControl = PWC_STOP,
            /* LRCControl */
            .LRCControl = PWC_WORKING,
            /* PLLHControl */
            .PLLHControl = PWC_STOP,
            /* PLLAControl */
            .PLLAControl = PWC_STOP,
            /* Reserved */
            .Reserved = 0U
};

static const Power_Ip_FCG0Type l_Power_Ip_StopModeFuncClockFCG0_1_st = {
            /* DCU8 */
            .DCU8 = 1U,
            /* DCU7 */
            .DCU7 = 1U,
            /* DCU6 */
            .DCU6 = 1U,
            /* DCU4 */
            .DCU4 = 1U,
            /* DCU5 */
            .DCU5 = 1U,
            /* DCU2 */
            .DCU2 = 1U,
            /* DCU3 */
            .DCU3 = 1U,
            /* DCU1 */
            .DCU1 = 1U,
            /* CRC */
            .CRC = 1U,
            /* TRNG */
            .TRNG = 1U,
            /* HASH */
            .HASH = 1U,
            /* SKE_AES   */
            .SKE_AES = 1U,
            /* MAU */
            .MAU = 1U,
            /* CTC */
            .CTC = 1U,
            /* AOS */
            .AOS = 1U,
            /* FCM */
            .FCM = 1U,
            /* DMA2 */
            .DMA2 = 1U,
            /* DMA1 */
            .DMA1 = 1U,
            /* KEY */
            .KEY = 1U,
            /* PKE */
            .PKE = 1U,
            /* Reserved1 */
            .Reserved1 = 0U,
            /* SRAMB */
            .SRAMB = 1U,
            /* Reserved2 */
            .Reserved2 = 0U,
            /* ERMU */
            .ERMU = 1U,
            /* SRAM4 */
            .SRAM4 = 1U,
            /* SRAM3 */
            .SRAM3 = 1U,
            /* SRAM2 */
            .SRAM2 = 1U,
            /* SRAM1 */
            .SRAM1 = 1U,
            /* Reserved3 */
            .Reserved3 = 0U,
            /* SRAMH */
            .SRAMH = 1U
};

static const Power_Ip_FCG1Type l_Power_Ip_StopModeFuncClockFCG1_1_st = 
{
            /* Reserved */
            .Reserved = 0U,
            /* MCAN2 */
            .MCAN2 = 0U,
            /* MCAN1 */
            .MCAN1 = 0U,
            /* FMAC4 */
            .FMAC4 = 1U,
            /* FMAC3 */
            .FMAC3 = 1U,
            /* FMAC2 */
            .FMAC2 = 1U,
            /* FMAC1 */
            .FMAC1 = 1U,
            /* USBHS */
            .USBHS = 0U,
            /* USBFS */
            .USBFS = 0U,
            /* SPI6 */
            .SPI6 = 1U,
            /* SPI5 */
            .SPI5 = 1U,
            /* SPI4 */
            .SPI4 = 1U,
            /* SPI3 */
            .SPI3 = 1U,
            /* SPI2 */
            .SPI2 = 1U,
            /* SPI1 */
            .SPI1 = 1U,
            /* I2S4 */
            .I2S4 = 1U,
            /* I2S3 */
            .I2S3 = 1U,
            /* I2S2 */
            .I2S2 = 1U,
            /* I2S1 */
            .I2S1 = 1U,
            /* SDIOC2 */
            .SDIOC2 = 1U,
            /* SDIOC1 */
            .SDIOC1 = 1U,
            /* I2C6 */
            .I2C6 = 1U,
            /* I2C5 */
            .I2C5 = 1U,
            /* I2C4 */
            .I2C4 = 1U,
            /* I2C3 */
            .I2C3 = 1U,
            /* I2C2 */
            .I2C2 = 1U,
            /* I2C1 */
            .I2C1 = 1U,
            /* QSPI */
            .QSPI = 1U,
            /* ETHMAC */
            .ETHMAC = 1U,
            /* CAN2 */
            .CAN2 = 1U,
            /* CAN1 */
            .CAN1 = 1U
};

static const Power_Ip_FCG2Type l_Power_Ip_StopModeFuncClockFCG2_1_st = {
            /* TMRA_12 */
            .TMRA_12 = 1U,
            /* TMRA_11 */
            .TMRA_11 = 1U,
            /* TMRA_10 */
            .TMRA_10 = 1U,
            /* TMRA_9 */
            .TMRA_9 = 1U,
            /* TMRA_8 */
            .TMRA_8 = 1U,
            /* TMRA_7 */
            .TMRA_7 = 1U,
            /* TMRA_6 */
            .TMRA_6 = 1U,
            /* TMRA_5 */
            .TMRA_5 = 1U,
            /* TMRA_4 */
            .TMRA_4 = 1U,
            /* TMRA_3 */
            .TMRA_3 = 1U,
            /* TMRA_2 */
            .TMRA_2 = 1U,
            /* TMRA_1 */
            .TMRA_1 = 1U,
            /* TMR2_4 */
            .TMR2_4 = 0U,
            /* TMR2_3 */
            .TMR2_3 = 0U,
            /* TMR2_2 */
            .TMR2_2 = 0U,
            /* TMR2_1 */
            .TMR2_1 = 0U,
            /* EMB */
            .EMB = 1U,
            /* TMR0_3 */
            .TMR0_3 = 0U,
            /* TMR0_2 */
            .TMR0_2 = 0U,
            /* TMR0_1 */
            .TMR0_1 = 0U,
            /* HRPWM */
            .HRPWM = 1U,
            /* TMR4_3 */
            .TMR4_3 = 1U,
            /* TMR4_2 */
            .TMR4_2 = 1U,
            /* TMR4_1 */
            .TMR4_1 = 1U, 
            /* TMR6_8 */
            .TMR6_8 = 1U,
            /* TMR6_7 */
            .TMR6_7 = 1U,
            /* TMR6_6 */
            .TMR6_6 = 1U,
            /* TMR6_5 */
            .TMR6_5 = 1U,
            /* TMR6_4 */
            .TMR6_4 = 1U,
            /* TMR6_3 */
            .TMR6_3 = 1U,
            /* TMR6_2 */
            .TMR6_2 = 1U,
            /* TMR6_1 */
            .TMR6_1 = 1U
};

static const Power_Ip_FCG3Type l_Power_Ip_StopModeFuncClockFCG3_1_st = {

            /* Reserved1 */
            .Reserved1 = 0U,
            /* USART10 */
            .USART10 = 1U,
            /* USART9 */
            .USART9 = 1U,
            /* USART8 */
            .USART8 = 1U,
            /* USART7 */
            .USART7 = 1U,
            /* USART6 */
            .USART6 = 1U,
            /* USART5 */
            .USART5 = 1U,
            /* USART4 */
            .USART4 = 1U,
            /* USART3 */
            .USART3 = 1U,
            /* USART2 */
            .USART2 = 1U,
            /* USART1 */
            .USART1 = 1U,
            /* Reserved2 */
            .Reserved2 = 0U,
            /* NFC */
            .NFC = 1U,
            /* DMC */
            .DMC = 1U,
            /* SMC */
            .SMC = 1U,
            /* DVP */
            .DVP = 1U,
            /* Reserved3 */
            .Reserved3 = 0U,
            /* OTS */
            .OTS = 1U,
            /* TMR0_5 */
            .TMR0_5 = 1U,
            /* TMR0_4 */
            .TMR0_4 = 1U,
            /* CMP34 */
            .CMP34 = 0U,
            /* CMP12 */
            .CMP12 = 0U,
            /* Reserved4 */
            .Reserved4 = 0U,
            /* DAC2 */
            .DAC2 = 1U,
            /* DAC1 */
            .DAC1 = 1U,
            /* CMBIAS */
            .CMBIAS = 1U,
            /* ADC3 */
            .ADC3 = 1U,
            /* ADC2 */
            .ADC2 = 1U,
            /* ADC1 */
            .ADC1 = 1U
};

static const Power_Ip_TurnoffRamType l_Power_Ip_StopModeRamTurnoff_1_st = {

            /* RAMPDC10 */
            .RAMPDC10 = 1U,
            /* RAMPDC9 */
            .RAMPDC9 = 1U,
            /* RAMPDC8 */
            .RAMPDC8 = 1U,
            /* RAMPDC7 */
            .RAMPDC7 = 1U,
            /* RAMPDC6 */
            .RAMPDC6 = 1U,
            /* RAMPDC5 */
            .RAMPDC5 = 1U,
            /* RAMPDC4 */
            .RAMPDC4 = 1U,
            /* RAMPDC3 */
            .RAMPDC3 = 1U,
            /* RAMPDC2 */
            .RAMPDC2 = 1U,
            /* RAMPDC1 */
            .RAMPDC1 = 1U,
            /* RAMPDC0 */
            .RAMPDC0 = 1U,
            /* PRAMPDC11 */
            .PRAMPDC11 = 1U,
            /* PRAMPDC10 */
            .PRAMPDC10 = 1U,
            /* PRAMPDC9 */
            .PRAMPDC9 = 1U,
            /* PRAMPDC8 */
            .PRAMPDC8 = 1U,
            /* PRAMPDC7 */
            .PRAMPDC7 = 1U,
            /* PRAMPDC6 */
            .PRAMPDC6 = 1U,
            /* PRAMPDC5 */
            .PRAMPDC5 = 1U,
            /* PRAMPDC4 */
            .PRAMPDC4 = 1U,
            /* PRAMPDC3 */
            .PRAMPDC3 = 1U,
            /* PRAMPDC2 */
            .PRAMPDC2 = 1U,
            /* PRAMPDC1 */
            .PRAMPDC1 = 1U,
            /* PRAMPDC0 */
            .PRAMPDC0 = 1U,
            /* SRAMB */
            .SRAMB = 1U,
            /* Reserved */
            .Reserved = 0U
};

/* Power_Ip_ConfigPeripheralType */
static const Power_Ip_ConfigPeripheralType l_Power_Ip_StopModePeripheral_1_st = {
            .ClockSource_ptr = &l_Power_Ip_StopModeClockSource_1_st,
            .FuncClockFCG0_ptr = &l_Power_Ip_StopModeFuncClockFCG0_1_st,
            .FuncClockFCG1_ptr = &l_Power_Ip_StopModeFuncClockFCG1_1_st,
            .FuncClockFCG2_ptr = &l_Power_Ip_StopModeFuncClockFCG2_1_st,
            .FuncClockFCG3_ptr = &l_Power_Ip_StopModeFuncClockFCG3_1_st,
            .RamTurnoff_ptr = &l_Power_Ip_StopModeRamTurnoff_1_st,
};

/* Power_Ip_StopModeCfgType */
static const Power_Ip_StopModeCfgType l_Power_Ip_StopModeCfg_1_st = {
            .McuStopModeState_u8 = (uint8)MCU_ENABLE,
            .Clock_en = PWC_STOP_CLK_KEEP,
            .StopDrv_en = PWC_STOP_DRV_HIGH,
            .ExBusHold_en = PWC_STOP_EXBUS_HIZ,
            .FlashWait_en = PWC_STOP_FLASH_WAIT_ON,
            .StopMode_Wkusrc_ptr = &l_Power_Ip_StopModeWKUSrc_1_st,
            .Peripheral_ptr = &l_Power_Ip_StopModePeripheral_1_st
};




/* --------------------- PostBuild for Power Down Mode -------------------------- */
/* Power_Ip_WKUPWDEnableType */
static const Power_Ip_WKUPWDEnableType l_Power_Ip_PwdWkuEnable_2_st = {
            /* PWC_PD_WKUP_WKUP13 */
            .PWC_PD_WKUP_WKUP13 = 0U,
            /* PWC_PD_WKUP_WKUP12 */
            .PWC_PD_WKUP_WKUP12 = 0U,
            /* PWC_PD_WKUP_WKUP11 */
            .PWC_PD_WKUP_WKUP11 = 0U,
            /* PWC_PD_WKUP_WKUP10 */
            .PWC_PD_WKUP_WKUP10 = 0U,
            /* PWC_PD_WKUP_WKUP03 */
            .PWC_PD_WKUP_WKUP03 = 0U,
            /* PWC_PD_WKUP_WKUP02 */
            .PWC_PD_WKUP_WKUP02 = 0U,
            /* PWC_PD_WKUP_WKUP01 */
            .PWC_PD_WKUP_WKUP01 = 0U,
            /* PWC_PD_WKUP_WKUP00 */
            .PWC_PD_WKUP_WKUP00 = 1U,
            /* PWC_PD_WKUP_WKUP33 */
            .PWC_PD_WKUP_WKUP33 = 0U,
            /* PWC_PD_WKUP_WKUP32 */
            .PWC_PD_WKUP_WKUP32 = 0U,
            /* PWC_PD_WKUP_WKUP31 */
            .PWC_PD_WKUP_WKUP31 = 0U,
            /* PWC_PD_WKUP_WKUP30 */
            .PWC_PD_WKUP_WKUP30 = 0U,
            /* PWC_PD_WKUP_WKUP23 */
            .PWC_PD_WKUP_WKUP23 = 0U,
            /* PWC_PD_WKUP_WKUP22 */
            .PWC_PD_WKUP_WKUP22 = 0U,
            /* PWC_PD_WKUP_WKUP21 */
            .PWC_PD_WKUP_WKUP21 = 0U,
            /* PWC_PD_WKUP_WKUP20 */
            .PWC_PD_WKUP_WKUP20 = 0U,
            /* PWC_PD_WKUP_WKTM */
            .PWC_PD_WKUP_WKTM = 1U,
            /* Reserved1 */
            .Reserved1 = 0U,
            /* PWC_PD_WKUP_RTCALM */
            .PWC_PD_WKUP_RTCALM = 0U,
            /* PWC_PD_WKUP_RTCPRD */
            .PWC_PD_WKUP_RTCPRD = 0U,
            /* Reserved2 */
            .Reserved2 = 0U,
            /* Reserved3 */
            .Reserved3 = 0U,
            /* PWC_PD_WKUP_PVD2 */
            .PWC_PD_WKUP_PVD2 = 0U,
            /* PWC_PD_WKUP_PVD1 */
            .PWC_PD_WKUP_PVD1 = 0U,
            /* Reserved */
            .Reserved = 0U
};

/* Power_Ip_PWDWKUEdgeSelType */
static const Power_Ip_PWDWKUEdgeSelType l_Power_Ip_PwdWkuEdgeSel_2_st = {
            .Reserved = 0U,
            .PWC_PD_WKUP_TRIG_PVD1 = PWC_PD_WKUP_TRIG_FALLING,
            .PWC_PD_WKUP_TRIG_PVD2 = PWC_PD_WKUP_TRIG_FALLING,
            .PWC_PD_WKUP_TRIG_WKUP0 = PWC_PD_WKUP_TRIG_FALLING,
            .PWC_PD_WKUP_TRIG_WKUP1 = PWC_PD_WKUP_TRIG_FALLING,
            .PWC_PD_WKUP_TRIG_WKUP2 = PWC_PD_WKUP_TRIG_FALLING,
            .PWC_PD_WKUP_TRIG_WKUP3 = PWC_PD_WKUP_TRIG_FALLING,
            .Reserved24 = 0U
};


/* Power_Ip_TurnOffClockSourceType */
static const Power_Ip_TurnOffClockSourceType l_Power_Ip_PwdModeClockSource_2_st = {
            .Xtal32Control = PWC_WORKING,
            .HRCControl = PWC_POWERDOWN,
            .MRCControl = PWC_POWERDOWN,
            .XtalControl = PWC_STOP,
            .LRCControl = PWC_WORKING,
            .PLLHControl = PWC_POWERDOWN,
            .PLLAControl = PWC_POWERDOWN,
            .Reserved = 0U
};

static const Power_Ip_FCG0Type l_Power_Ip_PwdModeFuncClockFCG0_2_st = {
            /* DCU8 */
            .DCU8 = 1U,
            /* DCU7 */
            .DCU7 = 1U,
            /* DCU6 */
            .DCU6 = 1U,
            /* DCU4 */
            .DCU4 = 1U,
            /* DCU5 */
            .DCU5 = 1U,
            /* DCU2 */
            .DCU2 = 1U,
            /* DCU3 */
            .DCU3 = 1U,
            /* DCU1 */
            .DCU1 = 1U,
            /* CRC */
            .CRC = 1U,
            /* TRNG */
            .TRNG = 1U,
            /* HASH */
            .HASH = 1U,
            /* SKE_AES */
            .SKE_AES = 1U,
            /* MAU */
            .MAU = 1U,
            /* CTC */
            .CTC = 1U,
            /* AOS */
            .AOS = 1U,
            /* FCM */
            .FCM = 1U,
            /* DMA2 */
            .DMA2 = 1U,
            /* DMA1 */
            .DMA1 = 1U,
            /* KEY */
            .KEY = 1U,
            /* PKE */
            .PKE = 0U,
            /* Reserved1 */
            .Reserved1 = 0U,
            /* SRAMB */
            .SRAMB = 1U,
            /* Reserved2 */
            .Reserved2 = 0U,
            /* ERMU */
            .ERMU = 0U,
            /* SRAM4 */
            .SRAM4 = 1U,
            /* SRAM3 */
            .SRAM3 = 1U,
            /* SRAM2 */
            .SRAM2 = 1U,
            /* SRAM1 */
            .SRAM1 = 1U,
            /* Reserved3 */
            .Reserved3 = 0U,
            /* SRAMH */
            .SRAMH = 1U
};

static const Power_Ip_FCG1Type l_Power_Ip_PwdModeFuncClockFCG1_2_st = 
{
            /* Reserved */
            .Reserved = 0U,
            /* MCAN2 */
            .MCAN2 = 0U,
            /* MCAN1 */
            .MCAN1 = 0U,
            /* FMAC4 */
            .FMAC4 = 1U,
            /* FMAC3 */
            .FMAC3 = 1U,
            /* FMAC2 */
            .FMAC2 = 1U,
            /* FMAC1 */
            .FMAC1 = 1U,
            /* USBHS */
            .USBHS = 0U,
            /* USBFS */
            .USBFS = 0U,
            /* SPI6 */
            .SPI6 = 1U,
            /* SPI5 */
            .SPI5 = 1U,
            /* SPI4 */
            .SPI4 = 1U,
            /* SPI3 */
            .SPI3 = 1U,
            /* SPI2 */
            .SPI2 = 1U,
            /* SPI1 */
            .SPI1 = 1U,
            /* I2S4 */
            .I2S4 = 1U,
            /* I2S3 */
            .I2S3 = 1U,
            /* I2S2 */
            .I2S2 = 1U,
            /* I2S1 */
            .I2S1 = 1U,
            /* SDIOC2 */
            .SDIOC2 = 1U,
            /* SDIOC1 */
            .SDIOC1 = 1U,
            /* I2C6 */
            .I2C6 = 1U,
            /* I2C5 */
            .I2C5 = 1U,
            /* I2C4 */
            .I2C4 = 1U,
            /* I2C3 */
            .I2C3 = 1U,
            /* I2C2 */
            .I2C2 = 1U,
            /* I2C1 */
            .I2C1 = 1U,
            /* QSPI */
            .QSPI = 1U,
            /* ETHMAC */
            .ETHMAC = 1U,
            /* CAN2 */
            .CAN2 = 1U,
            /* CAN1 */
            .CAN1 = 1U
};

static const Power_Ip_FCG2Type l_Power_Ip_PwdModeFuncClockFCG2_2_st = {

            /* TMRA_12 */
            .TMRA_12 = 1U,
            /* TMRA_11 */
            .TMRA_11 = 1U,
            /* TMRA_10 */
            .TMRA_10 = 1U,
            /* TMRA_9 */
            .TMRA_9 = 1U,
            /* TMRA_8 */
            .TMRA_8 = 1U,
            /* TMRA_7 */
            .TMRA_7 = 1U,
            /* TMRA_6 */
            .TMRA_6 = 1U,
            /* TMRA_5 */
            .TMRA_5 = 1U,
            /* TMRA_4 */
            .TMRA_4 = 1U,
            /* TMRA_3 */
            .TMRA_3 = 1U,
            /* TMRA_2 */
            .TMRA_2 = 1U,
            /* TMRA_1 */
            .TMRA_1 = 1U,
            /* TMR2_4 */
            .TMR2_4 = 1U,
            /* TMR2_3 */
            .TMR2_3 = 1U,
            /* TMR2_2 */
            .TMR2_2 = 1U,
            /* TMR2_1 */
            .TMR2_1 = 1U,
            /* EMB */
            .EMB = 1U,
            /* TMR0_3 */
            .TMR0_3 = 0U,
            /* TMR0_2 */
            .TMR0_2 = 1U,
            /* TMR0_1 */
            .TMR0_1 = 1U,
            /* HRPWM */
            .HRPWM = 1U,
            /* TMR4_3 */
            .TMR4_3 = 1U,
            /* TMR4_2 */
            .TMR4_2 = 1U,
            /* TMR4_1 */
            .TMR4_1 = 1U, 
            /* TMR6_8 */
            .TMR6_8 = 1U,
            /* TMR6_7 */
            .TMR6_7 = 1U,
            /* TMR6_6 */
            .TMR6_6 = 1U,
            /* TMR6_5 */
            .TMR6_5 = 1U,
            /* TMR6_4 */
            .TMR6_4 = 1U,
            /* TMR6_3 */
            .TMR6_3 = 1U,
            /* TMR6_2 */
            .TMR6_2 = 1U,
            /* TMR6_1 */
            .TMR6_1 = 1U
};

static const Power_Ip_FCG3Type l_Power_Ip_PwdModeFuncClockFCG3_2_st = {

            /* Reserved1 */
            .Reserved1 = 0U,
            /* USART10 */
            .USART10 = 1U,
            /* USART9 */
            .USART9 = 1U,
            /* USART8 */
            .USART8 = 1U,
            /* USART7 */
            .USART7 = 1U,
            /* USART6 */
            .USART6 = 1U,
            /* USART5 */
            .USART5 = 1U,
            /* USART4 */
            .USART4 = 1U,
            /* USART3 */
            .USART3 = 1U,
            /* USART2 */
            .USART2 = 1U,
            /* USART1 */
            .USART1 = 1U,
            /* Reserved2 */
            .Reserved2 = 0U,
            /* NFC */
            .NFC = 1U,
            /* DMC */
            .DMC = 1U,
            /* SMC */
            .SMC = 1U,
            /* DVP */
            .DVP = 1U,
            /* Reserved3 */
            .Reserved3 = 0U,
            /* OTS */
            .OTS = 1U,
            /* TMR0_5 */
            .TMR0_5 = 0U,
            /* TMR0_4 */
            .TMR0_4 = 0U,
            /* CMP34 */
            .CMP34 = 0U,
            /* CMP12 */
            .CMP12 = 0U,
            /* Reserved4 */
            .Reserved4 = 0U,
            /* DAC2 */
            .DAC2 = 1U,
            /* DAC1 */
            .DAC1 = 1U,
            /* CMBIAS */
            .CMBIAS = 1U,
            /* ADC3 */
            .ADC3 = 1U,
            /* ADC2 */
            .ADC2 = 1U,
            /* ADC1 */
            .ADC1 = 1U
};

static const Power_Ip_TurnoffRamType l_Power_Ip_PwdModeRamTurnoff_2_st = {

            /* RAMPDC10 */
            .RAMPDC10 = 0U,
            /* RAMPDC9 */
            .RAMPDC9 = 0U,
            /* RAMPDC8 */
            .RAMPDC8 = 0U,
            /* RAMPDC7 */
            .RAMPDC7 = 0U,
            /* RAMPDC6 */
            .RAMPDC6 = 0U,
            /* RAMPDC5 */
            .RAMPDC5 = 0U,
            /* RAMPDC4 */
            .RAMPDC4 = 0U,
            /* RAMPDC3 */
            .RAMPDC3 = 0U,
            /* RAMPDC2 */
            .RAMPDC2 = 0U,
            /* RAMPDC1 */
            .RAMPDC1 = 0U,
            /* RAMPDC0 */
            .RAMPDC0 = 0U,
            /* PRAMPDC11 */
            .PRAMPDC11 = 0U,
            /* PRAMPDC10 */
            .PRAMPDC10 = 0U,
            /* PRAMPDC9 */
            .PRAMPDC9 = 0U,
            /* PRAMPDC8 */
            .PRAMPDC8 = 0U,
            /* PRAMPDC7 */
            .PRAMPDC7 = 0U,
            /* PRAMPDC6 */
            .PRAMPDC6 = 0U,
            /* PRAMPDC5 */
            .PRAMPDC5 = 0U,
            /* PRAMPDC4 */
            .PRAMPDC4 = 0U,
            /* PRAMPDC3 */
            .PRAMPDC3 = 0U,
            /* PRAMPDC2 */
            .PRAMPDC2 = 0U,
            /* PRAMPDC1 */
            .PRAMPDC1 = 0U,
            /* PRAMPDC0 */
            .PRAMPDC0 = 0U,
            /* SRAMB */
            .SRAMB = 0U,
            /* Reserved */
            .Reserved = 0U
};

/* Power_Ip_ConfigPeripheralType */
static const Power_Ip_ConfigPeripheralType l_Power_Ip_PwdModePeripheral_2_st = {
            .ClockSource_ptr = &l_Power_Ip_PwdModeClockSource_2_st,
            .FuncClockFCG0_ptr = &l_Power_Ip_PwdModeFuncClockFCG0_2_st,
            .FuncClockFCG1_ptr = &l_Power_Ip_PwdModeFuncClockFCG1_2_st,
            .FuncClockFCG2_ptr = &l_Power_Ip_PwdModeFuncClockFCG2_2_st,
            .FuncClockFCG3_ptr = &l_Power_Ip_PwdModeFuncClockFCG3_2_st,
            .RamTurnoff_ptr = &l_Power_Ip_PwdModeRamTurnoff_2_st,
};

/* Pwc_Ip_PowerDownModeCfgType */
static const Power_Ip_PowerDownModeCfgType l_Power_Ip_PowerDownModeCfg_2_st = {
            .McuPwdState_u8 = (uint8)MCU_ENABLE,
            .Mode_en = PWC_PD_MD1,
            .IOState_en = PWC_PD_IO_KEEP1,
            .VcapCtrl_en = PWC_PD_VCAP_0P1UF,
            .PWDWakeupTrigger_ptr = &l_Power_Ip_PwdWkuEdgeSel_2_st,
            .PWDWKUSrc_ptr = &l_Power_Ip_PwdWkuEnable_2_st,
            .Peripheral_ptr = &l_Power_Ip_PwdModePeripheral_2_st
};

/* --------------------- PostBuild for Sleep Mode -------------------------- */
/* Power_Ip_TurnOffClockSourceType */
static const Power_Ip_TurnOffClockSourceType l_Power_Ip_SleepModeClockSource_3_st = {
            .Xtal32Control = PWC_WORKING,
            .HRCControl = PWC_WORKING,
            .MRCControl = PWC_WORKING,
            .XtalControl = PWC_WORKING,
            .LRCControl = PWC_WORKING,
            .PLLHControl = PWC_WORKING,
            .PLLAControl = PWC_WORKING,
            .Reserved = 0U
};

static const Power_Ip_FCG0Type l_Power_Ip_SleepModeFuncClockFCG0_3_st = {
            /* DCU8 */
            .DCU8 = 0U,
            /* DCU7 */
            .DCU7 = 0U,
            /* DCU6 */
            .DCU6 = 0U,
            /* DCU4 */
            .DCU4 = 0U,
            /* DCU5 */
            .DCU5 = 0U,
            /* DCU2 */
            .DCU2 = 0U,
            /* DCU3 */
            .DCU3 = 0U,
            /* DCU1 */
            .DCU1 = 0U,
            /* CRC */
            .CRC = 0U,
            /* TRNG */
            .TRNG = 0U,
            /* HASH */
            .HASH = 0U,
            /* SKE_AES */
            .SKE_AES = 0U,
            /* MAU */
            .MAU = 0U,
            /* CTC */
            .CTC = 0U,
            /* AOS */
            .AOS = 0U,
            /* FCM */
            .FCM = 0U,
            /* DMA2 */
            .DMA2 = 0U,
            /* DMA1 */
            .DMA1 = 0U,
            /* KEY */
            .KEY = 0U,
            /* PKE */
            .PKE = 0U,
            /* Reserved1 */
            .Reserved1 = 0U,
            /* SRAMB */
            .SRAMB = 0U,
            /* Reserved2 */
            .Reserved2 = 0U,
            /* ERMU */
            .ERMU = 0U,
            /* SRAM4 */
            .SRAM4 = 0U,
            /* SRAM3 */
            .SRAM3 = 0U,
            /* SRAM2 */
            .SRAM2 = 0U,
            /* SRAM1 */
            .SRAM1 = 0U,
            /* Reserved3 */
            .Reserved3 = 0U,
            /* SRAMH */
            .SRAMH = 0U
};

static const Power_Ip_FCG1Type l_Power_Ip_SleepModeFuncClockFCG1_3_st = 
{
            /* Reserved */
            .Reserved = 0U,
            /* MCAN2 */
            .MCAN2 = 0U,
            /* MCAN1 */
            .MCAN1 = 0U,
            /* FMAC4 */
            .FMAC4 = 0U,
            /* FMAC3 */
            .FMAC3 = 0U,
            /* FMAC2 */
            .FMAC2 = 0U,
            /* FMAC1 */
            .FMAC1 = 0U,
            /* USBHS */
            .USBHS = 1U,
            /* USBFS */
            .USBFS = 1U,
            /* SPI6 */
            .SPI6 = 0U,
            /* SPI5 */
            .SPI5 = 0U,
            /* SPI4 */
            .SPI4 = 0U,
            /* SPI3 */
            .SPI3 = 0U,
            /* SPI2 */
            .SPI2 = 0U,
            /* SPI1 */
            .SPI1 = 0U,
            /* I2S4 */
            .I2S4 = 0U,
            /* I2S3 */
            .I2S3 = 0U,
            /* I2S2 */
            .I2S2 = 0U,
            /* I2S1 */
            .I2S1 = 0U,
            /* SDIOC2 */
            .SDIOC2 = 0U,
            /* SDIOC1 */
            .SDIOC1 = 0U,
            /* I2C6 */
            .I2C6 = 0U,
            /* I2C5 */
            .I2C5 = 0U,
            /* I2C4 */
            .I2C4 = 0U,
            /* I2C3 */
            .I2C3 = 0U,
            /* I2C2 */
            .I2C2 = 0U,
            /* I2C1 */
            .I2C1 = 0U,
            /* QSPI */
            .QSPI = 0U,
            /* ETHMAC */
            .ETHMAC = 0U,
            /* CAN2 */
            .CAN2 = 0U,
            /* CAN1 */
            .CAN1 = 0U
};

static const Power_Ip_FCG2Type l_Power_Ip_SleepModeFuncClockFCG2_3_st = {

            /* TMRA_12 */
            .TMRA_12 = 0U,
            /* TMRA_11 */
            .TMRA_11 = 0U,
            /* TMRA_10 */
            .TMRA_10 = 0U,
            /* TMRA_9 */
            .TMRA_9 = 0U,
            /* TMRA_8 */
            .TMRA_8 = 0U,
            /* TMRA_7 */
            .TMRA_7 = 0U,
            /* TMRA_6 */
            .TMRA_6 = 0U,
            /* TMRA_5 */
            .TMRA_5 = 0U,
            /* TMRA_4 */
            .TMRA_4 = 0U,
            /* TMRA_3 */
            .TMRA_3 = 0U,
            /* TMRA_2 */
            .TMRA_2 = 0U,
            /* TMRA_1 */
            .TMRA_1 = 0U,
            /* TMR2_4 */
            .TMR2_4 = 0U,
            /* TMR2_3 */
            .TMR2_3 = 0U,
            /* TMR2_2 */
            .TMR2_2 = 0U,
            /* TMR2_1 */
            .TMR2_1 = 0U,
            /* EMB */
            .EMB = 0U,
            /* TMR0_3 */
            .TMR0_3 = 0U,
            /* TMR0_2 */
            .TMR0_2 = 0U,
            /* TMR0_1 */
            .TMR0_1 = 0U,
            /* HRPWM */
            .HRPWM = 0U,
            /* TMR4_3 */
            .TMR4_3 = 0U,
            /* TMR4_2 */
            .TMR4_2 = 0U,
            /* TMR4_1 */
            .TMR4_1 = 0U, 
            /* TMR6_8 */
            .TMR6_8 = 0U,
            /* TMR6_7 */
            .TMR6_7 = 0U,
            /* TMR6_6 */
            .TMR6_6 = 0U,
            /* TMR6_5 */
            .TMR6_5 = 0U,
            /* TMR6_4 */
            .TMR6_4 = 0U,
            /* TMR6_3 */
            .TMR6_3 = 0U,
            /* TMR6_2 */
            .TMR6_2 = 0U,
            /* TMR6_1 */
            .TMR6_1 = 0U
};

static const Power_Ip_FCG3Type l_Power_Ip_SleepModeFuncClockFCG3_3_st = {

            /* Reserved1 */
            .Reserved1 = 0U,
            /* USART10 */
            .USART10 = 0U,
            /* USART9 */
            .USART9 = 0U,
            /* USART8 */
            .USART8 = 0U,
            /* USART7 */
            .USART7 = 0U,
            /* USART6 */
            .USART6 = 0U,
            /* USART5 */
            .USART5 = 0U,
            /* USART4 */
            .USART4 = 0U,
            /* USART3 */
            .USART3 = 0U,
            /* USART2 */
            .USART2 = 0U,
            /* USART1 */
            .USART1 = 0U,
            /* Reserved2 */
            .Reserved2 = 0U,
            /* NFC */
            .NFC = 0U,
            /* DMC */
            .DMC = 0U,
            /* SMC */
            .SMC = 0U,
            /* DVP */
            .DVP = 0U,
            /* Reserved3 */
            .Reserved3 = 0U,
            /* OTS */
            .OTS = 0U,
            /* TMR0_5 */
            .TMR0_5 = 0U,
            /* TMR0_4 */
            .TMR0_4 = 0U,
            /* CMP34 */
            .CMP34 = 0U,
            /* CMP12 */
            .CMP12 = 0U,
            /* Reserved4 */
            .Reserved4 = 0U,
            /* DAC2 */
            .DAC2 = 0U,
            /* DAC1 */
            .DAC1 = 0U,
            /* CMBIAS */
            .CMBIAS = 0U,
            /* ADC3 */
            .ADC3 = 0U,
            /* ADC2 */
            .ADC2 = 0U,
            /* ADC1 */
            .ADC1 = 0U
};

/* TurnOffUnusedRam */
static const Power_Ip_TurnoffRamType l_Power_Ip_SleepModeRamTurnoff_3_st = {

            /* RAMPDC10 */
            .RAMPDC10 = 0U,
            /* RAMPDC9 */
            .RAMPDC9 = 0U,
            /* RAMPDC8 */
            .RAMPDC8 = 0U,
            /* RAMPDC7 */
            .RAMPDC7 = 0U,
            /* RAMPDC6 */
            .RAMPDC6 = 0U,
            /* RAMPDC5 */
            .RAMPDC5 = 0U,
            /* RAMPDC4 */
            .RAMPDC4 = 0U,
            /* RAMPDC3 */
            .RAMPDC3 = 0U,
            /* RAMPDC2 */
            .RAMPDC2 = 0U,
            /* RAMPDC1 */
            .RAMPDC1 = 0U,
            /* RAMPDC0 */
            .RAMPDC0 = 0U,
            /* PRAMPDC11 */
            .PRAMPDC11 = 0U,
            /* PRAMPDC10 */
            .PRAMPDC10 = 0U,
            /* PRAMPDC9 */
            .PRAMPDC9 = 0U,
            /* PRAMPDC8 */
            .PRAMPDC8 = 0U,
            /* PRAMPDC7 */
            .PRAMPDC7 = 0U,
            /* PRAMPDC6 */
            .PRAMPDC6 = 0U,
            /* PRAMPDC5 */
            .PRAMPDC5 = 0U,
            /* PRAMPDC4 */
            .PRAMPDC4 = 0U,
            /* PRAMPDC3 */
            .PRAMPDC3 = 0U,
            /* PRAMPDC2 */
            .PRAMPDC2 = 0U,
            /* PRAMPDC1 */
            .PRAMPDC1 = 0U,
            /* PRAMPDC0 */
            .PRAMPDC0 = 0U,
            /* SRAMB */
            .SRAMB = 0U,
            /* Reserved */
            .Reserved = 0U
};

static const Power_Ip_ConfigPeripheralType l_Power_Ip_SleepModePeripheral_3_st = {
            .ClockSource_ptr = &l_Power_Ip_SleepModeClockSource_3_st,
            .FuncClockFCG0_ptr = &l_Power_Ip_SleepModeFuncClockFCG0_3_st,
            .FuncClockFCG1_ptr = &l_Power_Ip_SleepModeFuncClockFCG1_3_st,
            .FuncClockFCG2_ptr = &l_Power_Ip_SleepModeFuncClockFCG2_3_st,
            .FuncClockFCG3_ptr = &l_Power_Ip_SleepModeFuncClockFCG3_3_st,
            .RamTurnoff_ptr = &l_Power_Ip_SleepModeRamTurnoff_3_st,
};

/* Power_Ip_SleepModeCfgType */
static const Power_Ip_SleepModeCfgType l_Power_Ip_SleepModeCfg_3_st = {
            /* .Peripheral_ptr = */                       
            .Peripheral_ptr = &l_Power_Ip_SleepModePeripheral_3_st
};


/* Mcu_ModeConfig */
const Power_Ip_HwModeConfigType g_McuModeConfigPB_aa[MCU_NUMBER_OF_MODECONFIGS] = {
  {
            .ModeConfigId_dd = (uint8)0U,
            .PowerMode_en = PWC_IP_SLEEP_MODE,
            .McuSetModeConfig_ptr = &l_Power_Ip_SleepModeCfg_0_st
  },
  {
            .ModeConfigId_dd = (uint8)1U,
            .PowerMode_en = PWC_IP_STOP_MODE,
            .McuSetModeConfig_ptr = &l_Power_Ip_StopModeCfg_1_st
  },
  {
            .ModeConfigId_dd = (uint8)2U,
            .PowerMode_en = PWC_IP_PWD_MODE,
            .McuSetModeConfig_ptr = &l_Power_Ip_PowerDownModeCfg_2_st
  },
  {
            .ModeConfigId_dd = (uint8)3U,
            .PowerMode_en = PWC_IP_SLEEP_MODE,
            .McuSetModeConfig_ptr = &l_Power_Ip_SleepModeCfg_3_st
  }
}; /* Mcu_ModeConfig */

/* Power_Ip_PWC_PVD1ConfigType */
static const Power_Ip_PWC_PVD1ConfigType l_McuPVD1Config_st = {
            .PWC_PVDState_u32 = PWC_PVD_ON,
            .PWC_PVDCompareOutputState_u32 = PWC_PVD_CMP_OFF,
            .PWC_PVDExceptionType_u32 = PWC_PVD_EXP_TYPE_NONE,
            .PWC_PVDFilter_u32 = PWC_PVD_FILTER_ON,
            .PWC_PVDFilterClock_u32 = PWC_PVD_FILTER_LRC_DIV1,
            .PWC_PVDThresholdVoltage_u32 = PWC_PVD_THRESHOLD_LVL0,
            .PWC_PVDTriggerEdge_u32 = PWC_PVD_TRIG_FALLING
}; /* Power_Ip_PWC_PVD1ConfigType */

/* Power_Ip_PWC_PVD2ConfigType */
static const Power_Ip_PWC_PVD2ConfigType l_McuPVD2Config_st = {
            .PWC_PVDState_u32 = PWC_PVD_ON,
            .PWC_PVDCompareOutputState_u32 = PWC_PVD_CMP_OFF,
            .PWC_PVDExceptionType_u32 = PWC_PVD_EXP_TYPE_NONE,
            .PWC_PVDFilter_u32 = PWC_PVD_FILTER_ON,
            .PWC_PVDFilterClock_u32 = PWC_PVD_FILTER_LRC_DIV1,
            .PWC_PVDThresholdVoltage_u32 = PWC_PVD_THRESHOLD_LVL0,
            .PWC_PVDTriggerEdge_u32 = PWC_PVD_TRIG_FALLING
}; /* Power_Ip_PWC_PVD2ConfigType */

/* Power_Ip_PowerMonitorConfigType */
static const Power_Ip_PowerMonitorConfigType l_McuPowerMonitorConfig_st = {
            .PWC_PowerMonitorCmd_u32 = PWC_POWER_MONITOR_DISABLE,
            .PWC_PowerMonitorSrc_u32 = PWC_POWER_MONITOR_SRC_INTERNAL,
            .PWC_VbatMonitorCmd_u32 = PWC_VBAT_MONITOR_DISABLE,
            .PWC_VbatMonitorSrc_u32 = PWC_VBAT_MONITOR_SRC_1_8_V
}; /* Power_Ip_PowerMonitorConfigType */

/* Mcu_PVDConfig */
const Power_Ip_HwConfigType g_McuHwIPsConfigPB_st = {
            .McuPVD1Config_ptr = &l_McuPVD1Config_st,
            .McuPVD2Config_ptr = &l_McuPVD2Config_st,
            .McuPowerMonitorConfig_ptr = &l_McuPowerMonitorConfig_st
}; /* Mcu_PVDConfig */



#define MCU_STOP_SEC_CONFIG_DATA_UNSPECIFIED
#include "Mcu_MemMap.h"


#ifdef __cplusplus
}
#endif

/**
 * @}
 */

/*---------------------- End of File ----------------------------------------*/
