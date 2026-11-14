/***********************************************************************************************************************
 * Project Name: XHSC AUTOSAR MCAL
 * File Name: Terminal.c
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


/**
 *   @file       Terminal.c
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
#include "Terminal.h"
#include "hc32a4a8.h"
#include "Osr_Internal.h"
#include "Common.h"
#include "Hmi.h"

#ifdef STD_LIB_SUPPORT
#include <stdio.h>
#include <stdarg.h>
#endif

/***********************************************************************************************************************
 *  DEFINES & MACROS
 **********************************************************************************************************************/
/*section start: user can update according to application*/
/*note: here should be a strong relationship among:
* T_UART_CH/UART_TX_PORT/UART_TX_PIN/UART_TX_ALT/UART_RX_PORT/UART_RX_PIN/UART_RX_PFSR_OFFSET
*/
#define T_UART_PORT_SET                                                  (1U)
#define T_UART_STRING_MAX                                                (256U)
#define T_UART_PUT_TIMEOUT                                               (0xFFFFFFFFU)
#define T_UART_CH                                                        (1U)
#define T_UART_BAUDRATE                                                  (115200U)

#if (1U == T_UART_PORT_SET)
/*PH15*/
#define UART_TX_PORT                                                     (7U)
#define UART_TX_PIN                                                      (15U)
#define UART_TX_ALT                                                      (32U)/* Usart1_Tx */
/*PH13*/
#define UART_RX_PORT                                                     (0U)
#define UART_RX_PIN                                                      (13U)
#define UART_RX_ALT                                                      (33U)/* Usart1_Rx */
#define UART_TX_PFSR_OFFSET                                              (0x5FE)/*0x402 + 0x40*7 + 0x4*0xF = 0x5FE */
#define UART_RX_PFSR_OFFSET                                              (0x5F6)/*0x402 + 0x40*7 + 0x4*0xD = 0x5F6*/
#define PORT_PWPR_OFFSET                                                 (0x3FC)
#define PORT_PINAER_OFFSET                                               (0x3FA)
#endif
/*section end: user can update according to application*/

#define T_UART_FORMAT_MAX                                                (3U)
#define T_UART_HEX_MAX                                                   (8U)
/*uart base*/
#define T_UART_BASE                                                      (CM_USART1_BASE + (T_UART_CH-1U)*0x400U)
#define USART_CR1_RESET                                                  (0x80000000U)
#define USART_CR2_RESET                                                  (0x00000600U)
#define USART_CR3_RESET                                                  (0x00000000U)
#define USART_PR_RESET                                                   (0x00000000U)
#define USART_BRR_RESET                                                  (0x0000FFFFU)
#define USART_CR1_TE_M(x)                                                (((uint32_t)(((uint32_t)(x)) << USART_CR1_TE_POS)) & USART_CR1_TE)
#define USART_CR1_RE_M(x)                                                (((uint32_t)(((uint32_t)(x)) << USART_CR1_RE_POS)) & USART_CR1_RE)
#define USART_CR1_FBME_M(x)                                              (((uint32_t)(((uint32_t)(x)) << USART_CR1_FBME_POS)) & USART_CR1_FBME)
#define USART_CR1_OVER8_M(x)                                             (((uint32_t)(((uint32_t)(x)) << USART_CR1_OVER8_POS)) & USART_CR1_OVER8)
#define USART_PR_PSC_M(x)                                                (((uint32_t)(((uint32_t)(x)) << USART_PR_PSC_POS)) & USART_PR_PSC)
#define USART_BRR_DIV_INTEGER_M(x)                                       (((uint32_t)(((uint32_t)(x)) << USART_BRR_DIV_INTEGER_POS)) & USART_BRR_DIV_INTEGER)
#define USART_BRR_DIV_FRACTION_M(x)                                      (((uint32_t)(((uint32_t)(x)) << USART_BRR_DIV_FRACTION_POS)) & USART_BRR_DIV_FRACTION)
#if (1U == T_UART_PORT_SET)
/*port base*/
#define T_PORT_BASE                                                      (CM_GPIO_BASE)
#define PORT_PWPR_BASE                                                   (T_PORT_BASE + PORT_PWPR_OFFSET)
#define UART_TX_PFSR_BASE                                                (T_PORT_BASE + UART_TX_PFSR_OFFSET)
#define UART_RX_PFSR_BASE                                                (T_PORT_BASE + UART_RX_PFSR_OFFSET)
#define UART_RX_PINAER_BASE                                              (T_PORT_BASE + PORT_PINAER_OFFSET)

