/**
 * @file Modem.h
 * @author catt
 * @version 1.0
 * @date 2018-6-4
 * @brief Modem Interface Wrapper.
 * @details Requires the following definitions.
 */

#ifndef _MODEM_H
#define _MODEM_H

#ifdef __cplusplus
extern "C" {
#endif

/// SubService number for MODEM.
#define SSID_MODEM      40

/// SubService name for MODEM.
#define SSNAME_MODEM    "Modem"

/**
 * Options for Modem to get or set.
 */
enum MODEM_OPTION
{
    MODEM_OPTION_BASE = 0,
    MODEM_TEST_MODE_INST,       /**< start or stop test mode */
    MODEM_OPTION_TEST_MODE_INSTRUCTION = MODEM_TEST_MODE_INST, /**< alias */
    MODEM_OPTION_SIMPLE_CONF,
    MODEM_OPTION_RETX_SWITCH,
    MODEM_OPTION_PERFM_TEST,
};

/**
 * MSGID for Modem service.
 */
enum MODEM_MSGID
{
    MODEM_MSGID_SAMPLE_STATUS = 0,  /**< Modem Status */
    MODEM_MSGID_DETAIL_STATUS,
    MODEM_MSGID_RF_TEMPERATURE,     /**< Modem temperature */
    MODEM_MSGID_HOST_UEID,
    MODEM_MSGID_PHY_RX_PARAM,
    MODEM_MSGID_CP_PERFM,
    MODEM_MSGID_RXCNT_TRACE,
};

/**
 * Possible Errors for MODEM service. Only for xds internal, not user errno!
 */
enum MODEM_ERROR_INTERNAL
{
    MODEM_ERROR_BASE = E_RSPS2C_SERVICE_BASE,
    MODEM_ERROR_MSGID_ILLEGAL,
    MODEM_ERROR_MSGDATA_ILLEGAL,
    MODEM_ERROR_OPTIONCMD_ILLEGAL,
    MODEM_ERROR_OPTIONDATA_ILLEGAL,
    MODEM_ERROR_OPTIONCMD_SET,
    MODEM_ERROR_LOGICAL_ILLEGAL,
    MODEM_ERROR_TRY_AGAIN_LATER,
};

/**
 * Message content for MODEM_MSGID_SAMPLE_STATUS. Status Type.
 */
enum MODEM_STATUS
{
    MODEM_STATUS_ABNORMAL = 0,
    MODEM_STATUS_DEACTIVE,
    MODEM_STATUS_ACTIVE,
};

/**
 * Message content for MODEM_MSGID_SAMPLE_STATUS. Data Type Definition.
 */
typedef struct modem_status_s
{
    uint32_t value;
}modem_status_t;

/**
 * Message content for MODEM_MSGID_HOST_UEID. Data Type Definition.
 */
typedef uint32_t modem_ueid_t;

/**
 * Message content for MODEM_MSGID_RF_TEMPERATURE. Status Type.
 */
typedef struct modem_rf_tempt_s
{
    int32_t value;
}modem_rf_tempt_t;
typedef int32_t modem_temperature_t;    /**< alias for modem_rf_tempt_t */

typedef struct modem_detail_status_s
{
    int32_t hl_status;
    int32_t phy_status;
}modem_detail_status_t;

/**
 * Message content for MODEM_OPTION_SIMPLE_CONF.
 */
typedef struct modem_simple_conf_s
{
    int8_t power;
    int8_t pad[3];                      /** < Maybe expand... */
}modem_simple_conf_t;

#define LTEV_MAX_UE_NUM 10
typedef struct rx_pssch_info_tag
{
    uint16_t u16_mcs;
    uint16_t u16_start_prb;

    uint16_t u16_prb_num;
    uint16_t u16_retrans;

    uint16_t u16_crc;
    int16_t  s16_rsrp;

    int16_t s16_np;
    int16_t s16_snr;

    int16_t s16_ta;
    int16_t s16_reserved;

    uint32_t u32_src_id;
    uint32_t u32_dst_id;
}rx_pssch_info_t;

typedef struct modem_rx_param_trace_s
{
    uint16_t u16_sfn;       /**< Frame Number  */
    uint16_t u16_ue_num;    /**< UE Number */
    int16_t  s16_rssi[20];
    rx_pssch_info_t rx_pssch_info[LTEV_MAX_UE_NUM];
}modem_rx_param_trace_t;

typedef struct modem_test_inst_s
{
    uint16_t u16_start;
    uint16_t u16_mode;
    uint16_t u16_prb_num;
    uint16_t u16_mcs;
}modem_test_inst_t;

enum modem_retx_flag
{
    RETX_OFF,
    RETX_ON,
};

typedef struct modem_rx_cnt_per_app_s
{
    uint32_t aid;
    uint32_t pssch_num;
    uint32_t pdcp_num;
}modem_rx_cnt_per_app_t;

typedef struct modem_rx_cnt_trace_s
{
    uint32_t pscch_num;
    uint32_t app_num;   /* number of the following structures */
    modem_rx_cnt_per_app_t per_app[0];
}modem_rx_cnt_trace_t;

#ifdef __cplusplus
}
#endif

#endif
