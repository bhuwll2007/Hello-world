/*
 * SpatDeal.c
 *
 *  Created on: Sep 17, 2018
 *      Author: root
 */

#include "SpatDeal.h"

StcUdpClient *g_prectSpatUdpClient = NULL; //连接信号机

nint32_t g_nSpatCliFrameFlag = 0;
uint16_t g_usSpatCliStartFlag = 0;

uint8_t g_ucaSpatTcpCliBuf[512];
nint32_t g_nSpatTcpCliLen = 0;

static void FS_YuTongSpatSendData(tSpatMsg p_rectSpatInfo)
{
	char databuf[1000];
	char databuf1[100];
	char desbuf[1000];
	uint32_t desLen;
	int nIndex = 0;

	//区域号
	databuf[nIndex++] = (g_tYuTong_SpatInfo.RegionID >> 8) & 0xFF;
	databuf[nIndex++] = (g_tYuTong_SpatInfo.RegionID) & 0xFF;

	//路口号
	databuf[nIndex++] = (g_tYuTong_SpatInfo.IntersectionID >> 8) & 0xFF;
	databuf[nIndex++] = (g_tYuTong_SpatInfo.IntersectionID) & 0xFF;

	//时间戳
	uint64_t pTimeStamp = 0;
	int nRet = GetTimeStamp(&pTimeStamp);
	if (nRet == -1)
		pTimeStamp = 0;

	pTimeStamp /= 1000;

	databuf[nIndex++] = (pTimeStamp >> 24) & 0xFF;
	databuf[nIndex++] = (pTimeStamp >> 16) & 0xFF;
	databuf[nIndex++] = (pTimeStamp >> 8) & 0xFF;
	databuf[nIndex++] = (pTimeStamp) & 0xFF;

	memset(&databuf[nIndex], 0x00, 16 + 1 + 16 + 1);
	//信号灯设备序列号
	memcpy(&databuf[nIndex], g_tYuTong_SpatInfo.SpatSelId,
			strlen(g_tYuTong_SpatInfo.SpatSelId));
	nIndex += 16;
	//当前是否正在执行优先？？？
	g_tYuTong_SpatInfo.BusFirst_Status = 0;
	databuf[nIndex] = g_tYuTong_SpatInfo.BusFirst_Status > 0 ? 1 : 0;
	nIndex += 1;
	//优先车辆的设备序列号
	memcpy(&databuf[nIndex], g_tYuTong_SpatInfo.ObuId,
			strlen(g_tYuTong_SpatInfo.ObuId));
	nIndex += 16;
	//执行优先的相位号
	g_tYuTong_SpatInfo.BusFirst_ParseID = 0;
	databuf[nIndex] = g_tYuTong_SpatInfo.BusFirst_ParseID;
	nIndex += 1;
	//多个相位依次排列即可
	int nTmpi = 0;
	int PhaseLength[10];
	memset(PhaseLength, 0x00, sizeof(PhaseLength));
	for (nTmpi = 0; nTmpi < p_rectSpatInfo.pIntersectionInfo[0].PhaseCnt; nTmpi++)
	{
		//相位长度
		PhaseLength[nTmpi] = nIndex;
		nIndex++;
		//相位ID
		databuf[nIndex] = p_rectSpatInfo.pIntersectionInfo[0].pPhaseInfo[nTmpi].ID;
		nIndex++;
		//转向长度
		nIndex++;
		//对应转向
		memset(databuf1, 0x00, sizeof(databuf1));
		if (p_rectSpatInfo.pIntersectionInfo[0].pPhaseInfo[nTmpi].Property == 1)
			memcpy(databuf1, "EnterDirection90-ExitDirection90",
					strlen("EnterDirection90-ExitDirection90"));
		if (p_rectSpatInfo.pIntersectionInfo[0].pPhaseInfo[nTmpi].Property == 2)
			memcpy(databuf1, "EnterDirection180-ExitDirection180",
					strlen("EnterDirection180-ExitDirection180"));
		if (p_rectSpatInfo.pIntersectionInfo[0].pPhaseInfo[nTmpi].Property == 3)
			memcpy(databuf1, "EnterDirection270-ExitDirection270",
					strlen("EnterDirection270-ExitDirection270"));
		if (p_rectSpatInfo.pIntersectionInfo[0].pPhaseInfo[nTmpi].Property == 4)
			memcpy(databuf1, "EnterDirection0-ExitDirection0",
					strlen("EnterDirection0-ExitDirection0"));
		memcpy(&databuf[nIndex], databuf1, strlen(databuf1));

		databuf[nIndex - 1] = strlen(databuf1);
		nIndex += strlen(databuf1);
		//绿时
		databuf[nIndex++] =
				p_rectSpatInfo.pIntersectionInfo[0].pPhaseInfo[nTmpi].GreenTime/10;
		//黄时
		databuf[nIndex++] =
				p_rectSpatInfo.pIntersectionInfo[0].pPhaseInfo[nTmpi].YellowTime/10;
		//红时
		databuf[nIndex++] =
				p_rectSpatInfo.pIntersectionInfo[0].pPhaseInfo[nTmpi].RedTime/10;
		//状态
		databuf[nIndex++] =
				p_rectSpatInfo.pIntersectionInfo[0].pPhaseInfo[nTmpi].Status;
		//剩余秒数
		databuf[nIndex++] =
				p_rectSpatInfo.pIntersectionInfo[0].pPhaseInfo[nTmpi].TimeLeft/10;
	}
	PhaseLength[nTmpi] = nIndex;
	//相位长度 赋值
	for (nTmpi = 0; nTmpi < p_rectSpatInfo.pIntersectionInfo[0].PhaseCnt; nTmpi++)
	{
		//相位长度
		databuf[PhaseLength[nTmpi]] = PhaseLength[nTmpi + 1]
				- PhaseLength[nTmpi + 0] - 1;
	}
//	databuf[PhaseLength[nTmpi]] = 0;

	FG_YuTong2CloudEncodeFrame(0x02, 0x43, databuf, nIndex, desbuf, &desLen);

	if (g_prectYuTongTcpClient != NULL)
	{
		TcpClt_Send(g_prectYuTongTcpClient, desbuf, desLen);
	}
}


