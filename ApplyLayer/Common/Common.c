/*
 * Common.c
 *
 *  Created on: May 25, 2018
 *      Author: root
 */
#include "Common.h"
#include <time.h>
#define SECOND_OF_DAY 86400
/****************************************************
 * 函数名称:
 * 功能描述:
 * 输入参数:
 * 输出参数:
 * 返 回 值:
 * 创建日期:
 ****************************************************/
int GetTimeInfo(uint8_t *pTimeBuf)
{
	time_t at_time = time(NULL);
	struct tm ptr;
	char YMDHMS[24];
	localtime_r(&at_time, &ptr);

	strftime(YMDHMS, 22, "%Y-%m-%d %H:%M:%S", &ptr);
	memcpy(pTimeBuf, YMDHMS, strlen(YMDHMS));

	return 0;
}

void DateBackChange(int n, char *pDate)
{
    char date[30];
    time_t lt;
    lt = time(NULL);
    long seconds = 24 * 3600 * n;//24 小时 * 小时秒 * 天数
    lt -= seconds;//计算后N天
    struct tm *p = localtime(&lt);
    localtime_r(&lt, p);
	strftime(date, 22, "%Y-%m-%d", p);
	sprintf(pDate, "./Log/%s.txt", date);
}

int GetTime_YmdHMS(uint8_t *pTimeBuf)
{
	uint16_t i, j, iDay;
	unsigned long lDay;
	uint8_t DayOfMon[12] = { 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };
	struct timeval tv;
	struct timezone tz;
	gettimeofday(&tv, &tz);
	unsigned long lSec;
	lSec = tv.tv_sec;
	lDay = lSec / SECOND_OF_DAY;
	lSec = lSec % SECOND_OF_DAY; //

	i = 1970;
	while (lDay > 365) {
		if (((i % 4 == 0) && (i % 100 != 0)) || (i % 400 == 0))
			lDay -= 366;
		else
			lDay -= 365;
		i++;
	}
	if ((lDay == 365)
			&& !(((i % 4 == 0) && (i % 100 != 0)) || (i % 400 == 0))) {
		lDay -= 365;
		i++;
	}
	pTimeBuf[0] = i - 2000;
	for (j = 0; j < 12; j++) {
		if ((j == 1) && (((i % 4 == 0) && (i % 100 != 0)) || (i % 400 == 0)))
			iDay = 29;
		else
			iDay = DayOfMon[j];
		if (lDay >= iDay)
			lDay -= iDay;
		else
			break;
	}
	pTimeBuf[1] = j + 1;
	pTimeBuf[2] = lDay + 1;
	pTimeBuf[3] = ((lSec / 3600) + 8) % 24;
	pTimeBuf[4] = (lSec % 3600) / 60;
	pTimeBuf[5] = (lSec % 3600) % 60;

	return 0;
}

/****************************************************
 * 函数名称:
 * 功能描述:
 * 输入参数:
 * 输出参数:
 * 返 回 值:
 * 创建日期:
 ****************************************************/
int GetTimeStamp(uint64_t *pTimeStamp)
{
	struct timeval tv;

	gettimeofday(&tv, NULL);
	*pTimeStamp = (uint64_t) ((uint64_t) tv.tv_sec * 1000
			+ (uint64_t) (tv.tv_usec / 1000));

	return 0;
}

int GetTimeSeqInfo_YT(nint8_t pTimeBuf[])
{
	time_t at_time = time(NULL);
	struct tm ptr;
	char YMDHMS[24];
	localtime_r(&at_time, &ptr);

	strftime(YMDHMS, 22, "%04Y%02m%02d%02H%02M%02S", &ptr);
	struct timeval tv;

	gettimeofday(&tv, NULL);
	sprintf(pTimeBuf, "%s%03d%03d", YMDHMS,(int)(tv.tv_usec / 1000),g_u8IndexId_YT);
	return 0;
}
/****************************************************
 * 函数名称:
 * 功能描述:
 * 输入参数:
 * 输出参数:
 * 返 回 值:
 * 创建日期:
 ****************************************************/
