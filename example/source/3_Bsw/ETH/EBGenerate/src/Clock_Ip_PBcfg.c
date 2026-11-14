/***********************************************************************************************************************
 * Project Name: XHSC AUTOSAR MCAL
 * File Name: Clock_Ip_PBcfg.c
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
 *   @file       Clock_Ip_PBcfg.c
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
#include "Clock_Ip_PBcfg.h"
#include "StandardTypes.h"
#include "Clock_Ip.h"

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



/* ClockIpXtalConfig */
static const Clock_Ip_XtalConfigType l_ClockIpXtalConfig_0_st = {
              .McuXtalState_u8 = (uint8)MCU_ENABLE,
              .McuXtalMode_u8 = CLK_XTAL_MD_OSC,
              .McuXtalDriv_u8 = CLK_XTAL_DRV_LOW,
              .McuXtalStableTime_u8 = CLK_XTAL_STB_2MS,
              .McuXtalStdState_u8 = (uint8)CLK_XTALSTD_ON,
              .McuXtalStdMode_u8 = CLK_XTALSTD_MD_RST,
              .McuXtalStdInt_u8 = CLK_XTALSTD_INT_OFF,
              .McuXtalStdReset_u8 = CLK_XTALSTD_RST_ON
};

/* ClockIpWaitCycleConfig */
static const Clock_Ip_WaitCycleConfigType l_ClockIpWaitCycleConfig_0_st = {
            .McuSysClockFrequency_u32 = (uint32)2.4E8,
            .McuSramWaitCycle_u32 = MCU_SRAM_WAIT_CYCLE1,
            .McuSramHWaitCycle_u32 = MCU_SRAM_WAIT_CYCLE0,
            .McuFlashWaitCycle_u32 = MCU_EFM_WAIT_CYCLE5,
            .McuGPIOWaitCycle_u16 = MCU_GPIO_RD_WAIT4
};

/* ClockIpSelSysClockSource */
static const Clock_Ip_SelSysClockSourceType l_ClockIpSelSysClockSource_0_st = {
              .McuSelectSysClock_en = MCU_CMU_SELPLLHP_CLK,
              .McuWaitCycleConfig_ptr = &l_ClockIpWaitCycleConfig_0_st
};

/* ClockIpMrcConfig */
static const Clock_Ip_MrcConfigType l_ClockIpMrcConfig_0_st = {
    /* .McuMrcState_u8 = */
    .McuMrcState_u8 = (uint8)MCU_ENABLE,
    /* .McuMrcTrim_s8 = */
    .McuMrcTrim_s8 = (sint8)0
};

/* ClockIpPllhConfig */
static const Clock_Ip_PllhConfigType l_ClockIpPllhConfig_0_st = {
            .McuPllState_u8 = (uint8)MCU_ENABLE,
            .McuPllhMDiv_u8 = (1U - 1U),
            .McuPllhNMul_u8 = (60U - 1U),
            .McuPllhRDiv_u8 = (2U - 1U),
            .McuPllhQDiv_u8 = (2U - 1U),
            .McuPllhPDiv_u8 = (4U - 1U),
            .McuSrcClock_u8 = (uint8)MCU_CMU_HRC_CLK
};

/* ClockIpPllaConfig */
static const Clock_Ip_PllaConfigType l_ClockIpPllaConfig_0_st = {
            .McuPllState_u8 = (uint8)MCU_ENABLE,
            .McuPllaMDiv_u8 = (2U - 1U),
            .McuPllaNMul_u16 = (50U - 1U),
            .McuPllaRDiv_u8 = (4U - 1U),
            .McuPllaQDiv_u8 = (4U - 1U),
            .McuPllaPDiv_u8 = (4U - 1U)
};

/* ClockIpHrcConfig */
static const Clock_Ip_HrcConfigType l_ClockIpHrcConfig_0_st = {
              .McuHrcState_u8 = (uint8)MCU_ENABLE,
              .McuHrcTrim_s8 = (sint8)0
};

/* ClockIpXtal32Config */
static const Clock_Ip_Xtal32ConfigType l_ClockIpXtal32Config_0_st = {
            .McuXtal32State_u8 = (uint8)MCU_ENABLE,
            .McuXtal32Driv_u8 = CLK_XTAL32_DRV_MID,
            .McuXtal32Filter_u8 = CLK_XTAL32_FILTER_ALL_MD
};

/* ClockIpLrcConfig */
static const Clock_Ip_LrcConfigType l_ClockIpLrcConfig_0_st = {
            .McuLrcState_u8 = (uint8)MCU_ENABLE,
            .McuLrcTrim_s8 = (sint8)0U
};

/* ClockIpRtcLrcConfig */
static const Clock_Ip_RtcLrcConfigType l_ClockIpRtcLrcConfig_0_st = {
            .McuRtcLrcTrim_s8 = (sint8)0U
};

/* ClockIpXtalDivConfig */
static const Clock_Ip_XtalDivConfigType l_ClockIpXtalDivConfig_0_st = {
            .McuXtalDivState_u32 = (uint32)MCU_DISABLE,
            .McuXtalDivNumer_u32 = (uint32)0U,
            .McuXtalDivDenom_u32 = (uint32)0U
};

/* McuClockOut1Config */
static const Clock_Ip_PinOutConfigType l_ClockIpPinOut1Config_0_st = {
             .McuMcoState_u8 = (uint8)MCU_ENABLE,
             .McuMcoChannel_u8 = CLK_MCO1,
             .McuMcoSource_u8 = CLK_MCO_SRC_PLLHP,
             .McuMcoDivider_u8 = CLK_MCO_DIV4
};

/* McuClockOut2Config */
static const Clock_Ip_PinOutConfigType l_ClockIpPinOut2Config_0_st = {
             .McuMcoState_u8 = (uint8)MCU_ENABLE,
             .McuMcoChannel_u8 = CLK_MCO2,
             .McuMcoSource_u8 = CLK_MCO_SRC_HRC,
             .McuMcoDivider_u8 = CLK_MCO_DIV1
};

/* Clock_Ip_TpiuConfigType */
static const Clock_Ip_TpiuConfigType l_ClockIpTpiuConfig_0_4_st = {
             .McuTpiuState_u8 = MCU_ENABLE,
             .McuSysClockDiv_u8 = CLK_TPIUCLK_DIV4
};

/* ClockIpI2sConfig */
static const Clock_Ip_I2sConfigType l_ClockIpI2sConfig_0_12_st = {
              .McuI2sUnit_u8 = MCU_I2S1CLK,
              .McuI2sSource_u8 = CLK_PERIPHCLK_PCLK
};
static const Clock_Ip_I2sConfigType l_ClockIpI2sConfig_0_13_st = {
              .McuI2sUnit_u8 = MCU_I2S2CLK,
              .McuI2sSource_u8 = CLK_PERIPHCLK_PCLK
};
static const Clock_Ip_I2sConfigType l_ClockIpI2sConfig_0_14_st = {
              .McuI2sUnit_u8 = MCU_I2S3CLK,
              .McuI2sSource_u8 = CLK_PERIPHCLK_PCLK
};
static const Clock_Ip_I2sConfigType l_ClockIpI2sConfig_0_15_st = {
              .McuI2sUnit_u8 = MCU_I2S4CLK,
              .McuI2sSource_u8 = CLK_PERIPHCLK_PCLK
};

/* ClockIpSysDivConfig */
static const Clock_Ip_SelClockSourceType l_ClockIpSysDivConfig_0_0_st = {
              .McuClockSelect_en = MCU_CMU_SELSYSDIV_CLK,
              .SysClockDiv_u32 = CLK_CANCLK_SYSCLK_DIV3
};

static const Clock_Ip_SelClockSourceType l_ClockIpSysDivConfig_0_1_st = {
              .McuClockSelect_en = MCU_CMU_SELSYSDIV_CLK,
              .SysClockDiv_u32 = CLK_CANCLK_SYSCLK_DIV3
};

