/*
 * Common.c
 *
 *  Created on: May 25, 2018
 *      Author: root
 */

#ifndef __APPLYLAYER_COMMON_COMMON_C_
#define __APPLYLAYER_COMMON_COMMON_C_

#include "../../Include/BaseInterface_Include.h"
#include "../../DataSet/DataSet.h"
#include "../../Algorithm/DataSet/DataSet.h"
#include "./libxdsc/xds_application.h"

#define EARTH_RADIUS1  6371004
#define PI 3.1415926
#define PROC_NET_DEV_FNAME "/proc/net/dev"
/****************************************************
 * 函数名称:
 * 功能描述:
 * 输入参数:
 * 输出参数:
 * 返 回 值:
 * 创建日期:
 ****************************************************/
int GetTimeInfo(uint8_t *pTimeBuf);
/****************************************************
 * 函数名称:
 * 功能描述:
 * 输入参数:
 * 输出参数:
 * 返 回 值:
 * 创建日期:
 ****************************************************/
int GetTimeStamp(uint64_t *pTimeStamp);
/****************************************************
 * 函数名称:CreatDir
 * 功能描述:创建指定名称的目录，如果该目录存在，则直接返回成功
 * 输入参数:char *path 需要创建的文件夹名称，不支持多级目录同时创建
 * 输出参数:
 * 返 回 值:
 * 创建日期:
 ****************************************************/
int CreatDir(char *path);
/****************************************************
 * 函数名称:
 * 功能描述:
 * 输入参数:
 * 输出参数:
 * 返 回 值:
 * 创建日期:
 ****************************************************/
int Encode_Frame_YT_Veh(int nMsgType, int nPRIO, char VID[], uint8_t srcbuf[], uint32_t srcLen,
                        uint8_t desbuf[], uint32_t *desLen);
/****************************************************
 * 函数名称:
 * 功能描述:
 * 输入参数:
 * 输出参数:
 * 返 回 值:
 * 创建日期:
 ****************************************************/
int Encode_Frame(uint8_t CmdId, uint8_t SubCmdId, uint8_t State, uint8_t srcbuf[], uint32_t srcLen,
                 uint8_t desbuf[], uint32_t *desLen);
/****************************************************
 * 函数名称:
 * 功能描述:
 * 输入参数:
 * 输出参数:
 * 返 回 值:
 * 创建日期:
 ****************************************************/
int Encode_Frame_Clt(uint8_t CmdId, uint8_t SubCmdId, uint8_t srcbuf[], uint32_t srcLen,
                     uint8_t desbuf[], uint32_t *desLen);

/*从字符串中间截取n个字符*/
/****************************************************
 * 函数名称:
 * 功能描述:
 * 输入参数:
 * 输出参数:
 * 返 回 值:
 * 创建日期:
 ****************************************************/
char * mid(char *dst, char *src, int n, int m); /*n为长度 m为位置*/

/**********************************************
 * 函数名 :  double FG_Getdistance(double p_dLatObj, double p_dlngObj, double p_dLatTag, double p_dlngTag)
 * 功能描述: 根据经纬度计算两车车距
 * 输入参数:
 * 			p_dLatObj 自车纬度坐标
 * 			p_dlngObj 自车经度坐标
 * 			p_dLatTag 目标车纬度坐标
 * 			p_dlngTag 目标车经度坐标
 * 输出参数: *
 * 返回值 : p_dDistance 车距
 * 作者 : 黄翔宇
 * 创建日期: 2017/2/17 11:00:10
 **********************************************/

double FG_Getdistance1(double p_dLatObj, double p_dlngObj, double p_dLatTag,
                       double p_dlngTag);

void InitV2VParameter();
void InitSpatParameter();
void InitV2IParameter();
void InitRSMParameter();
void DateBackChange(int n, char *pDate);
int GetTime_YmdHMS(uint8_t *pTimeBuf);
int GetTimeStamps(uint32_t *pTimeStamp);
int GetTimeStampus(uint32_t *pTimeStamp);
uint8_t BCC_CheckSum(nint8_t *pbuf, uint32_t len, uint32_t sByteNum);

typedef struct netdev_stats
{
	unsigned long long rx_packets_m;/* total packets received       */
	unsigned long long tx_packets_m;
	/* total packets transmitted    */
	unsigned long long rx_bytes_m;
	/* total bytes received       */
	unsigned long long tx_bytes_m;
	/* total bytes transmitted    */
	unsigned long rx_errors_m;
	/* bad packets received       */
	unsigned long tx_errors_m;
	/* packet transmit problems    */
	unsigned long rx_dropped_m;
	/* no space in linux buffers    */
	unsigned long tx_dropped_m;
	/* no space available in linux */
	unsigned long rx_multicast_m;
	/* multicast packets received */
	unsigned long rx_compressed_m;
	unsigned long tx_compressed_m;
	unsigned long collisions_m;

	/* detailed rx_errors: */
	unsigned long rx_length_errors_m;
	unsigned long rx_over_errors_m;/* receiver ring buff overflow */
	unsigned long rx_crc_errors_m;
	/* recved pkt with crc error    */
	unsigned long rx_frame_errors_m;
	/* recv'd frame alignment error */
	unsigned long rx_fifo_errors_m;/* recv'r fifo overrun       */
	unsigned long rx_missed_errors_m;/* receiver missed packet    */
	/* detailed tx_errors */
	unsigned long tx_aborted_errors_m;
	unsigned long tx_carrier_errors_m;
	unsigned long tx_fifo_errors_m;
	unsigned long tx_heartbeat_errors_m;
	unsigned long tx_window_errors_m;
}stcnetdev_stats;
int  GetModemStatus(modem_status_t *st1);

int GetDevIPAdd(char *ifname, char *ipv4);

int get_devstats(char * ifname, struct netdev_stats * pstats);

//void LoadXml(MessageFrame_t **pSendMsg, char *pConfigFileName);
#endif /* __APPLYLAYER_COMMON_COMMON_C_ */
