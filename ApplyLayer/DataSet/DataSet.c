#include "DataSet.h"

tMapInfo g_MapInfo;
tMapInfo g_MapInfo_SL;
//上传D0帧 全局结构体
UpD0Data g_UpD0Data;
UpD9Data g_UpD9Data;
uint16_t g_u16LimitHighSpeed = 200;
sqlite3 * sqldb;
//sqlite3 * sqldb_Read;
ReadHisDataBase g_ReadHisDataBase;
//距离信号机的距离
uint16_t g_u16SpatDistence = 200;

double g_dSpatLat = 0.0;
double g_dSpatLong = 0.0;

tVehData AroundHV;

pthread_mutex_t mutex_AroudVeh;
pthread_mutex_t mutex_AroudRsi;

tAPP_RunParam g_tAPP_RunParam;
int g_nIntersectionId = 0;

tYuTong_SpatInfo g_tYuTong_SpatInfo;
tYuTong_RsuBaseInfo g_tYuTong_RsuBaseInfo;

tSPAT_INFO g_stcSpatInfo;

//20190820 add
tPlatform_LinkState g_tTcpState;
char g_LogFileName[30];
tPlatform g_tPlatform;
int g_nRelogInterval = 10;