static const Clock_Ip_SelClockSourceType l_ClockIpSysDivConfig_0_2_st = {
              .McuClockSelect_en = MCU_CMU_SELSYSDIV_CLK,
              .SysClockDiv_u32 = CLK_CANCLK_SYSCLK_DIV3
};

static const Clock_Ip_SelClockSourceType l_ClockIpSysDivConfig_0_3_st = {
              .McuClockSelect_en = MCU_CMU_SELSYSDIV_CLK,
              .SysClockDiv_u32 = CLK_CANCLK_SYSCLK_DIV3
};

static const Clock_Ip_SelClockSourceType l_ClockIpSysDivConfig_0_5_st = {
              .McuClockSelect_en = MCU_CMU_SELSYSDIV_CLK,
              .SysClockDiv_u32 = CLK_HCLK_DIV1
};

static const Clock_Ip_SelClockSourceType l_ClockIpSysDivConfig_0_6_st = {
              .McuClockSelect_en = MCU_CMU_SELSYSDIV_CLK,
              .SysClockDiv_u32 = CLK_PCLK0_DIV1
};

static const Clock_Ip_SelClockSourceType l_ClockIpSysDivConfig_0_7_st = {
              .McuClockSelect_en = MCU_CMU_SELSYSDIV_CLK,
              .SysClockDiv_u32 = CLK_PCLK1_DIV2
};

static const Clock_Ip_SelClockSourceType l_ClockIpSysDivConfig_0_8_st = {
              .McuClockSelect_en = MCU_CMU_SELSYSDIV_CLK,
              .SysClockDiv_u32 = CLK_PCLK2_DIV4
};

static const Clock_Ip_SelClockSourceType l_ClockIpSysDivConfig_0_9_st = {
              .McuClockSelect_en = MCU_CMU_SELSYSDIV_CLK,
              .SysClockDiv_u32 = CLK_PCLK3_DIV4
};

static const Clock_Ip_SelClockSourceType l_ClockIpSysDivConfig_0_10_st = {
              .McuClockSelect_en = MCU_CMU_SELSYSDIV_CLK,
              .SysClockDiv_u32 = CLK_PCLK4_DIV2
};

static const Clock_Ip_SelClockSourceType l_ClockIpSysDivConfig_0_11_st = {
              .McuClockSelect_en = MCU_CMU_SELSYSDIV_CLK,
              .SysClockDiv_u32 = CLK_EXCLK_DIV2
};

static const Clock_Ip_SelClockSourceType l_ClockIpSysDivConfig_0_16_st = {
              .McuClockSelect_en = MCU_CMU_SELSYSDIV_CLK,
              .SysClockDiv_u32 = CLK_USBCLK_SYSCLK_DIV6
};

static const Clock_Ip_SelClockSourceType l_ClockIpSysDivConfig_0_17_st = {
              .McuClockSelect_en = MCU_CMU_SELRTCLRC_CLK,
              .SysClockDiv_u32 = 1U
};

static const Clock_Ip_SelClockSourceType l_ClockIpSysDivConfig_0_18_st = {
              .McuClockSelect_en = MCU_CMU_DEFAULT_CLK,
              .SysClockDiv_u32 = 0U
};

static const Clock_Ip_SelClockSourceType l_ClockIpSysDivConfig_0_19_st = {
              .McuClockSelect_en = MCU_CMU_DEFAULT_CLK,
              .SysClockDiv_u32 = 0U
};

/* ClockIpPeripheralConfig */
static const Clock_Ip_PeriphClockSourceType l_ClockIpPeriphClocks_0_0_st = {
              .McuPeriphClock_en = MCU_CAN1CLK,
              .McuPeriphConfig_ptr = &l_ClockIpSysDivConfig_0_0_st
};
static const Clock_Ip_PeriphClockSourceType l_ClockIpPeriphClocks_0_1_st = {
              .McuPeriphClock_en = MCU_CAN2CLK,
              .McuPeriphConfig_ptr = &l_ClockIpSysDivConfig_0_1_st
};
static const Clock_Ip_PeriphClockSourceType l_ClockIpPeriphClocks_0_2_st = {
              .McuPeriphClock_en = MCU_MCAN1CLK,
              .McuPeriphConfig_ptr = &l_ClockIpSysDivConfig_0_2_st
};
static const Clock_Ip_PeriphClockSourceType l_ClockIpPeriphClocks_0_3_st = {
              .McuPeriphClock_en = MCU_MCAN2CLK,
              .McuPeriphConfig_ptr = &l_ClockIpSysDivConfig_0_3_st
};
static const Clock_Ip_PeriphClockSourceType l_ClockIpPeriphClocks_0_4_st = {
              .McuPeriphClock_en = MCU_TPIUCLK,
              .McuPeriphConfig_ptr = &l_ClockIpTpiuConfig_0_4_st
};
static const Clock_Ip_PeriphClockSourceType l_ClockIpPeriphClocks_0_5_st = {
              .McuPeriphClock_en = MCU_HCLK,
              .McuPeriphConfig_ptr = &l_ClockIpSysDivConfig_0_5_st
};
static const Clock_Ip_PeriphClockSourceType l_ClockIpPeriphClocks_0_6_st = {
              .McuPeriphClock_en = MCU_PCLK0,
              .McuPeriphConfig_ptr = &l_ClockIpSysDivConfig_0_6_st
};
static const Clock_Ip_PeriphClockSourceType l_ClockIpPeriphClocks_0_7_st = {
              .McuPeriphClock_en = MCU_PCLK1,
              .McuPeriphConfig_ptr = &l_ClockIpSysDivConfig_0_7_st
};
static const Clock_Ip_PeriphClockSourceType l_ClockIpPeriphClocks_0_8_st = {
              .McuPeriphClock_en = MCU_PCLK2,
              .McuPeriphConfig_ptr = &l_ClockIpSysDivConfig_0_8_st
};
static const Clock_Ip_PeriphClockSourceType l_ClockIpPeriphClocks_0_9_st = {
              .McuPeriphClock_en = MCU_PCLK3,
              .McuPeriphConfig_ptr = &l_ClockIpSysDivConfig_0_9_st
};
static const Clock_Ip_PeriphClockSourceType l_ClockIpPeriphClocks_0_10_st = {
              .McuPeriphClock_en = MCU_PCLK4,
              .McuPeriphConfig_ptr = &l_ClockIpSysDivConfig_0_10_st
};
static const Clock_Ip_PeriphClockSourceType l_ClockIpPeriphClocks_0_11_st = {
              .McuPeriphClock_en = MCU_EXCLK,
              .McuPeriphConfig_ptr = &l_ClockIpSysDivConfig_0_11_st
};
static const Clock_Ip_PeriphClockSourceType l_ClockIpPeriphClocks_0_12_st = {
              .McuPeriphClock_en = MCU_I2SCLK,
              .McuPeriphConfig_ptr = &l_ClockIpI2sConfig_0_12_st
};
static const Clock_Ip_PeriphClockSourceType l_ClockIpPeriphClocks_0_13_st = {
              .McuPeriphClock_en = MCU_I2SCLK,
              .McuPeriphConfig_ptr = &l_ClockIpI2sConfig_0_13_st
};
static const Clock_Ip_PeriphClockSourceType l_ClockIpPeriphClocks_0_14_st = {
              .McuPeriphClock_en = MCU_I2SCLK,
              .McuPeriphConfig_ptr = &l_ClockIpI2sConfig_0_14_st
};
static const Clock_Ip_PeriphClockSourceType l_ClockIpPeriphClocks_0_15_st = {
              .McuPeriphClock_en = MCU_I2SCLK,
              .McuPeriphConfig_ptr = &l_ClockIpI2sConfig_0_15_st
};
static const Clock_Ip_PeriphClockSourceType l_ClockIpPeriphClocks_0_16_st = {
              .McuPeriphClock_en = MCU_USBCLK,
              .McuPeriphConfig_ptr = &l_ClockIpSysDivConfig_0_16_st
};
static const Clock_Ip_PeriphClockSourceType l_ClockIpPeriphClocks_0_17_st = {
              .McuPeriphClock_en = MCU_RTCCLK,
              .McuPeriphConfig_ptr = &l_ClockIpSysDivConfig_0_17_st
};
static const Clock_Ip_PeriphClockSourceType l_ClockIpPeriphClocks_0_18_st = {
              .McuPeriphClock_en = MCU_STCLK,
              .McuPeriphConfig_ptr = &l_ClockIpSysDivConfig_0_18_st
};
static const Clock_Ip_PeriphClockSourceType l_ClockIpPeriphClocks_0_19_st = {
              .McuPeriphClock_en = MCU_SWDTCLK,
              .McuPeriphConfig_ptr = &l_ClockIpSysDivConfig_0_19_st
};
static const Clock_Ip_PeriphClockSourceType l_ClockIpPeriphClocks_0_20_st = {
              .McuPeriphClock_en = MCU_MCO1CLK,
              .McuPeriphConfig_ptr = &l_ClockIpPinOut1Config_0_st
};
static const Clock_Ip_PeriphClockSourceType l_ClockIpPeriphClocks_0_21_st = {
              .McuPeriphClock_en = MCU_MCO2CLK,
              .McuPeriphConfig_ptr = &l_ClockIpPinOut2Config_0_st
};

