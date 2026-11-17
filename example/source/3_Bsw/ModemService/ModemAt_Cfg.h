/*******************************************************
  * Name    :ModemAt_Cfg.h
  * Author  :LoH
  * Function:Modem AT command and response configuration.
  * Version :V1.0.0
  * Data    :2026.08.26
*******************************************************/
#ifndef __MODEM_AT_CFG_H__
#define __MODEM_AT_CFG_H__

/* AC7600 AT commands and response tokens. */
#define MODEM_AT_CMD_BASIC                      "AT\r\n"
#define MODEM_AT_CMD_ECHO_OFF                   "ATE0\r\n"
#define MODEM_AT_CMD_SIM_STATUS                 "AT+CPIN?\r\n"
#define MODEM_AT_CMD_SIGNAL_QUALITY             "AT+CSQ\r\n"
#define MODEM_AT_CMD_CIRCUIT_REGISTRATION       "AT+CEREG?\r\n"
#define MODEM_AT_CMD_PDP_STATE                  "AT+NETOPEN?\r\n"
#define MODEM_AT_CMD_PDP_OPEN                   "AT+NETOPEN\r\n"
#define MODEM_AT_CMD_PDP_IPADDR                 "AT+IPADDR\r\n"
#define MODEM_AT_CMD_TCP_KEEPALIVE               "AT+CTCPKA=1,2,5,1\r\n"
#define MODEM_AT_CMD_TCP_DATA_ECHO_OFF          "AT+CIPCFG=\"DATAECHO\",OFF\r\n"
#define MODEM_AT_CMD_TCP_RX_MODE                 "AT+CIPRXGET=1\r\n"
#define MODEM_AT_CMD_TCP_RX_LENGTH_FORMAT        "AT+CIPRXGET=4,%u\r\n"
#define MODEM_AT_CMD_TCP_RX_DATA_FORMAT          "AT+CIPRXGET=3,%u,%u\r\n"
#define MODEM_AT_CMD_TCP_OPEN_FORMAT            "AT+CIPOPEN=%u,\"TCP\",\"%s\",%u\r\n"
#define MODEM_AT_CMD_TCP_SEND_FORMAT            "AT+CIPSEND=%u,%u\r\n"
#define MODEM_AT_CMD_TCP_CLOSE_FORMAT           "AT+CIPCLOSE=%u\r\n"
#define MODEM_AT_CMD_PDP_CLOSE                  "AT+NETCLOSE\r\n"
#define MODEM_AT_CMD_EXTENDED_ERROR_REPORT      "AT+CEER\r\n"
#define MODEM_AT_CMD_ANSWER_CALL                "ATA\r\n"
#define MODEM_AT_CMD_GNSS_POWER_OFF              "AT+CGNSSPWR=0\r\n"
#define MODEM_AT_CMD_GNSS_POWER_ON               "AT+CGNSSPWR=1,0,1\r\n"
#define MODEM_AT_CMD_GNSS_POSITION               "AT+CGPSINFO\r\n"
#define MODEM_AT_CMD_TIME_NITZ_QUERY             "AT+CTZU?\r\n"
#define MODEM_AT_CMD_TIME_NITZ_ENABLE            "AT+CTZU=1\r\n"
#define MODEM_AT_CMD_TIME_CLOCK_QUERY            "AT+CCLK?\r\n"
#define MODEM_AT_CMD_UART_SLEEP_RX               "AT+CSCLK=2\r\n"
#define MODEM_AT_CMD_AUDIO_ID_QUERY              "AT+CRIIC=B0,00,1\r\n"
#define MODEM_AT_CMD_AUDIO_SPEAKER_GAIN          "AT+CWIIC=B0,06,0E,1\r\n"
#define MODEM_AT_CMD_AUDIO_SPEAKER_ENABLE        "AT+CWIIC=B0,01,78,1\r\n"
#define MODEM_AT_CMD_AUDIO_GAIN_READ             "AT+CRIIC=B0,05,2\r\n"
#define MODEM_AT_CMD_AUDIO_CONTROL_READ          "AT+CRIIC=B0,01,2\r\n"
#define MODEM_AT_CMD_AUDIO_SPEAKER_DISABLE       "AT+CWIIC=B0,01,38,1\r\n"

