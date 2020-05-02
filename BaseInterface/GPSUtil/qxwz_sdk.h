/*
* Copyright (c) 2015-2019 QXSI. All rights reserved.
*/
#ifndef QXWZ_SDK_H__
#define QXWZ_SDK_H__
#ifdef __cplusplus
extern "C" {
#endif
#include "qxwz_types.h"
/*******************************************************************************
 * SDK status Definitions
 *******************************************************************************/

/**********************************
 * status code, 1 - 999
 **********************************/

/*
 * interface related, 1 - 100
 */
#define QXWZ_SDK_STAT_OK                                    0

/*
 * cap related, 101 - 200
 */
#define QXWZ_SDK_STAT_CAP_START_SUCC                        101

/*
 * auth related, 201 - 300
 */
#define QXWZ_SDK_STAT_AUTH_SUCC                             201

/*
 * unknown status code
 */
#define QXWZ_SDK_STAT_UNKNOWN                               999


/**********************************
 * error code, from -1 to -999
 **********************************/

/*
 * interface related, -1 - -100
 */
#define QXWZ_SDK_ERR_FAIL                                   -1 
#define QXWZ_SDK_ERR_INVALID_PARAM                          -2
#define QXWZ_SDK_ERR_INVALID_CONFIG                         -3
#define QXWZ_SDK_ERR_NOT_INITED                             -4
#define QXWZ_SDK_ERR_NOT_AUTHED                             -5
#define QXWZ_SDK_ERR_NOT_STARTED                            -6
#define QXWZ_SDK_ERR_AUTHING                                -7 
#define QXWZ_SDK_ERR_STARTING                               -8
#define QXWZ_SDK_ERR_ALREADY_INITED                         -9
#define QXWZ_SDK_ERR_ALREADY_AUTHED                         -10
#define QXWZ_SDK_ERR_ALREADY_STARTED                        -11
#define QXWZ_SDK_ERR_GETTING_COORD_SYS                      -12
#define QXWZ_SDK_ERR_SETTING_COORD_SYS                      -13
#define QXWZ_SDK_ERR_QUERYING_EXEC_STRATEGY                 -14


/*
 * network related, -101 - -200
 */
#define QXWZ_SDK_ERR_NETWORK_UNAVAILABLE                    -101

/*
 * cap related, -201 - -300
 */
#define QXWZ_SDK_ERR_GGA_OUT_OF_SERVICE_AREA                -201
#define QXWZ_SDK_ERR_INVALID_GGA                            -202
#define QXWZ_SDK_ERR_CAP_START_FAIL                         -203
#define QXWZ_SDK_ERR_CAP_GET_CONF_FAIL                      -204
#define QXWZ_SDK_ERR_CAP_NOT_FOUND                          -205
#define QXWZ_SDK_ERR_CAP_NOT_IN_SERVICE                     -206
#define QXWZ_SDK_ERR_CAP_MANUAL_ACT_REQUIRED                -207
#define QXWZ_SDK_ERR_CAP_ACT_ON_TERM_REQUIRED               -208
#define QXWZ_SDK_ERR_CAP_ALREADY_ACTIVATED                  -209
#define QXWZ_SDK_ERR_CAP_CANNOT_ACT_ON_TERM                 -210
#define QXWZ_SDK_ERR_CAP_SYSTEM_ERROR                       -211
#define QXWZ_SDK_ERR_CAP_NOT_INCLUDE                        -212
#define QXWZ_SDK_ERR_CAP_PAUSE                              -213
#define QXWZ_SDK_ERR_CAP_ACT_FAIL                           -214
#define QXWZ_SDK_ERR_GGA_OUT_OF_CONTROL_AREA                -215
#define QXWZ_SDK_ERR_CAP_INACTIVE                           -216
#define QXWZ_SDK_ERR_CAP_EXPIRED                            -217
#define QXWZ_SDK_ERR_CAP_DISABLED                           -218

/*
 * auth related, -301 - -400
 */
#define QXWZ_SDK_ERR_AUTH_FAIL                              -301
#define QXWZ_SDK_ERR_NO_AVAIL_ACC                           -302
#define QXWZ_SDK_ERR_MANUAL_BIND_REQUIRED                   -303
#define QXWZ_SDK_ERR_ACC_BEING_PROCESSED                    -304
#define QXWZ_SDK_ERR_UNMATCH_DID_DSK                        -305
#define QXWZ_SDK_ERR_DSK_NOT_EXIST                          -306
#define QXWZ_SDK_ERR_ACC_NOT_BIND                           -307
#define QXWZ_SDK_ERR_ACC_EXPIRED                            -308
#define QXWZ_SDK_ERR_ACC_NOT_ENOUGH                         -309
#define QXWZ_SDK_ERR_ACC_UNSUPPORT_OP                       -310
#define QXWZ_SDK_ERR_INVAL_KEY                              -311
#define QXWZ_SDK_ERR_ACC_INACTIVE                           -312
#define QXWZ_SDK_ERR_ACC_DUPLICATED                         -313

/*
 * openapi related, -401 - -500
 */
#define QXWZ_SDK_ERR_CALL_API_FAIL                          -401
#define QXWZ_SDK_ERR_INVAL_API_RESP                         -402

/*
 * system related, -501 - -600
 */
#define QXWZ_SDK_ERR_OUT_OF_MEMORY                          -501
#define QXWZ_SDK_ERR_OUT_OF_STORAGE                         -502
#define QXWZ_SDK_ERR_FILE_NOT_FOUND                         -503
#define QXWZ_SDK_ERR_FILE_NO_ACCESS                         -504
#define QXWZ_SDK_ERR_INTERNAL_ERROR                         -505
#define QXWZ_SDK_ERR_SERV_FAULT                             -506


/*
 * ssr decoder related, -601 - -700
 */
#define QXWZ_SDK_ERR_DEC_INIT_FAIL                          -601
#define QXWZ_SDK_ERR_DEC_INVALID_DATA                       -602
#define QXWZ_SDK_ERR_DEC_DECODE_FAIL                        -603
#define QXWZ_SDK_ERR_DEC_CRC_CHECK_FAIL                     -604
#define QXWZ_SDK_ERR_DEC_DECRYPT_FAIL                       -605
#define QXWZ_SDK_ERR_DEC_XOR_CHECK_FAIL                     -606
#define QXWZ_SDK_ERR_DEC_INTERNAL_ERR                       -607
#define QXWZ_SDK_ERR_DEC_UNKNOWN                            -700

/*
 * section charge related, -701 - -800
 */
#define QXWZ_SDK_ERR_NO_STRATEGY_FOUND                      -701
#define QXWZ_SDK_ERR_DSK_CHECK_FAIL                         -702

/*
 * unknown error code
 */
#define QXWZ_SDK_ERR_UNKNOWN                                -999
#if !defined(__WINDOWS__) && (defined(WIN32) || defined(WIN64) || defined(_MSC_VER) || defined(_WIN32))
#define __WINDOWS__
#endif

#ifdef __WINDOWS__

#define _STDCALL __stdcall
#if defined(QXWZ_EXPORT_SYMBOLS)
#define QXWZ_PUBLIC(type)    __declspec(dllexport) type _STDCALL
#else
#define QXWZ_PUBLIC(type)   __declspec(dllimport) type _STDCALL
#endif
#else /* !__WINDOWS__ */
#if defined(__GNUC__)
#define QXWZ_PUBLIC(type) __attribute__((visibility("default"))) type
#else
#define QXWZ_PUBLIC(type) type
#endif
#endif
#define QXWZ_SDK_MAX_KEY_LEN                    128
#define QXWZ_SDK_MAX_SECRET_LEN                 128
#define QXWZ_SDK_MAX_DEV_ID_LEN                 128
#define QXWZ_SDK_MAX_DEV_TYPE_LEN               128
#define QXWZ_SDK_MAX_EXEC_STRATEGY_NUM          8

#define QXWZ_SDK_MAX_CAPS                       8

/*
 * capability identifier
 */
#define QXWZ_SDK_CAP_ID_NOSR                    (1)
#define QXWZ_SDK_CAP_ID_NSSR                    (1 << 1)
#define QXWZ_SDK_CAP_ID_LSSR                    (1 << 2)
#define QXWZ_SDK_CAP_ID_PDR                     (1 << 3)
#define QXWZ_SDK_CAP_ID_VDR                     (1 << 4)
#define QXWZ_SDK_CAP_ID_EPH                     (1 << 5)
#define QXWZ_SDK_CAP_ID_QXSUPL                  (1 << 6)

/*
 * capability state
 */
#define QXWZ_SDK_CAP_STATE_INSERVICE            0
#define QXWZ_SDK_CAP_STATE_INACTIVE             1
#define QXWZ_SDK_CAP_STATE_SUSPENDED            2
#define QXWZ_SDK_CAP_STATE_EXPIRED              3
#define QXWZ_SDK_CAP_STATE_DISABLED             9

/*
 * capability activation method
 */
#define QXWZ_SDK_CAP_ACT_METHOD_AUTO            0
#define QXWZ_SDK_CAP_ACT_METHOD_MANUAL          1
#define QXWZ_SDK_CAP_ACT_METHOD_TERMINAL        2

typedef enum {
    QXWZ_SDK_DATA_TYPE_NONE                 = 0,    /* type guard */
    /* raw nosr data */
    QXWZ_SDK_DATA_TYPE_RAW_NOSR             = 1,



    QXWZ_SDK_DATA_TYPE_CEIL                 = 0x7fffffff,    /* type guard */
} qxwz_sdk_data_type_e;

typedef enum {
    QXWZ_SDK_KEY_TYPE_NONE      = 0,
    QXWZ_SDK_KEY_TYPE_AK        = 1,    /* appkey */
    QXWZ_SDK_KEY_TYPE_DSK       = 2     /* DSK */
} qwxz_sdk_key_type_e;

typedef struct {
    qxwz_uint32_t caps_num;
    struct {
        qxwz_uint32_t cap_id;
        qxwz_uint8_t state;
        qxwz_uint8_t act_method;
        qxwz_uint64_t expire_time;
    } caps[QXWZ_SDK_MAX_CAPS];
} qxwz_sdk_cap_info_t;



/*
 * persistent callbacks
 */
typedef qxwz_void_t (*qxwz_sdk_data_callback_t)(qxwz_sdk_data_type_e type, const qxwz_void_t *data, qxwz_uint32_t len);
typedef qxwz_void_t (*qxwz_sdk_status_callback_t)(qxwz_int32_t status_code);
typedef qxwz_void_t (*qxwz_sdk_auth_callback_t)(qxwz_int32_t status_code, qxwz_sdk_cap_info_t *cap_info);
typedef qxwz_void_t (*qxwz_sdk_act_callback_t)(qxwz_int32_t status_code, qxwz_uint32_t cap_id);
typedef qxwz_void_t (*qxwz_sdk_start_callback_t)(qxwz_int32_t status_code, qxwz_uint32_t cap_id);
typedef qxwz_void_t (*qxwz_sdk_stop_callback_t)(qxwz_int32_t status_code, qxwz_uint32_t cap_id);


/*
 * oneshot callbacks
 */


typedef struct {
    qwxz_sdk_key_type_e key_type;                         /* AK/DSK */
    qxwz_char_t key[QXWZ_SDK_MAX_KEY_LEN];
    qxwz_char_t secret[QXWZ_SDK_MAX_SECRET_LEN];

    qxwz_char_t dev_id[QXWZ_SDK_MAX_DEV_ID_LEN];
    qxwz_char_t dev_type[QXWZ_SDK_MAX_DEV_TYPE_LEN];

    qxwz_sdk_data_callback_t data_cb;
    qxwz_sdk_status_callback_t status_cb;
    qxwz_sdk_auth_callback_t auth_cb;
    qxwz_sdk_start_callback_t start_cb;
} qxwz_sdk_config_t;


/*
 * get SDK version string
 *
 * @return:
 *      pointer of the version string;
 */
QXWZ_PUBLIC(const qxwz_char_t*) qxwz_sdk_version();

/*
 * initialize SDK
 *
 * @return:
 *      0: succeeds;
 *     <0: fails;
 *
 * Notice:
 *      If something wrong happens to the SDK, the error code would be published through
 *      the `status_cb` callback.
 */
QXWZ_PUBLIC(qxwz_int32_t) qxwz_sdk_init(qxwz_sdk_config_t *config);

/*
 * do authenticate
 *
 * @return:
 *      0: succeeds;
 *     <0: fails;
 *
 * Notice:
 *      The result of authentication would be notified through the `auth_cb`,
 *      which is registered in the `qxwz_sdk_init` call.
 */
QXWZ_PUBLIC(qxwz_int32_t) qxwz_sdk_auth();

/*
 * start the service corresponding to the specific capability
 *
 * @param[in]  cap: identifier of the capability;
 *
 * @return:
 *      0: succeeds;
 *     <0: fails;
 *
 * Notice:
 *      The result of start a capability would be notified through the `start_cb`,
 *      which is registered in the `qxwz_sdk_init` call.
 */
QXWZ_PUBLIC(qxwz_int32_t) qxwz_sdk_start(qxwz_uint32_t cap_id);


/*
 * stop the service corresponding to the specific capability
 *
 * @param[in]  cap: identifier of the capability;
 *
 * @return:
 *      0: succeeds;
 *     <0: fails;
 */
QXWZ_PUBLIC(qxwz_int32_t) qxwz_sdk_stop(qxwz_uint32_t cap);

/*
 * cleanup SDK
 *
 * @return:
 *      0: succeeds;
 *     <0: fails;
 */
QXWZ_PUBLIC(qxwz_int32_t) qxwz_sdk_cleanup();

/*
 * upload gga
 *
 * @param[in]  gga: pointer of GGA string;
 * @param[in]  len: length of the string;
 *
 * @return:
 *      0: succeeds;
 *     <0: fails;
 */
QXWZ_PUBLIC(qxwz_int32_t) qxwz_sdk_upload_gga(const qxwz_char_t *gga, qxwz_uint32_t len);


/*
 * get detailed information of the capabilities bound to the account
 *
 * @param[in]  cap_info: structure pointer of capability information;
 *
 * @return:
 *      0: succeeds;
 *     <0: fails;
 */
QXWZ_PUBLIC(qxwz_int32_t) qxwz_sdk_get_cap_info(qxwz_sdk_cap_info_t *cap_info);


#ifdef __cplusplus
}
#endif
#endif
