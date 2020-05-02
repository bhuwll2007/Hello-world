/*
 * Platform.c
 *
 *  Created on: Jul 11, 2019
 *      Author: wanji-hxy
 */
#include "RsiDeal.h"

static pthread_t g_threadRsiLinkCloud;
static pthread_t g_threadRsiTcpRecv;

int FG_PlatformLinkinit()
{
	int nRet = 0;

	nRet = pthread_create(&g_threadRsiLinkCloud, NULL, SF_PlatformThreadLinklive, NULL);

	return nRet;
	return 0;
}

static void SF_FileUpLoadInit(char *p_pcFilePath)
{
    char Buf[1024] = {0};
    int len = 0;

    int filesize = FileGetsize(p_pcFilePath);
    if (filesize == -1)
    {
    	remove(p_pcFilePath);
    	DEBUG("No such file %s", p_pcFilePath);
    }
    else
    {
    	DEBUG("File Size %d Bytes", filesize);
    }


	int j = 0;
	for (; j <= filesize; j += 1024)
	{
		len = FileRead(p_pcFilePath, Buf, j);

		Tcp_TcpCltSnd2Data(g_tTcpState.Socket, Buf, len);
		DEBUG("TCP send buff size: %d", len);

		memset(Buf, 0x00, sizeof(Buf));
		usleep(1000);
	}
	remove(p_pcFilePath);

}


static void Platform_Recv_Cmd(char *p_pcRecvBuf, uint32_t p_nRecvBufSize)
{

	int i = 0;
	if (p_nRecvBufSize > 0)
	  {
		printf("Recv %d Bytes.\n", p_nRecvBufSize);
		for (i = 0; i < p_nRecvBufSize; i++)
		   {
			  printf("%02X ", p_pcRecvBuf[i]);
		   }
		printf("\n");
	  }

	if ((p_pcRecvBuf[0] == 0x7E))
	{
		switch(p_pcRecvBuf[1])
		{
		case 0x01:
			if (p_pcRecvBuf[2] == 0x00)
			{
				g_tTcpState.isRegist = 1;
				DEBUG("0x01 Response Register OK!");
			}

			break;
		case 0x04:

			break;
		case 0xD0:
			if (p_pcRecvBuf[2] == 0x00)
			{
				g_tTcpState.isD0Send = 1;
				DEBUG("0xD0 Response VehAround OK!");
			}

			break;
		case 0xD1:
			if (p_pcRecvBuf[2] == 0x00)
			{
				g_tTcpState.isD1Send = 1;
				DEBUG("0xD1 Response hostState OK!");
			}
			break;
		case 0xD3:
			if (p_pcRecvBuf[2] == 0x00)
			{
				g_tTcpState.isD3Send = 1;
				DEBUG("0xD3 Response VehSafety OK!");
			}
			break;
		case 0xD6:
			if (p_pcRecvBuf[2] == 0x00)
			{
				g_tTcpState.isD6Send = 1;
				DEBUG("0xD6 Response SPAT OK!");
			}
			break;
		default:
			break;
		}
	}
	else
	{
		return;
	}
}

static void *Platform_Thread_Recv(void *p_pvArgs)
{
	int numbytes = 0;
	char buf[512] = {0};

	while (1)
	  {
		memset(buf, 0, sizeof(buf));
		//接受从服务器返回的信息
		 if((numbytes = Tcp_TcpCltRcvData(g_tTcpState.Socket, buf, &numbytes)) == -1)
		   {
			 memset(&g_tTcpState, 0x00, sizeof(tPlatform_LinkState));
			 perror("recv");
             return NULL;

		   }
	   else if (numbytes == 0)
	      {
			 memset(&g_tTcpState, 0x00, sizeof(tPlatform_LinkState));
	         return NULL;
		  }
	   else
	      {
			//TODO:处理接收到的数据
			Platform_Recv_Cmd(buf, numbytes);
		 }
	  }
	DEBUG("Platform_Thread_Recv exit");
    return NULL;

}


