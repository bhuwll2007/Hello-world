/*
 * AroundVeh.c
 *
 *  Created on: Jun 7, 2018
 *      Author: root
 */

#include "AroundVeh.h"
#define BQMODE 0
int AddInfo(UpD0Data *pUpD0Data, tVehData *RV, int nIsSafe)
{
	int i = 0;
	int j = 0;
	for (i = 0; i < pUpD0Data->nAroundVehNum; i++)
	{
		if ((pUpD0Data->ArdVehData[i].UEID[0] == RV->Id[0])
				&& (pUpD0Data->ArdVehData[i].UEID[1] == RV->Id[1])
				&& (pUpD0Data->ArdVehData[i].UEID[2] == RV->Id[2])
				&& (pUpD0Data->ArdVehData[i].UEID[3] == RV->Id[3])
				&& (pUpD0Data->ArdVehData[i].UEID[4] == RV->Id[4])
				&& (pUpD0Data->ArdVehData[i].UEID[5] == RV->Id[5])
				&& (pUpD0Data->ArdVehData[i].UEID[6] == RV->Id[6])
				&& (pUpD0Data->ArdVehData[i].UEID[7] == RV->Id[7]))
		{
			break;
		}
	}
	if (i < pUpD0Data->nAroundVehNum)
	{

	} else
	{
		for (j = 0; j < pUpD0Data->nAroundVehNum; j++)
		{
			if (pUpD0Data->ArdVehData[j].IsLive <= 0)
				break;
		}
		if (j == i)
		{
			if (pUpD0Data->nAroundVehNum < MAXAROUNDVEHNUM)
				pUpD0Data->nAroundVehNum++;
			else
			{
				return 1;
			}
		} else
		{
			i = j;
		}
	}
	pthread_mutex_lock(&mutex_AroudVeh);
	pUpD0Data->ArdVehData[i].IsLive = VEHLiveMaxTime;
	pUpD0Data->ArdVehData[i].IsSafe = nIsSafe;

	memcpy(&pUpD0Data->ArdVehData[i].UEID[0], RV->Id, sizeof(RV->Id));
	pUpD0Data->ArdVehData[i].dDuiLng = RV->Longitude;
	pUpD0Data->ArdVehData[i].dDuiLat = RV->Latitude;

	pUpD0Data->ArdVehData[i].nDistance = (uint16_t) FG_Getdistance1(
			RV->Latitude, RV->Longitude, pUpD0Data->SlfVehData.dSelfLat,
			pUpD0Data->SlfVehData.dSelfLng);
	if (pUpD0Data->ArdVehData[i].nDistance > RV->VehicleLength / 2)
		pUpD0Data->ArdVehData[i].nDistance -= RV->VehicleLength / 2;

	pUpD0Data->ArdVehData[i].nDuiHigh = (nint16_t) RV->Altitude;
	pUpD0Data->ArdVehData[i].nDuiSpeed = (uint16_t) (3.6 * RV->Speed);
	pUpD0Data->ArdVehData[i].nDuiTrack = (nint16_t) RV->Heading;

	pthread_mutex_unlock(&mutex_AroudVeh);
	return 0;
}
/****************************************************
 * 函数名称:void *Thread_AroundVeh(void * arg)
 * 功能描述:定时上传周围车辆相关信息的线程，目前设置的每隔100ms上传一次
 * 输入参数:空
 * 输出参数:空
 * 返 回 值:空
 * 创建日期:
 ****************************************************/