static int FS_SpatUdpDataDeal(uint8_t *p_cRecvBuf, uint32_t pnRecvBufSize)
{
	tSpatMsg SpatInfo;
#if 0
	struct timeval tv1, tv2, tv3;
	gettimeofday(&tv1, NULL);
#endif
	int nIndex = 4, i;
	if ((p_cRecvBuf[nIndex] == 0xAA) && (p_cRecvBuf[nIndex + 1] == 0xBB))
	{

	}
	else
	{
		return -1;
	}
	nIndex += 3;
	SpatInfo.IntersectionCnt = 1;
	SpatInfo.pIntersectionInfo = ALLOC_OBJ(tIntersectionInfo, 1);

	SpatInfo.pIntersectionInfo[0].Cycle = 26 + 3 + 29; //(p_cRecvBuf[nIndex] & 0xFF) + ((p_cRecvBuf[nIndex + 1] & 0xFF) << 8);

	SpatInfo.pIntersectionInfo[0].ID = g_nIntersectionId; // p_cRecvBuf[nIndex];
	nIndex++;

	SpatInfo.pIntersectionInfo[0].PhaseCnt = 4;

	SpatInfo.pIntersectionInfo->pPhaseInfo = ALLOC_OBJ(tPhaseInfo,
			SpatInfo.pIntersectionInfo[0].PhaseCnt);

	for (i = 0; i < SpatInfo.pIntersectionInfo[0].PhaseCnt; i++)
	{
		//相位
		SpatInfo.pIntersectionInfo[0].pPhaseInfo[i].ID = i;

		//相位属性
		SpatInfo.pIntersectionInfo[0].pPhaseInfo[i].Property = i + 1;

		//灯色
		SpatInfo.pIntersectionInfo[0].pPhaseInfo[i].Status = (p_cRecvBuf[nIndex
				+ 4 * i] & 0x0F);
		if (SpatInfo.pIntersectionInfo[0].pPhaseInfo[i].Status == 0x01)
			SpatInfo.pIntersectionInfo[0].pPhaseInfo[i].Status = 1; //5;
		if (SpatInfo.pIntersectionInfo[0].pPhaseInfo[i].Status == 0x02)
			SpatInfo.pIntersectionInfo[0].pPhaseInfo[i].Status = 2; //7;
		if (SpatInfo.pIntersectionInfo[0].pPhaseInfo[i].Status == 0x04)
			SpatInfo.pIntersectionInfo[0].pPhaseInfo[i].Status = 3;

		//倒计时
		SpatInfo.pIntersectionInfo[0].pPhaseInfo[i].TimeLeft = p_cRecvBuf[nIndex
				+ 20 + 8 * i];

		SpatInfo.pIntersectionInfo[0].pPhaseInfo[i].GreenTime = 26;

		SpatInfo.pIntersectionInfo[0].pPhaseInfo[i].YellowTime = 3;

		SpatInfo.pIntersectionInfo[0].pPhaseInfo[i].RedTime = 29;
	}
	
	FG_SpatRecvMsgSet(SpatInfo);

	if (g_prectYuTongTcpClient != NULL)
	{
		FS_YuTongSpatSendData(SpatInfo);
	}

	free(SpatInfo.pIntersectionInfo->pPhaseInfo);
	free(SpatInfo.pIntersectionInfo);
	usleep(15 * 1000);
	return 0;
}
/*
 * 函数名称：	UdpData_Process()
 * 功能描述：	对串口数据进行处理（和715进行通信）。
 * 参数列表：	fd -- 串口文件描述符
 * 返回结果：	0--成功，非0--失败
 */
