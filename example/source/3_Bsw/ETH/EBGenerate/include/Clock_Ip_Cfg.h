/***********************************************************************************************************************
 * Project Name: XHSC AUTOSAR MCAL
 * File Name: Clock_Ip_Cfg.h
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
 *   @file       Clock_Ip_Cfg.h
 *   @version    V1.0.3
 *   @brief      Autosar 4.4.0 MCU Driver
 *   @addtogroup mcu_driver
 *   @{
 */

#ifndef CLOCK_IP_CFG_H
#define CLOCK_IP_CFG_H

#ifdef __cplusplus
extern "C"{
#endif

/***********************************************************************************************************************
 *  Includes
 **********************************************************************************************************************/
#include "Clock_Ip_PBcfg.h"
#include "Osr.h"
    
/***********************************************************************************************************************
 *   VERSION CHECK
 **********************************************************************************************************************/


/***********************************************************************************************************************
 *  DEFINES & MACROS
 **********************************************************************************************************************/
/**
 *  @brief   Define Timeout Method.
 */
#define CLOCK_IP_TIMEOUT_TYPE                       (OSR_COUNTER_DUMMY)

/**
 *  @brief   Define Timeout Value.
 */
#define CLOCK_IP_TIMEOUT_VALUE                      (100000U)



/**
 * @brief Word Clear mask.
 */
#define CLK_WORD_ZERO               (uint16)0x0000U

/**
 * @brief Double word clear mask.
 */
#define CLK_DOUBLE_WORD_ZERO        (uint32)0x00000000UL

/**
 * @brief HRC State ON or OFF
 */
#define CLK_HRC_OFF                 (CMU_HRCCR_HRCSTP)
#define CLK_HRC_ON                  (0x00U)

/**
 * @brief LRC State ON or OFF
 */
#define CLK_LRC_OFF                 (CMU_LRCCR_LRCSTP)
#define CLK_LRC_ON                  (0x00U)

/**
 * @brief MRC State ON or OFF
 */
#define CLK_MRC_OFF                 (CMU_MRCCR_MRCSTP)
#define CLK_MRC_ON                  (0x80U)

/**
 * @brief PLL function config
 */
#define CLK_PLL_OFF                 (0x01U)
#define CLK_PLL_ON                  (0x00U)

/**
 * @brief XTAL function config
 */
#define CLK_XTAL_OFF                (CMU_XTALCR_XTALSTP)
#define CLK_XTAL_ON                 (0x00U)

/**
 * @brief XTAL32 function config
 */
#define CLK_XTAL32_OFF              (CMU_XTAL32CR_XTAL32STP)
#define CLK_XTAL32_ON               (0x00U)

/**
 * @brief XTAL mode selection osc or exclk
 */
#define CLK_XTAL_MD_OSC             (0x00U)
#define CLK_XTAL_MD_EXCLK           (CMU_XTALCFGR_XTALMS)

/**
 * @brief MCO Channel Select
 */
#define CLK_MCO1                    (0x00U)
#define CLK_MCO2                    (0x01U)

/**
 * @brief XTAL driver ability
 * @note
 *  High: [20~25]
 *  Mid:  [16~20)
 *  Low:  (8~16)
 *  ULow: [4~8]
 */
#define CLK_XTAL_DRV_HIGH           (0x00U << CMU_XTALCFGR_XTALDRV_POS)
#define CLK_XTAL_DRV_MID            (0x01U << CMU_XTALCFGR_XTALDRV_POS)
#define CLK_XTAL_DRV_LOW            (0x02U << CMU_XTALCFGR_XTALDRV_POS)
#define CLK_XTAL_DRV_ULOW           (0x03U << CMU_XTALCFGR_XTALDRV_POS)

/**
 * @brief XTAL stable time selection.
 * @note  a cycle of stable counter equal to cycle of LRC divide by 8
 */
/* 35 stable count cycle, approx. 133us  */
#define CLK_XTAL_STB_133US          (0x01U)
/* 67 stable count cycle, approx. 255us  */
#define CLK_XTAL_STB_255US          (0x02U)
/* 131 stable count cycle, approx. 499us */
#define CLK_XTAL_STB_499US          (0x03U)
/* 259 stable count cycle, approx. 988us */
#define CLK_XTAL_STB_988US          (0x04U)
/* 547 stable count cycle, approx. 2ms   */
#define CLK_XTAL_STB_2MS            (0x05U)
/* 1059 stable count cycle, approx. 4ms  */
#define CLK_XTAL_STB_4MS            (0x06U)
/* 2147 stable count cycle, approx. 8ms  */
#define CLK_XTAL_STB_8MS            (0x07U)
/* 4291 stable count cycle, approx. 16ms */
#define CLK_XTAL_STB_16MS           (0x08U)
/* 8163 stable count cycle, approx. 32ms */
#define CLK_XTAL_STB_31MS           (0x09U)

/**
 * @brief XTAL32 driver ability.
 */
#define CLK_XTAL32_DRV_MID          (0x00U)
#define CLK_XTAL32_DRV_HIGH         (0x01U)

/**
 * @brief XTAL32 filtering selection.
 */
#define CLK_XTAL32_FILTER_ALL_MD    (0x00U)
/**
 * @brief Valid in run mode.
 */
#define CLK_XTAL32_FILTER_RUN_MD    (0x01U)
/**
 * @brief Invalid in run,stop,power down mode.
 */
#define CLK_XTAL32_FILTER_OFF       (0x03U)
/**
 * @brief Internal high speed RC freq.
 */
#define CMU_HRC_16MHz_VALUE         (16000000U)
/**
 * @brief Internal high speed RC freq.
 */
#define CMU_HRC_20MHz_VALUE         (20000000U)
/**
 * @brief HRC select.
 */
#define CMU_HRC_FREQ_MON            (*((volatile uint32 *)(0x40010684U)))
/**
 * @brief Internal middle speed RC freq.
 */
#define CMU_MRC_VALUE               (8000000U)
/**
 * @brief Internal low speed RC freq.
 */
#define CMU_LRC_VALUE               (32768U)
/**
 * @brief External high speed OSC freq.
 */
#define CMU_XTAL_VALUE              (8000000U)
/**
 * @brief External low speed OSC freq.
 */
#define CMU_XTAL32_VALUE            (32768U)

/**
 * @brief MCO Clock Source.
 */
#define CLK_MCO_SRC_HRC             (0x00U)
#define CLK_MCO_SRC_MRC             (0x01U)
#define CLK_MCO_SRC_LRC             (0x02U)
#define CLK_MCO_SRC_XTAL            (0x03U)
#define CLK_MCO_SRC_XTAL32          (0x04U)
#define CLK_MCO_SRC_PLLHP           (0x06U)
#define CLK_MCO_SRC_PLLAP           (0x07U)
#define CLK_MCO_SRC_PLLHQ           (0x08U)
#define CLK_MCO_SRC_PLLAQ           (0x09U)
#define CLK_MCO_SRC_PLLAR           (0x0AU)
#define CLK_MCO_SRC_HCLK            (0x0BU)

/**
 * @brief MCO Clock Prescaler.
 */
#define CLK_MCO_DIV1                (0x00U << CMU_MCOCFGR_MCODIV_POS)
#define CLK_MCO_DIV2                (0x01U << CMU_MCOCFGR_MCODIV_POS)
#define CLK_MCO_DIV4                (0x02U << CMU_MCOCFGR_MCODIV_POS)
#define CLK_MCO_DIV8                (0x03U << CMU_MCOCFGR_MCODIV_POS)
#define CLK_MCO_DIV16               (0x04U << CMU_MCOCFGR_MCODIV_POS)
#define CLK_MCO_DIV32               (0x05U << CMU_MCOCFGR_MCODIV_POS)
#define CLK_MCO_DIV64               (0x06U << CMU_MCOCFGR_MCODIV_POS)
#define CLK_MCO_DIV128              (0x07U << CMU_MCOCFGR_MCODIV_POS)

/**
 * @brief I2S Clock.
 */
#define MCU_I2S1CLK                 (0x00U)
#define MCU_I2S2CLK                 (0x01U)
#define MCU_I2S3CLK                 (0x02U)
#define MCU_I2S4CLK                 (0x03U)

/**
 * @brief RTC Clock Source.
 */
#define RTC_CLK_SRC_XTAL32          (0U)
#define RTC_CLK_SRC_LRC             (RTC_CR3_RCKSEL | RTC_CR3_LRCEN)

/**
 * @brief TPIU clock divider.
 */
#define CLK_TPIUCLK_DIV1            (0x00U)
#define CLK_TPIUCLK_DIV2            (0x01U)
#define CLK_TPIUCLK_DIV4            (0x02U)

/**
 * @brief Peripheral Clock Selection ADC,I2S,DAC,TRANG
 *        PCLK2 is used for ADC clock, PCLK1 is used for I2S clock, PCLK4 is used for DAC/TRANG clock
 */
#define CLK_PERIPHCLK_PCLK          (0x0000U)
#define CLK_PERIPHCLK_PLLHQ         (0x0008U)
#define CLK_PERIPHCLK_PLLHR         (0x0009U)
#define CLK_PERIPHCLK_PLLAP         (0x000AU)
#define CLK_PERIPHCLK_PLLAQ         (0x000BU)
#define CLK_PERIPHCLK_PLLAR         (0x000CU)

/**
 * @brief CLK Stable Flags.
 */
#define CLK_STB_FLAG_HRC            (CMU_OSCSTBSR_HRCSTBF)
#define CLK_STB_FLAG_XTAL           (CMU_OSCSTBSR_XTALSTBF)
#define CLK_STB_FLAG_PLLH           (CMU_OSCSTBSR_PLLHSTBF)
#define CLK_STB_FLAG_PLLA           (CMU_OSCSTBSR_PLLASTBF)

/**
 * @brief System Clock Source.
 */
#define CLK_SYSCLK_SRC_HRC          (0x00U)
#define CLK_SYSCLK_SRC_MRC          (0x01U)
#define CLK_SYSCLK_SRC_LRC          (0x02U)
#define CLK_SYSCLK_SRC_XTAL         (0x03U)
#define CLK_SYSCLK_SRC_XTAL32       (0x04U)
#define CLK_SYSCLK_SRC_PLL          (0x05U)

/**
 * @brief System Clock Divider.
 */
#define CLK_SYSCLK_DIV1             (0x00U)
#define CLK_SYSCLK_DIV2             (0x01U)
#define CLK_SYSCLK_DIV4             (0x02U)
#define CLK_SYSCLK_DIV8             (0x03U)
#define CLK_SYSCLK_DIV16            (0x04U)
#define CLK_SYSCLK_DIV32            (0x05U)
#define CLK_SYSCLK_DIV64            (0x06U)

/**
 * @brief Clock Bus Clock Category Selection.
 */
#define CLK_BUS_PCLK0               (CMU_SCFGR_PCLK0S)
#define CLK_BUS_PCLK1               (CMU_SCFGR_PCLK1S)
#define CLK_BUS_PCLK2               (CMU_SCFGR_PCLK2S)
#define CLK_BUS_PCLK3               (CMU_SCFGR_PCLK3S)
#define CLK_BUS_PCLK4               (CMU_SCFGR_PCLK4S)
#define CLK_BUS_EXCLK               (CMU_SCFGR_EXCKS)
#define CLK_BUS_HCLK                (CMU_SCFGR_HCLKS)

/**
 * @brief CLK HCLK Divider.
 */
#define CLK_HCLK_DIV1               (CLK_SYSCLK_DIV1  << CMU_SCFGR_HCLKS_POS)
#define CLK_HCLK_DIV2               (CLK_SYSCLK_DIV2  << CMU_SCFGR_HCLKS_POS)
#define CLK_HCLK_DIV4               (CLK_SYSCLK_DIV4  << CMU_SCFGR_HCLKS_POS)
#define CLK_HCLK_DIV8               (CLK_SYSCLK_DIV8  << CMU_SCFGR_HCLKS_POS)
#define CLK_HCLK_DIV16              (CLK_SYSCLK_DIV16 << CMU_SCFGR_HCLKS_POS)
#define CLK_HCLK_DIV32              (CLK_SYSCLK_DIV32 << CMU_SCFGR_HCLKS_POS)
#define CLK_HCLK_DIV64              (CLK_SYSCLK_DIV64 << CMU_SCFGR_HCLKS_POS)

/**
 * @brief CLK PCLK0 Divider.
 */
#define CLK_PCLK0_DIV1              (CLK_SYSCLK_DIV1  << CMU_SCFGR_PCLK0S_POS)
#define CLK_PCLK0_DIV2              (CLK_SYSCLK_DIV2  << CMU_SCFGR_PCLK0S_POS)
#define CLK_PCLK0_DIV4              (CLK_SYSCLK_DIV4  << CMU_SCFGR_PCLK0S_POS)
#define CLK_PCLK0_DIV8              (CLK_SYSCLK_DIV8  << CMU_SCFGR_PCLK0S_POS)
#define CLK_PCLK0_DIV16             (CLK_SYSCLK_DIV16 << CMU_SCFGR_PCLK0S_POS)
#define CLK_PCLK0_DIV32             (CLK_SYSCLK_DIV32 << CMU_SCFGR_PCLK0S_POS)
#define CLK_PCLK0_DIV64             (CLK_SYSCLK_DIV64 << CMU_SCFGR_PCLK0S_POS)

/**
 * @brief CLK PCLK1 Divider.
 */
#define CLK_PCLK1_DIV1              (CLK_SYSCLK_DIV1  << CMU_SCFGR_PCLK1S_POS)
#define CLK_PCLK1_DIV2              (CLK_SYSCLK_DIV2  << CMU_SCFGR_PCLK1S_POS)
#define CLK_PCLK1_DIV4              (CLK_SYSCLK_DIV4  << CMU_SCFGR_PCLK1S_POS)
#define CLK_PCLK1_DIV8              (CLK_SYSCLK_DIV8  << CMU_SCFGR_PCLK1S_POS)
#define CLK_PCLK1_DIV16             (CLK_SYSCLK_DIV16 << CMU_SCFGR_PCLK1S_POS)
#define CLK_PCLK1_DIV32             (CLK_SYSCLK_DIV32 << CMU_SCFGR_PCLK1S_POS)
#define CLK_PCLK1_DIV64             (CLK_SYSCLK_DIV64 << CMU_SCFGR_PCLK1S_POS)

/**
 * @brief CLK PCLK2 Divider.
 */
#define CLK_PCLK2_DIV1              (CLK_SYSCLK_DIV1  << CMU_SCFGR_PCLK2S_POS)
#define CLK_PCLK2_DIV2              (CLK_SYSCLK_DIV2  << CMU_SCFGR_PCLK2S_POS)
#define CLK_PCLK2_DIV4              (CLK_SYSCLK_DIV4  << CMU_SCFGR_PCLK2S_POS)
#define CLK_PCLK2_DIV8              (CLK_SYSCLK_DIV8  << CMU_SCFGR_PCLK2S_POS)
#define CLK_PCLK2_DIV16             (CLK_SYSCLK_DIV16 << CMU_SCFGR_PCLK2S_POS)
#define CLK_PCLK2_DIV32             (CLK_SYSCLK_DIV32 << CMU_SCFGR_PCLK2S_POS)
#define CLK_PCLK2_DIV64             (CLK_SYSCLK_DIV64 << CMU_SCFGR_PCLK2S_POS)

/**
 * @brief CLK PCLK3 Divider.
 */
#define CLK_PCLK3_DIV1              (CLK_SYSCLK_DIV1  << CMU_SCFGR_PCLK3S_POS)
#define CLK_PCLK3_DIV2              (CLK_SYSCLK_DIV2  << CMU_SCFGR_PCLK3S_POS)
#define CLK_PCLK3_DIV4              (CLK_SYSCLK_DIV4  << CMU_SCFGR_PCLK3S_POS)
#define CLK_PCLK3_DIV8              (CLK_SYSCLK_DIV8  << CMU_SCFGR_PCLK3S_POS)
#define CLK_PCLK3_DIV16             (CLK_SYSCLK_DIV16 << CMU_SCFGR_PCLK3S_POS)
#define CLK_PCLK3_DIV32             (CLK_SYSCLK_DIV32 << CMU_SCFGR_PCLK3S_POS)
#define CLK_PCLK3_DIV64             (CLK_SYSCLK_DIV64 << CMU_SCFGR_PCLK3S_POS)

/**
 * @brief CLK PCLK4 Divider.
 */
#define CLK_PCLK4_DIV1              (CLK_SYSCLK_DIV1  << CMU_SCFGR_PCLK4S_POS)
#define CLK_PCLK4_DIV2              (CLK_SYSCLK_DIV2  << CMU_SCFGR_PCLK4S_POS)
#define CLK_PCLK4_DIV4              (CLK_SYSCLK_DIV4  << CMU_SCFGR_PCLK4S_POS)
#define CLK_PCLK4_DIV8              (CLK_SYSCLK_DIV8  << CMU_SCFGR_PCLK4S_POS)
#define CLK_PCLK4_DIV16             (CLK_SYSCLK_DIV16 << CMU_SCFGR_PCLK4S_POS)
#define CLK_PCLK4_DIV32             (CLK_SYSCLK_DIV32 << CMU_SCFGR_PCLK4S_POS)
#define CLK_PCLK4_DIV64             (CLK_SYSCLK_DIV64 << CMU_SCFGR_PCLK4S_POS)

/**
 * @brief CLK EXCLK Divider.
 */
#define CLK_EXCLK_DIV1              (CLK_SYSCLK_DIV1  << CMU_SCFGR_EXCKS_POS)
#define CLK_EXCLK_DIV2              (CLK_SYSCLK_DIV2  << CMU_SCFGR_EXCKS_POS)
#define CLK_EXCLK_DIV4              (CLK_SYSCLK_DIV4  << CMU_SCFGR_EXCKS_POS)
#define CLK_EXCLK_DIV8              (CLK_SYSCLK_DIV8  << CMU_SCFGR_EXCKS_POS)
#define CLK_EXCLK_DIV16             (CLK_SYSCLK_DIV16 << CMU_SCFGR_EXCKS_POS)
#define CLK_EXCLK_DIV32             (CLK_SYSCLK_DIV32 << CMU_SCFGR_EXCKS_POS)
#define CLK_EXCLK_DIV64             (CLK_SYSCLK_DIV64 << CMU_SCFGR_EXCKS_POS)

/**
 * @brief USB Clock Selection.
 */
#define CLK_USBCLK_SYSCLK_DIV2      (0x01U << CMU_USBCKCFGR_USBCKS_POS)
#define CLK_USBCLK_SYSCLK_DIV3      (0x02U << CMU_USBCKCFGR_USBCKS_POS)
#define CLK_USBCLK_SYSCLK_DIV4      (0x03U << CMU_USBCKCFGR_USBCKS_POS)
#define CLK_USBCLK_SYSCLK_DIV5      (0x04U << CMU_USBCKCFGR_USBCKS_POS)
#define CLK_USBCLK_SYSCLK_DIV6      (0x05U << CMU_USBCKCFGR_USBCKS_POS)
#define CLK_USBCLK_SYSCLK_DIV7      (0x06U << CMU_USBCKCFGR_USBCKS_POS)
#define CLK_USBCLK_SYSCLK_DIV8      (0x07U << CMU_USBCKCFGR_USBCKS_POS)
#define CLK_USBCLK_PLLQ             (0x08U << CMU_USBCKCFGR_USBCKS_POS)
#define CLK_USBCLK_PLLR             (0x09U << CMU_USBCKCFGR_USBCKS_POS)
#define CLK_USBCLK_PLLAP            (0x0AU << CMU_USBCKCFGR_USBCKS_POS)
#define CLK_USBCLK_PLLAQ            (0x0BU << CMU_USBCKCFGR_USBCKS_POS)
#define CLK_USBCLK_PLLAR            (0x0CU << CMU_USBCKCFGR_USBCKS_POS)

/**
 * @brief CAN Clock Selection.
 */
#define CLK_CANCLK_SYSCLK_DIV2      (0x01U)
#define CLK_CANCLK_SYSCLK_DIV3      (0x02U)
#define CLK_CANCLK_SYSCLK_DIV4      (0x03U)
#define CLK_CANCLK_SYSCLK_DIV5      (0x04U)
#define CLK_CANCLK_SYSCLK_DIV6      (0x05U)
#define CLK_CANCLK_SYSCLK_DIV7      (0x06U)
#define CLK_CANCLK_SYSCLK_DIV8      (0x07U)
#define CLK_CANCLK_PLLQ             (0x08U)
#define CLK_CANCLK_PLLR             (0x09U)
#define CLK_CANCLK_PLLAP            (0x0AU)
#define CLK_CANCLK_PLLAQ            (0x0BU)
#define CLK_CANCLK_PLLAR            (0x0CU)
#define CLK_CANCLK_XTAL             (0x0DU)

/**
 * @brief Clk FCG Default Value.
 */
#define CLK_FCG0_DEFAULT            (0xFFFFFA0EU)
#define CLK_FCG1_DEFAULT            (0xFFFFFFFFU)
#define CLK_FCG2_DEFAULT            (0xFFFFFFFFU)
#define CLK_FCG3_DEFAULT            (0xFFFFFFFFU)

/**
 * @brief Time out.
 */
#define CLK_LRC_TIMEOUT             ((uint32)0x200U)
#define CLK_MRC_TIMEOUT             ((uint32)0x200U)
#define CLK_XTAL32_TIMEOUT          ((uint32)0x200U)

/**
 * @brief PLL source clock.
 */
#define CLK_PLL_SRC_XTAL            (0x00U)
#define CLK_PLL_SRC_HRC             (0x01U)

/**
 * @brief Default value.
 */
#define CLK_DEFAULT_DIV             (0x01U)
#define CLK_PLLHP_DEFAULT           (0x01U)
#define CLK_PLLHQ_DEFAULT           (0x01U)
#define CLK_PLLHR_DEFAULT           (0x01U)
#define CLK_PLLHN_DEFAULT           (0x13U)
#define CLK_PLLHM_DEFAULT           (0x00U)
#define CLK_PLLAP_DEFAULT           (0x01U)
#define CLK_PLLAQ_DEFAULT           (0x01U)
#define CLK_PLLAR_DEFAULT           (0x01U)
#define CLK_PLLAN_DEFAULT           (0x13U)
#define CLK_PLLAM_DEFAULT           (0x00U)

/**
 * @brief FCG0 peripheral.
 */
#define MCU_FCG0_PERIPH_SRAMH       (PWC_FCG0_SRAMH)
#define MCU_FCG0_PERIPH_SRAM1       (PWC_FCG0_SRAM1)
#define MCU_FCG0_PERIPH_SRAM2       (PWC_FCG0_SRAM2)
#define MCU_FCG0_PERIPH_SRAM3       (PWC_FCG0_SRAM3)
#define MCU_FCG0_PERIPH_SRAM4       (PWC_FCG0_SRAM4)
#define MCU_FCG0_PERIPH_ERMU        (PWC_FCG0_ERMU)
#define MCU_FCG0_PERIPH_SRAMB       (PWC_FCG0_SRAMB)
#define MCU_FCG0_PERIPH_PKE         (PWC_FCG0_PKE)
#define MCU_FCG0_PERIPH_KEY         (PWC_FCG0_KEY)
#define MCU_FCG0_PERIPH_DMA1        (PWC_FCG0_DMA1)
#define MCU_FCG0_PERIPH_DMA2        (PWC_FCG0_DMA2)
#define MCU_FCG0_PERIPH_FCM         (PWC_FCG0_FCM)
#define MCU_FCG0_PERIPH_AOS         (PWC_FCG0_AOS)
#define MCU_FCG0_PERIPH_CTC         (PWC_FCG0_CTC)
#define MCU_FCG0_PERIPH_MAU         (PWC_FCG0_MAU)
#ifdef PWC_FCG0_SKE
#define MCU_FCG0_PERIPH_SKE_AES     (PWC_FCG0_SKE)
#endif
#ifdef PWC_FCG0_AES
#define MCU_FCG0_PERIPH_SKE_AES     (PWC_FCG0_AES)
#endif
#define MCU_FCG0_PERIPH_HASH        (PWC_FCG0_HASH)
#define MCU_FCG0_PERIPH_TRNG        (PWC_FCG0_TRNG)
#define MCU_FCG0_PERIPH_CRC         (PWC_FCG0_CRC)
#define MCU_FCG0_PERIPH_DCU1        (PWC_FCG0_DCU1)
#define MCU_FCG0_PERIPH_DCU2        (PWC_FCG0_DCU2)
#define MCU_FCG0_PERIPH_DCU3        (PWC_FCG0_DCU3)
#define MCU_FCG0_PERIPH_DCU4        (PWC_FCG0_DCU4)
#define MCU_FCG0_PERIPH_DCU5        (PWC_FCG0_DCU5)
#define MCU_FCG0_PERIPH_DCU6        (PWC_FCG0_DCU6)
#define MCU_FCG0_PERIPH_DCU7        (PWC_FCG0_DCU7)
#define MCU_FCG0_PERIPH_DCU8        (PWC_FCG0_DCU8)

/**
 * @brief FCG1 peripheral.
 */
#define MCU_FCG1_PERIPH_CAN1        (PWC_FCG1_CAN1)
#define MCU_FCG1_PERIPH_CAN2        (PWC_FCG1_CAN2)
#define MCU_FCG1_PERIPH_MCAN1       (PWC_FCG1_MCAN1)
#define MCU_FCG1_PERIPH_MCAN2       (PWC_FCG1_MCAN2)
#define MCU_FCG1_PERIPH_ETHMAC      (PWC_FCG1_ETHMAC)
#define MCU_FCG1_PERIPH_QSPI        (PWC_FCG1_QSPI)
#define MCU_FCG1_PERIPH_I2C1        (PWC_FCG1_I2C1)
#define MCU_FCG1_PERIPH_I2C2        (PWC_FCG1_I2C2)
#define MCU_FCG1_PERIPH_I2C3        (PWC_FCG1_I2C3)
#define MCU_FCG1_PERIPH_I2C4        (PWC_FCG1_I2C4)
#define MCU_FCG1_PERIPH_I2C5        (PWC_FCG1_I2C5)
#define MCU_FCG1_PERIPH_I2C6        (PWC_FCG1_I2C6)
#define MCU_FCG1_PERIPH_SDIOC1      (PWC_FCG1_SDIOC1)
#define MCU_FCG1_PERIPH_SDIOC2      (PWC_FCG1_SDIOC2)
#define MCU_FCG1_PERIPH_I2S1        (PWC_FCG1_I2S1)
#define MCU_FCG1_PERIPH_I2S2        (PWC_FCG1_I2S2)
#define MCU_FCG1_PERIPH_I2S3        (PWC_FCG1_I2S3)
#define MCU_FCG1_PERIPH_I2S4        (PWC_FCG1_I2S4)
#define MCU_FCG1_PERIPH_SPI1        (PWC_FCG1_SPI1)
#define MCU_FCG1_PERIPH_SPI2        (PWC_FCG1_SPI2)
#define MCU_FCG1_PERIPH_SPI3        (PWC_FCG1_SPI3)
#define MCU_FCG1_PERIPH_SPI4        (PWC_FCG1_SPI4)
#define MCU_FCG1_PERIPH_SPI5        (PWC_FCG1_SPI5)
#define MCU_FCG1_PERIPH_SPI6        (PWC_FCG1_SPI6)
#define MCU_FCG1_PERIPH_USBFS       (PWC_FCG1_USBFS)
#define MCU_FCG1_PERIPH_USBHS       (PWC_FCG1_USBHS)
#define MCU_FCG1_PERIPH_FMAC1       (PWC_FCG1_FMAC1)
#define MCU_FCG1_PERIPH_FMAC2       (PWC_FCG1_FMAC2)
#define MCU_FCG1_PERIPH_FMAC3       (PWC_FCG1_FMAC3)
#define MCU_FCG1_PERIPH_FMAC4       (PWC_FCG1_FMAC4)

/**
 * @brief FCG2 peripheral.
 */
#define MCU_FCG2_PERIPH_TMR6_1      (PWC_FCG2_TMR6_1)
#define MCU_FCG2_PERIPH_TMR6_2      (PWC_FCG2_TMR6_2)
#define MCU_FCG2_PERIPH_TMR6_3      (PWC_FCG2_TMR6_3)
#define MCU_FCG2_PERIPH_TMR6_4      (PWC_FCG2_TMR6_4)
#define MCU_FCG2_PERIPH_TMR6_5      (PWC_FCG2_TMR6_5)
#define MCU_FCG2_PERIPH_TMR6_6      (PWC_FCG2_TMR6_6)
#define MCU_FCG2_PERIPH_TMR6_7      (PWC_FCG2_TMR6_7)
#define MCU_FCG2_PERIPH_TMR6_8      (PWC_FCG2_TMR6_8)
#define MCU_FCG2_PERIPH_TMR4_1      (PWC_FCG2_TMR4_1)
#define MCU_FCG2_PERIPH_TMR4_2      (PWC_FCG2_TMR4_2)
#define MCU_FCG2_PERIPH_TMR4_3      (PWC_FCG2_TMR4_3)
#ifdef PWC_FCG2_HRPWM
#define MCU_FCG2_PERIPH_HRPWM       (PWC_FCG2_HRPWM)
#endif
#define MCU_FCG2_PERIPH_TMR0_1      (PWC_FCG2_TMR0_1)
#define MCU_FCG2_PERIPH_TMR0_2      (PWC_FCG2_TMR0_2)
#define MCU_FCG2_PERIPH_TMR0_3      (PWC_FCG2_TMR0_3)
#define MCU_FCG2_PERIPH_EMB         (PWC_FCG2_EMB)
#define MCU_FCG2_PERIPH_TMR2_1      (PWC_FCG2_TMR2_1)
#define MCU_FCG2_PERIPH_TMR2_2      (PWC_FCG2_TMR2_2)
#define MCU_FCG2_PERIPH_TMR2_3      (PWC_FCG2_TMR2_3)
#define MCU_FCG2_PERIPH_TMR2_4      (PWC_FCG2_TMR2_4)
#define MCU_FCG2_PERIPH_TMRA_1      (PWC_FCG2_TMRA_1)
#define MCU_FCG2_PERIPH_TMRA_2      (PWC_FCG2_TMRA_2)
#define MCU_FCG2_PERIPH_TMRA_3      (PWC_FCG2_TMRA_3)
#define MCU_FCG2_PERIPH_TMRA_4      (PWC_FCG2_TMRA_4)
#define MCU_FCG2_PERIPH_TMRA_5      (PWC_FCG2_TMRA_5)
#define MCU_FCG2_PERIPH_TMRA_6      (PWC_FCG2_TMRA_6)
#define MCU_FCG2_PERIPH_TMRA_7      (PWC_FCG2_TMRA_7)
#define MCU_FCG2_PERIPH_TMRA_8      (PWC_FCG2_TMRA_8)
#define MCU_FCG2_PERIPH_TMRA_9      (PWC_FCG2_TMRA_9)
#define MCU_FCG2_PERIPH_TMRA_10     (PWC_FCG2_TMRA_10)
#define MCU_FCG2_PERIPH_TMRA_11     (PWC_FCG2_TMRA_11)
#define MCU_FCG2_PERIPH_TMRA_12     (PWC_FCG2_TMRA_12)

/**
 * @brief FCG3 peripheral.
 */
#define MCU_FCG3_PERIPH_ADC1        (PWC_FCG3_ADC1)
#define MCU_FCG3_PERIPH_ADC2        (PWC_FCG3_ADC2)
#define MCU_FCG3_PERIPH_ADC3        (PWC_FCG3_ADC3)
#define MCU_FCG3_PERIPH_CMBIAS      (PWC_FCG3_CMBIAS)
#define MCU_FCG3_PERIPH_DAC1        (PWC_FCG3_DAC1)
#define MCU_FCG3_PERIPH_DAC2        (PWC_FCG3_DAC2)
#define MCU_FCG3_PERIPH_CMP1_2      (PWC_FCG3_CMP12)
#define MCU_FCG3_PERIPH_CMP3_4      (PWC_FCG3_CMP34)
#define MCU_FCG3_PERIPH_TMR0_4      (PWC_FCG3_TMR0_4)
#define MCU_FCG3_PERIPH_TMR0_5      (PWC_FCG3_TMR0_5)
#define MCU_FCG3_PERIPH_OTS         (PWC_FCG3_OTS)
#define MCU_FCG3_PERIPH_DVP         (PWC_FCG3_DVP)
#define MCU_FCG3_PERIPH_SMC         (PWC_FCG3_SMC)
#define MCU_FCG3_PERIPH_DMC         (PWC_FCG3_DMC)
#define MCU_FCG3_PERIPH_NFC         (PWC_FCG3_NFC)
#define MCU_FCG3_PERIPH_USART1      (PWC_FCG3_USART1)
#define MCU_FCG3_PERIPH_USART2      (PWC_FCG3_USART2)
#define MCU_FCG3_PERIPH_USART3      (PWC_FCG3_USART3)
#define MCU_FCG3_PERIPH_USART4      (PWC_FCG3_USART4)
#define MCU_FCG3_PERIPH_USART5      (PWC_FCG3_USART5)
#define MCU_FCG3_PERIPH_USART6      (PWC_FCG3_USART6)
#define MCU_FCG3_PERIPH_USART7      (PWC_FCG3_USART7)
#define MCU_FCG3_PERIPH_USART8      (PWC_FCG3_USART8)
#define MCU_FCG3_PERIPH_USART9      (PWC_FCG3_USART9)
#define MCU_FCG3_PERIPH_USART10     (PWC_FCG3_USART10)

/**
 * @brief EFM Wait Cycle.
 */
#define MCU_EFM_WAIT_CYCLE0         (0U << EFM_FRMC_FLWT_POS)
#define MCU_EFM_WAIT_CYCLE1         (1U << EFM_FRMC_FLWT_POS)
#define MCU_EFM_WAIT_CYCLE2         (2U << EFM_FRMC_FLWT_POS)
#define MCU_EFM_WAIT_CYCLE3         (3U << EFM_FRMC_FLWT_POS)
#define MCU_EFM_WAIT_CYCLE4         (4U << EFM_FRMC_FLWT_POS)
#define MCU_EFM_WAIT_CYCLE5         (5U << EFM_FRMC_FLWT_POS)
#define MCU_EFM_WAIT_CYCLE6         (6U << EFM_FRMC_FLWT_POS)
#define MCU_EFM_WAIT_CYCLE7         (7U << EFM_FRMC_FLWT_POS)
#define MCU_EFM_WAIT_CYCLE8         (8U << EFM_FRMC_FLWT_POS)
#define MCU_EFM_WAIT_CYCLE9         (9U << EFM_FRMC_FLWT_POS)
#define MCU_EFM_WAIT_CYCLE10        (10U << EFM_FRMC_FLWT_POS)
#define MCU_EFM_WAIT_CYCLE11        (11U << EFM_FRMC_FLWT_POS)
#define MCU_EFM_WAIT_CYCLE12        (12U << EFM_FRMC_FLWT_POS)
#define MCU_EFM_WAIT_CYCLE13        (13U << EFM_FRMC_FLWT_POS)
#define MCU_EFM_WAIT_CYCLE14        (14U << EFM_FRMC_FLWT_POS)
#define MCU_EFM_WAIT_CYCLE15        (15U << EFM_FRMC_FLWT_POS)

/**
 * @brief GPIO Pin Read Wait Cycle Selection.
 */
#define MCU_GPIO_RD_WAIT0           (0x00U << GPIO_PCCR_RDWT_POS)
#define MCU_GPIO_RD_WAIT1           (0x01U << GPIO_PCCR_RDWT_POS)
#define MCU_GPIO_RD_WAIT2           (0x02U << GPIO_PCCR_RDWT_POS)
#define MCU_GPIO_RD_WAIT3           (0x03U << GPIO_PCCR_RDWT_POS)
#define MCU_GPIO_RD_WAIT4           (0x04U << GPIO_PCCR_RDWT_POS)
#define MCU_GPIO_RD_WAIT5           (0x05U << GPIO_PCCR_RDWT_POS)
#define MCU_GPIO_RD_WAIT6           (0x06U << GPIO_PCCR_RDWT_POS)
#define MCU_GPIO_RD_WAIT7           (0x07U << GPIO_PCCR_RDWT_POS)

/**
 * @brief SRAM Access Wait Cycle.
 */
#define MCU_SRAM_WAIT_CYCLE0        (0U)
#define MCU_SRAM_WAIT_CYCLE1        (1U)
#define MCU_SRAM_WAIT_CYCLE2        (2U)
#define MCU_SRAM_WAIT_CYCLE3        (3U)
#define MCU_SRAM_WAIT_CYCLE4        (4U)
#define MCU_SRAM_WAIT_CYCLE5        (5U)
#define MCU_SRAM_WAIT_CYCLE6        (6U)
#define MCU_SRAM_WAIT_CYCLE7        (7U)

/**
 * @brief FCM Target Clock Source.
 */
#define FCM_TARGET_CLK_XTAL         (0x00U << FCM_MCCR_MCKS_POS)
#define FCM_TARGET_CLK_XTAL32       (0x01U << FCM_MCCR_MCKS_POS)
#define FCM_TARGET_CLK_HRC          (0x02U << FCM_MCCR_MCKS_POS)
#define FCM_TARGET_CLK_LRC          (0x03U << FCM_MCCR_MCKS_POS)
#define FCM_TARGET_CLK_SWDTLRC      (0x04U << FCM_MCCR_MCKS_POS)
#define FCM_TARGET_CLK_PCLK1        (0x05U << FCM_MCCR_MCKS_POS)
#define FCM_TARGET_CLK_PLLAP        (0x06U << FCM_MCCR_MCKS_POS)
#define FCM_TARGET_CLK_MRC          (0x07U << FCM_MCCR_MCKS_POS)
#define FCM_TARGET_CLK_PLLHP        (0x08U << FCM_MCCR_MCKS_POS)
#define FCM_TARGET_CLK_RTCLRC       (0x09U << FCM_MCCR_MCKS_POS)

/**
 * @brief FCM Target Clock Division.
 */
#define FCM_TARGET_CLK_DIV1         (0x00U << FCM_MCCR_MDIVS_POS)
#define FCM_TARGET_CLK_DIV4         (0x01U << FCM_MCCR_MDIVS_POS)
#define FCM_TARGET_CLK_DIV8         (0x02U << FCM_MCCR_MDIVS_POS)
#define FCM_TARGET_CLK_DIV32        (0x03U << FCM_MCCR_MDIVS_POS)

/**
 * @brief FCM External Reference Clock Config.
 */
#define FCM_EXT_REF_OFF             (0x00U)
#define FCM_EXT_REF_ON              (FCM_RCCR_EXREFE)

/**
 * @brief FCM Reference Clock Edge.
 */
#define FCM_REF_CLK_RISING          (0x00U)
#define FCM_REF_CLK_FALLING         (FCM_RCCR_EDGES_0)
#define FCM_REF_CLK_BOTH            (FCM_RCCR_EDGES_1)

/**
 * @brief FCM Digital Filter Config.
 */
#define FCM_DIG_FILTER_OFF          (0x00U)
#define FCM_DIG_FILTER_DIV1         (FCM_RCCR_DNFS_0)
#define FCM_DIG_FILTER_DIV4         (FCM_RCCR_DNFS_1)
#define FCM_DIG_FILTER_DIV16        (FCM_RCCR_DNFS)

/**
 * @brief FCM Feference Clock Source.
 */
#define FCM_REF_CLK_EXTCLK          (0x00U << FCM_RCCR_RCKS_POS)
#define FCM_REF_CLK_XTAL            (0x10U << FCM_RCCR_RCKS_POS)
#define FCM_REF_CLK_XTAL32          (0x11U << FCM_RCCR_RCKS_POS)
#define FCM_REF_CLK_HRC             (0x12U << FCM_RCCR_RCKS_POS)
#define FCM_REF_CLK_LRC             (0x13U << FCM_RCCR_RCKS_POS)
#define FCM_REF_CLK_SWDTLRC         (0x14U << FCM_RCCR_RCKS_POS)
#define FCM_REF_CLK_PCLK1           (0x15U << FCM_RCCR_RCKS_POS)
#define FCM_REF_CLK_PLLAP           (0x16U << FCM_RCCR_RCKS_POS)
#define FCM_REF_CLK_MRC             (0x17U << FCM_RCCR_RCKS_POS)
#define FCM_REF_CLK_PLLHP           (0x18U << FCM_RCCR_RCKS_POS)
#define FCM_REF_CLK_RTCLRC          (0x19U << FCM_RCCR_RCKS_POS)

/**
 * @brief FCM Reference Clock Division.
 */
#define FCM_REF_CLK_DIV32           (0x00U << FCM_RCCR_RDIVS_POS)
#define FCM_REF_CLK_DIV128          (0x01U << FCM_RCCR_RDIVS_POS)
#define FCM_REF_CLK_DIV1024         (0x02U << FCM_RCCR_RDIVS_POS)
#define FCM_REF_CLK_DIV8192         (0x03U << FCM_RCCR_RDIVS_POS)

/**
 * @brief FCM Abnormal Reset Function Config.
 */
#define FCM_ERR_RST_OFF             (0x00U)
#define FCM_ERR_RST_ON              (FCM_RIER_ERRE)

/**
 * @brief FCM Exception Type.
 */
#define FCM_EXP_TYPE_INT            (0x00U)
#define FCM_EXP_TYPE_RST            (FCM_RIER_ERRINTRS)

/**
 * @brief FCM Interrupt Type.
 */
#define FCM_INT_OVF                 (FCM_RIER_OVFIE)
#define FCM_INT_END                 (FCM_RIER_MENDIE)
#define FCM_INT_ERR                 (FCM_RIER_ERRIE)

/**
 * @brief XTAL error detection on or off.
 */
#define CLK_XTALSTD_OFF             (0x00U)
#define CLK_XTALSTD_ON              (CMU_XTALSTDCR_XTALSTDE)

/**
 * @brief XTALSTD mode selection.
 */
#define CLK_XTALSTD_MD_RST          (CMU_XTALSTDCR_XTALSTDRIS)
#define CLK_XTALSTD_MD_INT          (0x00U)

/**
 * @brief XTALSTD reset on or off.
 */
#define CLK_XTALSTD_RST_OFF         (0x00U)
#define CLK_XTALSTD_RST_ON          (CMU_XTALSTDCR_XTALSTDRE)

/**
 * @brief XTALSTD interrupt on or off.
 */
#define CLK_XTALSTD_INT_OFF         (0x00U)
#define CLK_XTALSTD_INT_ON          (CMU_XTALSTDCR_XTALSTDIE)

/***********************************************************************************************************************
 *  EXTERN
 **********************************************************************************************************************/
#define MCU_START_SEC_CODE
#include "Mcu_MemMap.h"

#define MCU_STOP_SEC_CODE
#include "Mcu_MemMap.h"
/***********************************************************************************************************************
 *  TYPEDEFS
 **********************************************************************************************************************/

/***********************************************************************************************************************
 *  CONSTANTS
 **********************************************************************************************************************/

/***********************************************************************************************************************
 *  GLOBAL VARIABLES
 **********************************************************************************************************************/

#ifdef __cplusplus
}
#endif

/**
 * @}
 */

#endif /* CLOCK_IP_CFG_H */

/*---------------------- End of File ----------------------------------------*/
