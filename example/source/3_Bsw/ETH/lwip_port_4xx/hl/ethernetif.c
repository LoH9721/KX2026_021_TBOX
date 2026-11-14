/*
 * Copyright (c) 2001-2004 Swedish Institute of Computer Science.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 * 3. The name of the author may not be used to endorse or promote products
 *    derived from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR IMPLIED
 * WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT
 * SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT
 * OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING
 * IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY
 * OF SUCH DAMAGE.
 *
 * This file is part of the lwIP TCP/IP stack.
 *
 * Author: Adam Dunkels <adam@sics.se>
 *
 */

#include "Gpio.h"

#include "lwip/opt.h"

#include "lwip/def.h"
#include "lwip/mem.h"
#include "lwip/pbuf.h"
#include "lwip/stats.h"
#include "lwip/snmp.h"
#include "lwip/ethip6.h"
#include "lwip/etharp.h"
#include "netif/ppp/pppoe.h"

#include "lwip/sys.h"

#include "ethernetif.h"

#include <string.h>
#include "hc32_ll_fcg.h"
#include "hc32_ll_gpio.h"
#include "hc32_ll_utility.h"
#if !NO_SYS
#include "FreeRTOS.h"
#endif /* defined(USING_OS_FREERTOS) */

#define TIMEOUT                         (0xFFFU)
/* Define PHY info. */
#define PHY_YT8010A_ADDR               (0x00U)
#define PHY_REG_BMCR                   (0x00U)
#define PHY_REG_BMSR                   (0x01U)
#define PHY_REG_PHYID1                  (0x02U)
#define PHY_REG_PHYID2                  (0x03U)
#define PHY_REG_EXT_ADDR                (0x1EU)
#define PHY_REG_EXT_DATA                (0x1FU)
#define PHY_BMCR_RESET                 (0x8000U)
#define PHY_BMCR_AUTO_NEGOTIATION      (0x1000U)
#define PHY_EXT_REG_AUTO100_CTRL1       (0x1000U)
#define PHY_EXT_PMA_CONFIG_MASK         (0x0200U)

/* Define those to better describe your network interface. */
#define IFNAME0 'e'
#define IFNAME1 'n'

/* Set pin to low at least 10ms */
#define PHY_POWER_SETTLE_DELAY_MS       (10U)
#define PHY_HW_RST_DELAY_MS             (10U)
/**
 * Helper struct to hold private data used to operate your ethernet interface.
 * Keeping the ethernet address of the MAC in this struct is not necessary
 * as it is already kept in the struct netif.
 */
struct ethernetif {
  uint8_t CtrlIdx;
#if !NO_SYS
  sys_sem_t sem;
#endif /* !NO_SYS */
};

#if !NO_SYS
static sys_thread_t poll_thread;
#endif

static struct netif *s_netif;
static struct ethernetif s_ethStatus;
static volatile uint16_t s_u16PhyId1;
static volatile uint16_t s_u16PhyId2;
static volatile uint16_t s_u16PhyBmsr;


/**
 * @brief  Initializes the Ethernet GPIO.
 * @param  None
 * @retval None
 */
static void Ethernet_GpioInit(void)
{
    /* OutputService_Init() enables PWR_EN_PHY before EthService_Init(). */
    SysTick_Delay(PHY_POWER_SETTLE_DELAY_MS);
    GPIO_SetOutputLevel(E_OUTPUT_PHY_RESET, LOW);
    SysTick_Delay(PHY_HW_RST_DELAY_MS);
    GPIO_SetOutputLevel(E_OUTPUT_PHY_RESET, HIGH);
    SysTick_Delay(PHY_POWER_SETTLE_DELAY_MS);
}

/* Forward declarations. */
static void Eth_PhySetup(void);

