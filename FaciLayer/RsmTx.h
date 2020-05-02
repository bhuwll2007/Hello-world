/***************************************************************//**
 * @defgroup RsmTx RsmTx API
 * @ingroup FaciLayer
 * @{
 *
 *  This module provides APIs to do the operate about RsmMessage.
 *
 * @file RsmTx.h
 * @brief API file.
 *
 *
 *******************************************************************/

#ifndef RSMTX_H_
#define RSMTX_H_

//------------------------------------------------------------------------------
// Included headers
//------------------------------------------------------------------------------
#include "../MsgFrame/MessageFrame.h"
#include "../BaseInterface/V2XUtil/V2X_INTERFACE.h"
#include "../BaseInterface/ConfUtil/Config.h"

#include "ExtFunction.h"
#include "DataSet.h"
#include "../Algorithm/MercatorProj.h"

#define is_err -1
#define is_ok 0
#define HL_PRESENT 0xA5
#define HL_ABSENT 0xCC
//------------------------------------------------------------------------------
// Function Prototypes
//------------------------------------------------------------------------------
typedef struct HL_OCTET_STRING
{
    char    HL_buf[10];
    int HL_size;    /* Size of the buffer */
} HL_OCTET_STRING_t;
typedef struct HL_Position3D
{
    int      HL_lat;
    int     HL_Long;
    int HL_elevation_present;
    int    HL_elevation    /* OPTIONAL -4096 61439*/;
}HL_Position3D_t;
typedef struct HL_Position_LL_24B
{
    int    HL_lon;
    int    HL_lat;
} HL_Position_LL_24B_t;
typedef struct HL_Position_LL_28B
{
    int    HL_lon;
    int    HL_lat;
} HL_Position_LL_28B_t;
typedef struct HL_Position_LL_32B
{
    int    HL_lon;
    int    HL_lat;
} HL_Position_LL_32B_t;
typedef struct HL_Position_LL_36B
{
    int    HL_lon;
    int    HL_lat;
} HL_Position_LL_36B_t;
typedef struct HL_Position_LL_44B
{
    int     HL_lon;
    int     HL_lat;
} HL_Position_LL_44B_t;
typedef struct HL_Position_LL_48B
{
    int     HL_lon;
    int     HL_lat;
} HL_Position_LL_48B_t;
typedef struct HL_Position_LLmD_64b
{
    int     HL_lon;
    int     HL_lat;
} HL_Position_LLmD_64b_t;
typedef enum HL_PositionOffsetLL_PR
{
    HL_PositionOffsetLL_PR_NOTHING,    /* No components present */
    HL_PositionOffsetLL_PR_position_LL1,
    HL_PositionOffsetLL_PR_position_LL2,
    HL_PositionOffsetLL_PR_position_LL3,
    HL_PositionOffsetLL_PR_position_LL4,
    HL_PositionOffsetLL_PR_position_LL5,
    HL_PositionOffsetLL_PR_position_LL6,
    HL_PositionOffsetLL_PR_position_LatLon
} HL_PositionOffsetLL_PR;
typedef struct HL_PositionOffsetLL
{
    HL_PositionOffsetLL_PR HL_enumpresent;
    union HL_PositionOffsetLL_u {
        HL_Position_LL_24B_t     HL_position_LL1;
        HL_Position_LL_28B_t     HL_position_LL2;
        HL_Position_LL_32B_t     HL_position_LL3;
        HL_Position_LL_36B_t     HL_position_LL4;
        HL_Position_LL_44B_t     HL_position_LL5;
        HL_Position_LL_48B_t     HL_position_LL6;
        HL_Position_LLmD_64b_t     HL_position_LatLon;
    } choice;
} HL_PositionOffsetLL_t;
typedef enum HL_VerticalOffset_PR
{
    HL_VerticalOffset_PR_NOTHING,    /* No components present */
    HL_VerticalOffset_PR_offset1,
    HL_VerticalOffset_PR_offset2,
    HL_VerticalOffset_PR_offset3,
    HL_VerticalOffset_PR_offset4,
    HL_VerticalOffset_PR_offset5,
    HL_VerticalOffset_PR_offset6,
    HL_VerticalOffset_PR_elevation
} HL_VerticalOffset_PR;
typedef struct HL_VerticalOffset
{
    HL_VerticalOffset_PR HL_enumVerticalOffset_PRpresent;
    union HL_VerticalOffset_u
    {
        int     HL_offset1;
        int     HL_offset2;
        int     HL_offset3;
        int     HL_offset4;
        int     HL_offset5;
        int     HL_offset6;
        int     HL_elevation;
    } choice;
} HL_VerticalOffset_t;
typedef struct HL_PositionOffsetLLV
{
    HL_PositionOffsetLL_t HL_offsetLL;
    int HL_offsetVpresent;
    HL_VerticalOffset_t HL_offsetV;
} HL_PositionOffsetLLV_t;
typedef struct HL_PositionConfidenceSet
{
    int HL_pos;
    int HL_elevationPresent;
    int HL_elevation;
}HL_PositionConfidenceSet_t;
typedef struct HL_MotionConfidenceSet
{
    int HL_speedCfdPresent;
    int  HL_speedCfd;
    int HL_headingCfdPresent;
    int HL_headingCfd;
    int HL_steerCfdPresent;
    int HL_steerCfd;
} HL_MotionConfidenceSet_t;
typedef struct HL_AccelerationSet4Way
{
    int HL_Long;
    int HL_lat;
    int HL_vert;
    int HL_yaw;
} HL_AccelerationSet4Way_t;
typedef struct HL_VehicleSize
{
    int HL_width;
    int HL_length;
    int HL_heightPresent;
    int HL_height;
}HL_VehicleSize_t;
typedef struct HL_VehicleClassification
{
    int HL_classification;
} HL_VehicleClassification_t;
typedef struct HL_ParticipantData
{
    int HL_ptcType;
    int HL_ptcId;
    int HL_source;
    int HL_idPresent;
    HL_OCTET_STRING_t HL_id;
    int HL_plateNoPresent;
    HL_OCTET_STRING_t HL_plateNo;
    int HL_secMark;
    HL_PositionOffsetLLV_t HL_pos;
    HL_PositionConfidenceSet_t HL_accuracy;
    int HL_transmissionPresent;
    int HL_transmission;
    int HL_speed;
    int HL_heading;
    int HL_anglePresent;
    int  HL_angle	/* OPTIONAL */;
    int HL_motionCfdPresent;
    HL_MotionConfidenceSet_t HL_motionCfd	/* OPTIONAL */;
    int HL_accelSetPresent;
    HL_AccelerationSet4Way_t HL_accelSet	/* OPTIONAL */;
    HL_VehicleSize_t    HL_size;
    int HL_vehicleClassPresent;
	HL_VehicleClassification_t    HL_vehicleClass	/* OPTIONAL */;
}HL_ParticipantData_t;
typedef struct HL_ParticipantList
{
    int HL_participantsNum;
    HL_ParticipantData_t HL_participantsList[16];
}HL_ParticipantList_t;
typedef struct HL_RoadsideSafetyMessage
{
    int HL_msgCnt;
    HL_OCTET_STRING_t  HL_id;
    HL_Position3D_t    HL_refPos;
    HL_ParticipantList_t   HL_participants;
}HL_RoadsideSafetyMessage_t;