int CreatDir(char *path)
{
	extern int errno;
//	char *path = "/Log";
	if ((access(path, F_OK)) == -1)
	{
		if (mkdir(path, 0766) == 0)
		{
			printf("created the directory %s . \n", path);
		}
		else
		{
			printf("can't creat the directoty %s.\n", path);
			printf("errno : %d\n", errno);
			printf("ERR : %s\n", strerror(errno));
		}
	}
	else
	{
		return 0;
	}
	return 0;
}

/****************************************************
 * 函数名称:
 * 功能描述:
 * 输入参数:
 * 输出参数:
 * 返 回 值:
 * 创建日期:
 ****************************************************/
int Encode_Frame(uint8_t CmdId, uint8_t SubCmdId, uint8_t State, uint8_t srcbuf[], uint32_t srcLen,
		uint8_t desbuf[], uint32_t *desLen)
{
	int lnBufLen = 0;
	//帧头
	desbuf[lnBufLen++] = 0xFF;
	desbuf[lnBufLen++] = 0xFF;
	//帧序号
	desbuf[lnBufLen++] = g_u8IndexId;
	g_u8IndexId += 1;
	g_u8IndexId &= 0x0F;

	//帧主命令号
	desbuf[lnBufLen++] = CmdId;
	//帧子命令号
	desbuf[lnBufLen++] = SubCmdId;
	//帧状态
	desbuf[lnBufLen++] = State;
	//帧长度
	desbuf[lnBufLen++] = ((srcLen)) & 0xFF;
	desbuf[lnBufLen++] = ((srcLen) >> 8) & 0xFF;
	//拷贝数据
	memcpy(&desbuf[lnBufLen], srcbuf, srcLen);
	lnBufLen += srcLen;

	//校验位
	desbuf[lnBufLen++] = 0x00;
	//帧尾
	desbuf[lnBufLen++] = 0xFF;

	(*desLen) = lnBufLen;
	return lnBufLen;
}

/****************************************************
 * 函数名称:
 * 功能描述:
 * 输入参数:
 * 输出参数:
 * 返 回 值:
 * 创建日期:
 ****************************************************/
int Encode_Frame_Clt(uint8_t CmdId, uint8_t SubCmdId, uint8_t srcbuf[], uint32_t srcLen,
		uint8_t desbuf[], uint32_t *desLen)
{
	int lnBufLen = 0;
	//帧头
	desbuf[lnBufLen++] = 0x5E;
	desbuf[lnBufLen++] = 0x5D;

	//
	desbuf[lnBufLen++] = CmdId;
	//
	desbuf[lnBufLen++] = SubCmdId;
	//
	lnBufLen += 6;
	//
	desbuf[lnBufLen++] = 0x10;
	//
	desbuf[lnBufLen++] = 0x00;
	//帧长度
	desbuf[lnBufLen++] = ((srcLen)) & 0xFF;
	desbuf[lnBufLen++] = ((srcLen) >> 8) & 0xFF;
	//拷贝数据
	memcpy(&desbuf[lnBufLen], srcbuf, srcLen);
	lnBufLen += srcLen;

	//校验位
	desbuf[lnBufLen++] = 0x00;
	//帧尾
	desbuf[lnBufLen++] = 0x5C;
	desbuf[lnBufLen++] = 0x5B;

	(*desLen) = lnBufLen;
	return lnBufLen;
}

/*从字符串中间截取n个字符*/
/****************************************************
 * 函数名称:
 * 功能描述:
 * 输入参数:
 * 输出参数:
 * 返 回 值:
 * 创建日期:
 ****************************************************/
char * mid(char *dst,char *src, int n,int m) /*n为长度 m为位置*/
{
    char *p = src;
    char *q = dst;
    int len = strlen(src);
    if(n>len) n = len-m;    /**/
    if(m<0) m=0;
    if(m>len) return NULL;
    p += m;
    while(n--) *(q++) = *(p++);
    *(q++)='\0';
    return dst;
}

/**********************************************
 * 函数名  : double FG_Angletoradian(double p_dA)
 * 功能描述: 角度转化弧度值
 * 输入参数: p_dAangel  角度值
 * 输出参数: *
 * 返回值  :  弧度值
 * 作者 : 黄翔宇
 * 创建日期: 2017/2/17 10:57:36
 **********************************************/
double FG_Angletoradian1(double p_dAangel)
{
    return p_dAangel * PI / 180.0;
}

