#ifndef __DATASET_H_
#define __DATASET_H_
#include "../Include/BaseInterface_Include.h"
#include "../Include/FaciLayer_Include.h"

#define Program_Version (nint8_t *)"V2X_APP_LTEV_DMD31_V3.1.1_20200116"

#define _DEBUG

#ifdef _DEBUG
#define DEBUG(format,...) printf("FILE: " __FILE__ ", LINE: %d: " format "\n", __LINE__, ##__VA_ARGS__)
#else
#define DEBUG(format,...) ;
#endif

#ifdef DEBUG
#define HLOG(format,...) printf("FILE: " __FILE__ ", LINE: %d: " format "\n", __LINE__, ##__VA_ARGS__)
#else
#define HLOG(format,...)
#endif

extern StcTcpServer g_StcTcpServer;
extern StcTcpServer g_StcTcpServer_Rsm;
extern StcUdpClient g_UdpClient_Rsm;
extern StcTcpClient g_TcpClient;
extern StcUdpClient g_UdpClient;
extern tStateStore pApp;
extern int msgid;

extern pthread_t tidp_DealBSM;
extern pthread_t tidp_DealMAP;
extern pthread_t tidp_DealRSM;
extern pthread_t tidp_DealRSI;
extern pthread_t tidp_DealSPAT;
extern pthread_t tidp_DealRSM_Trans;

extern pthread_t tidp_HostStatus;
extern pthread_t tidp_AroundVeh;
extern pthread_t tidp_AroundRsi;
extern pthread_t tidp_UpD1_2WebService;
extern pthread_t tidp_UpV2X_2WebService;

//主动发送指令时的帧序号
extern uint8_t g_u8IndexId;
//主动发送指令时的帧序号 宇通
extern int g_u8IndexId_YT;

//应用是否采用地图模式　0－－是　　1－－不是
extern uint8_t g_IsMapMode;

extern pthread_mutex_t g_mutex_A;

extern int g_nMsgId_RecvTransferSpat;
extern int g_nMsgId_RecvTransferRsm;
extern int g_nMsgId_Send2VehTransefer;

#endif /*__DATASET_H_*/