static int FS_SpatUdpDataProcess(uint8_t *p_ucBuf, uint32_t p_unLen)
{
	uint32_t i = 0;
	uint8_t *pBuf = (uint8_t *) p_ucBuf;

	for (i = 0; i < p_unLen; i++)
	{
		if (g_nSpatCliFrameFlag == 0)
		{
			g_usSpatCliStartFlag = (g_usSpatCliStartFlag << 8) + (*pBuf);
			pBuf++;	//add by wll 2015-7-14 13:29:29
			//printf("g_usSpatCliStartFlag = %04X ",g_usSpatCliStartFlag);
			if (g_usSpatCliStartFlag == 0xFD43)	//帧头
			{
				g_nSpatCliFrameFlag = 1;
				g_ucaSpatTcpCliBuf[0] = 0xFD;
				g_ucaSpatTcpCliBuf[1] = 0x43;
				g_nSpatTcpCliLen = 2;
			}
		}
		else if (g_nSpatCliFrameFlag == 1)
		{
			//避免连续的0xFD 0x43导致的问题
			g_usSpatCliStartFlag = (g_usSpatCliStartFlag << 8) + (*pBuf);
			if (g_usSpatCliStartFlag == 0xFD43)	//帧头
			{
				g_nSpatCliFrameFlag = 1;
				g_ucaSpatTcpCliBuf[0] = 0xFD;
				g_ucaSpatTcpCliBuf[1] = 0x43;
				g_nSpatTcpCliLen = 2;
				pBuf++;	//add by wll 2015-7-14 13:29:29
				continue;
			}
			g_ucaSpatTcpCliBuf[g_nSpatTcpCliLen++] = *pBuf;

			if (g_nSpatTcpCliLen > 255) //越界
			{
				g_usSpatCliStartFlag = 0;
				g_nSpatCliFrameFlag = 0;
				g_nSpatTcpCliLen = 0;
				return -1;
			}
		//	printf("len = %d\n",g_nSpatTcpCliLen);
			if (g_nSpatTcpCliLen == 71)
			{
				printf("OK\n");
				FS_SpatUdpDataDeal(g_ucaSpatTcpCliBuf, g_nSpatTcpCliLen);
				g_usSpatCliStartFlag = 0;
				g_nSpatCliFrameFlag = 0;
				g_nSpatTcpCliLen = 0;
			}
			if (g_nSpatTcpCliLen > 80) //越界
			{
				g_usSpatCliStartFlag = 0;
				g_nSpatCliFrameFlag = 0;
				g_nSpatTcpCliLen = 0;
				return -1;
			}
			pBuf++;
		}
#if 0	//负载信息打印
		int jj = 0;
		printf("g_usSpatCliStartFlag = %d g_nSpatCliFrameFlag = %d g_nSpatTcpCliLen = %d BUF: ",g_usSpatCliStartFlag,g_nSpatCliFrameFlag,g_nSpatTcpCliLen);
		for(jj = 0;jj <g_nSpatTcpCliLen;jj ++)
		{
			printf("%02X ",g_ucaSpatTcpCliBuf[jj] );
		}
		printf("\n");
#endif
	}
	return 0;
}

