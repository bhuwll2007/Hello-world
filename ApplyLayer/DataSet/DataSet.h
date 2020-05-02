#ifndef __APPLYLAYER_DATASET_H_
#define __APPLYLAYER_DATASET_H_
#include "../../Algorithm/Type.h"
#include "../../Include/BaseInterface_Include.h"
//------------------------------------------------------------------------------
// Macros & Constants
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Macros & Constants
//------------------------------------------------------------------------------
//匹配加权因子
#define weight_Distance 0.5
#define weight_Heading  0.3
#define weight_Same 	0.2

#define OPT_PRINTF 1

extern tMapInfo g_MapInfo;
extern tMapInfo g_MapInfo_SL;

#define VEHLiveMaxTime  20
#define RSILiveMaxTime  25
#define MAXAROUNDVEHNUM	10
#define MAXAROUNDRSINUM	10

//分析数据
typedef struct _STCSelfVehData
{
    uint8_t UEID[8];			//自己的车辆UEID
    double dSelfLat;			//自己的坐标的纬度 单位：度
    double dSelfLng;			//自己的坐标的经度 单位：度
    uint16_t nSelfSpeed;		//自己的速度	单位：km/h
    uint16_t nLimitHighSpeed;	//当前道路限制的最高速度	无限速时默认200，单位：km/h
    nint16_t nSelfTrack;		//自己的方向	单位：度
    nint16_t nSelfHigh;			//自己的海拔	单位：米
    nint16_t nSpatDistence;		//距离信号机的距离
} __attribute__ ((packed)) STCSelfVehData;

//分析数据
typedef struct _stcSelVeh_V2
{
	uint8_t UEID[8];			//自己的车辆UEID
	nint32_t nSelfLat;			//自己的坐标的纬度 单位：1e-7度
	nint32_t nSelfLng;			//自己的坐标的经度 单位：1e-7度
	uint16_t nSelfSpeed;		//自己的速度	单位：km/h
	uint16_t nLimitHighSpeed;	//当前道路限制的最高速度	无限速时默认200，单位：km/h
	nint16_t nSelfTrack;		//自己的方向	单位：度
	nint16_t nSpatDistence;		//距离信号机的距离		单位：米
}__attribute__ ((packed))stcSelVeh_V2;

typedef struct _stcArdVeh_V2
{
	uint8_t UEID[8];			//对方车辆UEID
	nint16_t IsLive;			//该车存在状态 有车时默认设置为 100（100ms 减小一次）；
								//<5时 显示端删除即可
								//等于0时，设备端复用该位置结构体存储新车
	nint16_t IsSafe;			//该车安全状态 安全时默认设置为 -1；
								//>0时 代表当前车辆的危险状态
	nint32_t nDuiLat;			//对方的坐标的纬度	 单位：1e-7度
	nint32_t nDuiLng;			//对方的坐标的经度	 单位：1e-7度
	uint16_t nDistance;			//本机与对方的车距 单位：米
	uint16_t nDuiSpeed;			//对方速度	单位：km/h
	nint16_t nDuiTrack;			//对方的方向	单位：度
}__attribute__ ((packed)) stcArdVeh_V2;
//分析数据
typedef struct _STCAroundVehData
{
    uint8_t UEID[8];			//对方车辆UEID
    nint16_t IsLive;			//该车存在状态 有车时默认设置为 100（100ms 减小一次）；
    //<5时 显示端删除即可
    //等于0时，设备端复用该位置结构体存储新车
    nint16_t IsSafe;			//该车安全状态 安全时默认设置为 -1；
    //>0时 代表当前车辆的危险状态
    double dDuiLat;				//对方的坐标的纬度	 单位：度
    double dDuiLng;				//对方的坐标的经度	 单位：度
    uint16_t nDistance;			//本机与对方的车距 单位：米
    uint16_t nDuiSpeed;			//对方速度	单位：km/h
    nint16_t nDuiTrack;			//对方的方向	单位：度
    nint16_t nDuiHigh;			//对方海拔	单位：米

    nint32_t nFirstSeqNum;		//起始接收到的包序号
    nint32_t nCurRecvPktId;		//当前接收到的包序号
    uint16_t nRecvPkt_CurTime;	//本轮收包数
    uint32_t nRecvPktNum;		//当前总获取包数
    uint32_t nRecvPktLost;		//当前丢包数

    double dLatencyAvrg;		//延时时间平均值	单位：微秒
    double dRssiAAvrg;			//RSSIA平均值	单位：dbm
    double dRssiBAvrg;			//RSSIB平均值	单位：dbm

} __attribute__ ((packed)) STCAroundVehData;
typedef struct _STCUpD0Data
{
    STCSelfVehData SlfVehData;
    uint16_t nAroundVehNum;
    STCAroundVehData ArdVehData[MAXAROUNDVEHNUM];
} __attribute__ ((packed)) UpD0Data;