#define UART_TX_PCR_BASE                                                 (T_PORT_BASE + 0x0400 + UART_TX_PORT * 0x40 + UART_TX_PIN * 0x4)
#define UART_RX_PCR_BASE                                                 (T_PORT_BASE + 0x0400 + UART_RX_PORT * 0x40 + UART_RX_PIN * 0x4)
#define PORT_REG_Write_En                                                (0xA501)
#define PORT_REG_Write_Dis                                               (0xA500)
#define PORT_PCR_POUTE_M(x)                                              (((uint32_t)(((uint32_t)(x)) << GPIO_PCR_POUTE_POS)) & GPIO_PCR_POUTE)
#define PORT_PCR_CINSEL_M(x)                                             (((uint32_t)(((uint32_t)(x)) << GPIO_PCR_CINSEL_POS)) & GPIO_PCR_CINSEL)
#define PORT_PCR_PUU_M(x)                                                (((uint32_t)(((uint32_t)(x)) << GPIO_PCR_PUU_POS)) & GPIO_PCR_PUU)
#define PORT_PCR_DRV_M(x)                                                (((uint32_t)(((uint32_t)(x)) << GPIO_PCR_DRV_POS)) & GPIO_PCR_DRV)
#define PORT_PFSR_FSEL_M(x)                                              (((uint32_t)(((uint32_t)(x)) << GPIO_PFSR_FSEL_POS)) & GPIO_PFSR_FSEL)
#endif
/* UART Clock Frequency */
#ifndef TERMINAL_SRC_FREQUENCY_USER
#define TERMINAL_SRC_FREQUENCY                                           (120000000U)
#else
#define TERMINAL_SRC_FREQUENCY                                           (TERMINAL_SRC_FREQUENCY_USER)
#endif
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
 static CM_USART_TypeDef *s_Terminal_Ptr = (CM_USART_TypeDef *)T_UART_BASE;;
 static volatile uint8_t s_TerminalPrintfInit_u8;
 static volatile char HexBuffer[T_UART_HEX_MAX];

/***********************************************************************************************************************
 *  GLOBAL VARIABLES
 **********************************************************************************************************************/

/***********************************************************************************************************************
 *  LOCAL FUNCTION PROTOTYPES
 **********************************************************************************************************************/
static void Terminal_IpInit(void);
#if (1U == T_UART_PORT_SET)
static void Terminal_PortInit(void);
#endif
static uint8_t Terminal_PutByte_Static(uint8_t para_Value_u8);
static void Terminal_InitCheckAndSet(void);
static uint8_t Terminal_PutInt_Static(sint32 para_Value_i32);
static uint16_t Terminal_PutString_Static(const char* para_string_ptr);
static uint32_t Terminal_PutBuffer_Static(uint8_t * para_Buffer_ptr, uint32_t para_Size_u32);
#ifndef STD_LIB_SUPPORT
static uint8_t Terminal_IntToHex_Static(uint32_t para_Value_u32);
static void Terminal_Printf_Static(const char * para_Format_ptr, uint32_t * para_Arg_Ptr);
#endif
/***********************************************************************************************************************
 *  LOCAL FUNCTION
 **********************************************************************************************************************/
/**
 *  @brief          Int convert to Hex
 *
 *  @return         uint8_t       Hex value length
 *
 *  @note           None
 *
 *  @pre            None
 *
 */
