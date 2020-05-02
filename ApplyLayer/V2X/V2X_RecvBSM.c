/*
 * V2X_RecvBSM.c
 *
 *  Created on: May 25, 2018
 *      Author: root
 */

#include "V2X_RecvBSM.h"
#define __PRINT_DEBUGLOG_ 0

int getRvInfo(MessageFrame_t *pBsmMsg, tVehData *RV)
{
	memset(RV, 0x00, sizeof(tVehData));
	memset(RV->Id, 0x00, sizeof(RV->Id)); // = 0;
	if (pBsmMsg->choice.bsmFrame.id.buf != NULL)
	{
		memcpy(RV->Id, pBsmMsg->choice.bsmFrame.id.buf, sizeof(RV->Id));
	}
	RV->Latitude = (pBsmMsg->choice.bsmFrame.pos.lat * 1.0) / 10000000;
	RV->Longitude = (pBsmMsg->choice.bsmFrame.pos.Long * 1.0) / 10000000;
	RV->Heading = (pBsmMsg->choice.bsmFrame.heading * 0.0125);
	RV->Speed = pBsmMsg->choice.bsmFrame.speed * 0.02;

	RV->VehicleLength = (pBsmMsg->choice.bsmFrame.size.length * 1.0) / 100;
	RV->VehicleWidth = (pBsmMsg->choice.bsmFrame.size.width * 1.0) / 100;
	RV->VehicleHight =
			pBsmMsg->choice.bsmFrame.size.height == NULL ?
					0 : ((*pBsmMsg->choice.bsmFrame.size.height) * 1.0) / 20;

	if (pBsmMsg->choice.bsmFrame.safetyExt != NULL)
	{
		if (pBsmMsg->choice.bsmFrame.safetyExt->events != NULL)
		{
			if (pBsmMsg->choice.bsmFrame.safetyExt->events->buf != NULL)
			{
				uint8_t buff1[2];
				buff1[0] = pBsmMsg->choice.bsmFrame.safetyExt->events->buf[1];
				buff1[1] = pBsmMsg->choice.bsmFrame.safetyExt->events->buf[0];
				memcpy(&(RV->VehicleClass), buff1, 2);
			}
		}
		if (pBsmMsg->choice.bsmFrame.safetyExt->lights != NULL)
		{
			if (pBsmMsg->choice.bsmFrame.safetyExt->lights->buf != NULL)
			{
				uint8_t buff[2];
				buff[0] = pBsmMsg->choice.bsmFrame.safetyExt->lights->buf[1];
				buff[1] = pBsmMsg->choice.bsmFrame.safetyExt->lights->buf[0];
				memcpy(&(RV->Light), buff, 2);
				RV->Light = RV->Light >> 7;
			}
		}
	}
	RV->BreakPadel =
			pBsmMsg->choice.bsmFrame.brakes.brakePadel == NULL ?
					0 : (uint32_t) *pBsmMsg->choice.bsmFrame.brakes.brakePadel;

	RV->AuxBreakSystem =
			pBsmMsg->choice.bsmFrame.brakes.abs == NULL ?
					0 : (uint32_t) *pBsmMsg->choice.bsmFrame.brakes.abs;

	RV->BackNode = 0;
	RV->ForwardNode = 0;
//	RV->VehicleClass = pBsmMsg->choice.bsmFrame.vehicleClass.classification;

	if (RV->VehicleClass == 16)
	{
#if __PRINT_DEBUGLOG_
		xer_fprint(stdout, &asn_DEF_MessageFrame, pBsmMsg);
#endif
	}
	if ((RV->Id[0] == 0xE6)
			&& (RV->Id[1] == 0x99)
			&& (RV->Id[2] == 0x8B)
			&& (RV->Id[3] == 0x41)
			&& (RV->Id[4] == 0x31)
			&& (RV->Id[5] == 0x32)
			&& (RV->Id[6] == 0x33)
			&& (RV->Id[7] == 0x34))
	{
		RV->VehicleLength = 2.8;
		RV->VehicleWidth = 1.6;
	}
	return 0;
}