/**
 * @brief 解析SPAT信息回调函数， 将解析后的数据存入SpatFrame结构体.
 * @param[in] p_cRecvBuf      TCP客户端收到的红绿灯信息数据缓存.
 * @param[in] pnRecvBufSize   客户端收到的红绿灯信息数据长度.
 */
void FG_SpatTxCallBack(char *p_pcRecvBuf, uint32_t p_nRecvBufSize)
{
	//g_nRecvFlag = 1;
	printf("Get in SpatTx_CallBack!!!\n");
	FS_SpatUdpDataProcess((uint8_t *) p_pcRecvBuf, p_nRecvBufSize);
}

int FG_SpatInit(tStateStore *p_prectApp)
{
	int nRet = 0;
	//Init Spat Client
	if (Store.HostComInfo.SpatTx.TxEnable)
	{
		g_prectSpatUdpClient = calloc(1, sizeof(StcUdpClient));

		g_prectSpatUdpClient->m_selfPort = 6000;
		UdpClt_InitSoc(g_prectSpatUdpClient);

		g_prectSpatUdpClient->pCallBackDealData = &FG_SpatTxCallBack;
		UdpClt_LinkSer(g_prectSpatUdpClient, "192.168.20.123", 1234);
		printf("Link UDP Server Success !!! \n\n");
	}
	return nRet;
}
/*****************************************************************************
 * 函数名称:SpatTx_CallBack
 * 功能描述:V2X通讯模块中的接收信号机模块的数据
 * 输入参数:char *p_cRecvBuf：接收到的信号机数据缓存；uint32_t pnRecvBufSize：接收到的信号机数据长度
 * 输出参数:无
 * 返 回 值:无
 * 创建日期:
 *****************************************************************************/