/**********************************************
 * 函数名 :  double FG_Getdistance(double p_dLatObj, double p_dlngObj, double p_dLatTag, double p_dlngTag)
 * 功能描述: 根据经纬度计算两车车距
 * 输入参数: p_dLatObj 自车纬度坐标
		     p_dlngObj 自车经度坐标
		     p_dLatTag 目标车纬度坐标
		     p_dlngTag 目标车经度坐标
 * 输出参数: *
 * 返回值 : p_dDistance 车距
 * 作者 : 黄翔宇
 * 创建日期: 2017/2/17 11:00:10
 **********************************************/

double FG_Getdistance1(double p_dLatObj, double p_dlngObj, double p_dLatTag, double p_dlngTag)
{
	double p_dDistance = 0;

    double radLat1 = FG_Angletoradian1(p_dLatObj);
    double radLat2 = FG_Angletoradian1(p_dLatTag);

    double a = radLat1 - radLat2;
    double b = FG_Angletoradian1(p_dlngObj) - FG_Angletoradian1(p_dlngTag);
    double dst = 2 * asin((sqrt(pow(sin(a / 2), 2) + cos(radLat1) * cos(radLat2) * pow(sin(b / 2), 2) )));
    dst = dst * EARTH_RADIUS1/1000;

	p_dDistance = (dst * 10000000) / 10000;
    return p_dDistance;
}

/****************************************************
 * 函数名称:
 * 功能描述:
 * 输入参数:
 * 输出参数:
 * 返 回 值:
 * 创建日期:
 ****************************************************/
void InitV2VParameter()
{
// Parameter for MapMatch()
	g_V2V_Parameter.MapMatch_ConfidenceDis_toLink = 20;

/// Parameter for isDangerPossible()
	g_V2V_Parameter.isDanger_hv_warning_time = 10;
	g_V2V_Parameter.isDanger_hv_speed_low = 3;

/// Parameter forCalVehicleLocation() , CaseDetect()
	g_V2V_Parameter.VehLOC_SafeDis_Value = 1;

///Parameter for V2V_AVW_Section()
	g_V2V_Parameter.AVW_Speed_threshold_value = 10;

///Parameter for V2V_FCW_Section()
	g_V2V_Parameter.FCW_TTC_threshold_value = 3;
	g_V2V_Parameter.FCW_THW_threshold_value = 1;

///Parameter for V2V_LCW_Section()
	g_V2V_Parameter.LCW_Blind_Spot_Angle = 60;
	g_V2V_Parameter.LCW_Blind_Spot_Width = 8;
	g_V2V_Parameter.LCW_Blind_Spot_ToMirror = 0.5;

///Parameter for V2V_ICW_Section()
	g_V2V_Parameter.ICW_TTC_threshold_value = 5;
	g_V2V_Parameter.ICW_TTC_SafeDisVeh = 0.5;
}

/****************************************************
 * 函数名称:
 * 功能描述:
 * 输入参数:
 * 输出参数:
 * 返 回 值:
 * 创建日期:
 ****************************************************/
void InitSpatParameter()
{
	g_Spat_Parameter.LightDir_Dis_Value = 30;  //判断下游节点相对为 横向偏移阈值
/// Parameter for SpeedGuide()
	g_Spat_Parameter.SPATWarnDis = 100;
	g_Spat_Parameter.WarnCalcTime = 0.7;
	g_Spat_Parameter.DistoCrossRdMid = 18;
	g_Spat_Parameter.SpeedLimit_High = 15;
	g_Spat_Parameter.SpeedLimit_Low = 3;
}

/****************************************************
 * 函数名称:
 * 功能描述:
 * 输入参数:
 * 输出参数:
 * 返 回 值:
 * 创建日期:
 ****************************************************/
void InitV2IParameter()
{
/// Parameter for IVS()
	g_V2I_Parameter.IVSWarnDis = 50;
	g_V2I_Parameter.IVSSpeakDis = 50;
}
/****************************************************
 * 函数名称:
 * 功能描述:
 * 输入参数:
 * 输出参数:
 * 返 回 值:
 * 创建日期:
 ****************************************************/
void InitRSMParameter()
{
	g_RSM_Parameter.Laser_Lat = 0;
	g_RSM_Parameter.Laser_Long = 0;
	g_RSM_Parameter.Laser_Display = 50;
}
/****************************************************
 * 函数名称:
 * 功能描述:
 * 输入参数:
 * 输出参数:
 * 返 回 值:
 * 创建日期:
 ****************************************************/