/* Clock_Ip_Fcm1ConfigType */
static const Clock_Ip_FcmConfigType l_ClockIpFcm1Config_0_st = {
            .McuFcmState_u8 = (uint8)MCU_DISABLE,
            .McuFcmLowerLimit_u16 = 0U,
            .McuFcmUpperLimit_u16 = 0U,
            .McuFcmTargetClock_u32 = FCM_TARGET_CLK_XTAL,
            .McuFcmTargetClockDiv_u32 = FCM_TARGET_CLK_DIV1,
            .McuFcmExtRefClockEnable_u32 = FCM_EXT_REF_OFF,
            .McuFcmRefClockEdge_u32 = FCM_REF_CLK_RISING,
            .McuFcmDigitalFilter_u32 = FCM_DIG_FILTER_OFF,
            .McuFcmRefClock_u32 = FCM_REF_CLK_EXTCLK,
            .McuFcmRefClockDiv_u32 = FCM_REF_CLK_DIV32,
            .McuFcmExceptionType_u32 = FCM_EXP_TYPE_INT,
            .McuFcmIntOvf_u8 = (uint8)MCU_DISABLE,
            .McuFcmIntEnd_u8 = (uint8)MCU_DISABLE,
            .McuFcmIntErr_u8 = (uint8)MCU_DISABLE
}; /* Clock_Ip_Fcm1ConfigType */

/* Clock_Ip_Fcm2ConfigType */
static const Clock_Ip_FcmConfigType l_ClockIpFcm2Config_0_st = {
            .McuFcmState_u8 = (uint8)MCU_DISABLE,
            .McuFcmLowerLimit_u16 = 0U,
            .McuFcmUpperLimit_u16 = 0U,
            .McuFcmTargetClock_u32 = FCM_TARGET_CLK_XTAL,
            .McuFcmTargetClockDiv_u32 = FCM_TARGET_CLK_DIV1,
            .McuFcmExtRefClockEnable_u32 = FCM_EXT_REF_OFF,
            .McuFcmRefClockEdge_u32 = FCM_REF_CLK_RISING,
            .McuFcmDigitalFilter_u32 = FCM_DIG_FILTER_OFF,
            .McuFcmRefClock_u32 = FCM_REF_CLK_EXTCLK,
            .McuFcmRefClockDiv_u32 = FCM_REF_CLK_DIV32,
            .McuFcmExceptionType_u32 = FCM_EXP_TYPE_INT,
            .McuFcmIntOvf_u8 = (uint8)MCU_DISABLE,
            .McuFcmIntEnd_u8 = (uint8)MCU_DISABLE,
            .McuFcmIntErr_u8 = (uint8)MCU_DISABLE
}; /* Clock_Ip_Fcm2ConfigType */

/* Clock_Ip_Fcm3ConfigType */  
static const Clock_Ip_FcmConfigType l_ClockIpFcm3Config_0_st = {
            .McuFcmState_u8 = (uint8)MCU_DISABLE,
            .McuFcmLowerLimit_u16 = 0U,
            .McuFcmUpperLimit_u16 = 0U,
            .McuFcmTargetClock_u32 = FCM_TARGET_CLK_XTAL,
            .McuFcmTargetClockDiv_u32 = FCM_TARGET_CLK_DIV1, 
            .McuFcmExtRefClockEnable_u32 = FCM_EXT_REF_OFF,
            .McuFcmRefClockEdge_u32 = FCM_REF_CLK_RISING,
            .McuFcmDigitalFilter_u32 = FCM_DIG_FILTER_OFF,
            .McuFcmRefClock_u32 = FCM_REF_CLK_EXTCLK,
            .McuFcmRefClockDiv_u32 = FCM_REF_CLK_DIV32, 
            .McuFcmExceptionType_u32 = FCM_EXP_TYPE_INT,
            .McuFcmIntOvf_u8 = (uint8)MCU_DISABLE,
            .McuFcmIntEnd_u8 = (uint8)MCU_DISABLE,
            .McuFcmIntErr_u8 = (uint8)MCU_DISABLE
}; /* Clock_Ip_Fcm3ConfigType */


/* Clock_Ip_Fcm4ConfigType */
static const Clock_Ip_FcmConfigType l_ClockIpFcm4Config_0_st = {
            .McuFcmState_u8 = (uint8)MCU_DISABLE,
            .McuFcmLowerLimit_u16 = 0U,
            .McuFcmUpperLimit_u16 = 0U,
            .McuFcmTargetClock_u32 = FCM_TARGET_CLK_XTAL,
            .McuFcmTargetClockDiv_u32 = FCM_TARGET_CLK_DIV1, 
            .McuFcmExtRefClockEnable_u32 = FCM_EXT_REF_OFF,
            .McuFcmRefClockEdge_u32 = FCM_REF_CLK_RISING,
            .McuFcmDigitalFilter_u32 = FCM_DIG_FILTER_OFF,
            .McuFcmRefClock_u32 = FCM_REF_CLK_EXTCLK,
            .McuFcmRefClockDiv_u32 = FCM_REF_CLK_DIV32, 
            .McuFcmExceptionType_u32 = FCM_EXP_TYPE_INT,  
            .McuFcmIntOvf_u8 = (uint8)MCU_DISABLE,
            .McuFcmIntEnd_u8 = (uint8)MCU_DISABLE,
            .McuFcmIntErr_u8 = (uint8)MCU_DISABLE
}; /* Clock_Ip_Fcm4ConfigType */