static void Eth_PhySetup(void)
{
    uint16_t u16PhyStatus;
    uint16_t u16PhyId1;
    uint16_t u16PhyId2;

    u16PhyStatus = PHY_BMCR_RESET;
    u16PhyId1 = 0U;
    u16PhyId2 = 0U;
    Eth_WriteMii(EthConf_EthCtrlConfig_EthCtrlConfig_0, PHY_YT8010A_ADDR, PHY_REG_BMCR, PHY_BMCR_RESET);
    do
    {
        Eth_ReadMii(EthConf_EthCtrlConfig_EthCtrlConfig_0, PHY_YT8010A_ADDR, PHY_REG_BMCR, &u16PhyStatus);
    }
    while (0U != (u16PhyStatus & PHY_BMCR_RESET));

    Eth_WriteMii(EthConf_EthCtrlConfig_EthCtrlConfig_0, PHY_YT8010A_ADDR, PHY_REG_BMCR, PHY_BMCR_AUTO_NEGOTIATION);

    /* PHY ID and BMSR reads verify MDIO communication without requiring a cable link. */
    Eth_ReadMii(EthConf_EthCtrlConfig_EthCtrlConfig_0, PHY_YT8010A_ADDR, PHY_REG_PHYID1, &u16PhyId1);
    s_u16PhyId1 = u16PhyId1;
    Eth_ReadMii(EthConf_EthCtrlConfig_EthCtrlConfig_0, PHY_YT8010A_ADDR, PHY_REG_PHYID2, &u16PhyId2);
    s_u16PhyId2 = u16PhyId2;
    Eth_ReadMii(EthConf_EthCtrlConfig_EthCtrlConfig_0, PHY_YT8010A_ADDR, PHY_REG_BMSR, &u16PhyStatus);
    s_u16PhyBmsr = u16PhyStatus;
}

/*******************************************************
  * Name    :Eth_PhySetMasterSlaveMode
  * Author  :LoH
  * Function:Configure and verify the YT8010A master/slave mode.
  * Version :V1.0.0
  * Data    :2026.09.08
*******************************************************/
Std_ReturnType Eth_PhySetMasterSlaveMode(E_ETH_PHY_MODE enMode)
{
    Std_ReturnType Status_en;
    uint16_t u16PhyExtValue;
    uint16_t u16ExpectedValue;

    if ((E_ETH_PHY_MODE_SLAVE != enMode)
        && (E_ETH_PHY_MODE_MASTER != enMode))
    {
        return E_NOT_OK;
    }

    Status_en = Eth_WriteMii(EthConf_EthCtrlConfig_EthCtrlConfig_0, PHY_YT8010A_ADDR, PHY_REG_EXT_ADDR, PHY_EXT_REG_AUTO100_CTRL1);
    if (E_OK == Status_en)
    {
        Status_en = Eth_ReadMii(EthConf_EthCtrlConfig_EthCtrlConfig_0, PHY_YT8010A_ADDR, PHY_REG_EXT_DATA, &u16PhyExtValue);
    }

    if (E_OK == Status_en)
    {
        if (E_ETH_PHY_MODE_MASTER == enMode)
        {
            u16ExpectedValue = (uint16_t)(u16PhyExtValue | PHY_EXT_PMA_CONFIG_MASK);
        }
        else
        {
            u16ExpectedValue = (uint16_t)(u16PhyExtValue & (uint16_t)(~PHY_EXT_PMA_CONFIG_MASK));
        }

        Status_en = Eth_WriteMii(EthConf_EthCtrlConfig_EthCtrlConfig_0, PHY_YT8010A_ADDR, PHY_REG_EXT_DATA, u16ExpectedValue);
    }

    if (E_OK == Status_en)
    {
        Status_en = Eth_ReadMii(EthConf_EthCtrlConfig_EthCtrlConfig_0, PHY_YT8010A_ADDR, PHY_REG_EXT_DATA, &u16PhyExtValue);
    }

    if ((E_OK == Status_en)
        && ((u16PhyExtValue & PHY_EXT_PMA_CONFIG_MASK) != (u16ExpectedValue & PHY_EXT_PMA_CONFIG_MASK)))
    {
        Status_en = E_NOT_OK;
    }

    return Status_en;
}

/**
 * In this function, the hardware should be initialized.
 * Called from ethernetif_init().
 *
 * @param netif the already initialized lwip network interface structure
 *        for this ethernetif
 */
