/*
 * AroundRsi.c
 *
 *  Created on: Jun 7, 2018
 *      Author: root
 */

#include "AroundRsi.h"

/****************************************************
 * 函数名称:int AddRsiInfo(UpD9Data *pUpD9Data, tRsiMsg lRsiInfo, tResult_Rsi lResult_Rsi)
 * 功能描述:将RSI事件增加到记录RSI事件列表的全局变量中
 * 输入参数:
 * 输出参数:
 * 返 回 值:
 * 创建日期:
 ****************************************************/
int AddRsiInfo(UpD9Data *pUpD9Data, tRsiMsg lRsiInfo, tResult_Rsi lResult_Rsi)
{
	pthread_mutex_lock(&mutex_AroudRsi);
	int l_nTmpi = 0;
	int l_nTmpj = 0;

	//函数返回值
	int nRet = 0;
	//判断列表中是否存在
	for (l_nTmpi = 0; l_nTmpi < pUpD9Data->nAroundRsiNum; l_nTmpi++)
	{
		if ((pUpD9Data->ArdRsiData[l_nTmpi].RsiType == lRsiInfo.MsgType)
				&& (pUpD9Data->ArdRsiData[l_nTmpi].dDuiLat1
						== lRsiInfo.AlertPathList[0].AlertPath.Latitude)
				&& (pUpD9Data->ArdRsiData[l_nTmpi].dDuiLng1
						== lRsiInfo.AlertPathList[0].AlertPath.Longitude)
				&& (pUpD9Data->ArdRsiData[l_nTmpi].dDuiLat2
						== lRsiInfo.AlertPathList[1].AlertPath.Latitude)
				&& (pUpD9Data->ArdRsiData[l_nTmpi].dDuiLng2
						== lRsiInfo.AlertPathList[1].AlertPath.Longitude))
		{
			break;
		}
	}

	if (l_nTmpi < pUpD9Data->nAroundRsiNum)
	{

	}
	else
	{
		for (l_nTmpj = 0; l_nTmpj < pUpD9Data->nAroundRsiNum; l_nTmpj++)
		{
			if (pUpD9Data->ArdRsiData[l_nTmpj].IsLive <= 0)
				break;
		}
		if (l_nTmpj == l_nTmpi)
		{
			if (pUpD9Data->nAroundRsiNum < MAXAROUNDRSINUM - 1)
				pUpD9Data->nAroundRsiNum++;
			else
			{
				nRet = -1;
				goto ERROR_END;
			}
		}
		else
		{
			l_nTmpi = l_nTmpj;
			memset(pUpD9Data->ArdRsiData[l_nTmpi].ID, 0x00,
					sizeof(pUpD9Data->ArdRsiData[l_nTmpi].ID));
		}
	}

	///更新当前列表中的RSI信息

	//唯一事件ID
//	memset(pUpD9Data->ArdRsiData[l_nTmpi].ID, 0x00,
//			sizeof(pUpD9Data->ArdRsiData[l_nTmpi].ID));
	sprintf(pUpD9Data->ArdRsiData[l_nTmpi].ID, "%03d_%f_%f", (int) lRsiInfo.MsgType,
			lRsiInfo.AlertPathList[0].AlertPath.Longitude,
			lRsiInfo.AlertPathList[0].AlertPath.Latitude);

	//RSI事件类型
	pUpD9Data->ArdRsiData[l_nTmpi].RsiType = lRsiInfo.MsgType;
	//RSI生存周期
	pUpD9Data->ArdRsiData[l_nTmpi].IsLive = RSILiveMaxTime;

	//RSI事件经纬度
	pUpD9Data->ArdRsiData[l_nTmpi].dDuiLat1 =
			lRsiInfo.AlertPathList[0].AlertPath.Latitude;
	pUpD9Data->ArdRsiData[l_nTmpi].dDuiLng1 =
			lRsiInfo.AlertPathList[0].AlertPath.Longitude;
	pUpD9Data->ArdRsiData[l_nTmpi].dDuiLat2 =
			lRsiInfo.AlertPathList[1].AlertPath.Latitude;
	pUpD9Data->ArdRsiData[l_nTmpi].dDuiLng2 =
			lRsiInfo.AlertPathList[1].AlertPath.Longitude;

//	uint16_t nDistance1 = pUpD9Data->ArdRsiData[l_nTmpi].nDistance;
//	uint16_t nDistance2 = (uint16_t) (lResult_Rsi.dis);

//RSI事件距离
	pUpD9Data->ArdRsiData[l_nTmpi].nDistance = (uint16_t) (lResult_Rsi.dis);

	//显示端播报的距离
	pUpD9Data->ArdRsiData[l_nTmpi].nSpeakDis = (uint16_t) (lResult_Rsi.SpeakDis);

	//RSI事件描述
	memcpy(&pUpD9Data->ArdRsiData[l_nTmpi].RsiDscpInfo.TagVal,
			&lResult_Rsi.DscpInfo.TagVal, sizeof(tRsiDscpInfo));

	//优先级
	pUpD9Data->ArdRsiData[l_nTmpi].uPriority = lResult_Rsi.wPrio;

	//事件状态位
	pUpD9Data->ArdRsiData[l_nTmpi].WarningType = lResult_Rsi.WarningType;

	//方位角
	pUpD9Data->ArdRsiData[l_nTmpi].RelPostion = lResult_Rsi.RelPos;

	//RSI事件类型
	pUpD9Data->ArdRsiData[l_nTmpi].RsiA = lResult_Rsi.wStr;

	// 如果当前不需要提醒，则标志当前生存状态为 4
//	if (pUpD9Data->ArdRsiData[l_nTmpi].WarningType == 0)
//	{
//		if ((pUpD9Data->ArdRsiData[l_nTmpi].RelPostion == 44)
//				|| ((nDistance1 != 0) && (nDistance1 < nDistance2)))
//			pUpD9Data->ArdRsiData[l_nTmpi].IsLive = 4;
//	}
//	if ((nDistance1 != 0) && (nDistance1 < nDistance2))
//	{
//		pUpD9Data->ArdRsiData[l_nTmpi].RelPostion = 44;
//	}
	//当前路段信息不需要播报,显示端的下面列表也不需要显示
//	if (pUpD9Data->ArdRsiData[l_nTmpi].WarningType == InsideWarning)
//	{
//		pUpD9Data->ArdRsiData[l_nTmpi].IsLive = 4;
//	}

	ERROR_END:

	pthread_mutex_unlock(&mutex_AroudRsi);

	return nRet;
}
int AddMapSLInfo(UpD9Data *pUpD9Data, tResult_SL lResult_SL)
{
	int nRet = 0;
	pthread_mutex_lock(&mutex_AroudRsi);

	int l_nTmpi = 0;
	int l_nTmpj = 0;
	//判断列表中是否存在
	for (l_nTmpi = 0; l_nTmpi < pUpD9Data->nAroundRsiNum; l_nTmpi++)
	{
		if ((pUpD9Data->ArdRsiData[l_nTmpi].RsiType == 85)
				&& (pUpD9Data->ArdRsiData[l_nTmpi].dDuiLat1
						== lResult_SL.AlertPathList[0].AlertPath.Latitude)
				&& (pUpD9Data->ArdRsiData[l_nTmpi].dDuiLng1
						== lResult_SL.AlertPathList[0].AlertPath.Longitude)
				&& (pUpD9Data->ArdRsiData[l_nTmpi].dDuiLat2
						== lResult_SL.AlertPathList[1].AlertPath.Latitude)
				&& (pUpD9Data->ArdRsiData[l_nTmpi].dDuiLng2
						== lResult_SL.AlertPathList[1].AlertPath.Longitude))
		{
			break;
		}
	}
	if (l_nTmpi < pUpD9Data->nAroundRsiNum)
	{

	}
	else
	{
		for (l_nTmpj = 0; l_nTmpj < pUpD9Data->nAroundRsiNum; l_nTmpj++)
		{
			if (pUpD9Data->ArdRsiData[l_nTmpj].IsLive <= 0)
			{
				break;
			}
		}
		if (l_nTmpj == l_nTmpi)
		{
			if (pUpD9Data->nAroundRsiNum < MAXAROUNDRSINUM - 1)
				pUpD9Data->nAroundRsiNum++;
			else
			{
				nRet = -1;
				goto ERROR_END;
			}
		}
		else
		{
			l_nTmpi = l_nTmpj;
			memset(pUpD9Data->ArdRsiData[l_nTmpi].ID, 0x00,
					sizeof(pUpD9Data->ArdRsiData[l_nTmpi].ID));
		}
	}

	///更新当前列表中的RSI信息

	///唯一事件ID

	sprintf(pUpD9Data->ArdRsiData[l_nTmpi].ID, "%03d_%f_%f", 85,
			lResult_SL.AlertPathList[0].AlertPath.Longitude,
			lResult_SL.AlertPathList[0].AlertPath.Latitude);

	//RSI事件类型
	pUpD9Data->ArdRsiData[l_nTmpi].RsiType = 85;
	//RSI生存周期
	pUpD9Data->ArdRsiData[l_nTmpi].IsLive = RSILiveMaxTime;

	//RSI事件经纬度
	pUpD9Data->ArdRsiData[l_nTmpi].dDuiLat1 =
			lResult_SL.AlertPathList[0].AlertPath.Latitude;
	pUpD9Data->ArdRsiData[l_nTmpi].dDuiLng1 =
			lResult_SL.AlertPathList[0].AlertPath.Longitude;
	pUpD9Data->ArdRsiData[l_nTmpi].dDuiLat2 =
			lResult_SL.AlertPathList[1].AlertPath.Latitude;
	pUpD9Data->ArdRsiData[l_nTmpi].dDuiLng2 =
			lResult_SL.AlertPathList[1].AlertPath.Longitude;

	//RSI事件距离
	pUpD9Data->ArdRsiData[l_nTmpi].nDistance = 0;

	//显示端播报的距离
	pUpD9Data->ArdRsiData[l_nTmpi].nSpeakDis = 0;

	//RSI事件描述
	pUpD9Data->ArdRsiData[l_nTmpi].RsiDscpInfo.TagVal = lResult_SL.VehSpeedLimitHigh;
	pUpD9Data->ArdRsiData[l_nTmpi].RsiDscpInfo.AssisTag = 85;
	pUpD9Data->ArdRsiData[l_nTmpi].RsiDscpInfo.AssisVal = lResult_SL.VehSpeedLimitLow;

	//优先级
	pUpD9Data->ArdRsiData[l_nTmpi].uPriority = lResult_SL.wPrio;

	//事件状态位
	pUpD9Data->ArdRsiData[l_nTmpi].WarningType = lResult_SL.WarningType;

	//方位角
	pUpD9Data->ArdRsiData[l_nTmpi].RelPostion = 11;

	//RSI事件类型
	pUpD9Data->ArdRsiData[l_nTmpi].RsiA = lResult_SL.wStr;

	// 如果当前不需要提醒，则标志当前生存状态为 4
//	if ((pUpD9Data->ArdRsiData[l_nTmpi].WarningType == 2)
//			|| (pUpD9Data->ArdRsiData[l_nTmpi].WarningType == 6))
//	{
//	}
//	else
//	{
//		pUpD9Data->ArdRsiData[l_nTmpi].IsLive = 4;
//	}
	ERROR_END:

	pthread_mutex_unlock(&mutex_AroudRsi);

	return nRet;
}
/****************************************************
 * 函数名称:void *Thread_AroundRsi(void * arg)
 * 功能描述:定时上传周围RSI事件相关信息的线程，目前设置的每隔200ms上传一次
 * 输入参数:空
 * 输出参数:空
 * 返 回 值:空
 * 创建日期:
 ****************************************************/