static uint8_t Terminal_IntToHex_Static(uint32_t para_Value_u32)
{
    uint8_t i, ret;
    for(i =0; i< T_UART_HEX_MAX; i++)
    {
        HexBuffer[i] = ((para_Value_u32>>(28-4*i))&0xF);
    }
    /*convert*/
    for(i =0; i< T_UART_HEX_MAX; i++)
    {
        if (10U > HexBuffer[i])
        {
            HexBuffer[i] = HexBuffer[i] + '0';
        }
        else
        {
            HexBuffer[i] = HexBuffer[i] - 10 + 'A';
        }
    }
    ret = T_UART_HEX_MAX;
    for(i =0; i< T_UART_HEX_MAX; i++)
    {
        if(0U == HexBuffer[i])
        {
            ret--;
        }
        else
        {
            break;
        }
    }
    /*avoid no output*/
    if(0==ret)
    {
        ret = 1;
    }
    return ret;
}

/**
 *  @brief          Print init(eg, 115200/8/n/1)
 *
 *  @return         None
 *
 *  @note           None
 *
 *  @pre            None
 *
 */
static void Terminal_IpInit(void)
{
    uint32_t USART_BRR_DIV_I = 0U;
    uint32_t USART_BRR_DIV_F = 0U;
    /*Enter Init mode, Disable TE, RE*/
    /* OVER8 = 0, FBME = 0*/
    s_Terminal_Ptr->CR1 = USART_CR1_RESET;
    /* FBME = 1*/
    s_Terminal_Ptr->CR1 |= (USART_CR1_FBME_M(1)|USART_CR1_OVER8_M(1));
    s_Terminal_Ptr->CR2 = USART_CR2_RESET;
    s_Terminal_Ptr->CR3 = USART_CR3_RESET;
    s_Terminal_Ptr->PR = USART_PR_RESET;
    /* USART_PR.PSC -> 64 */
    s_Terminal_Ptr->PR |= USART_PR_PSC_M(3);

    s_Terminal_Ptr->BRR = USART_BRR_RESET;
    /* Baudrate calculate
    * when FBME = 0,
    * Baudrate = C/(8*(2-OVER8)*(USART_BRR.DIV_I+1))
    * C = PCLK1/USART_PR.PSC
    * when FBME = 1,
    * Baudrate = C*(128+USART_BRR.DIV_F)/(8*(2-OVER8)*(USART_BRR.DIV_I + 1)*256)
    * C = PCLK1/USART_PR.PSC
    */
    USART_BRR_DIV_I = TERMINAL_SRC_FREQUENCY/64/(8*1*T_UART_BAUDRATE) - 1;
    if((USART_BRR_DIV_I) < 0xFFU)
    {
        USART_BRR_DIV_F = ((USART_BRR_DIV_I+1)*T_UART_BAUDRATE*8*1)*256;
        USART_BRR_DIV_F = USART_BRR_DIV_F/(TERMINAL_SRC_FREQUENCY/64);
        if (USART_BRR_DIV_F < 0xFFU)
        {
            USART_BRR_DIV_F = USART_BRR_DIV_F - 128U;
        }
        else
        {
            USART_BRR_DIV_F = 0x7FU;
        }
    }
    else
    {
        USART_BRR_DIV_I = 0xFFU;
        USART_BRR_DIV_F = 0U;
    }
    s_Terminal_Ptr->BRR = (USART_BRR_DIV_INTEGER_M(USART_BRR_DIV_I)|USART_BRR_DIV_FRACTION_M(USART_BRR_DIV_F));
    /* Enable TE, RE */
    s_Terminal_Ptr->CR1 |= (USART_CR1_TE_M(1)|USART_CR1_RE_M(1));
}

#if (1U == T_UART_PORT_SET)
/**
 *  @brief          Print port init
 *
 *  @return         None
 *
 *  @note           None
 *
 *  @pre            None
 *
 */
