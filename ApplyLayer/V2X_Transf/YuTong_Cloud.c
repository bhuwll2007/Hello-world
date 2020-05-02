#include "YuTong_Cloud.h"
StcTcpClient *g_prectYuTongTcpClient = NULL; //连接宇通云平台
int g_nYuTongRecvRegisterOKFlag = 0;

static void FS_YuTongYunRegisterSendData(void)
{
	char databuf[100];
	char desbuf[100];
	uint32_t desLen;
	int nIndex = 0;

	//设备序列号
	memcpy(&databuf[nIndex], g_tYuTong_RsuBaseInfo.RsuSelId,
			strlen(g_tYuTong_RsuBaseInfo.RsuSelId));
	nIndex += strlen(g_tYuTong_RsuBaseInfo.RsuSelId);
	databuf[nIndex++] = ',';

	//RSU名称
	memcpy(&databuf[nIndex], g_tYuTong_RsuBaseInfo.RsuName,
			strlen(g_tYuTong_RsuBaseInfo.RsuName));
	nIndex += strlen(g_tYuTong_RsuBaseInfo.RsuName);
	databuf[nIndex++] = ',';

	//IP地址
	memcpy(&databuf[nIndex], g_tYuTong_RsuBaseInfo.RsuIPAddr,
			strlen(g_tYuTong_RsuBaseInfo.RsuIPAddr));
	nIndex += strlen(g_tYuTong_RsuBaseInfo.RsuIPAddr);
	databuf[nIndex++] = ',';
	//位置经度
	databuf[nIndex++] = ',';
	//位置纬度
	databuf[nIndex++] = ',';
	//高度
	databuf[nIndex++] = ',';
	//项目代号
	memcpy(&databuf[nIndex], g_tYuTong_RsuBaseInfo.ProjectId,
			strlen(g_tYuTong_RsuBaseInfo.ProjectId));
	nIndex += strlen(g_tYuTong_RsuBaseInfo.ProjectId);

	FG_YuTong2CloudEncodeFrame(0x02, 0x41, databuf, nIndex, desbuf, &desLen);
	if (g_prectYuTongTcpClient != NULL)
	{
		TcpClt_Send(g_prectYuTongTcpClient, desbuf, desLen);
	}
}
static void FS_YuTongYunTimerSendRegister(void)
{
	if (g_nYuTongRecvRegisterOKFlag == 0)
	{
		FS_YuTongYunRegisterSendData();
	}
}

/****************************************************
 * 函数名称:
 * 功能描述:
 * 输入参数:
 * 输出参数:
 * 返 回 值:
 * 创建日期:
 ****************************************************/
int FG_YuTong2CloudEncodeFrame(uint8_t p_ucVersionId, uint8_t p_ucFrameTypeId,
		nint8_t p_caDatabuf[], uint32_t p_unDataLen, nint8_t p_caDesbuf[], uint32_t *p_punDesLen)
{
	int nIndex = 0;

	//帧头
	p_caDesbuf[nIndex++] = 0xFA;
	p_caDesbuf[nIndex++] = 0xFB;

	//版本号
	p_caDesbuf[nIndex++] = p_ucVersionId;

	//命令号
	p_caDesbuf[nIndex++] = p_ucFrameTypeId;

	//帧长度
	p_caDesbuf[nIndex++] = (p_unDataLen >> 8) & 0xFF;
	p_caDesbuf[nIndex++] = (p_unDataLen) & 0xFF;

	//帧内容
	memcpy(&p_caDesbuf[nIndex], p_caDatabuf, p_unDataLen);
	nIndex += p_unDataLen;

	//校验位
	p_caDesbuf[nIndex++] = 0x00;

	//数据帧长度
	(*p_punDesLen) = nIndex;
	return nIndex;
}

static void FS_YuTongYunCallBack(char *p_pcRecvBuf, uint32_t p_pnRecvBufSize)
{
	printf("Get in FS_YuTongYunCallBack!!!\n");
	if ((p_pcRecvBuf[0] == 0xFA) && (p_pcRecvBuf[1] == 0xFB))
	{
		if (p_pcRecvBuf[3] == 0x41)
		{
			if (((p_pcRecvBuf[6] == 'o') || (p_pcRecvBuf[6] == 'O'))
					&& ((p_pcRecvBuf[7] == 'k') || (p_pcRecvBuf[7] == 'K')))
			{
				g_nYuTongRecvRegisterOKFlag = 1;
			}
		}
	}
	else
	{
		return;
	}
}
pthread_t tidp_LinkCloud;
static void *FS_ThreadLinkCloud(void * p_pvArg)
{
	int nRet = 0;
	while (1)
	{
		sleep(1);
		if(g_prectYuTongTcpClient == NULL)
		{
			g_nYuTongRecvRegisterOKFlag = 0;
			//连接宇通云平台
			g_prectYuTongTcpClient = calloc(1, sizeof(StcTcpClient));

			nRet = TcpClt_InitSoc(g_prectYuTongTcpClient);
			if (nRet < 0)
			{
				TcpClt_ClsSoc(g_prectYuTongTcpClient);
				free(g_prectYuTongTcpClient);
				g_prectYuTongTcpClient = NULL;
				continue;
			}
		
			g_prectYuTongTcpClient->pCallBackDealData = &FS_YuTongYunCallBack;
			nRet = TcpClt_LinkSer(g_prectYuTongTcpClient, g_tYuTong_RsuBaseInfo.CloudIpAdd, g_tYuTong_RsuBaseInfo.CloudPort);

			if (nRet < 0)
			{
				TcpClt_ClsSoc(g_prectYuTongTcpClient);
				free(g_prectYuTongTcpClient);
				g_prectYuTongTcpClient = NULL;
				continue;
			}
			g_prectYuTongTcpClient->is_Connect = 1;
			int nCount = 5;
			while (nCount > 0)
			{
				
				if(g_nYuTongRecvRegisterOKFlag == 0)
					FS_YuTongYunTimerSendRegister();
				else
					break;
				sleep(1);
				nCount -- ;
			}
		}
		else
		{
			//printf("---------------------------------------------------------------------------\n");
			if(g_prectYuTongTcpClient->is_Connect == 0)
			{
				TcpClt_ClsSoc(g_prectYuTongTcpClient);
				free(g_prectYuTongTcpClient);
				g_prectYuTongTcpClient = NULL;
			}
		}
	}
	return 0;
}
int FG_YuTongCloudInit(tStateStore *p_prectApp)
{
	int nRet = 0;

	pthread_create(&tidp_LinkCloud, NULL, FS_ThreadLinkCloud,
			 NULL);
	
	return nRet;
}
