/***********************************************************************************************************************
 *
 * Copyright (c) 2023 - 2026 Xiaohua Semiconductor Co., Ltd.
 *
 * All rights reserved.
 *
 * This software is licensed by XHSC PLATFORM SOFTWARE LICENSE AGREEMENT (the "License"). By downloading, installing, or
 * using the Software, you indicate that you accept the terms of the License, and you acknowledge that you have the
 * authority, for yourself or on behalf of your company, to bind your company to these terms. If you do not agree to all
 * of the terms of this License, you should not download or install the Software.
 *
 **********************************************************************************************************************/
#ifndef LWIP_HDR_APPS_LWIPERF_UDP_H
#define LWIP_HDR_APPS_LWIPERF_UDP_H

#include "lwip/opt.h"
#include "lwip/ip_addr.h"

#ifdef __cplusplus
extern "C" {
#endif

/** lwIPerf test results */
enum lwiperf_udp_report_type
{
  /** The server side test is done */
  LWIPERF_UDP_DONE_SERVER,
  /** The client side test is done */
  LWIPERF_UDP_DONE_CLIENT,
  /** Local error lead to test abort */
  LWIPERF_UDP_ABORTED_LOCAL,
  /** Data check error lead to test abort */
  LWIPERF_UDP_ABORTED_LOCAL_DATAERROR,
  /** Transmit error lead to test abort */
  LWIPERF_UDP_ABORTED_LOCAL_TXERROR,
  /** Remote side aborted the test */
  LWIPERF_UDP_ABORTED_REMOTE
};

typedef void (*lwiperf_udp_report_fn)(void *arg, enum lwiperf_udp_report_type report_type,
  const ip_addr_t* local_addr, u16_t local_port, const ip_addr_t* remote_addr, u16_t remote_port,
  u32_t bytes_transferred, u32_t ms_duration, u32_t bandwidth_kbitpsec);

void* lwiperf_start_udp_client(const ip_addr_t *local_addr, u16_t local_port,
                         const ip_addr_t *remote_addr, u16_t remote_port, u32_t buffer_len, s32_t rate,
                         enum lwiperf_client_type type, lwiperf_udp_report_fn report_fn, void *report_arg);
void* lwiperf_start_udp_server(const ip_addr_t* local_addr, u16_t local_port,
                               lwiperf_udp_report_fn report_fn, void* report_arg);
void lwiperf_poll_udp_client(void);

#ifdef __cplusplus
}
#endif

#endif /* LWIP_HDR_APPS_LWIPERF_UDP_H */
