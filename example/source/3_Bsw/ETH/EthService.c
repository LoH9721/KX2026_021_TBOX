#include "EthService.h"

#include "cmsis_os.h"
#include "FreeRTOS.h"


#include "Terminal.h"

/* lwIP core includes */
#include "lwip/opt.h"

#include "lwip/sys.h"
#include "lwip/timeouts.h"
#include "lwip/debug.h"
#include "lwip/stats.h"
#include "lwip/init.h"
#include "lwip/tcpip.h"
#include "lwip/netif.h"
#include "lwip/api.h"

#include "lwip/tcp.h"
#include "lwip/udp.h"
#include "lwip/dns.h"
#include "lwip/dhcp.h"
#include "lwip/autoip.h"

/* lwIP netif includes */
#include "lwip/etharp.h"
#include "netif/ethernet.h"

/* applications includes */
#include "lwip/apps/netbiosns.h"
#include "lwip/apps/httpd.h"
#include "lwip/apps/lwiperf.h"
#include "ping.h"

#include "ethernetif.h"
#include <stdint.h>
#include <DLib_Product_string.h>
#include "lwiperf_udp.h"
#include "arch/sys_arch.h"
#include "TimeService.h"


/***********************************************************************************************************************
 *  DEFINES & MACROS
 **********************************************************************************************************************/
/* IP address configuration. */
#define configIP_ADDR0 10
#define configIP_ADDR1 98
#define configIP_ADDR2 2
#define configIP_ADDR3 2

/* Netmask configuration. */
#define configNET_MASK0 255
#define configNET_MASK1 255
#define configNET_MASK2 255
#define configNET_MASK3 0

/* Gateway address configuration. */
#define configGW_ADDR0 10
#define configGW_ADDR1 98
#define configGW_ADDR2 2
#define configGW_ADDR3 0

/* TCP/UDP configuration. */
#define DEFAULT_TCP_UDP_PORT      (5001)
#define USER_TCP_UDP_PORT         (5002)
#define UDP_PORT_Srv_MobileNetOnOffCtrl   (55000U)
#define UDP_PORT_Srv_Weth                 (55001U)
#define UDP_PORT_Srv_VehPosng             (55002U)
#define UDP_PORT_Srv_GPSTi                (55003U)
#define UDP_PORT_Srv_Ecall                (55004U)
#define UDP_PORT_Srv_OTA                  (55005U)



#define DEFAULT_UDP_CLIENT_RATE   (200 * 1024 * 1024)
#define DEFAULT_UDP_CLIENT_LENGTH (1470)

/* Printf configuration. */
#if defined(XC277) || defined(XC275)
#define Example_Printf ADL_Printf
#define Example_Input ADL_Input
#define McuClockMacro McuConf_McuClockSettingConfig_McuClockSettingConfig_0
#else
#define Example_Printf Terminal_Printf
#define Example_Input Terminal_Input
#define McuClockMacro McuConf_McuClockSettingConf_McuClockSettingConfig_0
#endif
#ifndef LWIPERF_ALLOC
#define LWIPERF_ALLOC(type)         mem_malloc(sizeof(type))
#define LWIPERF_FREE(type, item)    mem_free(item)
#endif

/***********************************************************************************************************************
 *  LOCAL TYPEDEFS
 **********************************************************************************************************************/
typedef struct
{
    struct tcp_pcb *pcb;
    uint32_t poll_cnt;        /* poll????????????????? */
    uint32_t last_active_ms;  /* ??��?????????��?? */
}user_session_t;




/***********************************************************************************************************************
 *  LOCAL CONSTANTS
 **********************************************************************************************************************/
const char * s_TcpReportType_pp[] = {
    "TCP_DONE_SERVER",
    "TCP_DONE_CLIENT",
    "TCP_ABORTED_LOCAL",
    "TCP_ABORTED_LOCAL_DATAERROR",
    "TCP_ABORTED_LOCAL_TXERROR",
    "TCP_ABORTED_REMOTE"
};
const char * s_UdpReportType_pp[] = {
    "UDP_DONE_SERVER",
    "UDP_DONE_CLIENT",
    "UDP_ABORTED_LOCAL",
    "UDP_ABORTED_LOCAL_DATAERROR",
    "UDP_ABORTED_LOCAL_TXERROR",
    "UDP_ABORTED_REMOTE"
};

/***********************************************************************************************************************
 *  LOCAL VARIABLES
 **********************************************************************************************************************/
static struct tcp_pcb *pUser_pcb = NULL;
static struct netif netif;
/***********************************************************************************************************************
 *  LOCAL FUNCTION PROTOTYPES
 **********************************************************************************************************************/