static void
low_level_init(struct netif *netif)
{
  struct ethernetif *eth = netif->state;
#if LWIP_CHECKSUM_CTRL_PER_NETIF
  u16_t chksum_flags;
#endif /* LWIP_CHECKSUM_CTRL_PER_NETIF*/
  Std_ReturnType Status_en = E_NOT_OK;

  /* Enable ETH clock */
  FCG_Fcg1PeriphClockCmd(FCG1_PERIPH_ETHMAC, DISABLE);
  FCG_Fcg1PeriphClockCmd(FCG1_PERIPH_ETHMAC, ENABLE);
  /* Init Ethernet GPIO */
  Ethernet_GpioInit();
    
  /* set MAC hardware address length */
  netif->hwaddr_len = ETH_HWADDR_LEN;
  
  eth->CtrlIdx = EthConf_EthCtrlConfig_EthCtrlConfig_0;

  Eth_Init(NULL_PTR);
  Eth_PhySetup();
  Status_en = Eth_SetControllerMode(eth->CtrlIdx, ETH_MODE_ACTIVE);
  if (E_OK != Status_en)
  {
      Terminal_Printf("Enable Controller fail!\r\n");
      while(1);
  }

  /* set MAC hardware address */
  Eth_GetPhysAddr(eth->CtrlIdx, &netif->hwaddr[0]);

  /* maximum transfer unit */
  netif->mtu = 1500;

  /* device capabilities */
  /* don't set NETIF_FLAG_ETHARP if this device is not an ethernet one */
  netif->flags = NETIF_FLAG_BROADCAST | NETIF_FLAG_ETHARP | NETIF_FLAG_LINK_UP;
#if LWIP_IPV4 && LWIP_IGMP
  netif->flags |= NETIF_FLAG_IGMP;
#endif
#if LWIP_IPV6 && LWIP_IPV6_MLD
  netif->flags |= NETIF_FLAG_MLD6;
  /*
   * For hardware/netifs that implement MAC filtering.
   * All-nodes link-local is handled by default, so we must let the hardware know
   * to allow multicast packets in.
   * Should set mld_mac_filter previously. */
  if (netif->mld_mac_filter != NULL) {
    ip6_addr_t ip6_allnodes_ll;
    ip6_addr_set_allnodes_linklocal(&ip6_allnodes_ll);
    netif->mld_mac_filter(netif, &ip6_allnodes_ll, NETIF_ADD_MAC_FILTER);
  }
#endif /* LWIP_IPV6 && LWIP_IPV6_MLD */
}

/**
 * This function should do the actual transmission of the packet. The packet is
 * contained in the pbuf that is passed to the function. This pbuf
 * might be chained.
 *
 * @param netif the lwip network interface structure for this ethernetif
 * @param p the MAC packet to send (e.g. IP packet including MAC addresses and type)
 * @return ERR_OK if the packet could be sent
 *         an err_t value if the packet couldn't be sent
 *
 * @note Returning ERR_MEM here if a DMA queue of your MAC is full can lead to
 *       strange results. You might consider waiting for space in the DMA queue
 *       to become available since the stack doesn't retry to send a packet
 *       dropped because of memory failure (except for the TCP timers).
 */

