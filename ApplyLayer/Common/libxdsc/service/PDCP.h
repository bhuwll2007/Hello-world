/**
 * @file PDCP.h
 * @author catt
 * @version 1.0
 * @date 2018-9-4
 * @brief Access Layer Interface.
 * @details XDS-based transmission and reception over PDCP requires the following definitions.
 */

#ifndef _PDCP_H_
#define _PDCP_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "protocol/Protocol.h"

/// SubService number for PDCP.
#define SSID_PDCP       60

/// SubService name for PDCP.
#define SSNAME_PDCP     "PDCP"

/**
 * Primitives that can be attached or detached by PDCP service.
 */
enum PDCP_ADDITIONAL
{
    PDCP_ADDITIONAL_BASE = E_OPTION_SUBSERVICE_BASE,
    PDCP_ATTACH_SEND_REQUEST,       /**< transmit request primitive */
    PDCP_DETACH_RECV_INDICATION,    /**< receive indication primitive */
    PDCP_DETACH_RECV_RSRP,          /**< detach rsrp from last received packet */
};

/**
 * Possible Errors for PDCP service.
 */
enum PDCP_ERROR
{
    PDCP_ERROR_BASE = E_RSPS2C_SERVICE_BASE,
    PDCP_ERROR_NOT_ATTACH_REQUEST,  /**< Send primitives must be attached before sending */
    PDCP_ERROR_SEND_OPERATION,      /**< Send operation failed */
    PDCP_ERROR_PAYLOAD_EMPTY,       /**< Payload is empty */
    PDCP_ERROR_PRIORITY_INVALID,    /**< Priority field illegal, Should be [1~8]*/
    PDCP_ERROR_PERIOD_INVALID,      /**< Period field illegal, Should be [0~12]*/
    PDCP_ERROR_TRAFFICID_INVALID,   /**< Traffic_ID field illegal, Should be [0~9]*/
    PDCP_ERROR_PAYLOAD_LARGE,       /**< Payload is too large */
    PDCP_ERROR_TRAFFIC_BUSY,        /**< Traffic Busy! */
};

/**
 * SDU Type for sdu_type field in pdcp_request_t
 */
enum PDCP_SDUTYPE
{
    PDCP_SDUTYPE_NONIP, /**< Non-IP Data, Mainly refers to dsmp data. */
    PDCP_SDUTYPE_IP,
};

/**
 * The Primitive for Data sending request over PDCP.
 */
typedef struct pdcp_request_s
{
    uint32_t dst_addr;      /**< Destination address at L2. With the aid linked. */

    uint8_t  sdu_type;      /**< ip or non-ip */
    uint8_t  priority;      /**< priority [1~8] */
    uint8_t  traffic_id;    /**< Used to distinguish between different businesses using the same cycle.
                                 [0~9], in other words, at most 10 businesses could be supported at one moment. */
    uint8_t  period;        /**< 0 for one-shot; 1~10 for 100~1000ms; 11 for 20ms; 12 for 50ms.*/
}pdcp_request_t;

/**
 * The Primitive for Data arrival indication over PDCP.
 */
typedef struct pdcp_indication_s
{
    uint32_t src_addr;  /**< Source address at L2. Used to distinguish different terminals. */
    uint32_t dst_addr;  /**< Refer to the same field under pdcp_request_t */

    uint8_t  priority;  /**< Refer to the same field under pdcp_request_t */
    uint8_t  sdu_type;  /**< Refer to the same field under pdcp_request_t */
    uint16_t reserved;  /**< For bytes alignment */
}pdcp_indication_t;

#ifdef __cplusplus
}
#endif

#endif /* SERVICE_BUILTIN_PDCP_H_ */