void tcp_report(void *arg, enum lwiperf_report_type report_type, const ip_addr_t *local_addr,
                           u16_t local_port, const ip_addr_t *remote_addr, u16_t remote_port,
                           u32_t bytes_transferred, u32_t ms_duration, u32_t bandwidth_kbitpsec);
void udp_report(void *arg, enum lwiperf_udp_report_type report_type, const ip_addr_t *local_addr,
                           u16_t local_port, const ip_addr_t *remote_addr, u16_t remote_port,
                           u32_t bytes_transferred, u32_t ms_duration, u32_t bandwidth_kbitpsec);

static err_t user_poll(void *arg, struct tcp_pcb *tpcb);
static err_t user_recv(void *arg, struct tcp_pcb *tpcb, struct pbuf *p, err_t err);
static void  user_ReportErr(void *arg, err_t err);
static err_t user_accept(void *arg, struct tcp_pcb *newpcb, err_t err);
static int32_t user_tcp_server_start(void);
static uint8_t EthService_DecimalToBcd(uint8_t u8Value);

/***********************************************************************************************************************
 *  LOCAL FUNCTION
 **********************************************************************************************************************/
void tcp_report(void *arg, enum lwiperf_report_type report_type, const ip_addr_t *local_addr,
                           u16_t local_port, const ip_addr_t *remote_addr, u16_t remote_port,
                           u32_t bytes_transferred, u32_t ms_duration, u32_t bandwidth_kbitpsec)
{
    LWIP_PLATFORM_DIAG(("-------------------------------------------------\r\n"));
    if (report_type < (sizeof(s_TcpReportType_pp) / sizeof(s_TcpReportType_pp[0])))
    {
        LWIP_PLATFORM_DIAG((" %s \r\n", s_TcpReportType_pp[report_type]));
        if (local_addr && remote_addr)
        {
            LWIP_PLATFORM_DIAG((" Local address : %u.%u.%u.%u ", ((u8_t *)local_addr)[0], ((u8_t *)local_addr)[1],
                   ((u8_t *)local_addr)[2], ((u8_t *)local_addr)[3]));
            LWIP_PLATFORM_DIAG((" Port %d \r\n", local_port));
            LWIP_PLATFORM_DIAG((" Remote address : %u.%u.%u.%u ", ((u8_t *)remote_addr)[0], ((u8_t *)remote_addr)[1],
                   ((u8_t *)remote_addr)[2], ((u8_t *)remote_addr)[3]));
            LWIP_PLATFORM_DIAG((" Port %u \r\n", remote_port));
            LWIP_PLATFORM_DIAG((" Bytes Transferred %lu \r\n", bytes_transferred));
            LWIP_PLATFORM_DIAG((" Duration (ms) %u \r\n", ms_duration));
            LWIP_PLATFORM_DIAG((" Bandwidth (kbitpsec) %u \r\n", bandwidth_kbitpsec));
        }
    }
    else
    {
        LWIP_PLATFORM_DIAG((" IPERF Report error\r\n"));
    }
}

void udp_report(void *arg, enum lwiperf_udp_report_type report_type, const ip_addr_t *local_addr,
                           u16_t local_port, const ip_addr_t *remote_addr, u16_t remote_port,
                           u32_t bytes_transferred, u32_t ms_duration, u32_t bandwidth_kbitpsec)
{
    LWIP_PLATFORM_DIAG(("-------------------------------------------------\r\n"));
    if (report_type < (sizeof(s_UdpReportType_pp) / sizeof(s_UdpReportType_pp[0])))
    {
        LWIP_PLATFORM_DIAG((" %s \r\n", s_UdpReportType_pp[report_type]));
        if (local_addr && remote_addr)
        {
            LWIP_PLATFORM_DIAG((" Local address : %u.%u.%u.%u ", ((u8_t *)local_addr)[0], ((u8_t *)local_addr)[1],
                   ((u8_t *)local_addr)[2], ((u8_t *)local_addr)[3]));
            LWIP_PLATFORM_DIAG((" Port %d \r\n", local_port));
            LWIP_PLATFORM_DIAG((" Remote address : %u.%u.%u.%u ", ((u8_t *)remote_addr)[0], ((u8_t *)remote_addr)[1],
                   ((u8_t *)remote_addr)[2], ((u8_t *)remote_addr)[3]));
            LWIP_PLATFORM_DIAG((" Port %u \r\n", remote_port));
            LWIP_PLATFORM_DIAG((" Bytes Transferred %lu \r\n", bytes_transferred));
            LWIP_PLATFORM_DIAG((" Duration (ms) %u \r\n", ms_duration));
            LWIP_PLATFORM_DIAG((" Bandwidth (kbitpsec) %u \r\n", bandwidth_kbitpsec));
        }
    }
    else
    {
        LWIP_PLATFORM_DIAG((" IPERF Report error\r\n"));
    }
}

