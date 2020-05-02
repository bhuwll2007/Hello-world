/**
 * @file Utils.h
 * @author catt
 * @version 1.0
 * @date 2018-12-4
 * @brief Generic API Package
 * @details Read Device Serial Number, AP Temperature, Software Version
 */

#ifdef __cplusplus
extern "C" {
#endif

#include "protocol/Protocol.h"

/// SubService number for UTILS.
#define SSID_UTILS       50

/// SubService name for UTILS.
#define SSNAME_UTILS     "UTILS"

/**
 * Possible Errors for UTILS service.
 */
enum UTILS_ERROR
{
    UTILS_ERROR_BASE = E_RSPS2C_SERVICE_BASE,
    UTILS_ERROR_CREATE_HANDLE,              /**< Create Handle Error! */
    UTILS_ERROR_UNDERLYING_CHANNEL,         /**< Open Underlying Channel Error! */
    UTILS_ERROR_READ_CHANNEL,               /**< Read Underlying Channel Error! */
    UTILS_ERROR_SEARCH_CONTENT,             /**< Searching Content Error! */
    UTILS_ERROR_USR_BUFFER_ILLEGAL,         /**< User Buffer Illegal! */
    UTILS_ERROR_UNKNOWN_MSGID               /**< Unknown Message ID! */
};

/**
 * SDU Type for sdu_type field in pdcp_request_t
 */
enum UTILS_MSGID
{
    UTILS_MSGID_SERIAL_NUMBER,     /**< Read Serial Number  */
    UTILS_MSGID_AP_TEMPERATURE,    /**< Read AP Temperature */
    UTILS_MSGID_SW_VERSION,        /**< Read Software Version */
    UTILS_MSGID_HW_VERSION,        /**< Read Hardware Version */
};

/**
 * Type for reading temperature
 */
typedef int32_t temperature_t;

#ifdef __cplusplus
}
#endif