/* Dynamic commands must be assembled with snprintf and these format macros. */
#define MODEM_AT_CMD_DIAL_FORMAT                "ATD%s;\r\n"

/* Common response tokens. Actual URC handling remains module-specific. */
#define MODEM_AT_RSP_OK                         "OK"
#define MODEM_AT_RSP_SIM_READY                  "+CPIN: READY"
#define MODEM_AT_RSP_SIGNAL_QUALITY             "+CSQ:"
#define MODEM_AT_RSP_NETWORK_REGISTRATION       "+CEREG:"
#define MODEM_AT_RSP_PDP_STATE                  "+NETOPEN:"
#define MODEM_AT_RSP_PDP_OPENED                 "+NETOPEN: 0"
#define MODEM_AT_RSP_PDP_CLOSED                 "+NETCLOSE: 0"
#define MODEM_AT_RSP_TCP_OPEN                   "+CIPOPEN:"
#define MODEM_AT_RSP_TCP_CLOSED                 "+CIPCLOSE:"
#define MODEM_AT_RSP_TCP_RX_LENGTH              "+CIPRXGET: 4,"
#define MODEM_AT_RSP_PROMPT                      ">"
#define MODEM_AT_RSP_ERROR                      "ERROR"
#define MODEM_AT_RSP_BUSY                        "BUSY"
#define MODEM_AT_RSP_NO_CARRIER                  "NO CARRIER"
#define MODEM_AT_RSP_SEND                        "+CIPSEND:"
#define MODEM_AT_RSP_EXTENDED_ERROR_REPORT       "+CEER:"
#define MODEM_AT_RSP_GNSS_READY                  "+CGNSSPWR: READY!"
#define MODEM_AT_RSP_GNSS_POSITION               "+CGPSINFO:"
#define MODEM_AT_RSP_TIME_NITZ                   "+CTZU:"
#define MODEM_AT_RSP_TIME_CLOCK                  "+CCLK:"
#define MODEM_AT_RSP_AUDIO_READBACK              "+CRIIC: 0x"

/* Platform values remain candidates until AC7600 TCP verification completes. */
#define MODEM_PLATFORM_LINK_ID                  0U
#define MODEM_PLATFORM_SERVER_IP                "183.69.173.130"//"8.137.175.163"
#define MODEM_PLATFORM_SERVER_PORT              24201U//8902U
#define MODEM_PLATFORM_TEST_DEVICE_ID           0x7091U
#define MODEM_PLATFORM_TEST_VIN                 "LDC12345678900001"
#define MODEM_PLATFORM_DIAL_NUMBER              "02361752708"//"15823262547"
/* MSD V0.2 configuration. Empty identity values are omitted, never fabricated. */
#define MODEM_PLATFORM_MSD_EXTENSION_ENABLED    1U
#define MODEM_PLATFORM_SIM_PHONE_NUMBER        ""
#define MODEM_PLATFORM_ICCID                   ""
#define MODEM_PLATFORM_IMEI                    ""
#define MODEM_PLATFORM_HARDWARE_VERSION        ""
#define MODEM_PLATFORM_SOFTWARE_VERSION        ""
#define MODEM_AT_CMD_SUBSCRIBER_NUMBER         "AT+CNUM\r\n"
#define MODEM_MSD_ACK_TIMEOUT_MS               15000UL
#define MODEM_PLATFORM_REGISTER_ACK_TIMEOUT_MS  5000UL
#define MODEM_PLATFORM_TCP_CLOSE_GRACE_MS        500UL
#define MODEM_MSD_FIRST_RETRY_MS               30000UL
#define MODEM_MSD_LATER_RETRY_MS               120000UL
#define MODEM_MSD_RETRY_WINDOW_MS              3600000UL
#define MODEM_AT_CFG_MSD_SEND_RESULT_TIMEOUT_MS 30000UL