static err_t
low_level_output(struct netif *netif, struct pbuf *p)
{
    err_t result = ERR_OK;
    struct ethernetif *eth = netif->state;
    struct pbuf *q;
    Eth_BufIdxType BufIdx_en;
    uint8_t *BufData_ptr;
    uint32_t Len = 0;
    Eth_FrameType FrameType_en;
    uint16_t Length_u16 = p->tot_len - 14U;
#if ETH_PAD_SIZE
    pbuf_remove_header(p, ETH_PAD_SIZE); /* drop the padding word */
#endif
    uint32_t Count_u32 = 0U;

    for (Count_u32 = 0U; Count_u32 < TIMEOUT; Count_u32++)
    {
        if (BUFREQ_OK == Eth_ProvideTxBuffer(eth->CtrlIdx, 0U, &BufIdx_en, &BufData_ptr, &Length_u16))
        {
            break;
        }
    }
    if (TIMEOUT == Count_u32)
    {
        result = ERR_BUF;
    }

    if (ERR_OK == result)
    {
        BufData_ptr -= 14U;
        for (q = p; q != NULL; q = q->next) {
          /* read data from(q->payload, q->len); */
            memcpy(&BufData_ptr[Len], q->payload, q->len);
            Len += q->len;
        }
        FrameType_en = ((uint16_t)BufData_ptr[12] << 8U) | (uint16_t)BufData_ptr[13];
        if (E_OK != Eth_Transmit(eth->CtrlIdx, BufIdx_en, FrameType_en, FALSE, Length_u16, &BufData_ptr[0]))
        {
            Terminal_Printf("Send frame fail.\r\n");
            result = ERR_BUF;
        }
        else
        {
            MIB2_STATS_NETIF_ADD(netif, ifoutoctets, p->tot_len);
            if (((u8_t *)p->payload)[0] & 1) {
              /* broadcast or multicast packet*/
              MIB2_STATS_NETIF_INC(netif, ifoutnucastpkts);
            } else {
              /* unicast packet */
              MIB2_STATS_NETIF_INC(netif, ifoutucastpkts);
            }
            /* increase ifoutdiscards or ifouterrors on error */
#if ETH_PAD_SIZE
            pbuf_add_header(p, ETH_PAD_SIZE); /* reclaim the padding word */
#endif

            LINK_STATS_INC(link.xmit);
        }
    }
    else
    {
        //Terminal_Printf("Provide Tx buffer fail.\r\n");
    }

  return result;
}

/**
 * custom free for RX zero-copy buffer.
 */
static void ethernetif_custom_free(struct pbuf *p);
static void ethernetif_custom_free(struct pbuf *p)
{
    (void)p;
}

void EthIf_RxIndication(
    uint8_t CtrlIdx,
    Eth_FrameType FrameType,
    boolean IsBroadcast,
    const uint8_t *PhysAddrPtr,
    const Eth_DataType *DataPtr,
    uint16_t LenByte)
{
    (void)FrameType;
    (void)IsBroadcast;
    (void)PhysAddrPtr;
    const uint8_t* Buffer_ptr = DataPtr - 14U;
    uint32_t Length_u32       = LenByte + 14U;
#ifdef COPY_FRAME
    struct pbuf *p = NULL, *q;
    uint32_t LengthTemp_u32   = 0U;
#else
    struct pbuf_custom pTemp = {.custom_free_function = ethernetif_custom_free};
    struct pbuf *p           = (struct pbuf *)&pTemp;
#endif

#ifdef COPY_FRAME
    p = pbuf_alloc(PBUF_RAW, Length_u32, PBUF_POOL);
    if (p != NULL) {
#if ETH_PAD_SIZE
        pbuf_remove_header(p, ETH_PAD_SIZE); /* drop the padding word */
#endif
      /* We iterate over the pbuf chain until we have read the entire
       * packet into the pbuf. */
      for (q = p; q != NULL; q = q->next) {
        /* Read enough bytes to fill this pbuf in the chain. The
         * available data in the pbuf is given by the q->len
         * variable.
         * This does not necessarily have to be a memcpy, you can also preallocate
         * pbufs for a DMA-enabled MAC and after receiving truncate it to the
         * actually received size. In this case, ensure the tot_len member of the
         * pbuf is the sum of the chained pbuf len members.
         */
        /* read data into(q->payload, q->len); */
          memcpy(q->payload, &Buffer_ptr[LengthTemp_u32], q->len);
          LengthTemp_u32 += q->len;
      }
#else
      p->next    = NULL_PTR;
      p->tot_len = Length_u32;
      p->len     = Length_u32;
      p->payload = (void *)Buffer_ptr;
      p->ref     = 1;
      p->flags   = PBUF_FLAG_IS_CUSTOM;
#endif
      /* acknowledge that packet has been read(); */
      MIB2_STATS_NETIF_ADD(s_netif, ifinoctets, p->tot_len);
      if (((u8_t *)p->payload)[0] & 1) {
        /* broadcast or multicast packet*/
        MIB2_STATS_NETIF_INC(s_netif, ifinnucastpkts);
      } else {
        /* unicast packet*/
        MIB2_STATS_NETIF_INC(s_netif, ifinucastpkts);
      }
#if ETH_PAD_SIZE
      pbuf_add_header(p, ETH_PAD_SIZE); /* reclaim the padding word */
#endif
      LINK_STATS_INC(link.recv);
      /* pass all packets to ethernet_input, which decides what packets it supports */
      if (s_netif->input(p, s_netif) != ERR_OK) {
        LWIP_DEBUGF(NETIF_DEBUG, ("ethernetif_input: IP input error\n"));
#ifdef COPY_FRAME
        pbuf_free(p);
#endif
        p = NULL;
      }
#ifdef COPY_FRAME
    } else {
        /* drop packet(); */
        LINK_STATS_INC(link.memerr);
        LINK_STATS_INC(link.drop);
        MIB2_STATS_NETIF_INC(s_netif, ifindiscards);
    }
#endif
}