static void Terminal_PortInit(void)
{
    *(volatile uint16_t *)PORT_PWPR_BASE = PORT_REG_Write_En;

    /*TX, 2 - Strong*/
    *(volatile uint32_t *)UART_TX_PCR_BASE = PORT_PCR_POUTE_M(1)|PORT_PCR_DRV_M(2);
    /*PFSR*/
    *(volatile uint32_t *)UART_TX_PFSR_BASE = PORT_PFSR_FSEL_M(UART_TX_ALT);

    /*RX, 2 - TTL*/
    *(volatile uint32_t *)UART_RX_PCR_BASE = /*PORT_PCR_POUTE_M(1)|*/PORT_PCR_CINSEL_M(0)|PORT_PCR_PUU_M(0);
    /*PFSR*/
    *(volatile uint32_t *)UART_RX_PFSR_BASE = PORT_PFSR_FSEL_M(UART_RX_ALT);

    *(volatile uint16_t *)PORT_PWPR_BASE = PORT_REG_Write_Dis;
}
#endif
/**
 *  @brief          Adl output 1 byte
 *
 *  @return         uint8_t   send out ot not
 *
 *  @note           None
 *
 *  @pre            Uart/Port/Clock driver should be init
 *
 */
static uint8_t Terminal_PutByte_Static(uint8_t para_Value_u8)
{
    uint32_t ret = 0U;
    volatile uint32_t Timeout_u32 = T_UART_PUT_TIMEOUT;
    /*Wait TC first*/
    do
    {
        ret = (s_Terminal_Ptr->SR & USART_SR_TC);
        Timeout_u32--;
    } while ((0U == ret) && (0U < Timeout_u32));

    /*Write data*/
    *(volatile uint16_t *)(&s_Terminal_Ptr->TDR) = (para_Value_u8 & 0xFF);

    /*Wait TC set*/
    Timeout_u32 = T_UART_PUT_TIMEOUT;
    do{
        ret = (s_Terminal_Ptr->SR & USART_SR_TC);
        Timeout_u32--;
    } while ((0U == ret) && (0U < Timeout_u32));
    return ret;
}

/**
 *  @brief          Adl Init check and set
 *
 *  @return         None
 *
 *  @note           None
 *
 *  @pre            None
 *
 */
static void Terminal_InitCheckAndSet(void)
{
    if (0U == s_TerminalPrintfInit_u8)
    {
#if (1U == T_UART_PORT_SET)
        Terminal_PortInit();
#endif
        /*Init first*/
        Terminal_IpInit();
        s_TerminalPrintfInit_u8 = 1;
    }
}

/**
 *  @brief          sint32 print function without spin lock
 *
 *  @param[in]      para_Value_i32    input value
 *
 *  @return         uint8_t    Returns the number of sint written on the stdout
 *  @retval         None
 *
 *  @note           None
 *
 *  @pre            None
 *
 */
static uint8_t Terminal_PutInt_Static(
    sint32 para_Value_i32
)
{
    char output_c = '-';
    char remainderArray[16];
    uint8_t remCnt = 0;

    /*Init check*/
    Terminal_InitCheckAndSet();

    /* Put string */
    if (0 > para_Value_i32)
    {
        Terminal_PutByte_Static((uint8_t)output_c);
        para_Value_i32 = 0 - para_Value_i32;
    }
    do
    {
        remainderArray[remCnt++] = para_Value_i32 % 10U;
        para_Value_i32 /= 10U;
        if(remCnt >15U)
        {
          break;
        }
    } while(0 < para_Value_i32);

    para_Value_i32 = remCnt;
    do
    {
        output_c = remainderArray[--remCnt] + '0';
        Terminal_PutByte_Static((uint8_t)output_c);
    } while (remCnt>0);

    return para_Value_i32;
}

/**
 *  @brief          string print function without spin lock
 *
 *  @param[in]      para_string_ptr    input value
 *
 *  @return         uint16_t    Returns the number of char written on the stdout
 *  @retval         None
 *
 *  @note           None
 *
 *  @pre            None
 *
 */