static void *SF_PlatformThreadLinklive(void * p_pvArgs)
{
	int nRet = 0;
	int RegisterNum = 0;

	memset(&g_tTcpState, 0x00, sizeof(tPlatform_LinkState));

	while (1)
	{
		sleep(1);

		if (g_tTcpState.isConnect == 0)
		{
			//更新日志文件名
			FileUpdata(g_LogFileName);

			nRet = Tcp_InitTcpClientSoc(&g_tTcpState.Socket);
			if (nRet < 0) {
				continue;
			}

			nRet = Tcp_InitTcpCltCntSer(g_tPlatform.ServerIp,
					g_tPlatform.ServerPort, g_tTcpState.Socket);
			if (nRet < 0) {
				g_tTcpState.Socket = 0;
				continue;
			}

			//TCP连接成功
			g_tTcpState.isConnect = 1;

			//创建数据接受线程
			nRet = pthread_create(&g_threadRsiTcpRecv, NULL, Platform_Thread_Recv, NULL);
			if(!nRet)
				DEBUG("Platform_Thread_Recv create Success!");
			pthread_detach(g_threadRsiTcpRecv);

		}
		else
		{
			if (g_tTcpState.isRegist == 1)
			{

				//连接成功，日志上传初始化
				if (g_tTcpState.isReUpLoad == 0)
				{
					int k;
					char FileName[30];
					for (k=0;k<LOGLENG;k++)
					{
						DateBackChange(k, FileName);
						DEBUG("FileName: %s",FileName);
//						FileUpLoad_Init(FileName);
					}

					DEBUG("History Log UpLoad OK!");
					g_tTcpState.isReUpLoad = 1;
				}

				RegisterNum = 0;
			}
			else
			{
				//登录注册
				SF_PlatformSendLogin();
				sleep(g_nRelogInterval);
				RegisterNum++;

				DEBUG("RegisterNum %d", RegisterNum);
				if (RegisterNum < 3)
				{
					continue;
				}
				else
				{
					//重复3次登入无应答,关闭ＴＣＰ和接收线程,重启连接
					sleep(10);
					DEBUG("Close socket and reconnect!");
					shutdown(g_tTcpState.Socket, SHUT_RDWR);

					memset(&g_tTcpState, 0x00, sizeof(tPlatform_LinkState));
					pthread_cancel(g_threadRsiTcpRecv);

					RegisterNum = 0;

					//更新日志文件名
//					FileUpdata(g_LogFileName);
					continue;
				}
			}
		}

	}
	return NULL;
}

static void SF_PlatformSendLogin()
{
	char databuf[512];
	char desbuf[1024];
	uint32_t desLen;
	uint16_t nIndex = 0;
	uint8_t TimeInfo[6] = {0};
	static uint16_t LogInCnt = 0;
	static uint8_t LogIn_Num = 0;

	//时间信息
	GetTime_YmdHMS(TimeInfo);
	memcpy(&databuf[nIndex], TimeInfo, sizeof(TimeInfo));
	nIndex += sizeof(TimeInfo);

	//登录流水号
	databuf[nIndex++] = (LogInCnt >> 8) & 0xFF;
	databuf[nIndex++] = (LogInCnt) & 0xFF;

	//设备ID
	memcpy(&databuf[nIndex], &g_tPlatform.DevId[0], sizeof(g_tPlatform.DevId));
	nIndex += sizeof(g_tPlatform.DevId);

	SF_PlatformEncodeFrame(0x01, 0xFE, LogIn_Num, databuf, nIndex, desbuf, &desLen);

	if (g_tTcpState.isConnect > 0)
	{
		if(Tcp_TcpCltSnd2Data(g_tTcpState.Socket, desbuf, desLen)>0)
			DEBUG("Send Register Data!!!");

		LogInCnt++;
		if (LogInCnt > 65531)
			LogInCnt = 0;

		LogIn_Num++;
		if (LogInCnt > 254)
			LogInCnt = 0;
	}

}


static int SF_PlatformEncodeFrame(uint8_t p_ucCmdType, uint8_t p_ucResponseType, uint8_t p_ucFrameCnt, \
		nint8_t p_pcDatabuf[], uint16_t p_sDataLen, nint8_t p_pcDesbuf[], uint32_t *p_punDesLen)
{
	int nIndex = 0;
	char cBccByte = 0;

	uint8_t buf1[1024];
	uint32_t len = 0;

	//帧头
	buf1[nIndex++] = 0x7E;

	//命令标识
	buf1[nIndex++] = p_ucCmdType;
	//应答标识
	buf1[nIndex++] = p_ucResponseType;

	//设备VIN
//	memcpy(&buf1[nIndex], &g_tPlatform.VIN[0], sizeof(g_tPlatform.VIN));
//	nIndex += sizeof(g_tPlatform.VIN);

//	//加密方式
//	buf1[nIndex++] = 0x01;

	//帧序列号
	buf1[nIndex++] = p_ucFrameCnt;

	//帧长度
	buf1[nIndex++] = (p_sDataLen >> 8) & 0xFF;
	buf1[nIndex++] = (p_sDataLen) & 0xFF;

	//帧内容
	memcpy(&buf1[nIndex], p_pcDatabuf, p_sDataLen);
	nIndex += p_sDataLen;

	//校验位
	cBccByte = BCC_CheckSum((nint8_t *)buf1, nIndex-1, 1);
	buf1[nIndex++] = cBccByte;

	//帧尾
	buf1[nIndex++] = 0x7e;

	//转义处理
	len = pkt_code_DF((uint8_t*)buf1, (uint8_t*)p_pcDesbuf, (uint32_t)nIndex);
	//数据帧长度
	(*p_punDesLen) = len;
	return len;
}

