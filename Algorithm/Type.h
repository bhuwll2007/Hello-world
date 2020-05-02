/**
 * @ingroup Algorithm API
 * @{
 *
 * 本模块提供V2X算法接口层的基本公用函数的API接口.
 *
 * @file Type.h
 * @brief API file.
 *
 */
#ifndef __ALGORITHM_TYPE_H_
#define __ALGORITHM_TYPE_H_
#include "Appconfigure.h"
//------------------------------------------------------------------------------
// Type definitions
//------------------------------------------------------------------------------
//匹配加权因子
#define weight_Distance 0.5
#define weight_Heading  0.3
#define weight_Same 	0.2

/********************* 主动安全（BSM）应用 相关结构体声明 ********* BEGIN *************/
//历史轨迹点
typedef struct _PointHistory
{
	double Lng; /**< 经度 度. */
	double Lat; /**< 纬度 度. */
	double Heading; /**< 航向角 度. */
	double Speed; /**< 速度 米每秒. */
}__attribute__ ((packed)) tPointHistory;

//车辆数据信息

typedef struct _VehData
{
	uint8_t Id[8]; /**< 车辆UEID. */
	double Longitude; /**< 经度 度. */
	double Latitude; /**< 纬度 度. */
	double Altitude; /**< 海拔 米. */
	double Speed; /**< 速度 米每秒. */
	double Heading;/**< 航向角 度. */

	double SteeringWheelAngle; /**< 方向盘转角 度. */
	uint32_t BreakPadel; /**< 刹车踏板状态 1 未踩下 2 踩下. */
	uint32_t AuxBreakSystem; /**< 辅助制动系统，详见国标. */
	uint16_t Light; /**< 转向灯状态 详见定义. */

	double VehicleWidth; /**< 车宽. 单位 米*/
	double VehicleLength; /**< 车长. 单位 米*/
	double VehicleHight; /**< 车高. 单位 米*/
	double VehicleWheelDis; /**< 车轮轴距. 单位 米*/
	uint16_t VehicleClass; /**< 车辆类型. */

	nint32_t acc_Long;   //纵向加速度
	nint32_t acc_lat;	//横向加速度
	nint32_t acc_vert;	//垂直加速度
	nint32_t acc_yaw;	//横摆角加速度
	uint32_t transmission;//档位


	uint32_t ForwardNode; /**< 前方节点ＩＤ. */
	uint32_t BackNode; /**< 后方节点ＩＤ. */
	uint32_t LaneID;

	tPointHistory pointHistory; /**< 历史轨迹点. */
}__attribute__ ((packed)) tVehData;

//主动安全应用处理结果
typedef struct _Result_Bsm_V2
{
	uint8_t Id[8]; /**< 车辆UEID. */
	nint32_t Latitude; /**< 纬度 1e-7度. */
	nint32_t Longitude; /**< 经度 1e-7度. */
	nint8_t wStr; /**< 警告类型. */
	nint8_t wPrio; /**< 警告优先级. */

	uint16_t Distance; /**< 车距. */
	nint8_t Location; /**< 相对方位. */
}__attribute__ ((packed)) tResult_Bsm_V2;

//主动安全应用处理结果
typedef struct _Result_Bsm
{
	APP_TYPE wType; /**< 应用类型. */
	WARN_CASE wStr; /**< 警告类型. */
	WARN_PRIO wPrio; /**< 警告优先级. */
	WARN_LVL wLvl; /**< 警告等级. */

	double TTC; /**< 碰撞时间. */
	double Distance; /**< 车距. */
	double RelBearing; /**< 相对方向角. */
	nint32_t Location; /**< 相对方位. */
}__attribute__ ((packed)) tResult_Bsm;
/********************* 主动安全（BSM）应用 相关结构体声明   *********  END  *************/

/********************* 路侧消息（RSI）应用 相关结构体声明   ********* BEGIN *************/
typedef struct _RSIPos3D
{
	// xx.xx
	double Latitude; /**< 纬度值. */
	double Longitude; /**< 经度值. */
	double Altitude; /**< 海拔值. */

}__attribute__ ((packed)) RSIPos3D;

typedef struct _RSIPathPt
{
	RSIPos3D AlertPath; /**< 轨迹点的位置结构体（经度、纬度、海拔）. */
	double AlertPosDir; /**< 图标基于路网的角度. */

}__attribute__ ((packed)) RSIPathPt;

typedef struct _SpeedLimitInfo
{
	uint8_t IsActive; /**< 本限速状态是否有效. */
	double Speed; /**< 本限速状态的限速值. */

}__attribute__ ((packed)) SpeedLimitInfo;

typedef enum WARN_TYPE
{
	NoWarning = 0,
	PreWarning = (1 << 0),
	InsideWarning = (1 << 1),
	UponWarning = (1 << 2)
}__attribute__ ((packed)) WARN_TYPE;

