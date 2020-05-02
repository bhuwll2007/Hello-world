/*
 * V2X_RecvRSM.c
 *
 *  Created on: May 25, 2018
 *      Author: root
 */

#include "V2X_RecvRSM.h"
#define __PRINT_DEBUGLOG_ 0
#define __Print_RsmMessageDealTime_ 0
#define __RSM_SENDJSON_MODE 1
#include <signal.h>
#define RSMVehMaxNum 60	//记录的最多的RSM信息个数
//#define RecentVehTypeNum 10	//记录的最近的车辆类型个数

#define NFeature 2 //特征个数

int OutVehInfo1(tVehData Veh) {
#if __PRINT_DEBUGLOG_
	printf("-------------------------BEGIN--------------------------------\n");
	printf("Id = %s  ", Veh.Id);
	printf("Longitude = %f  ", Veh.Longitude);
	printf("Latitude = %f  ", Veh.Latitude);
	printf("Altitude = %f  ", Veh.Altitude);
	printf("Speed = %f  ", Veh.Speed);
	printf("Heading = %f\n", Veh.Heading);

	printf("VehicleWidth = %f  ", Veh.VehicleWidth);
	printf("Length = %f  ", Veh.VehicleLength);
	printf("Hight = %f  ", Veh.VehicleHight);
//	printf("VehicleWheelDis = %f  ", Veh.VehicleWheelDis);
	printf("VehClass = %d\n", Veh.VehicleClass);
	printf("-------------------------END--------------------------------\n");
#endif
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
int getRsmInfo(MessageFrame_t *pRsmMsg, tVehData *RV) {
	int nTmpi = 0;
	int nparticCount = pRsmMsg->choice.rsmFrame.participants.list.count;

	ParticipantData_t *pParticipantData = NULL;
	for (nTmpi = 0; nTmpi < nparticCount; nTmpi++) {
		memset(&(RV[nTmpi].Id), 0x00, sizeof(tVehData));

		pParticipantData =
				pRsmMsg->choice.rsmFrame.participants.list.array[nTmpi];

		memset(RV[nTmpi].Id, 0x00, 4); // = 0;
		memcpy(RV[nTmpi].Id, &(pParticipantData->ptcId), 4);

		RV[nTmpi].Latitude =
				((*pParticipantData).pos.offsetLL.choice.position_LatLon.lat
						* 1.0) / 10000000;
		RV[nTmpi].Longitude =
				((*pParticipantData).pos.offsetLL.choice.position_LatLon.lon
						* 1.0) / 10000000;
		RV[nTmpi].Heading = ((*pParticipantData).heading * 0.0125);
		RV[nTmpi].Speed = (*pParticipantData).speed * 0.02;

		RV[nTmpi].VehicleLength = ((*pParticipantData).size.length * 1.0) / 100;
		RV[nTmpi].VehicleWidth = ((*pParticipantData).size.width * 1.0) / 100;

		RV[nTmpi].VehicleHight =
				(*pParticipantData).size.height == NULL ?
						0 : ((*(*pParticipantData).size.height) * 1.0) / 20;

		RV[nTmpi].BackNode = 0;
		RV[nTmpi].ForwardNode = 0;
		RV[nTmpi].VehicleClass = (*pParticipantData).ptcType;
		OutVehInfo1(RV[nTmpi]);
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
void Time_DealRSM() {
	MessageFrame_t *pRSMMsg;
	tVehData pRV[20];
	uint32_t nParticCount = 0;
	uint32_t reslen;
	uint8_t res[10240];
	tVehData HV;
	int nTxAid = 0;

	int nRet = 0;
	usleep(1000);
	sem_wait(&g_Queue_Recv_rsm.m_product_number);
	{
		pRSMMsg = ALLOC_OBJ(MessageFrame_t, 1);
		GetLstRsmData(&nTxAid, pRSMMsg);
		if (pRSMMsg->present != MessageFrame_PR_rsmFrame) {
			printf("RSM continue;\n");
			ASN_STRUCT_FREE(asn_DEF_MessageFrame, pRSMMsg);
			return;
		}
		nParticCount = pRSMMsg->choice.rsmFrame.participants.list.count;
		nParticCount = 1;

		//获取本车信息
		getHvInfo(&HV);

		//计算距离
		double dDistance = 0;
		double Laser_Lat = 0;
		double Laser_Long = 0;
//		if ((g_RSM_Parameter.Laser_Lat <= 1.0)
//				&& (g_RSM_Parameter.Laser_Long <= 1.0))
//		{
//			Laser_Lat = pRSMMsg->choice.rsmFrame.refPos.lat / 10000000.0;
//			Laser_Long = pRSMMsg->choice.rsmFrame.refPos.Long / 10000000.0;
//		} else
//		{
//			Laser_Lat = g_RSM_Parameter.Laser_Lat;
//			Laser_Long = g_RSM_Parameter.Laser_Long;
//		}
//		Laser_Lat = g_RSM_Parameter.Laser_Lat;
//		Laser_Long = g_RSM_Parameter.Laser_Long;
//		dDistance = FG_Getdistance(HV.Latitude, HV.Longitude, Laser_Lat,
//				Laser_Long);
//		if (dDistance > g_RSM_Parameter.Laser_Display)
//		{
//			nParticCount = 0;
//		}
//		int RelBear = FG_GetRelBear(HV.Latitude, HV.Longitude, Laser_Lat,
//				Laser_Long, HV.Heading);
//		if (RelBear > 90 && RelBear < 270)
//		{
//			nParticCount = 0;
//		}
		getRsmInfo(pRSMMsg, pRV);
		Laser_Lat = pRV[0].Latitude; //.Laser_Lat;
		Laser_Long = pRV[0].Longitude; //g_RSM_Parameter.Laser_Long;
		dDistance = FG_Getdistance(HV.Latitude, HV.Longitude, Laser_Lat,
				Laser_Long);
		if (dDistance > g_RSM_Parameter.Laser_Display) {
			nParticCount = 0;
		}
//		int RelBear = FG_GetRelBear(HV.Latitude, HV.Longitude, Laser_Lat,
//				Laser_Long, HV.Heading);
//		if (RelBear > 90 && RelBear < 270)
//		{
//			nParticCount = 0;
//		}
		if (nParticCount > 0) {
			//组帧
			Point RV_Coord = gpsToRelativeLoc(HV.Longitude, HV.Latitude,
					HV.Heading, pRV[0].Longitude, pRV[0].Latitude);

			uint8_t local = CalVehicleLocation(HV, pRV[0], RV_Coord);

			pRV[0].BackNode = local;
			if ((local == FRONT_LOC_Right) || (local == FRONT_LOC_Left)
					|| (local == FRONT_LOC_DIRECT)) {
				nRet = EncodeD5(Laser_Lat, Laser_Long, HV, pRV, nParticCount,
						res, &reslen);
				if (nRet > 0) {
					fun_SendData2Pad((nint8_t *) res, reslen);
				}
			}
		} else {

		}
		ASN_STRUCT_FREE(asn_DEF_MessageFrame, pRSMMsg);
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
void *Thread_DealRSM(void * arg) {
	AppDupRSM_id();
	while (1) {
		Time_DealRSM();
		usleep(1000 * 5);
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
int Fun_Start_DealRSM(tStateStore *pApp) {
	pthread_create(&tidp_DealRSM, NULL, Thread_DealRSM, pApp);
	return 0;
}