static uint16_t Terminal_PutString_Static(
    const char* para_string_ptr
)
{
    char* string_ptr;
    uint16_t stringlen_u16 = 0;

    /*Init check*/
    Terminal_InitCheckAndSet();

    string_ptr = (char*)para_string_ptr;
    /* Put string */
    while (*string_ptr!='\0')
    {
        Terminal_PutByte_Static(*(uint8_t *)string_ptr);
        string_ptr++;
        stringlen_u16++;
        if(stringlen_u16>=T_UART_STRING_MAX)
        {
            break;
        }
    }

    return stringlen_u16;
}

/**
 *  @brief          Adl output a buffer with a size value without spin lock
 *
 *  @return         uint32_t   how many byte had be sent out
 *
 *  @note           None
 *
 *  @pre            None
 *
 */
static uint32_t Terminal_PutBuffer_Static(uint8_t * para_Buffer_ptr, uint32_t para_Size_u32)
{
    uint32_t i, ret;
    uint8_t* put_ptr = para_Buffer_ptr;
    /*Init check*/
    Terminal_InitCheckAndSet();
    for(i=0; i<para_Size_u32; i++)
    {
        ret = Terminal_PutByte_Static(*put_ptr);
        if(0U==ret)
        {
            break;
        }
        put_ptr++;
    }
    return i;
}

#ifndef STD_LIB_SUPPORT
static void Terminal_Printf_Static(
    const char * para_Format_ptr,
    uint32_t * para_Arg_Ptr
)
{
    uint16_t remCnt = 0U;
    boolean FormatValid_u8 = TRUE;

    /*Init check*/
    Terminal_InitCheckAndSet();
    while (* para_Format_ptr) {
        if (* para_Format_ptr == '%')
        {
            /*Jump '%'*/
            para_Format_ptr++;
            switch (* para_Format_ptr)
            {
                case 'd':
                    /*print int value*/
                    Terminal_PutInt_Static(*(sint32*)para_Arg_Ptr);
                    /*Jump to next parameter*/
                    para_Arg_Ptr++;
                    break;
                case 's':
                    /*print string value*/
                    Terminal_PutString_Static((const char*)* para_Arg_Ptr);
                    /*Jump to next parameter*/
                    para_Arg_Ptr++;
                    break;
                case 'c':
                    Terminal_PutByte_Static(*(uint8_t*)para_Arg_Ptr);
                    /*Jump to next parameter*/
                    para_Arg_Ptr++;
                    break;
                case '%':
                    Terminal_PutByte_Static(*(uint8_t*)para_Format_ptr);
                    /*Jump to next parameter*/
                    para_Arg_Ptr++;
                    break;
                case 'x':
                case 'X':
                    remCnt = Terminal_IntToHex_Static(*(uint32_t*)para_Arg_Ptr);
                    if(remCnt>=T_UART_HEX_MAX)
                    {
                        remCnt = T_UART_HEX_MAX;
                    }
                    Terminal_PutBuffer_Static((uint8_t *)(&HexBuffer[T_UART_HEX_MAX-remCnt]), remCnt);
                    /*Jump to next parameter*/
                    para_Arg_Ptr++;
                    break;
                default:
                    /*invalid format*/
                    Terminal_PutByte_Static('%');
                    Terminal_PutByte_Static(*(uint8_t*)para_Format_ptr);
                    FormatValid_u8 = FALSE;
                    break;
            }
        }
        else
        {
            Terminal_PutByte_Static(*(uint8_t*)para_Format_ptr);
        }
        if(FALSE == FormatValid_u8)
        {
            break;
        }
        /*Jump to next char*/
        para_Format_ptr++;
    }
}
#endif


/**
 *  @brief          Adl print lock
 *
 *  @return         None
 *
 *  @note           None
 *
 *  @pre            None
 *
 */
static void Terminal_PrintLock(void)
{
    SuspendAllInterrupts();
}
/**
 *  @brief          Adl print unlock
 *
 *  @return         None
 *
 *  @note           None
 *
 *  @pre            None
 *
 */