/*******************************************************
  * Name    :EthService_DecimalToBcd
  * Author  :LoH
  * Function:Convert one decimal time field to packed BCD for Ethernet payload.
  * Version :V1.0.0
  * Data    :2026.09.13
*******************************************************/
static uint8_t EthService_DecimalToBcd(uint8_t u8Value)
{
    return (uint8_t)(((u8Value / 10U) << 4U) | (u8Value % 10U));
}

/*******************************************************
  * Name    :user_poll
  * Author  :LoH
  * Function:Periodically send the current UTC time in six packed BCD bytes.
  * Version :V1.0.0
  * Data    :2026.09.13
*******************************************************/
static err_t user_poll(void *arg, struct tcp_pcb *tpcb)
{
    user_session_t *sess = (user_session_t *)arg;
    u32_t now = sys_now();

    if((sess == NULL) || (sess->pcb != tpcb) || (tpcb->state != ESTABLISHED))
    {
        return ERR_OK;
    }

    /* ???��????10s?????????????? */
    if((now - sess->last_active_ms) > 10000U)
    {
        Example_Printf("user tcp idle timeout, close conn\r\n");
        tcp_close(tpcb);
        LWIPERF_FREE(user_session_t, sess);
        return ERR_OK;
    }

    sess->poll_cnt ++;
    /* ?3?????????????poll?1s??????��?????>=3 */
    if(sess->poll_cnt >= 3U)
    {
        sess->poll_cnt = 0U;
        if(tcp_sndbuf(tpcb) >= 8U)
        {
            Eth_GpsTime_Def gpsTime_Eth = {0};
            TimeServiceTime_t stUtcTime;

            if (TimeService_GetUtc(&stUtcTime) != TIME_SERVICE_SOURCE_INVALID)
            {
                gpsTime_Eth.B.gpsTime_Year = EthService_DecimalToBcd(stUtcTime.u8Year);
                gpsTime_Eth.B.gpsTime_Month = EthService_DecimalToBcd(stUtcTime.u8Month);
                gpsTime_Eth.B.gpsTime_Day = EthService_DecimalToBcd(stUtcTime.u8Day);
                gpsTime_Eth.B.gpsTime_Hour = EthService_DecimalToBcd(stUtcTime.u8Hour);
                gpsTime_Eth.B.gpsTime_Minute = EthService_DecimalToBcd(stUtcTime.u8Minute);
                gpsTime_Eth.B.gpsTime_Second = EthService_DecimalToBcd(stUtcTime.u8Second);
            }
      
            err_t ret = tcp_write(tpcb, &gpsTime_Eth.U[0], sizeof(Eth_GpsTime_Def), TCP_WRITE_FLAG_COPY);
            if(ret == ERR_OK)
            {
                tcp_output(tpcb);
                sess->last_active_ms = now;
            }
        }
    }
    return ERR_OK;
}

static err_t user_recv(void *arg, struct tcp_pcb *tpcb, struct pbuf *p, err_t err)
{
    user_session_t *sess = (user_session_t *)arg;
    if(err != ERR_OK)
    {
        tcp_close(tpcb);
        LWIPERF_FREE(user_session_t, sess);
        return err;
    }
    /* ?????????????? */
    if(p == NULL)
    {
        Example_Printf("biz client close connection\r\n");
        tcp_close(tpcb);
        LWIPERF_FREE(user_session_t, sess);
        return ERR_OK;
    }

    /* ??????????????????? */
    sess->last_active_ms = sys_now();
    Example_Printf("biz recv len:%d\r\n", p->tot_len);

    /* =========????��???????????????======== */

    tcp_recved(tpcb, p->tot_len);
    pbuf_free(p);
    return ERR_OK;
}

static void user_ReportErr(void *arg, err_t err)
{
    LWIP_UNUSED_ARG(err);
    user_session_t *sess = (user_session_t *)arg;
    if(sess != NULL)
    {
        LWIPERF_FREE(user_session_t, sess);
    }
}

static err_t user_accept(void *arg, struct tcp_pcb *newpcb, err_t err)
{
    LWIP_UNUSED_ARG(arg);
    if(err != ERR_OK || newpcb == NULL)
    {
        return ERR_ARG;
    }

    /* ???????? */
    user_session_t *sess = (user_session_t *)LWIPERF_ALLOC(user_session_t);
    if(sess == NULL)
    {
        tcp_close(newpcb);
        return ERR_MEM;
    }
    memset(sess,0,sizeof(user_session_t));
    sess->pcb = newpcb;
    sess->last_active_ms = sys_now();

    /* ????��???pcb */
    tcp_arg(newpcb, sess);
    tcp_recv(newpcb, user_recv);
    tcp_poll(newpcb, user_poll, 2);
    tcp_err(newpcb, user_ReportErr);

    Example_Printf("new user client connected port=%d\r\n", UDP_PORT_Srv_GPSTi);
    return ERR_OK;
}