//周围RSI消息数据
typedef struct _STCAroundRsiData
{
    nint8_t ID[40];				//RSI消息唯一ID
    nint16_t RsiType;			//RSI消息TYPE
    nint16_t IsLive;			//RSI消息存在状态 有RSI消息时默认设置为 20；
    //<5时 显示端删除即可
    //等于0时，设备端复用该位置结构体存储新RSI消息
    double dDuiLat1;			//RSI消息坐标的纬度	 单位：度
    double dDuiLng1;			//RSI消息坐标的经度	 单位：度
    double dDuiLat2;			//RSI消息坐标的纬度	 单位：度
    double dDuiLng2;			//RSI消息坐标的经度	 单位：度
    uint16_t nDistance;			//本机与RSI消息的车距 单位：米
    uint16_t nSpeakDis;			//显示端播报的距离
    tRsiDscpInfo RsiDscpInfo;	//RSI消息 描述，之前的Description字段
    uint32_t uPriority;			//优先级
    uint32_t WarningType;		//警告类型 原来的事件状态标志位
    uint8_t RelPostion;			//相对方位角
    uint8_t RsiA;				//事件类型(V2I应用类型)
} __attribute__ ((packed)) STCAroundRsiData;
//周围RSI消息数据
typedef struct _STCAroundRsiData_V2
{
    nint16_t RsiType;			//RSI消息TYPE
    nint16_t IsLive;			//RSI消息存在状态 有RSI消息时默认设置为 20；
    //<5时 显示端删除即可
    //等于0时，设备端复用该位置结构体存储新RSI消息
    nint32_t n32RsiLat1;			//RSI消息坐标的纬度	 单位：1e7度
    nint32_t n32RsiLng1;			//RSI消息坐标的经度	 单位：1e7度
    uint16_t nDistance;			//本机与RSI消息的车距 单位：米
    uint16_t nSpeakDis;			//显示端播报的距离
    uint16_t TagVal;			//事件值
    nint8_t uPriority;			//优先级
    nint8_t WarningType;		//警告类型 原来的事件状态标志位
    uint8_t RelPostion;			//相对方位角
    uint8_t RsiAppType;			//事件类型(V2I应用类型)
} __attribute__ ((packed)) STCAroundRsiData_V2;
typedef struct _STCUpD9Data
{
    uint16_t nAroundRsiNum;
    STCAroundRsiData ArdRsiData[MAXAROUNDRSINUM];
} __attribute__ ((packed)) UpD9Data;

typedef struct _ReadHisDataBase
{
    uint32_t u32SaveTxBsmEnable;
    uint32_t u32HisDataBaseIndex_BSM;
    uint32_t u32HisDataBaseIndex_RSI;
    uint32_t u32HisDataBaseIndex_RSM;
    uint32_t u32HisDataBaseIndex_MAP;
    uint32_t u32HisDataBaseIndex_SPAT;
    uint32_t u32HisDataBaseIndex_APP_ALL;
    uint32_t u32HisDataBaseIndex_RX_ALL;
} ReadHisDataBase;

extern tMapInfo g_MapInfo;

extern UpD0Data g_UpD0Data;
extern UpD9Data g_UpD9Data;
extern uint16_t g_u16LimitHighSpeed;
extern sqlite3 * sqldb;
extern ReadHisDataBase g_ReadHisDataBase;

extern double g_dSpatLat;
extern double g_dSpatLong;

extern tVehData AroundHV;

extern pthread_mutex_t mutex_AroudVeh;
extern pthread_mutex_t mutex_AroudRsi;

typedef struct _APP_RunParam
{
    int nSwitch_UpD1;
    int nSwitch_UpD0;
    int nSwitch_UpD9;
    int nInterval_UpD1;
    int nInterval_UpD0;
    int nInterval_UpD9;
} tAPP_RunParam;
extern tAPP_RunParam g_tAPP_RunParam;

typedef struct _Platform_LinkState
{
	int Socket;
	int isConnect;
	int isRegist;
	int isD0Send;
	int isD1Send;
	int isD3Send;
	int isD6Send;
	int isReUpLoad;

}tPlatform_LinkState;

typedef struct _Platform
{
	char ServerIp[50];
	int ServerPort;
	char VIN[17];
	char DevId[20];
	char vehliscence[20];
}tPlatform;

extern tPlatform_LinkState g_tTcpState;
extern tPlatform g_tPlatform;
extern char g_LogFileName[30];
extern int g_nRelogInterval;
//配置文件路口ＩＤ
extern int g_nIntersectionId;

#define MODE_PROTOCOL2PAD_V1 0
#define MODE_PROTOCOL2PAD_V2 1
#define MODE_PROTOCOL2PAD_V3 1

typedef struct _YuTong_SpatInfo
{
    int BusFirst_Status;		//公交优先-状态 0 没执行，>0 执行时长
    int BusFirst_BeginDis;		//公交优先-开始的距离
    int BusFirst_Duration;		//公交优先-持续时间
    int RegionID;				//区域号
    int IntersectionID;			//路口号
    char SpatSelId[20];			//信号灯设备序列号
    char RsmSelId[20];			//RSM设备序列号
    char ObuId[20];				//OBUID
    int Obu_Distance;			//OBU距离
    int Obu_Speed;				//OBU速度
    int Obu_Heading;			//OBU车头方向
    int Obu_IsNearRSU;			//OBU是否向RSU靠近 0 否 1 是
    int BusFirst_ParseID;		//优先相位ID  //-1代表无优先相位
} tYuTong_SpatInfo;

typedef struct _YuTong_RsuBaseInfo
{
    int Is_Send2Cloud; 		//发送给云端标志  1 发送 0 不发送
    char RsuSelId[20];		//设备序列号
    char RsuName[20];		//RSU名称
    char RsuIPAddr[20];		//IP地址
    char ProjectId[20];		//项目代号
    char CloudIpAdd[20];	//云端IP地址
    int  CloudPort;			//云端端口号
} tYuTong_RsuBaseInfo;

extern tYuTong_SpatInfo g_tYuTong_SpatInfo;
extern tYuTong_RsuBaseInfo g_tYuTong_RsuBaseInfo;
extern tSPAT_INFO g_stcSpatInfo;



#endif /*__APPLYLAYER_DATASET_H_*/