typedef struct _RsiWarnInfo
{
	double TagVal;
	int AssisTag;
	double AssisVal;

}__attribute__ ((packed)) tRsiDscpInfo;

//*****************处理结果结构体***************//
typedef struct _Result_Rsi
{
	APP_TYPE wType; /**< 应用类型. */
	V2I_WARN_CASE wStr; /**< V2I应用场景. */
	WARN_PRIO wPrio; /**< 优先级. */
	uint8_t WarningType; /**< 警告类型. */

	///消息类型
	uint32_t MsgType;
	///参考位置（经纬海拔）
	RSIPos3D AlertPos;
	///事件提醒相关消息轨迹
	RSIPathPt AlertPathList[2];  // 由2个点组成

	///当前车与提醒点间的距离
	double dis;
	///显示端播报的距离
	int SpeakDis;
	///当前车与提醒点间的相对方位
	uint8_t RelPos;
	///轨迹长度
	double PathLen;
	///从Rsi的Description中提取出的事件相关信息
	tRsiDscpInfo DscpInfo;

}__attribute__ ((packed)) tResult_Rsi;

typedef struct _Result_SL
{
	APP_TYPE wType; /**< 应用类型. */
	V2I_WARN_CASE wStr; /**< V2I应用场景. */
	WARN_PRIO wPrio; /**< 优先级. */
	uint8_t WarningType; /**< 警告类型. */

	///事件提醒相关消息轨迹
	RSIPathPt AlertPathList[2];  // 由2个点组成
	///当前车辆限速最高值
	uint32_t VehSpeedLimitHigh;
	///当前车辆限速最低值
	uint32_t VehSpeedLimitLow;

}__attribute__ ((packed)) tResult_SL;
//*****************算法输入结构体***************//

typedef struct _RsiMsg
{
	///------------------------------
	/// 从Rsi中提取的有效信息
	///RSU设备ID
	uint32_t RSUID;
	///消息ID
	long RSIMsgID;
	///消息类型
	long MsgType;
	///优先级
	uint8_t Priority;
	///消息描述
	char Description[128];
	///消息半径(m)
	double AlertRadius;
	///参考位置
	RSIPos3D AlertPos;
	///消息轨迹点个数
	uint8_t AlertPathNbr;
	///消息点位置
	RSIPathPt AlertPathList[8];

}__attribute__ ((packed)) tRsiMsg;

typedef struct Al_RSITimeDetails {
	int startTime;//生效时刻 单位 分钟
	int	endTime;  //结束时刻 单位 分钟
}Al_RSITimeDetails_t;
typedef struct Al_RTEData {
	///消息ID
	long RTEID;
	///消息类型
	long MsgType;
	///参考位置
	RSIPos3D AlertPos;
	///消息描述
	char Description[512];
	///生效时间属性。
	Al_RSITimeDetails_t mRSITimeDetails;
	///优先级
	uint8_t Priority;
	///消息半径(m)
	double AlertRadius;
	///消息轨迹点个数
	uint8_t AlertPathNbr;
	///消息点位置
	RSIPathPt* AlertPathList;
} Al_RTEData_t;
/* RTSData */
typedef struct Al_RTSData {
	///消息ID
	long RTSID;
	///消息类型
	long MsgType;
	///参考位置
	RSIPos3D AlertPos;
	///消息描述
	char Description[512];
	///生效时间属性。
	Al_RSITimeDetails_t mRSITimeDetails;
	///优先级
	uint8_t Priority;
	///消息半径(m)
	double AlertRadius;
	///消息轨迹点个数
	uint8_t AlertPathNbr;
	///消息点位置
	RSIPathPt* AlertPathList;
} Al_RTSData_t;

typedef struct _RsiMsg_Arry
{
	///RSU设备ID
	uint32_t RSUID;

	///参考位置
	RSIPos3D AlertPos;

	///RTE消息个数
	uint8_t Al_RTEDataNum;
	Al_RTEData_t *Al_RTEData;

	///RTS消息个数
	uint8_t Al_RTSDataNum;
	Al_RTSData_t *Al_RTSData;

}__attribute__ ((packed)) tRsiMsg_Arry;

/********************* 路侧消息（RSI）应用 相关结构体声明   *********  END  *************/

/********************* 红绿灯消息（SPAT）应用 相关结构体声明 ********* BEGIN *************/
#define PHASEINFO_MAX_NUM	5

//*****************前方路口信号灯信息结构体***************//
typedef struct _algPhaseInfo
{
	nint32_t ID; /**< 相位ID. */
	nint32_t Direction; /**< 行车方向. */
	nint32_t GreenTime; /**< 绿灯时间. */
	nint32_t YellowTime; /**< 黄灯时间. */
	nint32_t RedTime; /**< 红灯时间. */
	nint32_t Status; /**< 灯色状态. */
	nint32_t TimeLeft; /**< 剩余时间. */
}__attribute__ ((packed)) talgPhaseInfo;