void FG_SpatTxCallBack_Old(char *p_pcRecvBuf, uint32_t p_nRecvBufSize)
{
	printf("Get in SpatTx_CallBack!!!\n");
	tSpatMsg SpatInfo;
	int nIndex = 0, i;
	nIndex = 5;

	SpatInfo.IntersectionCnt = 1;
	SpatInfo.pIntersectionInfo = ALLOC_OBJ(tIntersectionInfo, 1);

	SpatInfo.pIntersectionInfo[0].Cycle = (p_pcRecvBuf[nIndex] & 0xFF)
			+ ((p_pcRecvBuf[nIndex + 1] & 0xFF) << 8);

	printf("SpatInfo Cycle = %d\n", SpatInfo.pIntersectionInfo[0].Cycle);
	SpatInfo.pIntersectionInfo[0].ID = g_nIntersectionId;
	SpatInfo.pIntersectionInfo[0].PhaseCnt = p_pcRecvBuf[nIndex + 2];

//	printf("SpatInfo PhaseCnt = %d\n", SpatInfo.pIntersectionInfo[0].PhaseCnt);
	SpatInfo.pIntersectionInfo->pPhaseInfo = ALLOC_OBJ(tPhaseInfo,
			SpatInfo.pIntersectionInfo[0].PhaseCnt);

	nIndex = 9;

	for (i = 0; i < SpatInfo.pIntersectionInfo[0].PhaseCnt; i++)
	{
		SpatInfo.pIntersectionInfo[0].pPhaseInfo[i].ID = (p_pcRecvBuf[nIndex]
				& 0xFF) + ((p_pcRecvBuf[nIndex + 1] & 0xFF) << 8);
		nIndex += 2;

		SpatInfo.pIntersectionInfo[0].pPhaseInfo[i].GreenTime =
				(p_pcRecvBuf[nIndex] & 0xFF)
						+ ((p_pcRecvBuf[nIndex + 1] & 0xFF) << 8);
		nIndex += 2;

		SpatInfo.pIntersectionInfo[0].pPhaseInfo[i].YellowTime =
				(p_pcRecvBuf[nIndex] & 0xFF)
						+ ((p_pcRecvBuf[nIndex + 1] & 0xFF) << 8);
		nIndex += 2;

		SpatInfo.pIntersectionInfo[0].pPhaseInfo[i].RedTime =
				(p_pcRecvBuf[nIndex] & 0xFF)
						+ ((p_pcRecvBuf[nIndex + 1] & 0xFF) << 8);
		nIndex += 2;

		SpatInfo.pIntersectionInfo[0].pPhaseInfo[i].Status = (p_pcRecvBuf[nIndex]
				& 0xFF) + ((p_pcRecvBuf[nIndex + 1] & 0xFF) << 8);
		nIndex += 2;

		SpatInfo.pIntersectionInfo[0].pPhaseInfo[i].TimeLeft =
				(p_pcRecvBuf[nIndex] & 0xFF)
						+ ((p_pcRecvBuf[nIndex + 1] & 0xFF) << 8);
		nIndex += 2;
#if 0
		printf("SpatInfo PhaseID :------------------------------------------------------------------ %d\n",
				i);
		printf("SpatInfo ID = %d\n",
				SpatInfo.pIntersectionInfo[0].pPhaseInfo[i].ID);
		printf("SpatInfo GreenTime = %d\n",
				SpatInfo.pIntersectionInfo[0].pPhaseInfo[i].GreenTime);
		printf("SpatInfo YellowTime = %d\n",
				SpatInfo.pIntersectionInfo[0].pPhaseInfo[i].YellowTime);
		printf("SpatInfo RedTime = %d\n",
				SpatInfo.pIntersectionInfo[0].pPhaseInfo[i].RedTime);
		printf("SpatInfo Status = %d\n",
				SpatInfo.pIntersectionInfo[0].pPhaseInfo[i].Status);
		printf("SpatInfo TimeLeft = %d\n",
				SpatInfo.pIntersectionInfo[0].pPhaseInfo[i].TimeLeft);
#endif
	}

	FG_SpatRecvMsgSet(SpatInfo);
	usleep(100*1000);
	FG_SpatRecvMsgSet(SpatInfo);
	usleep(100*1000);
	FG_SpatRecvMsgSet(SpatInfo);
	usleep(100*1000);
	FG_SpatRecvMsgSet(SpatInfo);

	free(SpatInfo.pIntersectionInfo->pPhaseInfo);
	SpatInfo.pIntersectionInfo->pPhaseInfo = NULL;

	free(SpatInfo.pIntersectionInfo);
	SpatInfo.pIntersectionInfo = NULL;

	memset(&SpatInfo, 0x00, sizeof(tSpatMsg));
}