void EthIf_TxConfirmation(
    uint8_t CtrlIdx,
    Eth_BufIdxType BufIdx,
    Std_ReturnType Result)
{
    /* Reserve input parameters */
    (void)CtrlIdx;
    (void)BufIdx;
    (void)Result;
}

/**
 * This function should be called when a packet is ready to be read
 * from the interface. It uses the function low_level_input() that
 * should handle the actual reception of bytes from the network
 * interface. Then the type of the received packet is determined and
 * the appropriate input function is called.
 *
 * @param netif the lwip network interface structure for this ethernetif
 */
void
ethernetif_input(struct netif *netif)
{
  struct ethernetif *eth = netif->state;
  Eth_RxStatusType RxStatus_en    = ETH_NOT_RECEIVED;

  /* move received packet into a new pbuf */
  Eth_Receive(eth->CtrlIdx, 0U, &RxStatus_en);
}

#if (NO_SYS == 0)
static void ethernetif_poll_thread(void *arg)
{
    while (TRUE)
    {
        ethernetif_input(s_netif);
    }
}
#endif
/**
 * Should be called at the beginning of the program to set up the
 * network interface. It calls the function low_level_init() to do the
 * actual setup of the hardware.
 *
 * This function should be passed as a parameter to netif_add().
 *
 * @param netif the lwip network interface structure for this ethernetif
 * @return ERR_OK if the loopif is initialized
 *         ERR_MEM if private data couldn't be allocated
 *         any other err_t on error
 */
err_t
ethernetif_init(struct netif *netif)
{
  LWIP_ASSERT("netif != NULL", (netif != NULL));

#if LWIP_NETIF_HOSTNAME
  /* Initialize interface hostname */
  netif->hostname = "lwip";
#endif /* LWIP_NETIF_HOSTNAME */

  /*
   * Initialize the snmp variables and counters inside the struct netif.
   * The last argument should be replaced with your link speed, in units
   * of bits per second.
   */
  MIB2_INIT_NETIF(netif, snmp_ifType_ethernet_csmacd, 100000000);

  s_netif      = netif;
  netif->state = &s_ethStatus;
  netif->name[0] = IFNAME0;
  netif->name[1] = IFNAME1;
  /* We directly use etharp_output() here to save a function call.
   * You can instead declare your own function an call etharp_output()
   * from it if you have to do some checks before sending (e.g. if link
   * is available...) */
#if LWIP_IPV4
  netif->output = etharp_output;
#endif /* LWIP_IPV4 */
#if LWIP_IPV6
  netif->output_ip6 = ethip6_output;
#endif /* LWIP_IPV6 */
  netif->linkoutput = low_level_output;

  /* initialize the hardware */
  low_level_init(netif);

#if (NO_SYS == 0)
    /* Start the polling thread */
    poll_thread = sys_thread_new("ethernetif_poll_thread", ethernetif_poll_thread, (void*)((uint32_t)netif->num), DEFAULT_THREAD_STACKSIZE, DEFAULT_THREAD_PRIO);
#endif /* (NO_SYS == 0) */

  return ERR_OK;
}