int OutVehInfo(tVehData Veh)
{
#if __PRINT_DEBUGLOG_
	printf("-------------------------BEGIN--------------------------------\n");
	printf("uint32_t Id = %s\n", Veh.Id);
	printf("double Longitude = %f\n", Veh.Longitude);
	printf("double Latitude = %f\n", Veh.Latitude);
	printf("double Altitude = %f\n", Veh.Altitude);
	printf("double Speed = %f\n", Veh.Speed);
	printf("double Heading = %f\n", Veh.Heading);

	printf("double SteeringWheelAngle = %f\n", Veh.SteeringWheelAngle);
	printf("uint32_t BreakPadel = %d\n", Veh.BreakPadel);
	printf("uint32_t AuxBreakSystem = %d\n", Veh.AuxBreakSystem);
	printf("uint16_t Light = %d\n", Veh.Light);

	printf("double VehicleWidth = %f\n", Veh.VehicleWidth);
	printf("double VehicleLength = %f\n", Veh.VehicleLength);
	printf("double VehicleHight = %f\n", Veh.VehicleHight);
	printf("double VehicleWheelDis = %f\n", Veh.VehicleWheelDis);
	printf("uint16_t VehicleClass = %d\n", Veh.VehicleClass);
	printf("uint32_t ForwardNode = %d\n", Veh.ForwardNode);
	printf("uint32_t BackNode = %d\n", Veh.BackNode);

	printf("PointHistory_double Lat = %f\n", Veh.pointHistory.Lat);
	printf("PointHistory_double Lng = %f\n", Veh.pointHistory.Lng);
	printf("PointHistory_double Heading = %f\n", Veh.pointHistory.Heading);
	printf("PointHistory_double Speed = %f\n", Veh.pointHistory.Speed);

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
void *Thread_DealBSM(void * arg)
{
	MessageFrame_t *pBsmMsg = NULL;
	tVehData HV, RV;
	tResult_Bsm CResult;
	AppDupBSM_id();

	uint32_t reslen = 0;
	uint8_t res[1024];
	int nTxAid = 0;

	memset(&HV, 0x00, sizeof(tVehData));
	memset(&RV, 0x00, sizeof(tVehData));
	memset(&CResult, 0x00, sizeof(tResult_Bsm));
	memset(res, 0x00, sizeof(res));

	while (1)
	{
		usleep(1000);
		sem_wait(&g_Queue_Recv_bsm.m_product_number);
		{
			pBsmMsg = ALLOC_OBJ(MessageFrame_t, 1);
			int nRet = GetLstBsmData(&nTxAid, pBsmMsg);
			if (nRet != 0)
			{
				printf("BSM continue;\n");
				ASN_STRUCT_FREE(asn_DEF_MessageFrame, pBsmMsg);
				continue;
			}
			getHvInfo(&HV);
#if __PRINT_DEBUGLOG_
			OutVehInfo(HV);
#endif
#if __PRINT_DEBUGLOG_
			asn_fprint(stdout, &asn_DEF_MessageFrame, pBsmMsg);
#endif

			getRvInfo(pBsmMsg, &RV);
			ASN_STRUCT_FREE(asn_DEF_MessageFrame, pBsmMsg);

#if __PRINT_DEBUGLOG_
			OutVehInfo(RV);
#endif
			if (g_IsMapMode == 0)
			{
				MapMatch_Pojcet(g_MapInfo, &HV);
				MapMatch_Pojcet(g_MapInfo, &RV);
			}
			CResult = BSM_VehicleActiveSafety(HV, RV, g_IsMapMode);
			//DataBase_Insert_BSM(sqldb, HV, RV, CResult);
			if ((CResult.wStr != STR_DEFAULT)/* && (CResult.wStr != AVW_Warning)*/)
			{
				EncodeD3(HV, RV, CResult, res, &reslen);

				fun_SendData2Pad((nint8_t *) res, reslen);
			}

			g_UpD0Data.SlfVehData.dSelfLat = HV.Latitude;
			g_UpD0Data.SlfVehData.dSelfLng = HV.Longitude;
//			if(CResult.Distance < 100)
			{
				AddInfo(&g_UpD0Data, &RV, CResult.wStr);
			}
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
int Fun_Start_DealBSM(tStateStore *pApp)
{
	pthread_create(&tidp_DealBSM, NULL, Thread_DealBSM, pApp);
	return 0;
}
