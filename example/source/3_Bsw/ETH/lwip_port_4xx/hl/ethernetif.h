#ifndef __ETHERNETIF_H__
#define __ETHERNETIF_H__

#include "lwip/netif.h"
#include "Eth.h"
#if LWIP_CHECKSUM_CTRL_PER_NETIF
#define NETIF_CHECKSUM_GEN_ALL   (NETIF_CHECKSUM_GEN_IP     | \
                                  NETIF_CHECKSUM_GEN_UDP    | \
                                  NETIF_CHECKSUM_GEN_TCP    | \
                                  NETIF_CHECKSUM_GEN_ICMP   | \
                                  NETIF_CHECKSUM_GEN_ICMP6)

#define NETIF_CHECKSUM_CHECK_ALL (NETIF_CHECKSUM_CHECK_IP   | \
                                  NETIF_CHECKSUM_CHECK_UDP  | \
                                  NETIF_CHECKSUM_CHECK_TCP  | \
                                  NETIF_CHECKSUM_CHECK_ICMP | \
                                  NETIF_CHECKSUM_CHECK_ICMP6)
#endif /* LWIP_CHECKSUM_CTRL_PER_NETIF */

typedef enum
{
    E_ETH_PHY_MODE_SLAVE = 0U,
    E_ETH_PHY_MODE_MASTER,
    E_ETH_PHY_MODE_NUM
} E_ETH_PHY_MODE;

err_t ethernetif_init(struct netif *netif);
void  ethernetif_input(struct netif *netif);
void  ethernetif_check_link (struct netif *netif);

/**
 * @brief Configure the YT8010A master/slave mode through MDIO.
 * @note Call after Eth_Init() and before Eth_SetControllerMode().
 */
Std_ReturnType Eth_PhySetMasterSlaveMode(E_ETH_PHY_MODE enMode);

void EthIf_RxIndication(
    uint8_t CtrlIdx,
    Eth_FrameType FrameType,
    boolean IsBroadcast,
    const uint8_t *PhysAddrPtr,
    const Eth_DataType *DataPtr,
    uint16_t LenByte);
void EthIf_TxConfirmation(
    uint8_t CtrlIdx,
    Eth_BufIdxType BufIdx,
    Std_ReturnType Result);
#endif