typedef struct HL_Time
{
	int Year;
	int Month;
	int Day;
	int Hour;
	int Min;
	int Second;
}HL_Time_t;
/**
 * @brief 分配RSM消息对象，配置线程、关联RSM数据结构.
 * @param[in] ppRsm    指向RSM句柄的指针.
 * @param[in] pAttr    线程属性.
 * @return  0 执行成功， 非0 失败标识.
 */
extern int FG_RsmTxOpen(struct RsmTx **p_pprectRsm, pthread_attr_t *pAttr);

/**
 * @brief 停止执行RSMTx，释放线程及相关资源.
 * @param[in] pRsm  RSM句柄.
 */
extern void FG_RsmTxclose(struct RsmTx *p_prectRsm);

/**
 * @brief 周期执行RSM发送线程.
 * @param[in] pArg  线程调用传入参数指针.
 */
extern void FG_RsmTxThreadProc(void *p_pvArg);

/**
 * @brief 发送一条RSM消息.
 * @param[in] pRsm  RSM句柄.
 * @return  0 执行成功， 非0 失败标识.
 */

extern int FG_RsmTxSendMsg(struct RsmTx *p_prectRsm);
/**
 * @brief 创建一条RSM消息.
 *
 * @param[in] RsmInfo  RSM信息. @see tRsmMsg
 *
 * @return  0 执行成功， 非0 执行失败.
 */
extern int FG_RsmTxCreateMsg(tRsmMsg p_rectRsmInfo);
///**
// * @brief 打印收到的数据.
// * @param[in] p_cRecvBuf        接收数据的指针.
// * @param[in] pnRecvBufSize     接收数据的长度.
// */
//void RsmTx_CallBack(char *p_cRecvBuf, uint32_t pnRecvBufSize);

/**
 * @brief 读取设备最后发送的那一条Rsm消息.
 * @param[out] pRsmMsg Rsm句柄.
 * @return 0 读取失败，3 读取成功，本消息确实为Rsm信息. 其他 ，不合法
 */
extern int FG_RsmTxReadlstTxMsg(MessageFrame_t *p_prectRsmMsg);

#endif /* RSMTX_H_ */

/**
 * @}
 */