/******************
 *platform
 * 20190820*add*zhz***
 ******************/

static int SF_PlatformEncodeFrame1(uint8_t p_ucCmdId, uint8_t p_ucSubCmdId, uint8_t p_ucState, \
		char p_pcSrcbuf[], uint32_t p_unSrcLen, char p_pcDesbuf[], uint32_t *p_punDesLen)
{
	int lnBufLen = 0;
	uint32_t len = 0;
	uint8_t desbuf1[1024];

	//帧头
	desbuf1[lnBufLen++] = 0xFF;
	desbuf1[lnBufLen++] = 0xFF;
	//帧序号
	desbuf1[lnBufLen++] = g_u8IndexId;
	g_u8IndexId += 1;
	g_u8IndexId &= 0x0F;

	//帧主命令号
	desbuf1[lnBufLen++] = p_ucCmdId;
	//帧子命令号
	desbuf1[lnBufLen++] = p_ucSubCmdId;
	//帧状态
	desbuf1[lnBufLen++] = p_ucState;
	//帧长度
	desbuf1[lnBufLen++] = ((p_unSrcLen)) & 0xFF;
	desbuf1[lnBufLen++] = ((p_unSrcLen) >> 8) & 0xFF;
	//拷贝数据
	memcpy(&desbuf1[lnBufLen], p_pcSrcbuf, p_unSrcLen);
	lnBufLen += p_unSrcLen;

	//校验位
	desbuf1[lnBufLen++] = 0x00;
	//帧尾
	desbuf1[lnBufLen++] = 0xFF;

	//转义处理
	len = pkt_code((uint8_t*)desbuf1, (uint8_t*)p_pcDesbuf, (uint32_t)lnBufLen);

	(*p_punDesLen) = len;
	return len;
}

#if 0
void *PlatformUdp_Thread_Linklive(void * args)
{
	int nRet = 0;
	 struct sockaddr_in UdpSvr_addr;
	 nint32_t sockfd;
	memset(&g_tTcpState, 0x00, sizeof(tPlatform_LinkState));

	while (1)
	{
		if((sockfd = socket(AF_INET, SOCK_DGRAM, 0))==-1)
		{
			perror("socket");
//			return -1;
		}
		sleep(1);
		g_rectUdpClientRsiPlatform = calloc(1, sizeof(StcUdpClient));
		g_rectUdpClientRsiPlatform->m_selfPort = 6666;
		nint8_t p_acIp[] = "192.168.20.123";
		memset(&UdpSvr_addr, 0, sizeof(struct sockaddr_in));
		UdpSvr_addr.sin_family = AF_INET;
		UdpSvr_addr.sin_port = htons(g_rectUdpClientRsiPlatform->m_selfPort);
		UdpSvr_addr.sin_addr.s_addr = inet_addr(p_acIp);

	   if (bind(sockfd, (struct sockaddr *)&UdpSvr_addr, sizeof(UdpSvr_addr)) == -1)
			{
				perror("bind socket error");
//				return -2;
			}
		if (nRet < 0)
		{
			continue;
		}
	}
	g_rectUdpClientRsiPlatform->m_UdpClnSok = sockfd;
    return 0;
}
a {
	nint8_t p_acIp[] = "192.168.20.123";
		memset(&UdpSvr_addr, 0, sizeof(struct sockaddr_in));
		UdpSvr_addr.sin_family = AF_INET;
		UdpSvr_addr.sin_port = htons(g_rectUdpClientRsiPlatform->m_selfPort);
		UdpSvr_addr.sin_addr.s_addr = inet_addr(p_acIp);
	if (bind(sockfd, (struct sockaddr *)&UdpSvr_addr, sizeof(UdpSvr_addr)) == -1)
		{
			perror("bind socket error");
	//				return -2;
		}
	//	if (nRet < 0)
	//	{
	//		continue;
	//	}

	g_rectUdpClientRsiPlatform->m_UdpClnSok = sockfd;
}
#endif

