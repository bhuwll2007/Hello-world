/*
 * V2X_RecvMAP.c
 *
 *  Created on: May 25, 2018
 *      Author: root
 */

#include "V2X_RecvMAP.h"
#define __PRINT_DEBUGLOG_ 0
#define __READ_XMLFILE 1


void GetMapInfo_FromXml(tStateStore *pApp)
{
	MessageFrame_t *pMAPMsg;
	pMAPMsg = ALLOC_OBJ(MessageFrame_t, 1);

	char buf[50];
	memset(buf, 0x00, 50);
	memcpy(buf, pApp->HostComInfo.MapTx.CfgFileName, strlen(pApp->HostComInfo.MapTx.CfgFileName));
	printf("Map : = %s ---\n",pApp->HostComInfo.MapTx.CfgFileName);
	LoadXml(&pMAPMsg, buf);

	xer_fprint(stdout, &asn_DEF_MessageFrame, pMAPMsg);
	GetMapInfo(&g_MapInfo, pMAPMsg);
	GetMapInfo(&g_MapInfo_SL, pMAPMsg);
	ASN_STRUCT_FREE(asn_DEF_MessageFrame, pMAPMsg);

	printf("----------------\nGet Map Message!!!\n----------------\n");
	Out_Map(&g_MapInfo_SL);
}
/****************************************************
 * 函数名称:
 * 功能描述:
 * 输入参数:
 * 输出参数:
 * 返 回 值:
 * 创建日期:
 ****************************************************/
void *Thread_DealMAP(void * arg)
{
	MessageFrame_t *pMapMsg;
	tMapInfo lMapInfo;

	AppDupMAP_id();
	int nTxAid = 0;
	int nRet = 0;
	uint8_t desbuf[2000];
	uint32_t deslen;
	memset(&g_MapInfo, 0x00, sizeof(tMapInfo));
	memset(&g_MapInfo_SL, 0x00, sizeof(tMapInfo));
	memset(&lMapInfo, 0x00, sizeof(tMapInfo));
	while (1)
	{
		usleep(1000);
		sem_wait(&g_Queue_Recv_map.m_product_number);
		{
			pMapMsg = ALLOC_OBJ(MessageFrame_t, 1);
			nRet = GetLstMapData(&nTxAid, pMapMsg);
			if (nRet != 0)
			{
				printf("MAP continue;\n");
				ASN_STRUCT_FREE(asn_DEF_MessageFrame, pMapMsg);
				continue;
			}
#if __PRINT_DEBUGLOG_
			xer_fprint(stdout, &asn_DEF_MessageFrame, pMapMsg);
#endif
			memset(&lMapInfo, 0x00, sizeof(tMapInfo));
			//提取MAP数据
			GetMapInfo(&lMapInfo, pMapMsg);
			ASN_STRUCT_FREE(asn_DEF_MessageFrame, pMapMsg);

			ClearMapInfo(&g_MapInfo);
			ClearMapInfo(&g_MapInfo_SL);
			if(lMapInfo.NodesCnt > 1)
			{
				cpyMapInfo(&lMapInfo, &g_MapInfo);
				cpyMapInfo(&lMapInfo, &g_MapInfo_SL);
			}
			else
			{
				cpyMapInfo(&lMapInfo, &g_MapInfo);
				cpyMapInfo(&lMapInfo, &g_MapInfo_SL);
			}
			ClearMapInfo(&lMapInfo);

			//使用当前MAP帧组D7帧
			EncodeD7(g_MapInfo, desbuf, &deslen);

			//数据发送
			fun_SendData2Pad((nint8_t *) desbuf, deslen);
		}
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
int Fun_Start_DealMAP(tStateStore *pApp)
{
#if __READ_XMLFILE
	//从文件中读取MAP信息
	GetMapInfo_FromXml(pApp);
#else
	//创建线程接收MAP数据
	pthread_create(&tidp_DealMAP, NULL, Thread_DealMAP, pApp);
#endif
	return 0;
}