typedef struct _SpeedGudie_Input
{
	int IntersectionID;/**路口ID*/
	double Lng; /**< 信号灯经度. */
	double Lat; /**< 信号灯纬度. */
	nint32_t PhaseCnt; /**< 前方相位个数. */
	talgPhaseInfo pLightInfo[PHASEINFO_MAX_NUM]; /**< 包含相位信息结构体. */
}__attribute__ ((packed)) tSpeedGudie_Input;

//*****************处理结果结构体***************//
typedef struct _UpPhaseInfo
{
	nint32_t Status; /**< 灯色状态. */
	nint32_t TimeLeft; /**< 剩余时间. */
	nint32_t Direction; /**< 行车方向. */
	double SuggestSpeed_High; /**< 最高建议时速. */
	double SuggestSpeed_Low; /**< 最低建议时速. */
	double SuggestSpeed; /**< 建议时速. */
}__attribute__ ((packed)) UpPhaseInfo;

typedef struct _Result_Spat
{
	APP_TYPE wType; /**< 应用类型. */
	V2I_WARN_CASE wStr; /**< 应用场景. */
	WARN_PRIO wPrio; /**< 优先级. */
	uint32_t Cycle; /**< 信号灯周期. */
	nint32_t Distance; /**< 到红绿灯距离. */
	uint32_t RelBearing; /**< 红绿灯方位角. */
	nint32_t PhaseCnt; /**< 前方相位个数. */
	UpPhaseInfo tUpPhaseInfo[PHASEINFO_MAX_NUM]; /**< 包含相位信息结构体. */
}__attribute__ ((packed)) tResult_Spat;
/********************* 红绿灯消息（SPAT）应用 相关结构体声明 *********  END  *************/

/********************* 地图消息（MAP）应用 相关结构体声明 ********* BEGIN *************/
#define CONFIDENCE_DIS_TOLINK	10
#define CONFIDENCE_DIS_TOLANE	2
#define COFFIC_DIS	0.5
#define COFFIC_ANGLE	0.5

#define NODE_MAX_NUM	20
#define LINK_MAX_NUM	10
#define SPEEDLIMIT_MAX_NUM 10
#define MOVEMENT_MAX_NUM 10
#define LANE_MAX_NUM	5
#define ROADPOINT_MAX_NUM	31

//路段中间点信息
typedef struct _RoadPoint
{
	// xx.xx
	double Latitude; /**< 纬度值. */
	double Longitude; /**< 经度值. */
	double Altitude; /**< 海拔值. */
}__attribute__ ((packed)) tRoadPoint;

typedef struct _LaneInfo
{
	nint32_t ID;
	double LaneWidth;
	uint32_t roadPointCnt;
	tRoadPoint roadPointInfo[ROADPOINT_MAX_NUM];
}__attribute__ ((packed)) tLaneInfo;
//限速信息
typedef struct _MapSpeedLimitInfo
{
	nint32_t SpeedLimitType;
	double SpeedLimitValue;
}__attribute__ ((packed)) tMapSpeedLimitInfo;

//转向关系节点信息
typedef struct _MoveMent
{
	nint32_t ID;
	double Lat;
	double Lng;
	nint32_t *Phase;
}__attribute__ ((packed)) tMoveMent;
//上游节点信息(路段信息)
typedef struct _UpPointInfo
{
	nint32_t ID;
	double LaneWidth;
	tRoadPoint nodePos;
	nint32_t RoadPointsCnt;
	tRoadPoint RoadPointInfo[ROADPOINT_MAX_NUM];
	uint32_t LaneCnt;
	tLaneInfo  laneInfo[LANE_MAX_NUM];
	nint32_t MoveMentCnt;
	tMoveMent pMoveMents[MOVEMENT_MAX_NUM];
	nint32_t SpeedCnt;
	tMapSpeedLimitInfo MapSpeedLimits[SPEEDLIMIT_MAX_NUM];
}__attribute__ ((packed)) tUpPointInfo;

//节点信息结构体
typedef struct _NodeInfo
{
	nint32_t ID;
	tRoadPoint nodePos;
	nint32_t UpNodeCnt; /**< 上游节点个数. */
	tUpPointInfo pUpNode[LINK_MAX_NUM]; /**< 多个上游节点. */
}__attribute__ ((packed)) tNodeInfo;

//地图信息结构体
typedef struct _MapInfo
{
	nint32_t NodesCnt;
	tNodeInfo pNodeInfo[NODE_MAX_NUM];
}__attribute__ ((packed)) tMapInfo;
/********************* 地图消息（MAP）应用 相关结构体声明 *********  END  *************/

#endif /* __ALGORITHM_TYPE_H_ */

/**
 * @}
 */
