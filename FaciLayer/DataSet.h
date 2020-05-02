/*
 * DataSet.h
 *
 *  Created on: Jun 7, 2018
 *      Author: root
 */

#ifndef __FACILAYER_DATASET_H_
#define __FACILAYER_DATASET_H_
#include "../BaseInterface/DataSet.h"
#include "../BaseInterface/V2XUtil/V2X_INTERFACE.h"
#include "../BaseInterface/GPSUtil/GPS.h"
#include "../MsgFrame/MessageFrame.h"
#include "../SecLayer/SecLayer_Deal.h"

//------------------------------------------------------------------------------
// Macros & Constants
//------------------------------------------------------------------------------
#define DCR_OPTDUPBSM 0
#define DCR_OPTDUPRSI 0
#define DCR_OPTDUPMAP 0
#define DCR_OPTDUPSPAT 0
#define DCR_OPTDUPRSM 0

//------------------------------------------------------------------------------
// Macros & Constants
//------------------------------------------------------------------------------
/// DSM Init request accept -> 0  else 1
#define DCR_OPTDSMINIT 0

/// DSM Send request accept -> 0  else 1
#define DCR_OPTDSMSEND 0

/// DSM Terminate request accept -> 0  else 1
#define DCR_OPTDSMCLOSE 0

#define FACILAYER_PRINTLOG 0

//------------------------------------------------------------------------------
// Macros & Constants
//------------------------------------------------------------------------------

/// Alloc structure size with calloc
#define ALLOC_OBJ(type, size) \
  (type *)calloc(1, (sizeof(type)*size))


//------------------------------------------------------------------------------
// Type definitions
//------------------------------------------------------------------------------
/// Error codes

//-----------------------------------------------------------------------------
// Type Definitions
//-----------------------------------------------------------------------------
#define SHARE_MAXNUM 100

typedef enum
{
	Tcp_ERR_NONE = 0, Tcp_ERR_Socket = 1, Tcp_ERR_connect = 2,
} Tcp_ErrorCode;

/*------------------------------------------------------------------
 Facility Layer information
 ------------------------------------------------------------------*/
/* Uint Type */
typedef enum HostType_PR
{
	HostType_PR_OBU, HostType_PR_RSU,

} HostType_PR;

//Host Equipment Attribute Info
typedef struct HostAttriInfo
{
	char ID[8];
	HostType_PR present;
	union
	{
		struct
		{
			//Vehicle Info
			int VehLenght;
			int VehWidth;
			int VehHeight;
			int VehClassification;
		} OBUInfo;
		struct
		{
			//Location
			int Latitude;
			int Longitude;
			int Elevation;
		} RSUInfo;
	} choice;
} tHostAttriInfo;

typedef struct BreakStatus
{
	int BrakePedalStatus;
	int BrakeAppliedStatus;
	int TractionControlStatus;
	int AntiLockBrakeStatus;
	int StabilityControlStatus;
	int BrakeBoostApplied;
	int AuxiliaryBrakeStatus;
} BreakStatus_t;

typedef struct CANDATA
{
	int Transmission;
	int Speed;
	int SteeringWheelAngle;
	uint16_t ExteriorLights;
	BreakStatus_t Breaks;
} CanData;

///
typedef enum BsmTxThreadState
{
	/// Not initialized
	BSMTX_THREAD_STATE_NONE = 0x00,
	/// Initializing
	BSMTX_THREAD_STATE_INIT = 0x01,
	/// Running
	BSMTX_THREAD_STATE_RUN = 0x02,
	/// Stopping
	BSMTX_THREAD_STATE_STOP = 0x04,
	/// Stopped
	BSMTX_THREAD_STATE_END = 0x08,
} tBsmTxThreadState;

typedef struct BsmTxStats
{
	/// Transmit counters
	struct
	{
		uint32_t Okay;
		uint32_t Fail;
	} Tx;

} tBsmTxStats;