//void LoadXml(MessageFrame_t **pSendMsg, char *pConfigFileName)
//{
//	char *p;
//	char *xer_buf = NULL;
//	int fsize = 0;
//	FILE *pxerfp;
//	pxerfp = fopen(pConfigFileName, "r"); //Read all bytes in RSI config file
//	fseek(pxerfp, 0, SEEK_END);
//	fsize = ftell(pxerfp);
//	xer_buf = calloc(1, fsize);
//	p = xer_buf;
//	rewind(pxerfp);
//	fread(xer_buf, fsize, 1, pxerfp);
//	fclose(pxerfp);
//	asn_dec_rval_t rval;
//	rval = xer_decode(0, &asn_DEF_MessageFrame, (void **) pSendMsg, xer_buf,
//			fsize);
//	if (rval.code != RC_OK)
//	{
//		(*pSendMsg)->present = MessageFrame_PR_NOTHING;
//	//	printf("11111\n");
//	}
//
//	free(p);
//}

int Encode_Frame_YT_Veh(int nMsgType, int nPRIO, char VID[], uint8_t srcbuf[], uint32_t srcLen,
		uint8_t desbuf[], uint32_t *desLen)
{
//	printf("srcbuf = %s ",srcbuf);
//	printf("srcLen = %d ",srcLen);
	int lnBufLen = 0;

	//Head
	desbuf[lnBufLen++] = '^';
	desbuf[lnBufLen++] = '^';

	//MsgType
	char acMsgType[4];
	memset(acMsgType, 0x00, sizeof(acMsgType));
	sprintf(acMsgType, "%03d", nMsgType);
	memcpy(&desbuf[lnBufLen], acMsgType, 3);
	lnBufLen += 3;

	//length
	char aclength[7];
	memset(aclength, 0x00, sizeof(aclength));
	sprintf(aclength, "%06d", srcLen);
	memcpy(&desbuf[lnBufLen], aclength, 6);
	lnBufLen += 6;

	//VID
	memcpy(&desbuf[lnBufLen], VID, 16);
	lnBufLen += 16;

	//SEQ
	nint8_t YMDHMSZX[21];
	GetTimeSeqInfo_YT(YMDHMSZX);
	memcpy(&desbuf[lnBufLen], YMDHMSZX, 20);
	lnBufLen += 20;

	//PRIO
	char acPRIO[5];
	memset(acPRIO, 0x00, sizeof(acPRIO));
	sprintf(acPRIO, "%03d", nPRIO);
	memcpy(&desbuf[lnBufLen], acPRIO, 3);
	lnBufLen += 3;

	//Data
	memcpy(&desbuf[lnBufLen], srcbuf, srcLen);
	lnBufLen += srcLen;

	//Trail
	desbuf[lnBufLen++] = '$';
	desbuf[lnBufLen++] = '$';
//	desbuf[lnBufLen++] = '$';

	(*desLen) = lnBufLen;
	return lnBufLen;
}

int GetDevIPAdd(char *ifname, char *ipv4)
{
	int rc = 0;
	struct sockaddr_in *addr = NULL;

	struct ifreq ifr;
	memset(&ifr, 0, sizeof(struct ifreq));

	/* 0. create a socket */
	int fd = socket(AF_INET, SOCK_DGRAM, IPPROTO_IP);
	if (fd == -1)
		return -1;

	/* 1. set type of address to retrieve : IPv4 */
	ifr.ifr_addr.sa_family = AF_INET;

	/* 2. copy interface name to ifreq structure */
	strncpy(ifr.ifr_name, ifname, IFNAMSIZ - 1);

	/* 3. get the IP address */
	if ((rc = ioctl(fd, SIOCGIFADDR, &ifr)) != 0)
		goto done;

	addr = (struct sockaddr_in *) &ifr.ifr_addr;
	strncpy(ipv4, inet_ntoa(addr->sin_addr), 16);

	/* 4. get the mask */
	// if ((rc = ioctl(fd, SIOCGIFNETMASK, &ifr)) != 0)
	// 	goto done;

	// char mask[16] =
	// { 0 };
	// addr = (struct sockaddr_in *) &ifr.ifr_addr;
	// strncpy(mask, inet_ntoa(addr->sin_addr), sizeof(mask));

	/* 5. display */
	// printf("IFNAME:IPv4\n");
	// printf("%s:%s\n", ifname, ipv4);

	/* 6. close the socket */
	done: close(fd);

	return rc;
}
uint8_t BCC_CheckSum(nint8_t *pbuf, uint32_t len, uint32_t sByteNum)
{
	nint32_t i;
	uint8_t checksum = 0;

	for (i = sByteNum; i <=len; i++) {
		checksum ^= pbuf[i];
	}

	return checksum;
}
int  GetModemStatus(modem_status_t *st1)
{
	int hdl ;
	//获取模组状态
	hdl = v2x_open(SSID_MODEM, 0);
	if(hdl < 0)
		return -1;
	v2x_read(hdl, MODEM_MSGID_SAMPLE_STATUS, st1, sizeof(modem_status_t));
	if(hdl < 0)
	{
		v2x_close(hdl);
		return -2;
	}
	v2x_close(hdl);
	return 0;
}