void *Thread_AroundVeh(void * arg)
{
	int nTmpj = 0;
	uint8_t desbuf[1000];
	uint32_t deslen = 0;

	tVehData HV;
//	uint8_t res[1000];
//	uint32_t reslen = 0;
	struct timeval delay;
	memset(&g_UpD0Data.SlfVehData.UEID[0], 0x00, sizeof(UpD0Data));
	while (1)
	{
		msTimeDelay(delay, g_tAPP_RunParam.nInterval_UpD0);

		//该流程中增加检测当前地图的超速问题
		getHvInfo(&HV);

		//tResult_SL lResult_SL;
		tResult_SL lResult_SL = MapSL(&HV, &g_MapInfo_SL);
		if (lResult_SL.wStr != V2I_STR_DEFAULT)
		{
			AddMapSLInfo(&g_UpD9Data, lResult_SL);
			g_u16LimitHighSpeed = lResult_SL.VehSpeedLimitHigh;
		} else
		{
			g_u16LimitHighSpeed = 200;
		}

		//获取本车信息
		getHvInfo(&HV);

		memcpy(&g_UpD0Data.SlfVehData.UEID[0], HV.Id, sizeof(HV.Id));

		g_UpD0Data.SlfVehData.dSelfLat = HV.Latitude;
		g_UpD0Data.SlfVehData.dSelfLng = HV.Longitude;
		g_UpD0Data.SlfVehData.nSelfHigh = (int) HV.Altitude;
		g_UpD0Data.SlfVehData.nSelfSpeed = (int) (HV.Speed * 3.6);
		g_UpD0Data.SlfVehData.nLimitHighSpeed = g_u16LimitHighSpeed;
		g_UpD0Data.SlfVehData.nSelfTrack = (int) HV.Heading;

		double Distance = FG_Getdistance(HV.Latitude, HV.Longitude, g_dSpatLat,
				g_dSpatLong);

		//	CResult.Distance = (uint32_t) (Distance - pHV->VehicleLength / 2) - 20; // 距离为车头到停车线的距离
		g_UpD0Data.SlfVehData.nSpatDistence = (Distance - HV.VehicleLength / 2)
				- g_Spat_Parameter.DistoCrossRdMid;

		if (g_UpD0Data.SlfVehData.nSpatDistence < 0)
			g_UpD0Data.SlfVehData.nSpatDistence = 0;


		pthread_mutex_lock(&mutex_AroudVeh);

		EncodeD0(g_UpD0Data, desbuf, &deslen);

		for (nTmpj = 0; nTmpj < g_UpD0Data.nAroundVehNum; nTmpj++)
		{
			if (g_UpD0Data.ArdVehData[nTmpj].IsLive > 0)
			{
				g_UpD0Data.ArdVehData[nTmpj].IsLive--;
			}
		}
		pthread_mutex_unlock(&mutex_AroudVeh);
		fun_SendData2Pad((nint8_t *) desbuf, deslen);
		
		//insert into database
		if(sqldb != NULL)
		{
			GetDevIPAdd("lmi40", g_stcSpatInfo.acLmi40IpAddr);
			struct netdev_stats wl_pstats;
			get_devstats("lmi40", &wl_pstats);
			g_stcSpatInfo.ullLmi40RxBytes = wl_pstats.rx_bytes_m;
			g_stcSpatInfo.ullLmi40TxBytes = wl_pstats.tx_bytes_m;
			modem_status_t st;
			GetModemStatus(&st);
			g_stcSpatInfo.nModemStatus = st.value;
			DataBase_Insert_YuTong(sqldb, g_stcSpatInfo);
		}
		memset(&g_stcSpatInfo.nAddRecvbytes, 0, sizeof(int)*4);
	}
	return 0;
}

/****************************************************
 * 函数名称:int InitAroundVeh(int nSleepTime)
 * 功能描述:初始化上传周围车辆信息模块
 * 输入参数:int nSleepTime 上传间隔事件，单位ms。目前未启用
 * 输出参数:无
 * 返 回 值:0，无意义
 * 创建日期:
 ****************************************************/
int InitAroundVeh()
{
	pthread_mutex_init(&mutex_AroudVeh, NULL);
	pthread_create(&tidp_AroundVeh, NULL, Thread_AroundVeh, NULL);
	return 0;
}