/* Mcu_ClockConfig */
static const Clock_Ip_ClockHwConfigType l_McuClockSettingConfig_0_ClockConfigList_aa[] = {
  {
      .McuClockSource_en = MCU_CMU_XTAL_CLK,
      .McuClockConfig_ptr = &l_ClockIpXtalConfig_0_st
  },
  {
      .McuClockSource_en = MCU_CMU_LRC_CLK,
      .McuClockConfig_ptr = &l_ClockIpLrcConfig_0_st
  },
  {
      .McuClockSource_en = MCU_CMU_HRC_CLK,
      .McuClockConfig_ptr = &l_ClockIpHrcConfig_0_st
  },
  {
      .McuClockSource_en = MCU_CMU_XTAL32_CLK,
      .McuClockConfig_ptr = &l_ClockIpXtal32Config_0_st
  },
  {
      .McuClockSource_en = MCU_CMU_RTCLRC_CLK,
      .McuClockConfig_ptr = &l_ClockIpRtcLrcConfig_0_st
  },
  {
      .McuClockSource_en = MCU_CMU_PLLH_CLK,
      .McuClockConfig_ptr = &l_ClockIpPllhConfig_0_st
  },
  {
      .McuClockSource_en = MCU_CMU_PLLA_CLK,
      .McuClockConfig_ptr = &l_ClockIpPllaConfig_0_st
  },
  {
      .McuClockSource_en = MCU_CMU_XTALDIV_CLK,
      .McuClockConfig_ptr = &l_ClockIpXtalDivConfig_0_st
  },
  {
      .McuClockSource_en = MCU_CMU_SYS_CLK,
      .McuClockConfig_ptr = &l_ClockIpSelSysClockSource_0_st
  },
  {
      .McuClockSource_en = MCU_CMU_MRC_CLK,
      .McuClockConfig_ptr = &l_ClockIpMrcConfig_0_st
  },
  {
      .McuClockSource_en = MCU_CMU_PERIPH_CLK,
      .McuClockConfig_ptr = &l_ClockIpPeriphClocks_0_0_st
  },
  {
      .McuClockSource_en = MCU_CMU_PERIPH_CLK,
      .McuClockConfig_ptr = &l_ClockIpPeriphClocks_0_1_st
  },
  {
      .McuClockSource_en = MCU_CMU_PERIPH_CLK,
      .McuClockConfig_ptr = &l_ClockIpPeriphClocks_0_2_st
  },
  {
      .McuClockSource_en = MCU_CMU_PERIPH_CLK,
      .McuClockConfig_ptr = &l_ClockIpPeriphClocks_0_3_st
  },
  {
      .McuClockSource_en = MCU_CMU_PERIPH_CLK,
      .McuClockConfig_ptr = &l_ClockIpPeriphClocks_0_4_st
  },
  {
      .McuClockSource_en = MCU_CMU_PERIPH_CLK,
      .McuClockConfig_ptr = &l_ClockIpPeriphClocks_0_5_st
  },
  {
      .McuClockSource_en = MCU_CMU_PERIPH_CLK,
      .McuClockConfig_ptr = &l_ClockIpPeriphClocks_0_6_st
  },
  {
      .McuClockSource_en = MCU_CMU_PERIPH_CLK,
      .McuClockConfig_ptr = &l_ClockIpPeriphClocks_0_7_st
  },
  {
      .McuClockSource_en = MCU_CMU_PERIPH_CLK,
      .McuClockConfig_ptr = &l_ClockIpPeriphClocks_0_8_st
  },
  {
      .McuClockSource_en = MCU_CMU_PERIPH_CLK,
      .McuClockConfig_ptr = &l_ClockIpPeriphClocks_0_9_st
  },
  {
      .McuClockSource_en = MCU_CMU_PERIPH_CLK,
      .McuClockConfig_ptr = &l_ClockIpPeriphClocks_0_10_st
  },
  {
      .McuClockSource_en = MCU_CMU_PERIPH_CLK,
      .McuClockConfig_ptr = &l_ClockIpPeriphClocks_0_11_st
  },
  {
      .McuClockSource_en = MCU_CMU_PERIPH_CLK,
      .McuClockConfig_ptr = &l_ClockIpPeriphClocks_0_12_st
  },
  {
      .McuClockSource_en = MCU_CMU_PERIPH_CLK,
      .McuClockConfig_ptr = &l_ClockIpPeriphClocks_0_13_st
  },
  {
      .McuClockSource_en = MCU_CMU_PERIPH_CLK,
      .McuClockConfig_ptr = &l_ClockIpPeriphClocks_0_14_st
  },
  {
      .McuClockSource_en = MCU_CMU_PERIPH_CLK,
      .McuClockConfig_ptr = &l_ClockIpPeriphClocks_0_15_st
  },
  {
      .McuClockSource_en = MCU_CMU_PERIPH_CLK,
      .McuClockConfig_ptr = &l_ClockIpPeriphClocks_0_16_st
  },
  {
      .McuClockSource_en = MCU_CMU_PERIPH_CLK,
      .McuClockConfig_ptr = &l_ClockIpPeriphClocks_0_17_st
  },
  {
      .McuClockSource_en = MCU_CMU_PERIPH_CLK,
      .McuClockConfig_ptr = &l_ClockIpPeriphClocks_0_18_st
  },
  {
      .McuClockSource_en = MCU_CMU_PERIPH_CLK,
      .McuClockConfig_ptr = &l_ClockIpPeriphClocks_0_19_st
  },
  {
      .McuClockSource_en = MCU_CMU_PERIPH_CLK,
      .McuClockConfig_ptr = &l_ClockIpPeriphClocks_0_20_st
  },
  {
      .McuClockSource_en = MCU_CMU_PERIPH_CLK,
      .McuClockConfig_ptr = &l_ClockIpPeriphClocks_0_21_st
  },
  {
      .McuClockSource_en = MCU_CMU_FCM1_CLK,
      .McuClockConfig_ptr = &l_ClockIpFcm1Config_0_st
  },
  {
      .McuClockSource_en = MCU_CMU_FCM2_CLK,
      .McuClockConfig_ptr = &l_ClockIpFcm2Config_0_st
  },
  {
      .McuClockSource_en = MCU_CMU_FCM3_CLK,
      .McuClockConfig_ptr = &l_ClockIpFcm3Config_0_st
  },
  {
      .McuClockSource_en = MCU_CMU_FCM4_CLK,
      .McuClockConfig_ptr = &l_ClockIpFcm4Config_0_st
  }
};

