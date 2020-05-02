/**
 * @file GNSS.h
 * @author catt
 * @version 1.0
 * @date 2018-3-10
 * @brief GNSS Interface.
 * @details View GNSS status and get GNSS position data.
 */

#ifndef _GNSS_H_
#define _GNSS_H_

#include <stddef.h>
#include <inttypes.h>

#ifdef __cplusplus
extern "C" {
#endif

#include "protocol/Protocol.h"

/// SubService number for GNSS.
#define SSID_GNSS           10

/// SubService name for GNSS.
#define SSNAME_GNSS         "GNSS"

/// Reserved for extend GNSS sensor.
#define SSID_EXT_GNSS       12
#define SSNAME_EXT_GNSS     "GNSSEXT"

/**
 * Possible Errors for GNSS service. Only for xds internal, not user errno!
 */
enum GNSS_ERROR_INTERNAL
{
    GNSS_ERROR_MSGID_BASE = E_RSPS2C_SERVICE_BASE,
    GNSS_ERROR_MSGID_ILLEGAL,
};

/**
 * MSGID for GNSS service.
 */
enum GNSS_MSGID
{
    GNSS_MSGID_STATUS   = 0,    /**< GNSS status. Can only be read in active way.*/
    GNSS_MSGID_LOCATION = 1,    /**< Basic GNSS fix Info. Can only be received in passive way. */
};

/**
 * Message content for GNSS_MSGID_STATUS. Status Type.
 */
enum GNSS_STATUS
{
    GNSS_STATUS_UNKNOWN,
    GNSS_STATUS_UNINITIALIZE,
    GNSS_STATUS_INITIALIZED,
    GNSS_STATUS_LOCKED,
    GNSS_STATUS_UNLOCK,
};

/**
 * Message content for GNSS_MSGID_STATUS. Data Type Definition.
 */
typedef struct gnss_status_s{
    uint32_t        value;
} gnss_status_t;

#define GNSS_LOCATION_HAS_LAT_LONG   0x0001  /**< GnssLocation has valid latitude and longitude. */
#define GNSS_LOCATION_HAS_ALTITUDE   0x0002  /**< GnssLocation has valid altitude. */
#define GNSS_LOCATION_HAS_SPEED      0x0004  /**< GnssLocation has valid speed. */
#define GNSS_LOCATION_HAS_BEARING    0x0008  /**< GnssLocation has valid bearing. */
#define GNSS_LOCATION_HAS_ACCURACY   0x0010  /**< GnssLocation has valid accuracy. */
#define GNSS_LOCATION_HAS_DIFFERENCE 0x0020

typedef int64_t utc_time;

/**
 * Message content for GNSS_MSGID_LOCATION.
 */
typedef struct gnss_location_s{
    size_t          size;       /**< set to sizeof(GnssLocation) */
    uint16_t        flags;      /**< Contains GnssLocationFlags bits. */
    double          latitude;   /**< Represents latitude in degrees. */
    double          longitude;  /**< Represents longitude in degrees. */
    double          altitude;   /**< Represents altitude in meters above the WGS 84 reference ellipsoid. */
    float           speed;      /**< Represents speed in meters per second. */
    float           bearing;    /**< Represents heading in degrees. */
    float           accuracy;   /**< Represents expected accuracy in meters. */
    utc_time        timestamp;  /**< Timestamp for the location fix. */
} gnss_location_t;

#ifdef __cplusplus
}
#endif

#endif /* _GNSS_H_ */