typedef struct BsmTx
{
	/// ID of BSM transmitter thread
	pthread_t ThreadID;
	/// BSM transmitter thread state
	tBsmTxThreadState ThreadState;
	/// Attributes used for thread
	pthread_attr_t ThreadAttr;
	/// Module statistics
	tBsmTxStats Stats;
	/// Configuration
	MessageFrame_t Params;
} tBsmTx;


//Host Communication Info
typedef struct HostComInfo
{
	bool RxEnable;
	char RxAidLst[500];
	struct
	{
		bool TxEnable;
		int TxInterval;
	} BsmTx;
	struct
	{
		bool TxEnable;
		int TxInterval;
		int RsiCnt;
		char CfgFileName[20][50];
	} RsiTx;
	struct
	{
		bool TxEnable;
		int TxInterval;
		char ServerIp[50];
		int ServerPort;
	} RsmTx;
	struct
	{
		bool TxEnable;
		int TxInterval;
		char CfgFileName[50];
	} MapTx;
	struct
	{
		bool TxEnable;
		int TxInterval;
		char ServerIp[50];
		int ServerPort;
	} SpatTx;
//	tDsaTx_Base m_DsaTx;
//	struct
//	{
//		bool TxEnable;
//		int TxInterval;
//		uint8_t OptionIndicator;
//		int HdrExtCnt;
//		char ElmtId[50];
//	} DsaTx;
} tHostComInfo;

//Host Equipment Interface State Info
typedef struct HostStateInfo
{
	int GnssState;
	int CanState;
	int VicState;
	int TcpCilent;
	int TcpServer;
	int UdpCilent;
	int UdpServer;
} tHostStateInfo;

typedef struct StateStore
{
	tWAVEConfig WaveConfig;
	tHostAttriInfo HostAttriInfo;  //本机属性
	tHostComInfo HostComInfo;		//通信使能参数
	tHostStateInfo HostStateInfo;  //本机状态

	/// Application Module handles
	struct
	{
		struct BsmTx *pBSMTx;
		struct RsiTx *pRSITx;
		struct MapTx *pMAPTx;
		struct SpatTx *pSPATTx;
		struct RsmTx *pRSMTx;
		struct DsaTx *pDsaTx;
	} Hdl;

	int CANFlag;
} tStateStore;

//------------------------------------------------------------------------------
// Macros & Constants
//------------------------------------------------------------------------------
#define DCR_OPTHOSTINFO 0
#define DCR_OPTHOSTSTSTE 0
#define DCR_OPTGETCOMMINFO 0
#define DCR_OPTSETCOMMCFG 0
//------------------------------------------------------------------------------
// Type definitions
//------------------------------------------------------------------------------

/**
 * @brief 本机数据请求信息结构体
 */
typedef struct HostInfo
{
	tHostAttriInfo HostAttriInfo;
	GpsLocation GnssInfo;
} tHostInfo;

/**
 * @brief 本机通信状态信息结构体
 */
typedef struct CommStatus
{
//	tCHOpts pCHOpts;
//	tTxOpts pTxOpts;
//	tRxOpts pRxOpts;
	tHostComInfo CommInfo;
} tCommStatus;

//-----------------------------------------------------------------------------
// Type Definitions
//-----------------------------------------------------------------------------
///
/**
 * @brief Map信息发送线程状态枚举
 */
typedef enum MapTxThreadState
{
	/// Not initialized
	MAPTX_THREAD_STATE_NONE = 0x00,
	/// Initializing
	MAPTX_THREAD_STATE_INIT = 0x01,
	/// Running
	MAPTX_THREAD_STATE_RUN = 0x02,
	/// Stopping
	MAPTX_THREAD_STATE_STOP = 0x04,
	/// Stopped
	MAPTX_THREAD_STATE_END = 0x08,
} tMapTxThreadState;

/**
 * @brief Map信息发送状态结构体
 */
typedef struct MapTxStats
{
	/// Transmit counters
	struct
	{
		uint32_t Okay;
		uint32_t Fail;
	} Tx;

} tMapTxStats;

/**
 * @brief Map信息发送结构体
 */