static char * get_name(char * name, char * p)
{
	char * t = NULL;

	/*interface only contains lowercase letters*/
	while ((*p < 'a') || (*p > 'z'))
		p++;

	if ((t = strchr(p, ':')))
	{
		memcpy(name, p, t - p);
		return t + 1;
	} else
		return NULL;
}

int get_devstats(char * ifname, struct netdev_stats * pstats)
{
	FILE * fp;
	char name[256] =
	{ 0 };
	char buf[256] =
	{ 0 };
	char * s = NULL;
	int found = 0;

	if (!ifname || !pstats)
		return -1;

	fp = fopen(PROC_NET_DEV_FNAME, "r");

	if (!fp)
	{
		return -1;
	}
	else
	{
		/*by pass the first 2 lines, they are titles*/
		fgets(buf, sizeof(buf), fp);
		fgets(buf, sizeof(buf), fp);

		memset(buf, 0, sizeof(buf));
		while (fgets(buf, sizeof(buf), fp))
		{
			memset(name, 0, sizeof(name));
			s = get_name(name, buf);

			if (s)
			{
				sscanf(s,
						"%llu%llu"
						"%lu%lu%lu%lu%lu%lu"
						"%llu%llu"
						"%lu%lu%lu%lu%lu%lu",
						&pstats->rx_bytes_m, /* missing for 0 */
						&pstats->rx_packets_m,
						&pstats->rx_errors_m,
						&pstats->rx_dropped_m,
						&pstats->rx_fifo_errors_m,
						&pstats->rx_frame_errors_m,
						&pstats->rx_compressed_m, /* missing for <= 1 */
						&pstats->rx_multicast_m, /* missing for <= 1 */
						&pstats->tx_bytes_m, /* missing for 0 */
						&pstats->tx_packets_m,
						&pstats->tx_errors_m,
						&pstats->tx_dropped_m,
						&pstats->tx_fifo_errors_m,
						&pstats->collisions_m,
						&pstats->tx_carrier_errors_m,
						&pstats->tx_compressed_m /* missing for <= 1 */
						);

				if (!strncmp(name, ifname, strlen(ifname)))
				{
					found = 1;
					break;
					printf("name :%s\n", name);
					printf(
							"%llu:%llu:"
//							"%lu:%lu:%lu:%lu:%lu:%lu:"
							"%llu:%llu:",
//							"%lu:%lu:%lu:%lu:%lu:%lu:\n",
							pstats->rx_bytes_m, /* missing for 0 */
							pstats->rx_packets_m,
//							pstats->rx_errors_m,
//							pstats->rx_dropped_m, pstats->rx_fifo_errors_m,
//							pstats->rx_frame_errors_m, pstats->rx_compressed_m, /* missing for <= 1 */
//							pstats->rx_multicast_m, /* missing for <= 1 */
							pstats->tx_bytes_m, /* missing for 0 */
							pstats->tx_packets_m//,
//							pstats->tx_errors_m,
//							pstats->tx_dropped_m, pstats->tx_fifo_errors_m,
//							pstats->collisions_m, pstats->tx_carrier_errors_m,
//							pstats->tx_compressed_m /* missing for <= 1 */
							);
				}
			} else
				continue;
		}
		fclose(fp);
	}

	if (!found)
		return -2;
	else
		return 0;
}