#define MODEM_PLATFORM_MSD_VERSION              1U
#define MODEM_PLATFORM_MSD_IDENTIFIER           1U
#define MODEM_PLATFORM_MSD_TEST_CALL            1U
#define MODEM_PLATFORM_MSD_VEHICLE_TYPE         0U
#define MODEM_PLATFORM_MSD_POWER_TYPE           0U
#define MODEM_PLATFORM_MSD_VIN_LENGTH           17U

#define MODEM_PLATFORM_MSG_REGISTER             0x01U
#define MODEM_PLATFORM_MSG_HEARTBEAT            0x02U
#define MODEM_PLATFORM_MSG_MSD                  0x10U
#define MODEM_PLATFORM_REGISTER_JSON_FORMAT     "{\"role\":\"device\",\"deviceId\":\"%04X\"}"
#define MODEM_PLATFORM_HEARTBEAT_JSON_FORMAT    "{\"type\":\"heartbeat\",\"deviceId\":\"%04X\"}"

/* Timeouts are used by the future ModemPort AT transaction adapter. */
#define MODEM_AT_CFG_SYNC_TIMEOUT_MS            1000U
#define MODEM_AT_CFG_SIM_TIMEOUT_MS             2000U
#define MODEM_AT_CFG_NETWORK_TIMEOUT_MS         1000U
#define MODEM_AT_CFG_PDP_OPEN_TIMEOUT_MS        10000//120000U
#define MODEM_AT_CFG_TCP_CFG_TIMEOUT_MS         9000U
#define MODEM_AT_CFG_TCP_OPEN_TIMEOUT_MS        120000U
#define MODEM_AT_CFG_TCP_CLOSE_TIMEOUT_MS        10000U
#define MODEM_AT_CFG_SEND_PROMPT_TIMEOUT_MS      9000U
#define MODEM_AT_CFG_SEND_RESULT_TIMEOUT_MS      120000U
#define MODEM_AT_CFG_DIAL_TIMEOUT_MS            30000U
#define MODEM_AT_CFG_ANSWER_TIMEOUT_MS          30000U
#define MODEM_AT_CFG_EXTENDED_ERROR_TIMEOUT_MS    9000U
#define MODEM_AT_CFG_GNSS_POWER_TIMEOUT_MS        9000U
#define MODEM_AT_CFG_GNSS_READY_TIMEOUT_MS       120000U
#define MODEM_AT_CFG_GNSS_POWER_OFF_HOLD_MS        1000U
#define MODEM_AT_CFG_GNSS_POSITION_TIMEOUT_MS     2000U
#define MODEM_AT_CFG_GNSS_QUERY_PERIOD_MS         1000U
#define MODEM_AT_CFG_TIME_TIMEOUT_MS              2000U
#define MODEM_AT_CFG_UART_SLEEP_TIMEOUT_MS        2000U
#define MODEM_AT_CFG_AUDIO_I2C_TIMEOUT_MS         2000U
#define MODEM_AT_CFG_VOICE_RETRY_ENABLED              1U
#define MODEM_AT_CFG_VOICE_RETRY_FIRST_MS        30000U
#define MODEM_AT_CFG_VOICE_RETRY_SECOND_MS       120000U
#define MODEM_AT_CFG_VOICE_RETRY_LATER_MS       120000U
#define MODEM_AT_CFG_VOICE_RETRY_WINDOW_MS     3600000UL
#define MODEM_AT_CFG_SEND_TIMEOUT_MS            2000U
#define MODEM_PLATFORM_HEARTBEAT_PERIOD_MS      25000U

/* The bare-metal ML307A platform uploads a hexadecimal MSD frame by MIPSEND. */
#define MODEM_AT_CFG_MSD_UPLOAD_SUPPORTED       1U

#endif