typedef struct MapTx
{
	/// ID of Map transmitter thread
	pthread_t ThreadID;
	/// Map transmitter thread state
	tMapTxThreadState ThreadState;
	/// Attributes used for thread
	pthread_attr_t ThreadAttr;
	/// Module statistics
	tMapTxStats Stats;
	/// Configuration
	MessageFrame_t Params;
} tMapTx;

//-----------------------------------------------------------------------------
// Type Definitions
//-----------------------------------------------------------------------------

/**
 * @brief Rsi信息发送线程状态枚举
 */
typedef enum RsiTxThreadState
{
	/// Not initialized
	RSITX_THREAD_STATE_NONE = 0x00,
	/// Initializing
	RSITX_THREAD_STATE_INIT = 0x01,
	/// Running
	RSITX_THREAD_STATE_RUN = 0x02,
	/// Stopping
	RSITX_THREAD_STATE_STOP = 0x04,
	/// Stopped
	RSITX_THREAD_STATE_END = 0x08,
} tRsiTxThreadState;

/**
 * @brief Rsi信息发送状态结构体
 */
typedef struct RsiTxStats
{
	/// Transmit counters
	struct
	{
		uint32_t Okay;
		uint32_t Fail;
	} Tx;

} tRsiTxStats;

/**
 * @brief Rsi信息发送结构体
 */
typedef struct RsiTx
{
	/// ID of Rsi transmitter thread
	pthread_t ThreadID;
	/// Rsi transmitter thread state
	tRsiTxThreadState ThreadState;
	/// Attributes used for thread
	pthread_attr_t ThreadAttr;
	/// Module statistics
	tRsiTxStats Stats;
	/// Configuration
	MessageFrame_t Params;
} tRsiTx;

//-----------------------------------------------------------------------------
// Type Definitions
//-----------------------------------------------------------------------------
typedef struct RsmData
{
	nint32_t In;
	nint32_t Out;
	//MessageFrame_t MsgData;
	int BufLen;
	char pBuf[1024];
} tRsmData;

tRsmData RsmFrame;

/**
 * @brief 交通参与者类型枚举
 */
enum patricipantType
{
	PartiType_unknown = 0,
	PartiType_motor = 1,
	PartiType_non_motor = 2,
	PartiType_pedestrian = 3,
	PartiType_rsu = 4,
	PartiType_veh = 5
};

/**
 * @brief 交通参与者信息
 */
typedef struct _ParticipantInfo
{
	//编号ID
	uint32_t ID;
	//交通参与者类型
	int ParticType;
	//纬度 单位：度 或者 米
	double Latitude;
	//经度 单位：度 或者 米
	double Longitude;
	//海拔 单位：米
	double Altitude;
	//速度 单位：m/s
	double Speed;
	//航向角 单位：度
	double Heading;
	//长度 单位：米
	double ObjLength;
	//宽度 单位：米
	double ObjWidth;
	//高度 单位：米
	double ObjHeight;
	//数据来源
	uint32_t SourecType;

} tParticipantInfo;

/**
 * @brief 交通参与者类型枚举
 */
typedef struct _RsmMsg
{
	nint32_t RefPosLat;
	nint32_t RefPosLng;
	nint16_t RefPosAngel;
	uint32_t ParticipantCnt;
	tParticipantInfo *pParticipantInfo;
} tRsmMsg;

///
/**
 * @brief Rsm信息发送线程状态枚举
 */
typedef enum RsmTxThreadState
{
	/// Not initialized
	RSMTX_THREAD_STATE_NONE = 0x00,
	/// Initializing
	RSMTX_THREAD_STATE_INIT = 0x01,
	/// Running
	RSMTX_THREAD_STATE_RUN = 0x02,
	/// Stopping
	RSMTX_THREAD_STATE_STOP = 0x04,
	/// Stopped
	RSMTX_THREAD_STATE_END = 0x08,
} tRsmTxThreadState;

/**
 * @brief Rsi信息发送状态结构体
 */
