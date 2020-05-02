/**
 * @file DSMP.h
 * @author catt
 * @version 1.0
 * @date 2018-9-4
 * @brief Network Layer Interface Wrapper.
 * @details XDS-based transmission and reception over DSMP requires the following definitions.
 */

#ifndef _DSMP_H_
#define _DSMP_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "protocol/Protocol.h"

/// SubService number for DSMP.
#define SSID_DSMP       30

/// SubService name for DSMP.
#define SSNAME_DSMP     "DSMP"

/**
 * Primitives that can be attached or detached by DSMP service.
 */
enum DSMP_ADDITIONAL
{
    DSMP_ADDITIONAL_BASE = E_OPTION_SUBSERVICE_BASE,
	DSMP_ATTACH_SEND_REQUEST,       /**< transmit request primitive */
	DSMP_DETACH_RECV_INDICATION,    /**< receive indication primitive */
	DSMP_DETACH_RECV_RSRP,          /**< detach rsrp from last received packet */
};

/**
 * Possible Errors for DSMP service. Only for xds internal, not user errno!
 */
enum DSMP_ERROR_INTERNAL
{
    DSMP_ERROR_BASE = E_RSPS2C_SERVICE_BASE,
    DSMP_ERROR_OTHER_OCCUPY ,       /**< Other Client Occupy */
    DSMP_ERROR_ALREADY_SUBSCRIBE,   /**< DSMP Could Only Subscribe one Message */
    DSMP_ERROR_HASNOT_SUBSCRIBE,    /**< Not yet subscribed to a message */
    DSMP_ERROR_MSGID_ILLEGAL,       /**< UnSubscribe Message id Illegal */
    DSMP_ERROR_PRIO_ILLEGAL,        /**< Priority Illegal, should be[1~8] */
    DSMP_ERROR_PERIOD_ILLEGAL,      /**< Period Illegal, should be [0~12] */
    DSMP_ERROR_TRAFFICID_ILLEGAL,   /**< Traffic ID illegal, should be [0~9] */
    DSMP_ERROR_PROTOTYPE_ILLEGAL,   /**< Protocol type illegal [4 for dsmp] */
    DSMP_ERROR_PAYLOAD_LARGE,       /**< Payload is too large! */
    DSMP_ERROR_SEND_OPERATION,      /**< Send operation failed */
    DSMP_ERROR_NOT_ATTACH_REQUEST,  /**< Did not attach request primitive */
    DSMP_ERROR_PAYLOAD_EMPTY,       /**< Payload is empty */
    DSMP_ERROR_TRAFFIC_BUSY,        /**< traffic busy! */
};

/**
 * The Primitive for Data sending request over DSMP.
 */
typedef struct dsmp_request_s
{
    uint8_t  proto_type;    /**< 0 for IPv6, 4 for DSMP, 5 for IPv4 */
    uint8_t  priority;      /**< priority [1~8] */
    uint8_t  traffic_id;    /**< Used to distinguish between different businesses using the same cycle.
                                 [0~9], in other words, at most 10 businesses could be supported at one moment. */
    uint8_t  period;        /**< 0 for one-shot; 1~10 for 100~1000ms; 11 for 20ms; 12 for 50ms.*/
}dsmp_request_t;

/**
 * The Primitive for Data arrival indication over DSMP.
 */
typedef struct dsmp_indication_s
{
    uint32_t src_addr;      /**< source address at L2. Used to distinguish different terminals. */
    uint8_t  dsmp_ver;      /**< DSMP version */
    uint8_t  priority;      /**< Refer to the same field under dsmp_request_t */
}dsmp_indication_t;

#ifdef __cplusplus
}
#endif

#endif /* _DSMP_H_ */