void *Thread_AroundRsi(void * arg)
{
//	int nSleepTime = *(int *) arg;

	int nTmpj = 0;
	uint8_t desbuf[4000];
	uint32_t deslen = 0;

	uint8_t ntmpi = 0;
	memset(&g_UpD9Data.nAroundRsiNum, 0x00, sizeof(UpD9Data));

	tVehData HV;
	while (1)
	{
		usleep(g_tAPP_RunParam.nInterval_UpD9 * 1000);

		//获取本车当前信息
		getHvInfo(&HV);

		pthread_mutex_lock(&mutex_AroudRsi);
		for (ntmpi = 0; ntmpi < g_UpD9Data.nAroundRsiNum; ntmpi++)
		{
			//如果是点事件，那么重新计算距离
			//任何事件都重新计算距离（按照车与事件第一个点计算距离）
			g_UpD9Data.ArdRsiData[ntmpi].nDistance = (uint16_t) FG_Getdistance1(
					HV.Latitude, HV.Longitude,
					g_UpD9Data.ArdRsiData[ntmpi].dDuiLat1,
					g_UpD9Data.ArdRsiData[ntmpi].dDuiLng1);
//			if (g_UpD9Data.ArdRsiData[ntmpi].nDistance
//					< 2 * g_V2I_Parameter.IVSWarnDis)
//			{
//				//重新计算方向角
//				g_UpD9Data.ArdRsiData[ntmpi].RelPostion = GetPos(HV.Latitude,
//						HV.Longitude, g_UpD9Data.ArdRsiData[ntmpi].dDuiLat1,
//						g_UpD9Data.ArdRsiData[ntmpi].dDuiLng1, HV.Heading);
//				if (g_UpD9Data.ArdRsiData[ntmpi].RelPostion == 44)
//					g_UpD9Data.ArdRsiData[ntmpi].WarningType = 0;
//			}
//			//	printf("nDistance = %d\n",g_UpD9Data.ArdRsiData[ntmpi].nDistance);
//			if (g_UpD9Data.ArdRsiData[ntmpi].nDistance
//					>= 2 * g_V2I_Parameter.IVSWarnDis)
//			{
//				g_UpD9Data.ArdRsiData[ntmpi].RelPostion = 44;
//			}
			//printf("RelPos= %d \n", lResult_Rsi.RelPos);
//			if(g_UpD9Data.ArdRsiData[ntmpi].nDistance < 2 * g_V2I_Parameter.IVSWarnDis)
//			{
//				//lRsiInfo.MsgType =
//				if (g_IsMapMode == 1)
//				{
//					lResult_Rsi = IVS(&HV, &lRsiInfo);
//
//					if ((lResult_Rsi.dis > 2 * g_V2I_Parameter.IVSWarnDis)
//							&& (lResult_Rsi.RelPos != 44))
//					{
//					//	printf("RelPos= %d  >>>> ", lResult_Rsi.RelPos);
//						lResult_Rsi.RelPos = 44;
//					}
//					//printf("RelPos= %d \n", lResult_Rsi.RelPos);
//				}
//				else
//				{
//					MapMatch_Pojcet(g_MapInfo, &HV);
//					lResult_Rsi = IVS_MapMatch(&HV, &lRsiInfo, g_MapInfo);
//
//					if ((lResult_Rsi.dis > 2 * g_V2I_Parameter.IVSWarnDis)
//							&& (lResult_Rsi.RelPos != 44))
//					{
//						lResult_Rsi.RelPos = 44;
//					}
//				}
//			}
		}
		EncodeD9(g_UpD9Data, desbuf, &deslen);

		pthread_mutex_unlock(&mutex_AroudRsi);

		fun_SendData2Pad((nint8_t *) desbuf, deslen);

		pthread_mutex_lock(&mutex_AroudRsi);
		for (nTmpj = 0; nTmpj < g_UpD9Data.nAroundRsiNum; nTmpj++)
		{
			if (g_UpD9Data.ArdRsiData[nTmpj].IsLive > 0)
				g_UpD9Data.ArdRsiData[nTmpj].IsLive--;
		}
		pthread_mutex_unlock(&mutex_AroudRsi);

	}
	return 0;
}

/****************************************************
 * 函数名称:int InitAroundRsi(int nSleepTime)
 * 功能描述:初始化上传周围RSI事件信息模块
 * 输入参数:int nSleepTime 上传间隔事件，单位ms。目前未启用
 * 输出参数:无
 * 返 回 值:0，无意义
 * 创建日期:
 ****************************************************/
int InitAroundRsi()
{
	pthread_mutex_init(&mutex_AroudRsi, NULL);
	pthread_create(&tidp_AroundRsi, NULL, Thread_AroundRsi, NULL);
	return 0;
}