static void Terminal_PrintUnlock(void)
{
    ResumeAllInterrupts();
}
/***********************************************************************************************************************
 *  GLOBAL FUNCTION
 **********************************************************************************************************************/
#ifdef STD_LIB_SUPPORT
int putchar(int c)
{
    Terminal_PutByte_Static((uint8_t)c);
    return c;
}
#endif
/**
 *  @brief          ADL input function
 *
 *  @param[in]      * para_Buffer_ptr    Pointer for input value store
 *  @param[in]      para_Size_u32       Size for expected input value
 *  @param[in]      para_Timeout_u32    Timeout for input monitor
 *
 *  @return         uint32_t    Returns the receive result
 *  @retval         0U:       Reception successfully.
 *  @retval         1U:       Not reception successfully.
 *
 *  @note           None
 *
 *  @pre            None
 *
 */
uint32_t Terminal_Input(uint8_t* para_Buffer_ptr, uint32_t para_Size_u32, uint32_t para_Timeout_u32)
{
    uint32_t i = 0U;
    uint32_t ret = 0U;
    uint32_t RxCnt = 0U;

    /* lock first */
    Terminal_PrintLock();

    /*Init check*/
    Terminal_InitCheckAndSet();

    while ((i<para_Timeout_u32)&&(para_Size_u32>RxCnt))
    {
        ret = s_Terminal_Ptr->SR & USART_SR_RXNE;
        if(0U < ret)
        {
            * para_Buffer_ptr = *(volatile uint8_t *)(&s_Terminal_Ptr->RDR);
            para_Buffer_ptr++;
            RxCnt++;
        }
        i++;
    }
    /* release lock */
    Terminal_PrintUnlock();
    return RxCnt;
}
/**
 *  @brief          Adl output a buffer with a size value
 *
 *  @return         uint32_t   how many byte had be sent out
 *
 *  @note           None
 *
 *  @pre            None
 *
 */
uint32_t Terminal_PutBuffer(uint8_t * para_Buffer_ptr, uint32_t para_Size_u32)
{
    uint32_t i, ret;
    uint8_t* put_ptr = para_Buffer_ptr;

    /* lock first */
    Terminal_PrintLock();

    /*Init check*/
    Terminal_InitCheckAndSet();
    for(i=0; i<para_Size_u32; i++)
    {
        ret = Terminal_PutByte_Static(*put_ptr);
        if(0U==ret)
        {
            break;
        }
        put_ptr++;
    }
    /* release lock */
    Terminal_PrintUnlock();
    return i;
}

/**
 *  @brief          ADL ADL_Printf function
 *
 *  @param[in]      para_Input_ptr    input value
 *  @param[in]      para_Format_ptr   print format
 *
 *  @return         void
 *  @retval         None
 *
 *  @note           Only support %d,%s,%c,%x,%X
 *
 *  @pre            None
 *
 */
void Terminal_Printf(
    const char * para_Format_ptr,
    ...
)
{
    HMI_LOG_ETH(para_Format_ptr);
    return;
    uint32_t *Arg_Ptr = NULL_PTR;
    /* Get the address of format */
    Arg_Ptr = (uint32_t *)&para_Format_ptr;

    /* Jump format, point to the first parameter */
    Arg_Ptr++;
#ifdef _GREENHILLS_C_XC2XX_
    Arg_Ptr++;
    Arg_Ptr++;
    Arg_Ptr++;
#endif

    /* lock first */
    Terminal_PrintLock();
#ifdef STD_LIB_SUPPORT
    va_list args;

    Terminal_InitCheckAndSet();
    va_start(args, para_Format_ptr);
    (void)printf(para_Format_ptr, args);
    va_end(args);
#else
    Terminal_Printf_Static(para_Format_ptr, Arg_Ptr);
#endif
    /* release lock */
    Terminal_PrintUnlock();
}

#ifdef __cplusplus
}
#endif

/**
 * @}
 */