typedef struct RsmTxStats
{
	/// Transmit counters
	struct
	{
		uint32_t Okay;
		uint32_t Fail;
	} Tx;

} tRsmTxStats;

/**
 * @brief Rsi信息发送结构体
 */
typedef struct RsmTx
{
	/// ID of Rsm transmitter thread
	pthread_t ThreadID;
	/// Rsm transmitter thread state
	tRsmTxThreadState ThreadState;
	/// Attributes used for thread
	pthread_attr_t ThreadAttr;
	/// Module statistics
	tRsmTxStats Stats;
	/// Configuration
	MessageFrame_t Params;
} tRsmTx;


//tSpatData SpatFrame;

///
/**
 * @brief Spat信息发送线程状态枚举
 */
typedef enum SpatTxThreadState
{
	/// Not initialized
	SPATTX_THREAD_STATE_NONE = 0x00,
	/// Initializing
	SPATTX_THREAD_STATE_INIT = 0x01,
	/// Running
	SPATTX_THREAD_STATE_RUN = 0x02,
	/// Stopping
	SPATTX_THREAD_STATE_STOP = 0x04,
	/// Stopped
	SPATTX_THREAD_STATE_END = 0x08,
} tSpatTxThreadState;

/**
 * @brief Spat信息发送状态结构体
 */
typedef struct SpatTxStats
{
	/// Transmit counters
	struct
	{
		uint32_t Okay;
		uint32_t Fail;
	} Tx;

} tSpatTxStats;

/**
 * @brief Spat信息发送结构体
 */
typedef struct SpatTx
{
	/**
	 * SPAT帧转发线程的ID号
	 */
	pthread_t ThreadID;
	/**
	 * SPAT帧转发线程的状态
	 */
	tSpatTxThreadState ThreadState;
	/**
	 * 线程属性
	 */
	pthread_attr_t ThreadAttr;
	/**
	 * SPAT帧发送状态
	 */
	tSpatTxStats Stats;
	/**
	 * SPAT帧内容
	 */
	MessageFrame_t Params;
} tSpatTx;

/**
 * @brief RxMsgData 接收消息结构体
 */
typedef struct _RxMsgData
{
	char In;
	char Out;
	int RxCnt;
	MessageFrame_t *MsgData;
	int BuffLen;
	char Buff[1024];
} tConv_RxMsgDa;

/**
 * @brief RxMsgData 接收消息结构体
 */
typedef struct RxMsgData
{
	char In;
	char Out;
	int RxCnt;
	MessageFrame_t MsgData;
} tRxMsgData;


//extern MessageFrame_t tDecodedBuf;

extern tConv_RxMsgDa bsmFrame;
extern tConv_RxMsgDa rsiFrame;
extern tConv_RxMsgDa mapFrame;
extern tConv_RxMsgDa spatFrame;
extern tConv_RxMsgDa rsmFrame;

extern pthread_mutex_t mutex_BSM;
extern pthread_mutex_t mutex_RSI;
extern pthread_mutex_t mutex_MAP;
extern pthread_mutex_t mutex_SPAT;
extern pthread_mutex_t mutex_RSM;

//读取发送数据记录的文件的互斥锁
extern pthread_mutex_t mutex_StoreBSM;
extern pthread_mutex_t mutex_StoreRSI;
extern pthread_mutex_t mutex_StoreMAP;
extern pthread_mutex_t mutex_StoreSPAT;
extern pthread_mutex_t mutex_StoreRSM;

extern pthread_mutex_t mutex_TxSPAT;
extern pthread_mutex_t mutex_TxRSM;

extern tStateStore Store;


//接收消息
extern tQueue_RevLst g_Queue_Recv_bsm;
extern tQueue_RevLst g_Queue_Recv_spat;
extern tQueue_RevLst g_Queue_Recv_rsm;
extern tQueue_RevLst g_Queue_Recv_rsi;
extern tQueue_RevLst g_Queue_Recv_map;

//发送消息

extern tQueue_RevLst g_Queue_Send_Rsm;


#endif /* __FACILAYER_DATASET_H_ */
