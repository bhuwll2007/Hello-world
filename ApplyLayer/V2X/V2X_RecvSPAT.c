/*
 * V2X_RecvSPAT.c
 *
 *  Created on: May 25, 2018
 *      Author: root
 */

#include "V2X_RecvSPAT.h"
#define __PRINT_RECVSPAT_DEBUGLOG_ 0
#define __Print_SpatMessageDealTime_ 0

void Out_SpeedGudie(tSpeedGudie_Input SG)
{
	printf("%f\n", SG.Lng);
	printf("%f\n", SG.Lat);
	printf("%d\n", SG.PhaseCnt);
	printf("------------------------------------\n\n");
}

void Out_SpatMsg(tSpatMsg pSpatInfo)
{
	int i, j;
	printf("%d\n", pSpatInfo.IntersectionCnt);
	for (i = 0; i < pSpatInfo.IntersectionCnt; i++)
	{
		printf("\t%d\n", pSpatInfo.pIntersectionInfo[i].Cycle);
		printf("\t%d\n", pSpatInfo.pIntersectionInfo[i].ID);
		printf("\t%d\n", pSpatInfo.pIntersectionInfo[i].Status);
		for (j = 0; j < pSpatInfo.pIntersectionInfo[i].PhaseCnt; j++)
		{
			printf("ID:\t\t%d\n",
					pSpatInfo.pIntersectionInfo[i].pPhaseInfo[j].ID);
			printf("GreenTime:\t\t%d\n",
					pSpatInfo.pIntersectionInfo[i].pPhaseInfo[j].GreenTime);
			printf("YellowTime:\t\t%d\n",
					pSpatInfo.pIntersectionInfo[i].pPhaseInfo[j].YellowTime);
			printf("RedTime:\t\t%d\n",
					pSpatInfo.pIntersectionInfo[i].pPhaseInfo[j].RedTime);
			printf("Status:\t\t%d\n",
					pSpatInfo.pIntersectionInfo[i].pPhaseInfo[j].Status);
			printf("TimeLeft\t\t%d\n",
					pSpatInfo.pIntersectionInfo[i].pPhaseInfo[j].TimeLeft);
			printf("------------------------------------\n\n");
		}
	}
}

void Out_Result_Spat(tResult_Spat Res)
{
	printf("wType\t%d\n", Res.wType);
	printf("wStr\t%d\n", Res.wStr);
	printf("wPrio\t%d\n", Res.wPrio);
	printf("%d\n", Res.Cycle);
	printf("%d\n", Res.Distance);
	printf("%d\n", Res.RelBearing);
	printf("%d\n", Res.PhaseCnt);
	printf("------------------------------------\n\n");
}

/****************************************************
 * 函数名称:
 * 功能描述:
 * 输入参数:
 * 输出参数:
 * 返 回 值:
 * 创建日期:
 ****************************************************/
void *Thread_DealSPAT(void * arg)
{
	MessageFrame_t *pSpatMsg;
	tSpatMsg pSpatInfo;
	tVehData HV;
	tSpeedGudie_Input pSpeedGudie_Input;
	tResult_Spat Res;
	uint8_t desbuf[2000];
	uint32_t deslen;

	AppDupSPAT_id();

	int nTxAid = 0;
	while (1)
	{
		usleep(1000);
		sem_wait(&g_Queue_Recv_spat.m_product_number);
		{
			pSpatMsg = ALLOC_OBJ(MessageFrame_t, 1);
			GetLstSpatData(&nTxAid, pSpatMsg);

			if (pSpatMsg->present != MessageFrame_PR_spatFrame)
			{
				printf("SPAT continue;\n");
				ASN_STRUCT_FREE(asn_DEF_MessageFrame, pSpatMsg);
				continue;
			}
#if __PRINT_RECVSPAT_DEBUGLOG_
			xer_fprint(stdout, &asn_DEF_MessageFrame, pSpatMsg);
#endif
			//获取本车信息
			getHvInfo(&HV);
			MapMatch_Pojcet_test(g_MapInfo, &HV);
//			MapMatch_Pojcet(g_MapInfo, &HV);
//			HV.BackNode = 4;
//			HV.ForwardNode = 3;
#if __PRINT_RECVSPAT_DEBUGLOG_
			printf("HV.ForwardNode = %d,HV.BackNode = %d\n",HV.ForwardNode,HV.BackNode);
#endif
			//提取SPAT信息
			GetSpatInfo(pSpatMsg, &pSpatInfo);
			ASN_STRUCT_FREE(asn_DEF_MessageFrame, pSpatMsg);

			//Out_SpatMsg(pSpatInfo);
			GetForward_Intersection(&HV, &pSpatInfo, &g_MapInfo,
					&pSpeedGudie_Input);

#if __PRINT_RECVSPAT_DEBUGLOG_
			printf("==================================pSpeedGudie_Input.PhaseCnt = %d \n",
					pSpeedGudie_Input.PhaseCnt);
#endif
			if (pSpeedGudie_Input.PhaseCnt <= 0)
			{
				ClearSpatInfo(&pSpatInfo);
				continue;
			}
			Res = SpeedGuide(&HV, &pSpeedGudie_Input);
			if (Res.Distance <= 0)
			{
				Res.Distance = 0;
			}

			memset(desbuf, 0x00, sizeof(desbuf));
			EncodeD6_2VehYuTong(HV, pSpeedGudie_Input, Res, desbuf, &deslen);
			fun_SendData2Pad((nint8_t *) desbuf, deslen);
			
			memset(desbuf, 0x00, sizeof(desbuf));
			EncodeD6_YuTong(HV, pSpeedGudie_Input, Res, desbuf, &deslen);

			//存至全局变量
			g_dSpatLat = pSpeedGudie_Input.Lat;
			g_dSpatLong = pSpeedGudie_Input.Lng;

//			printf("红绿灯显示标志 Res.wStr = %d\n", Res.wStr);

//红绿灯显示标志，如果算法计算出来（不需要提醒）且（在红绿灯路口处），则状态位设置为1，表示显示
			if ((Res.wStr == -1) && Res.Distance <= 0)
			{
				desbuf[5] = 1;
			}

			if (Res.Distance <= 0)
			{
				Res.Distance = 0;
				desbuf[5] = 1;
			}
			//向TCP客户端发送数据
			fun_SendData2Pad((nint8_t *) desbuf, deslen);
			ClearSpatInfo(&pSpatInfo);
		}
		usleep(1000);
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
int Fun_Start_DealSPAT(tStateStore *pApp)
{
	pthread_create(&tidp_DealSPAT, NULL, Thread_DealSPAT, pApp);
	return 0;
}
