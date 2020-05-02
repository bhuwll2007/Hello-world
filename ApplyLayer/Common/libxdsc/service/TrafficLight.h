/*
 * trafficlight.h
 *
 *  Created on: 2018年1月25日
 *      Author: catt
 */

#ifndef SERVICE_BUILTIN_TRAFFICLIGHT_H_
#define SERVICE_BUILTIN_TRAFFICLIGHT_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "protocol/Protocol.h"

#define SSID_TRAFFIC_LIGHT       20
#define SSNAME_TRAFFIC_LIGHT     "TrafficLight"

enum TrafficLightError
{
    TL_ERROR_MSGID_ILLEGAL = E_RSPS2C_SERVICE_BASE,
};

enum TrafficLightMsgID
{
    TL_MSGID_DEFAULT = 1,
};

enum LIGHT_STATUS
{
    LIGHT_STATUS_GREEN = 1,
    LIGHT_STATUS_RED,
    LIGHT_STATUS_YELLOW,
};

#define TRAFFIC_LIGHT_MAX_PHASE_NUM    32
#define TRAFFIC_LIGHT_PRE_PHASE_NUM    3

typedef struct timing_detail_s
{
    uint16_t m_LightState;

    uint16_t m_StartTime;
    uint16_t m_MinEndTime;
    uint16_t m_MaxEndTime;
    uint16_t m_LikelyEndTime;
    uint16_t m_TimeCfd;
    uint16_t m_NextStartTime;
    uint16_t m_NextDuration;
}timing_detail_t;


typedef struct traffic_light_phase_s
{
    uint16_t m_PhaseId;
    uint16_t m_Status;

    timing_detail_t m_Timing[TRAFFIC_LIGHT_PRE_PHASE_NUM];
} traffic_light_phase_t;


typedef struct traffic_light_data_s
{
    int32_t m_RegionId;
    int32_t m_LocalId;

    uint32_t m_PhaseNum;

    traffic_light_phase_t m_Phases[TRAFFIC_LIGHT_MAX_PHASE_NUM];

}traffic_light_data_t;


#ifdef __cplusplus
}
#endif

#endif /* SERVICE_BUILTIN_TRAFFICLIGHT_H_ */
