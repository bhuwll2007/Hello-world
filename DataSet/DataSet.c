#include "DataSet.h"

StcTcpServer g_StcTcpServer;
StcTcpServer g_StcTcpServer_Rsm;
StcUdpClient g_UdpClient_Rsm;
StcTcpClient g_TcpClient;
StcUdpClient g_UdpClient;
tStateStore pApp;

int msgid;

pthread_t tidp_DealBSM;
pthread_t tidp_DealMAP;
pthread_t tidp_DealRSM;
pthread_t tidp_DealRSI;
pthread_t tidp_DealSPAT;

pthread_t tidp_DealRSM_Trans;

pthread_t tidp_HostStatus;
pthread_t tidp_AroundVeh;
pthread_t tidp_AroundRsi;
pthread_t tidp_UpD1_2WebService;
pthread_t tidp_UpV2X_2WebService;

//主动发送指令时的帧序号
uint8_t g_u8IndexId = 0x00;
//主动发送指令时的帧序号 宇通
int g_u8IndexId_YT = 0;

//应用是否采用地图模式　0－－是　　1－－不是
uint8_t g_IsMapMode = 0;

pthread_mutex_t g_mutex_A;


int g_nMsgId_RecvTransferSpat = 0;
int g_nMsgId_RecvTransferRsm = 0;
int g_nMsgId_Send2VehTransefer = 0;