/* Mcu_FuncClockControl */
static const Clock_Ip_FuncClockControlType l_McuClockSettingConfig_0_FuncClockControlList_aa[] = {
  {
    .McuPeripheralState_u8 = (uint8)MCU_ENABLE,
    .McuPeripheralGroup_u8 = MCU_FCG_FCG0_PERIPH_GROUP,
    .McuPeripheralName_u32 = MCU_FCG0_PERIPH_SRAMH
  },
  {
    .McuPeripheralState_u8 = (uint8)MCU_ENABLE,
    .McuPeripheralGroup_u8 = MCU_FCG_FCG0_PERIPH_GROUP,
    .McuPeripheralName_u32 = MCU_FCG0_PERIPH_SRAM1
  },
  {
    .McuPeripheralState_u8 = (uint8)MCU_ENABLE,
    .McuPeripheralGroup_u8 = MCU_FCG_FCG0_PERIPH_GROUP,
    .McuPeripheralName_u32 = MCU_FCG0_PERIPH_SRAM2
  },
  {
    .McuPeripheralState_u8 = (uint8)MCU_ENABLE,
    .McuPeripheralGroup_u8 = MCU_FCG_FCG0_PERIPH_GROUP,
    .McuPeripheralName_u32 = MCU_FCG0_PERIPH_SRAM3
  },
  {
    .McuPeripheralState_u8 = (uint8)MCU_ENABLE,
    .McuPeripheralGroup_u8 = MCU_FCG_FCG0_PERIPH_GROUP,
    .McuPeripheralName_u32 = MCU_FCG0_PERIPH_SRAM4
  },
  {
    .McuPeripheralState_u8 = (uint8)MCU_ENABLE,
    .McuPeripheralGroup_u8 = MCU_FCG_FCG0_PERIPH_GROUP,
    .McuPeripheralName_u32 = MCU_FCG0_PERIPH_ERMU
  },
  {
    .McuPeripheralState_u8 = (uint8)MCU_ENABLE,
    .McuPeripheralGroup_u8 = MCU_FCG_FCG0_PERIPH_GROUP,
    .McuPeripheralName_u32 = MCU_FCG0_PERIPH_SRAMB
  },
  {
    .McuPeripheralState_u8 = (uint8)MCU_ENABLE,
    .McuPeripheralGroup_u8 = MCU_FCG_FCG0_PERIPH_GROUP,
    .McuPeripheralName_u32 = MCU_FCG0_PERIPH_PKE
  },
  {
    .McuPeripheralState_u8 = (uint8)MCU_ENABLE,
    .McuPeripheralGroup_u8 = MCU_FCG_FCG0_PERIPH_GROUP,
    .McuPeripheralName_u32 = MCU_FCG0_PERIPH_KEY
  },
  {
    .McuPeripheralState_u8 = (uint8)MCU_ENABLE,
    .McuPeripheralGroup_u8 = MCU_FCG_FCG0_PERIPH_GROUP,
    .McuPeripheralName_u32 = MCU_FCG0_PERIPH_DMA1
  },
  {
    .McuPeripheralState_u8 = (uint8)MCU_ENABLE,
    .McuPeripheralGroup_u8 = MCU_FCG_FCG0_PERIPH_GROUP,
    .McuPeripheralName_u32 = MCU_FCG0_PERIPH_DMA2
  },
  {
    .McuPeripheralState_u8 = (uint8)MCU_ENABLE,
    .McuPeripheralGroup_u8 = MCU_FCG_FCG0_PERIPH_GROUP,
    .McuPeripheralName_u32 = MCU_FCG0_PERIPH_FCM
  },
  {
    .McuPeripheralState_u8 = (uint8)MCU_ENABLE,
    .McuPeripheralGroup_u8 = MCU_FCG_FCG0_PERIPH_GROUP,
    .McuPeripheralName_u32 = MCU_FCG0_PERIPH_AOS
  },
  {
    .McuPeripheralState_u8 = (uint8)MCU_ENABLE,
    .McuPeripheralGroup_u8 = MCU_FCG_FCG0_PERIPH_GROUP,
    .McuPeripheralName_u32 = MCU_FCG0_PERIPH_CTC
  },
  {
    .McuPeripheralState_u8 = (uint8)MCU_ENABLE,
    .McuPeripheralGroup_u8 = MCU_FCG_FCG0_PERIPH_GROUP,
    .McuPeripheralName_u32 = MCU_FCG0_PERIPH_MAU
  },
  {
    .McuPeripheralState_u8 = (uint8)MCU_ENABLE,
    .McuPeripheralGroup_u8 = MCU_FCG_FCG0_PERIPH_GROUP,
    .McuPeripheralName_u32 = MCU_FCG0_PERIPH_SKE_AES
  },
  {
    .McuPeripheralState_u8 = (uint8)MCU_ENABLE,
    .McuPeripheralGroup_u8 = MCU_FCG_FCG0_PERIPH_GROUP,
    .McuPeripheralName_u32 = MCU_FCG0_PERIPH_HASH
  },
  {
    .McuPeripheralState_u8 = (uint8)MCU_ENABLE,
    .McuPeripheralGroup_u8 = MCU_FCG_FCG0_PERIPH_GROUP,
    .McuPeripheralName_u32 = MCU_FCG0_PERIPH_TRNG
  },
  {
    .McuPeripheralState_u8 = (uint8)MCU_ENABLE,
    .McuPeripheralGroup_u8 = MCU_FCG_FCG0_PERIPH_GROUP,
    .McuPeripheralName_u32 = MCU_FCG0_PERIPH_CRC
  },
  {
    .McuPeripheralState_u8 = (uint8)MCU_ENABLE,
    .McuPeripheralGroup_u8 = MCU_FCG_FCG0_PERIPH_GROUP,
    .McuPeripheralName_u32 = MCU_FCG0_PERIPH_DCU1
  },
  {
    .McuPeripheralState_u8 = (uint8)MCU_ENABLE,
    .McuPeripheralGroup_u8 = MCU_FCG_FCG0_PERIPH_GROUP,
    .McuPeripheralName_u32 = MCU_FCG0_PERIPH_DCU2
  },
  {
    .McuPeripheralState_u8 = (uint8)MCU_ENABLE,
    .McuPeripheralGroup_u8 = MCU_FCG_FCG0_PERIPH_GROUP,
    .McuPeripheralName_u32 = MCU_FCG0_PERIPH_DCU3
  },
  {
    .McuPeripheralState_u8 = (uint8)MCU_ENABLE,
    .McuPeripheralGroup_u8 = MCU_FCG_FCG0_PERIPH_GROUP,
    .McuPeripheralName_u32 = MCU_FCG0_PERIPH_DCU4
  },
  {
    .McuPeripheralState_u8 = (uint8)MCU_ENABLE,
    .McuPeripheralGroup_u8 = MCU_FCG_FCG0_PERIPH_GROUP,
    .McuPeripheralName_u32 = MCU_FCG0_PERIPH_DCU5
  },
  {
    .McuPeripheralState_u8 = (uint8)MCU_ENABLE,
    .McuPeripheralGroup_u8 = MCU_FCG_FCG0_PERIPH_GROUP,
    .McuPeripheralName_u32 = MCU_FCG0_PERIPH_DCU6
  },
  {
    .McuPeripheralState_u8 = (uint8)MCU_ENABLE,
    .McuPeripheralGroup_u8 = MCU_FCG_FCG0_PERIPH_GROUP,
    .McuPeripheralName_u32 = MCU_FCG0_PERIPH_DCU7
  },
  {
    .McuPeripheralState_u8 = (uint8)MCU_ENABLE,
    .McuPeripheralGroup_u8 = MCU_FCG_FCG0_PERIPH_GROUP,
    .McuPeripheralName_u32 = MCU_FCG0_PERIPH_DCU8
  },
  {
    .McuPeripheralState_u8 = (uint8)MCU_ENABLE,
    .McuPeripheralGroup_u8 = MCU_FCG_FCG1_PERIPH_GROUP,
    .McuPeripheralName_u32 = MCU_FCG1_PERIPH_CAN1
  },
  {
    .McuPeripheralState_u8 = (uint8)MCU_ENABLE,
    .McuPeripheralGroup_u8 = MCU_FCG_FCG1_PERIPH_GROUP,
    .McuPeripheralName_u32 = MCU_FCG1_PERIPH_CAN2
  },
  {
    .McuPeripheralState_u8 = (uint8)MCU_ENABLE,
    .McuPeripheralGroup_u8 = MCU_FCG_FCG1_PERIPH_GROUP,
    .McuPeripheralName_u32 = MCU_FCG1_PERIPH_ETHMAC
  },
  {
    .McuPeripheralState_u8 = (uint8)MCU_ENABLE,
    .McuPeripheralGroup_u8 = MCU_FCG_FCG1_PERIPH_GROUP,
    .McuPeripheralName_u32 = MCU_FCG1_PERIPH_QSPI
  },
  {
    .McuPeripheralState_u8 = (uint8)MCU_ENABLE,
    .McuPeripheralGroup_u8 = MCU_FCG_FCG1_PERIPH_GROUP,
    .McuPeripheralName_u32 = MCU_FCG1_PERIPH_I2C1
  },
  {
    .McuPeripheralState_u8 = (uint8)MCU_ENABLE,
    .McuPeripheralGroup_u8 = MCU_FCG_FCG1_PERIPH_GROUP,
    .McuPeripheralName_u32 = MCU_FCG1_PERIPH_I2C2
  },
  {
    .McuPeripheralState_u8 = (uint8)MCU_ENABLE,
    .McuPeripheralGroup_u8 = MCU_FCG_FCG1_PERIPH_GROUP,
    .McuPeripheralName_u32 = MCU_FCG1_PERIPH_I2C3
  },
  {
    .McuPeripheralState_u8 = (uint8)MCU_ENABLE,
    .McuPeripheralGroup_u8 = MCU_FCG_FCG1_PERIPH_GROUP,
    .McuPeripheralName_u32 = MCU_FCG1_PERIPH_I2C4
  },
  {
    .McuPeripheralState_u8 = (uint8)MCU_ENABLE,
    .McuPeripheralGroup_u8 = MCU_FCG_FCG1_PERIPH_GROUP,
    .McuPeripheralName_u32 = MCU_FCG1_PERIPH_I2C5
  },
  {
    .McuPeripheralState_u8 = (uint8)MCU_ENABLE,
    .McuPeripheralGroup_u8 = MCU_FCG_FCG1_PERIPH_GROUP,
    .McuPeripheralName_u32 = MCU_FCG1_PERIPH_I2C6
  },
  {
    .McuPeripheralState_u8 = (uint8)MCU_ENABLE,
    .McuPeripheralGroup_u8 = MCU_FCG_FCG1_PERIPH_GROUP,
    .McuPeripheralName_u32 = MCU_FCG1_PERIPH_SDIOC1
  },
  {
    .McuPeripheralState_u8 = (uint8)MCU_ENABLE,
    .McuPeripheralGroup_u8 = MCU_FCG_FCG1_PERIPH_GROUP,
    .McuPeripheralName_u32 = MCU_FCG1_PERIPH_SDIOC2
  },
  {
    .McuPeripheralState_u8 = (uint8)MCU_ENABLE,
    .McuPeripheralGroup_u8 = MCU_FCG_FCG1_PERIPH_GROUP,
    .McuPeripheralName_u32 = MCU_FCG1_PERIPH_I2S1
  },
  {
    .McuPeripheralState_u8 = (uint8)MCU_ENABLE,
    .McuPeripheralGroup_u8 = MCU_FCG_FCG1_PERIPH_GROUP,
    .McuPeripheralName_u32 = MCU_FCG1_PERIPH_I2S2
  },
  {
    .McuPeripheralState_u8 = (uint8)MCU_ENABLE,
    .McuPeripheralGroup_u8 = MCU_FCG_FCG1_PERIPH_GROUP,
    .McuPeripheralName_u32 = MCU_FCG1_PERIPH_I2S3
  },
  {
    .McuPeripheralState_u8 = (uint8)MCU_ENABLE,
    .McuPeripheralGroup_u8 = MCU_FCG_FCG1_PERIPH_GROUP,
    .McuPeripheralName_u32 = MCU_FCG1_PERIPH_I2S4
  },
  {
    .McuPeripheralState_u8 = (uint8)MCU_ENABLE,
    .McuPeripheralGroup_u8 = MCU_FCG_FCG1_PERIPH_GROUP,
    .McuPeripheralName_u32 = MCU_FCG1_PERIPH_SPI1
  },
  {
    .McuPeripheralState_u8 = (uint8)MCU_ENABLE,
    .McuPeripheralGroup_u8 = MCU_FCG_FCG1_PERIPH_GROUP,
    .McuPeripheralName_u32 = MCU_FCG1_PERIPH_SPI2
  },
  {
    .McuPeripheralState_u8 = (uint8)MCU_ENABLE,
    .McuPeripheralGroup_u8 = MCU_FCG_FCG1_PERIPH_GROUP,
    .McuPeripheralName_u32 = MCU_FCG1_PERIPH_SPI3
  },
  {
    .McuPeripheralState_u8 = (uint8)MCU_ENABLE,
    .McuPeripheralGroup_u8 = MCU_FCG_FCG1_PERIPH_GROUP,
    .McuPeripheralName_u32 = MCU_FCG1_PERIPH_SPI4
  },
  {
    .McuPeripheralState_u8 = (uint8)MCU_ENABLE,
    .McuPeripheralGroup_u8 = MCU_FCG_FCG1_PERIPH_GROUP,
    .McuPeripheralName_u32 = MCU_FCG1_PERIPH_SPI5
  },
  {
    .McuPeripheralState_u8 = (uint8)MCU_ENABLE,
    .McuPeripheralGroup_u8 = MCU_FCG_FCG1_PERIPH_GROUP,
    .McuPeripheralName_u32 = MCU_FCG1_PERIPH_SPI6
  },
  {
    .McuPeripheralState_u8 = (uint8)MCU_ENABLE,
    .McuPeripheralGroup_u8 = MCU_FCG_FCG1_PERIPH_GROUP,
    .McuPeripheralName_u32 = MCU_FCG1_PERIPH_USBFS
  },
  {
    .McuPeripheralState_u8 = (uint8)MCU_ENABLE,
    .McuPeripheralGroup_u8 = MCU_FCG_FCG1_PERIPH_GROUP,
    .McuPeripheralName_u32 = MCU_FCG1_PERIPH_USBHS
  },
  {
    .McuPeripheralState_u8 = (uint8)MCU_ENABLE,
    .McuPeripheralGroup_u8 = MCU_FCG_FCG1_PERIPH_GROUP,
    .McuPeripheralName_u32 = MCU_FCG1_PERIPH_FMAC1
  },
  {
    .McuPeripheralState_u8 = (uint8)MCU_ENABLE,
    .McuPeripheralGroup_u8 = MCU_FCG_FCG1_PERIPH_GROUP,
    .McuPeripheralName_u32 = MCU_FCG1_PERIPH_FMAC2
  },
  {
    .McuPeripheralState_u8 = (uint8)MCU_ENABLE,
    .McuPeripheralGroup_u8 = MCU_FCG_FCG1_PERIPH_GROUP,
    .McuPeripheralName_u32 = MCU_FCG1_PERIPH_FMAC3
  },
  {
    .McuPeripheralState_u8 = (uint8)MCU_ENABLE,
    .McuPeripheralGroup_u8 = MCU_FCG_FCG1_PERIPH_GROUP,
    .McuPeripheralName_u32 = MCU_FCG1_PERIPH_FMAC4
  },
  {
    .McuPeripheralState_u8 = (uint8)MCU_ENABLE,
    .McuPeripheralGroup_u8 = MCU_FCG_FCG1_PERIPH_GROUP,
    .McuPeripheralName_u32 = MCU_FCG1_PERIPH_MCAN1
  },
  {
    .McuPeripheralState_u8 = (uint8)MCU_ENABLE,
    .McuPeripheralGroup_u8 = MCU_FCG_FCG1_PERIPH_GROUP,
    .McuPeripheralName_u32 = MCU_FCG1_PERIPH_MCAN2
  },
  {
    .McuPeripheralState_u8 = (uint8)MCU_ENABLE,
    .McuPeripheralGroup_u8 = MCU_FCG_FCG2_PERIPH_GROUP,
    .McuPeripheralName_u32 = MCU_FCG2_PERIPH_TMR6_1
  },
  {
    .McuPeripheralState_u8 = (uint8)MCU_ENABLE,
    .McuPeripheralGroup_u8 = MCU_FCG_FCG2_PERIPH_GROUP,
    .McuPeripheralName_u32 = MCU_FCG2_PERIPH_TMR6_2
  },
  {
    .McuPeripheralState_u8 = (uint8)MCU_ENABLE,
    .McuPeripheralGroup_u8 = MCU_FCG_FCG2_PERIPH_GROUP,
    .McuPeripheralName_u32 = MCU_FCG2_PERIPH_TMR6_3
  },
  {
    .McuPeripheralState_u8 = (uint8)MCU_ENABLE,
    .McuPeripheralGroup_u8 = MCU_FCG_FCG2_PERIPH_GROUP,
    .McuPeripheralName_u32 = MCU_FCG2_PERIPH_TMR6_4
  },
  {
    .McuPeripheralState_u8 = (uint8)MCU_ENABLE,
    .McuPeripheralGroup_u8 = MCU_FCG_FCG2_PERIPH_GROUP,
    .McuPeripheralName_u32 = MCU_FCG2_PERIPH_TMR6_5
  },
  {
    .McuPeripheralState_u8 = (uint8)MCU_ENABLE,
    .McuPeripheralGroup_u8 = MCU_FCG_FCG2_PERIPH_GROUP,
    .McuPeripheralName_u32 = MCU_FCG2_PERIPH_TMR6_6
  },
  {
    .McuPeripheralState_u8 = (uint8)MCU_ENABLE,
    .McuPeripheralGroup_u8 = MCU_FCG_FCG2_PERIPH_GROUP,
    .McuPeripheralName_u32 = MCU_FCG2_PERIPH_TMR6_7
  },
  {
    .McuPeripheralState_u8 = (uint8)MCU_ENABLE,
    .McuPeripheralGroup_u8 = MCU_FCG_FCG2_PERIPH_GROUP,
    .McuPeripheralName_u32 = MCU_FCG2_PERIPH_TMR6_8
  },
  {
    .McuPeripheralState_u8 = (uint8)MCU_ENABLE,
    .McuPeripheralGroup_u8 = MCU_FCG_FCG2_PERIPH_GROUP,
    .McuPeripheralName_u32 = MCU_FCG2_PERIPH_TMR4_1
  },
  {
    .McuPeripheralState_u8 = (uint8)MCU_ENABLE,
    .McuPeripheralGroup_u8 = MCU_FCG_FCG2_PERIPH_GROUP,
    .McuPeripheralName_u32 = MCU_FCG2_PERIPH_TMR4_2
  },
  {
    .McuPeripheralState_u8 = (uint8)MCU_ENABLE,
    .McuPeripheralGroup_u8 = MCU_FCG_FCG2_PERIPH_GROUP,
    .McuPeripheralName_u32 = MCU_FCG2_PERIPH_TMR4_3
  },
  {
    .McuPeripheralState_u8 = (uint8)MCU_ENABLE,
    .McuPeripheralGroup_u8 = MCU_FCG_FCG2_PERIPH_GROUP,
    .McuPeripheralName_u32 = MCU_FCG2_PERIPH_TMR0_1
  },
  {
    .McuPeripheralState_u8 = (uint8)MCU_ENABLE,
    .McuPeripheralGroup_u8 = MCU_FCG_FCG2_PERIPH_GROUP,
    .McuPeripheralName_u32 = MCU_FCG2_PERIPH_TMR0_2
  },
  {
    .McuPeripheralState_u8 = (uint8)MCU_ENABLE,
    .McuPeripheralGroup_u8 = MCU_FCG_FCG2_PERIPH_GROUP,
    .McuPeripheralName_u32 = MCU_FCG2_PERIPH_TMR0_3
  },
  {
    .McuPeripheralState_u8 = (uint8)MCU_ENABLE,
    .McuPeripheralGroup_u8 = MCU_FCG_FCG2_PERIPH_GROUP,
    .McuPeripheralName_u32 = MCU_FCG2_PERIPH_EMB
  },
  {
    .McuPeripheralState_u8 = (uint8)MCU_ENABLE,
    .McuPeripheralGroup_u8 = MCU_FCG_FCG2_PERIPH_GROUP,
    .McuPeripheralName_u32 = MCU_FCG2_PERIPH_TMR2_1
  },
  {
    .McuPeripheralState_u8 = (uint8)MCU_ENABLE,
    .McuPeripheralGroup_u8 = MCU_FCG_FCG2_PERIPH_GROUP,
    .McuPeripheralName_u32 = MCU_FCG2_PERIPH_TMR2_2
  },
  {
    .McuPeripheralState_u8 = (uint8)MCU_ENABLE,
    .McuPeripheralGroup_u8 = MCU_FCG_FCG2_PERIPH_GROUP,
    .McuPeripheralName_u32 = MCU_FCG2_PERIPH_TMR2_3
  },
  {
    .McuPeripheralState_u8 = (uint8)MCU_ENABLE,
    .McuPeripheralGroup_u8 = MCU_FCG_FCG2_PERIPH_GROUP,
    .McuPeripheralName_u32 = MCU_FCG2_PERIPH_TMR2_4
  },
  {
    .McuPeripheralState_u8 = (uint8)MCU_ENABLE,
    .McuPeripheralGroup_u8 = MCU_FCG_FCG2_PERIPH_GROUP,
    .McuPeripheralName_u32 = MCU_FCG2_PERIPH_TMRA_1
  },
  {
    .McuPeripheralState_u8 = (uint8)MCU_ENABLE,
    .McuPeripheralGroup_u8 = MCU_FCG_FCG2_PERIPH_GROUP,
    .McuPeripheralName_u32 = MCU_FCG2_PERIPH_TMRA_2
  },
  {
    .McuPeripheralState_u8 = (uint8)MCU_ENABLE,
    .McuPeripheralGroup_u8 = MCU_FCG_FCG2_PERIPH_GROUP,
    .McuPeripheralName_u32 = MCU_FCG2_PERIPH_TMRA_3
  },
  {
    .McuPeripheralState_u8 = (uint8)MCU_ENABLE,
    .McuPeripheralGroup_u8 = MCU_FCG_FCG2_PERIPH_GROUP,
    .McuPeripheralName_u32 = MCU_FCG2_PERIPH_TMRA_4
  },
  {
    .McuPeripheralState_u8 = (uint8)MCU_ENABLE,
    .McuPeripheralGroup_u8 = MCU_FCG_FCG2_PERIPH_GROUP,
    .McuPeripheralName_u32 = MCU_FCG2_PERIPH_TMRA_5
  },
  {
    .McuPeripheralState_u8 = (uint8)MCU_ENABLE,
    .McuPeripheralGroup_u8 = MCU_FCG_FCG2_PERIPH_GROUP,
    .McuPeripheralName_u32 = MCU_FCG2_PERIPH_TMRA_6
  },
  {
    .McuPeripheralState_u8 = (uint8)MCU_ENABLE,
    .McuPeripheralGroup_u8 = MCU_FCG_FCG2_PERIPH_GROUP,
    .McuPeripheralName_u32 = MCU_FCG2_PERIPH_TMRA_7
  },
  {
    .McuPeripheralState_u8 = (uint8)MCU_ENABLE,
    .McuPeripheralGroup_u8 = MCU_FCG_FCG2_PERIPH_GROUP,
    .McuPeripheralName_u32 = MCU_FCG2_PERIPH_TMRA_8
  },
  {
    .McuPeripheralState_u8 = (uint8)MCU_ENABLE,
    .McuPeripheralGroup_u8 = MCU_FCG_FCG2_PERIPH_GROUP,
    .McuPeripheralName_u32 = MCU_FCG2_PERIPH_TMRA_9
  },
  {
    .McuPeripheralState_u8 = (uint8)MCU_ENABLE,
    .McuPeripheralGroup_u8 = MCU_FCG_FCG2_PERIPH_GROUP,
    .McuPeripheralName_u32 = MCU_FCG2_PERIPH_TMRA_10
  },
  {
    .McuPeripheralState_u8 = (uint8)MCU_ENABLE,
    .McuPeripheralGroup_u8 = MCU_FCG_FCG2_PERIPH_GROUP,
    .McuPeripheralName_u32 = MCU_FCG2_PERIPH_TMRA_11
  },
  {
    .McuPeripheralState_u8 = (uint8)MCU_ENABLE,
    .McuPeripheralGroup_u8 = MCU_FCG_FCG2_PERIPH_GROUP,
    .McuPeripheralName_u32 = MCU_FCG2_PERIPH_TMRA_12
  },
  {
    .McuPeripheralState_u8 = (uint8)MCU_ENABLE,
    .McuPeripheralGroup_u8 = MCU_FCG_FCG3_PERIPH_GROUP,
    .McuPeripheralName_u32 = MCU_FCG3_PERIPH_USART1
  },
  {
    .McuPeripheralState_u8 = (uint8)MCU_ENABLE,
    .McuPeripheralGroup_u8 = MCU_FCG_FCG3_PERIPH_GROUP,
    .McuPeripheralName_u32 = MCU_FCG3_PERIPH_USART2
  },
  {
    .McuPeripheralState_u8 = (uint8)MCU_ENABLE,
    .McuPeripheralGroup_u8 = MCU_FCG_FCG3_PERIPH_GROUP,
    .McuPeripheralName_u32 = MCU_FCG3_PERIPH_USART3
  },
  {
    .McuPeripheralState_u8 = (uint8)MCU_ENABLE,
    .McuPeripheralGroup_u8 = MCU_FCG_FCG3_PERIPH_GROUP,
    .McuPeripheralName_u32 = MCU_FCG3_PERIPH_USART4
  },
  {
    .McuPeripheralState_u8 = (uint8)MCU_ENABLE,
    .McuPeripheralGroup_u8 = MCU_FCG_FCG3_PERIPH_GROUP,
    .McuPeripheralName_u32 = MCU_FCG3_PERIPH_USART5
  },
  {
    .McuPeripheralState_u8 = (uint8)MCU_ENABLE,
    .McuPeripheralGroup_u8 = MCU_FCG_FCG3_PERIPH_GROUP,
    .McuPeripheralName_u32 = MCU_FCG3_PERIPH_USART6
  },
  {
    .McuPeripheralState_u8 = (uint8)MCU_ENABLE,
    .McuPeripheralGroup_u8 = MCU_FCG_FCG3_PERIPH_GROUP,
    .McuPeripheralName_u32 = MCU_FCG3_PERIPH_USART7
  },
  {
    .McuPeripheralState_u8 = (uint8)MCU_ENABLE,
    .McuPeripheralGroup_u8 = MCU_FCG_FCG3_PERIPH_GROUP,
    .McuPeripheralName_u32 = MCU_FCG3_PERIPH_USART8
  },
  {
    .McuPeripheralState_u8 = (uint8)MCU_ENABLE,
    .McuPeripheralGroup_u8 = MCU_FCG_FCG3_PERIPH_GROUP,
    .McuPeripheralName_u32 = MCU_FCG3_PERIPH_USART9
  },
  {
    .McuPeripheralState_u8 = (uint8)MCU_ENABLE,
    .McuPeripheralGroup_u8 = MCU_FCG_FCG3_PERIPH_GROUP,
    .McuPeripheralName_u32 = MCU_FCG3_PERIPH_USART10
  },
  {
    .McuPeripheralState_u8 = (uint8)MCU_ENABLE,
    .McuPeripheralGroup_u8 = MCU_FCG_FCG3_PERIPH_GROUP,
    .McuPeripheralName_u32 = MCU_FCG3_PERIPH_ADC1
  },
  {
    .McuPeripheralState_u8 = (uint8)MCU_ENABLE,
    .McuPeripheralGroup_u8 = MCU_FCG_FCG3_PERIPH_GROUP,
    .McuPeripheralName_u32 = MCU_FCG3_PERIPH_ADC2
  },
  {
    .McuPeripheralState_u8 = (uint8)MCU_ENABLE,
    .McuPeripheralGroup_u8 = MCU_FCG_FCG3_PERIPH_GROUP,
    .McuPeripheralName_u32 = MCU_FCG3_PERIPH_ADC3
  },
  {
    .McuPeripheralState_u8 = (uint8)MCU_ENABLE,
    .McuPeripheralGroup_u8 = MCU_FCG_FCG3_PERIPH_GROUP,
    .McuPeripheralName_u32 = MCU_FCG3_PERIPH_CMBIAS
  },
  {
    .McuPeripheralState_u8 = (uint8)MCU_ENABLE,
    .McuPeripheralGroup_u8 = MCU_FCG_FCG3_PERIPH_GROUP,
    .McuPeripheralName_u32 = MCU_FCG3_PERIPH_DAC1
  },
  {
    .McuPeripheralState_u8 = (uint8)MCU_ENABLE,
    .McuPeripheralGroup_u8 = MCU_FCG_FCG3_PERIPH_GROUP,
    .McuPeripheralName_u32 = MCU_FCG3_PERIPH_DAC2
  },
  {
    .McuPeripheralState_u8 = (uint8)MCU_ENABLE,
    .McuPeripheralGroup_u8 = MCU_FCG_FCG3_PERIPH_GROUP,
    .McuPeripheralName_u32 = MCU_FCG3_PERIPH_CMP1_2
  },
  {
    .McuPeripheralState_u8 = (uint8)MCU_ENABLE,
    .McuPeripheralGroup_u8 = MCU_FCG_FCG3_PERIPH_GROUP,
    .McuPeripheralName_u32 = MCU_FCG3_PERIPH_CMP3_4
  },
  {
    .McuPeripheralState_u8 = (uint8)MCU_ENABLE,
    .McuPeripheralGroup_u8 = MCU_FCG_FCG3_PERIPH_GROUP,
    .McuPeripheralName_u32 = MCU_FCG3_PERIPH_TMR0_4
  },
  {
    .McuPeripheralState_u8 = (uint8)MCU_ENABLE,
    .McuPeripheralGroup_u8 = MCU_FCG_FCG3_PERIPH_GROUP,
    .McuPeripheralName_u32 = MCU_FCG3_PERIPH_TMR0_5
  },
  {
    .McuPeripheralState_u8 = (uint8)MCU_ENABLE,
    .McuPeripheralGroup_u8 = MCU_FCG_FCG3_PERIPH_GROUP,
    .McuPeripheralName_u32 = MCU_FCG3_PERIPH_OTS
  },
  {
    .McuPeripheralState_u8 = (uint8)MCU_ENABLE,
    .McuPeripheralGroup_u8 = MCU_FCG_FCG3_PERIPH_GROUP,
    .McuPeripheralName_u32 = MCU_FCG3_PERIPH_DVP
  },
  {
    .McuPeripheralState_u8 = (uint8)MCU_ENABLE,
    .McuPeripheralGroup_u8 = MCU_FCG_FCG3_PERIPH_GROUP,
    .McuPeripheralName_u32 = MCU_FCG3_PERIPH_SMC
  },
  {
    .McuPeripheralState_u8 = (uint8)MCU_ENABLE,
    .McuPeripheralGroup_u8 = MCU_FCG_FCG3_PERIPH_GROUP,
    .McuPeripheralName_u32 = MCU_FCG3_PERIPH_DMC
  },
  {
    .McuPeripheralState_u8 = (uint8)MCU_ENABLE,
    .McuPeripheralGroup_u8 = MCU_FCG_FCG3_PERIPH_GROUP,
    .McuPeripheralName_u32 = MCU_FCG3_PERIPH_NFC
  }
};