static int32_t user_tcp_server_start(void)
{
    struct tcp_pcb *pcb;
    if(pUser_pcb != NULL)
    {
        Example_Printf("user tcp server already start\r\n");
        return 0;
    }

    pcb = tcp_new_ip_type(IPADDR_TYPE_V4);
    if(pcb == NULL)
    {
        Example_Printf("user tcp new pcb fail\r\n");
        return -1;
    }

    err_t err = tcp_bind(pcb, IP_ADDR_ANY, UDP_PORT_Srv_GPSTi);
    if(err != ERR_OK)
    {
        Example_Printf("user tcp bind port %d fail\r\n",UDP_PORT_Srv_GPSTi);
        tcp_close(pcb);
        return -2;
    }

    /* backlog=1???????????1????????????? */
    pUser_pcb = tcp_listen_with_backlog(pcb,1);
    if(pUser_pcb == NULL)
    {
        tcp_close(pcb);
        Example_Printf("user tcp listen fail\r\n");
        return -3;
    }

    tcp_arg(pUser_pcb, NULL);
    tcp_accept(pUser_pcb, user_accept);
    Example_Printf("user tcp server start port=%d\r\n", UDP_PORT_Srv_GPSTi);
    return 0;
}

static void EthTask_Entry(void const *argument)
{
    (void)argument;
    Hmi_ShowTaskStarted("EthTask");

    for (;;)
    {
        /* Poll UDP client */
        lwiperf_poll_udp_client();
        /* Poll the driver, get any outstanding frames */
        ethernetif_input(&netif);
        sys_check_timeouts(); /* Handle all system timeouts for all core protocols */
        osDelay(10U);
    }
}

void EthService_Init(void)
{
    ip_addr_t netif_netmask = {0};
    ip_addr_t netif_ipaddr  = {0};
    ip_addr_t netif_gw      = {0};
    void *IperfState_ptr    = NULL_PTR;
    
    IP4_ADDR(&netif_netmask, configNET_MASK0, configNET_MASK1, configNET_MASK2, configNET_MASK3);
    IP4_ADDR(&netif_ipaddr, configIP_ADDR0, configIP_ADDR1, configIP_ADDR2, configIP_ADDR3);
    IP4_ADDR(&netif_gw, configGW_ADDR0, configGW_ADDR1, configGW_ADDR2, configGW_ADDR3);

    Example_Printf("\r\n************************************************\r\n");
    Example_Printf(" LWIP iperf example\r\n");
    Example_Printf("************************************************\r\n");
    Example_Printf(" IPv4 Address     : %d.%d.%d.%d\r\n", ((u8_t *)&netif_ipaddr)[0], ((u8_t *)&netif_ipaddr)[1],
           ((u8_t *)&netif_ipaddr)[2], ((u8_t *)&netif_ipaddr)[3]);
    Example_Printf(" IPv4 Gateway     : %d.%d.%d.%d\r\n", ((u8_t *)&netif_gw)[0], ((u8_t *)&netif_gw)[1],
           ((u8_t *)&netif_gw)[2], ((u8_t *)&netif_gw)[3]);
    Example_Printf("************************************************\r\n");
    
    lwip_init();
    netif_add(&netif, &netif_ipaddr, &netif_netmask, &netif_gw, NULL, ethernetif_init, ethernet_input);
    netif_set_default(&netif);
    netif_set_up(&netif);

    /* Start Tcp server! */
    IperfState_ptr = lwiperf_start_tcp_server(IP_ADDR_ANY, DEFAULT_TCP_UDP_PORT, tcp_report, 0);
    if (NULL_PTR == IperfState_ptr)
    {
        Example_Printf("\r\nIperf init fail!\r\n");
    }
    else
    {
        Example_Printf("\r\nStart Tcp server!\r\n");
    }

    /* Start Tcp client! */
    IperfState_ptr = lwiperf_start_tcp_client(&netif_gw, DEFAULT_TCP_UDP_PORT, LWIPERF_CLIENT, tcp_report, 0);
    if (NULL_PTR == IperfState_ptr)
    {
        Example_Printf("\r\nIperf init fail!\r\n");
    }
    else
    {
        Example_Printf("\r\nStart Tcp client!\r\n");
    }

    /* Start User Tcp client! */
    user_tcp_server_start();
}
/*******************************************************
  * Name    :CanTask_Create
  * Author  :LoH
  * Function:Create CanTask with real-time RTOS priority.
  * Version :V1.0.0
  * Data    :2026.08.26
*******************************************************/
void EthTask_Create(void)
{
    osThreadDef(EthTask, EthTask_Entry, osPriorityRealtime, 0U, 4096U);
    (void)osThreadCreate(osThread(EthTask), NULL);
}