static int SF_AnalysisPlatformBuff(rectRsiPlatformToV2X_t **p_prectRsiPlatformToV2Xt,nint8_t *p_pcRecvBuf, uint32_t p_nRecvBufSize)
{
	uint8_t datalen = 8;

	// RSI num
	memcpy(&((*p_prectRsiPlatformToV2Xt)->m_ucRsiNum), &p_pcRecvBuf[datalen], sizeof(uint8_t));
	datalen += 1;
	//RSU num
	datalen += 4;
	memcpy(&((*p_prectRsiPlatformToV2Xt)->m_ucRsuNum), &p_pcRecvBuf[datalen], 8);
	//RSI ID
	datalen += 8;
	memcpy(&((*p_prectRsiPlatformToV2Xt)->m_unRsiID), &p_pcRecvBuf[datalen], sizeof(uint32_t));
	// RSI type
	datalen += 4;
	memcpy(&((*p_prectRsiPlatformToV2Xt)->m_usRsiType), &p_pcRecvBuf[datalen], sizeof(uint16_t));
	// RSI Describe
	datalen += 2;
	memcpy(&((*p_prectRsiPlatformToV2Xt)->m_ucRSIDescrib), &p_pcRecvBuf[datalen], 32);
	// RSI lat
	datalen += 34;
	memcpy(&((*p_prectRsiPlatformToV2Xt)->m_unRsiLat), &p_pcRecvBuf[datalen], sizeof(nint32_t));
	// RSI long
	datalen += 4;
	memcpy(&((*p_prectRsiPlatformToV2Xt)->m_unRsiLong), &p_pcRecvBuf[datalen], sizeof(nint32_t));
	//RSI alt
	datalen += 4;
	memcpy(&((*p_prectRsiPlatformToV2Xt)->m_usRsiAlt), &p_pcRecvBuf[datalen], sizeof(uint16_t));
	//RSI Point1 lat
	datalen += 2;
	memcpy(&((*p_prectRsiPlatformToV2Xt)->m_unRsiPoint1lat), &p_pcRecvBuf[datalen], sizeof(nint32_t));
	//RSI Point1 long
	datalen += 4;
	memcpy(&((*p_prectRsiPlatformToV2Xt)->m_unRsiPoint1long), &p_pcRecvBuf[datalen], sizeof(nint32_t));
	//RSI Point2 lat
	datalen += 4;
	memcpy(&((*p_prectRsiPlatformToV2Xt)->m_unRsiPoint2lat), &p_pcRecvBuf[datalen], sizeof(nint32_t));
	//RSI Point2 long
	datalen += 4;
	memcpy(&((*p_prectRsiPlatformToV2Xt)->m_unRsiPoint2long), &p_pcRecvBuf[datalen], sizeof(nint32_t));
}

static void SF_RsiUdpRecvData(nint8_t *p_pcRecvBuf, uint32_t p_nRecvBufSize)
{
	int i = 0;

//	r_RSIData l_prectRsiRecvFromPlatform;

    if ((0 >= p_nRecvBufSize) || (NULL == p_pcRecvBuf))
    {
        printf("--%s %s file %s line:%d\n",
               __DATE__,__TIME__,__FILE__,__LINE__);
        return;
	}
    if ((0xFF == p_pcRecvBuf[0]) && (0xFF == p_pcRecvBuf[1])	\
    		&& (0xFF == p_pcRecvBuf[p_nRecvBufSize-1]) && (0xE1 == p_pcRecvBuf[3]))
    {
//		Fun_Xor((uint8_t*)p_cRecvBuf, (uint32_t)(pnRecvBufSize));
    	//局部变量解析平台Rsi数据包
    	rectRsiPlatformToV2X_t *l_prectRsiRecvFromPlatform = calloc(1, (sizeof(rectRsiPlatformToV2X_t)));
		if(NULL == l_prectRsiRecvFromPlatform)
		{
			printf("--%s %s file %s line:%d\n",
				__DATE__,__TIME__,__FILE__,__LINE__);
			return;
		}

		SF_AnalysisPlatformBuff(&l_prectRsiRecvFromPlatform, p_pcRecvBuf, p_nRecvBufSize);
		//填充接收到的平台数据结构变量
		FG_RsiRecvFrompPlatformMsgSet(l_prectRsiRecvFromPlatform);
    }
}

/********************************
 *Plarform
 *20190821
 *********************************/
StcUdpClient g_rectUdpClientRsiPlatform;
int FG_PlatformLinkUdpinit(int p_nCliPort)
{
	nint32_t sockfd;

	memset(&g_tTcpState, 0x00, sizeof(tPlatform_LinkState));

	if((sockfd = socket(AF_INET, SOCK_DGRAM, 0))==-1)
	{
		perror("socket");
			return -1;
	}
	g_rectUdpClientRsiPlatform.m_selfPort = p_nCliPort;
	UdpClt_InitSoc(&g_rectUdpClientRsiPlatform);
	g_rectUdpClientRsiPlatform.pCallBackDealData = SF_RsiUdpRecvData;
	UdpClt_LinkSer(&g_rectUdpClientRsiPlatform,"192.168.3.41", 6001);
	return 0;
}