/* Clock_Ip_HsrunConfigType */
static const Clock_Ip_HsrunConfigType l_McuClockSettingConfig_0_HsrunConfig_st = {
            .HCLKClockDiv_u32 = CLK_HCLK_DIV1,
            .EXCLKClockDiv_u32 = CLK_EXCLK_DIV2,
            .PCLK0ClockDiv_u32 = CLK_PCLK0_DIV1,
            .PCLK1ClockDiv_u32 = CLK_PCLK1_DIV2,
            .PCLK2ClockDiv_u32 = CLK_PCLK2_DIV4,
            .PCLK3ClockDiv_u32 = CLK_PCLK3_DIV4,
            .PCLK4ClockDiv_u32 = CLK_PCLK4_DIV2,
};

/* Clock_Ip_UlsrunConfigType */
static const Clock_Ip_UlsrunConfigType l_McuClockSettingConfig_0_UlsrunConfig_st = {
            .HCLKClockDiv_u32 = CLK_HCLK_DIV32,
            .EXCLKClockDiv_u32 = CLK_EXCLK_DIV32,
            .PCLK0ClockDiv_u32 = CLK_PCLK0_DIV32,
            .PCLK1ClockDiv_u32 = CLK_PCLK1_DIV32,
            .PCLK2ClockDiv_u32 = CLK_PCLK2_DIV32,
            .PCLK3ClockDiv_u32 = CLK_PCLK3_DIV32,
            .PCLK4ClockDiv_u32 = CLK_PCLK4_DIV32,
};

const Clock_Ip_ClockConfigSetType g_McuClockConfigPB_aa[MCU_NUMBER_OF_CLKCONFIGS] = {
  {
      .McuClockConfigId_u32 = (uint32)0U,
      .McuClocksConfigCount_dd = (uint8)(sizeof(l_McuClockSettingConfig_0_ClockConfigList_aa) / sizeof(Clock_Ip_ClockHwConfigType)),
      .McuClockConfigList_ptr = l_McuClockSettingConfig_0_ClockConfigList_aa,
      .McuFuncClocksCount_dd = (uint8)(sizeof(l_McuClockSettingConfig_0_FuncClockControlList_aa) / sizeof(Clock_Ip_FuncClockControlType)),
      .McuFuncClockControlList_ptr = l_McuClockSettingConfig_0_FuncClockControlList_aa,
      .McuHsrunConfig_ptr = &l_McuClockSettingConfig_0_HsrunConfig_st,
      .McuUlsrunConfig_ptr = &l_McuClockSettingConfig_0_UlsrunConfig_st
  }
};


#define MCU_STOP_SEC_CONFIG_DATA_UNSPECIFIED
#include "Mcu_MemMap.h"

#ifdef __cplusplus
}
#endif

/**
 * @}
 */

/*---------------------- End of File ----------------------------------------*/
