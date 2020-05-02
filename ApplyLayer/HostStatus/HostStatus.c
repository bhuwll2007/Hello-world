/*
 * HostStatus.c
 *
 *  Created on: May 25, 2018
 *      Author: root
 */
#include "HostStatus.h"

uint64_t g_u64TimeStamp_Start = 0;

//extern void *Thread_HostStatus(void * arg);
/****************************************************
 * 函数名称:
 * 功能描述:
 * 输入参数:
 * 输出参数:
 * 返 回 值:
 * 创建日期:
 ****************************************************/
int getHvInfo(tVehData *HV)
{
	int RetVal = MSG_ERR_NONE;
//	uint8_t buf[4];
//Get GPS Information
	GpsLocation *pGpsData;
	pGpsData = ALLOC_OBJ(GpsLocation, 1);
	RetVal = GPS_GetData(pGpsData);
//	memset(HV->Id, 0x00, 4);
	if (Store.HostAttriInfo.ID != NULL)
	{
//		buf[0] = Store.HostAttriInfo.ID[0];
//		buf[1] = Store.HostAttriInfo.ID[1];
//		buf[2] = Store.HostAttriInfo.ID[2];
//		buf[3] = Store.HostAttriInfo.ID[3];
		//HV->Id = ((buf[0]&0xFF)<<24) + ((buf[0+1]&0xFF)<<16) + ((buf[0+2]&0xFF)<<8) + (buf[0+3]&0xFF);
		memcpy(HV->Id, Store.HostAttriInfo.ID, sizeof(HV->Id));
	}

	if (RetVal)
	{
		if(pGpsData != NULL)
		{
			free( pGpsData);
			pGpsData = NULL;
		}
		return MSG_ERR_INTERFACE;
	}
	if (pGpsData)
	{
		HV->Latitude = pGpsData->latitude * 1.0 / 10000000;
		HV->Longitude = pGpsData->longitude * 1.0 / 10000000;
		HV->Altitude = pGpsData->altitude;
		HV->Heading = pGpsData->bearing;
	}

	if (pGpsData)
	{
		HV->Speed = (pGpsData->speed);
	}
	else
	{
		HV->Speed = 8191;
	}
//Vehicle Size
	HV->VehicleWidth = (Store.HostAttriInfo.choice.OBUInfo.VehWidth * 1.0)
			/ 100;
	HV->VehicleLength = (Store.HostAttriInfo.choice.OBUInfo.VehLenght * 1.0)
			/ 100;
	HV->VehicleHight = (Store.HostAttriInfo.choice.OBUInfo.VehHeight * 1.0)
			/ 20;

//Vehicle Classification
	HV->VehicleClass = Store.HostAttriInfo.choice.OBUInfo.VehClassification;

	HV->Light = 0;
	HV->BreakPadel = 0;
	HV->SteeringWheelAngle = 0;
	if (Store.CANFlag == 1)
	{
		int ConfigVal = 0;
		if (!(GetConfigIntValue("./HostInfo.ini", "CANDATA", "Light",
				&ConfigVal)))
			HV->Light = ConfigVal;

		if (!(GetConfigIntValue("./HostInfo.ini", "CANDATA", "Break",
				&ConfigVal)))
			HV->BreakPadel = ConfigVal;
	}

	CANData CanData ;
	RetVal = CAN_GetData(&CanData);

	if(CanData.speed > 3)
	{
		HV->Speed = CanData.speed / 3.6;
	}
//	HV->Light = CanData.vehicleLight;
//	HV->BreakPadel = CanData.brakes.brakePadel;
//	HV->transmission = CanData.transmission;
//	HV->SteeringWheelAngle = CanData.angle;

	if(pGpsData != NULL)
	{
		free( pGpsData);
		pGpsData = NULL;
	}
	HV->BackNode = 0;
	HV->ForwardNode = 0;
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
int Encode_HostStatus(uint8_t pbuf[], int MAXLEN, MessageFrame_t *BSM_Message)
{
	uint8_t *buf = pbuf;
	uint32_t lbufLen = 0;

	//本机UEID
	buf[lbufLen++] = (atoi(Store.HostAttriInfo.ID) >> 0) & 0xFF;
	buf[lbufLen++] = (atoi(Store.HostAttriInfo.ID) >> 8) & 0xFF;
	buf[lbufLen++] = (atoi(Store.HostAttriInfo.ID) >> 16) & 0xFF;
	buf[lbufLen++] = (atoi(Store.HostAttriInfo.ID) >> 24) & 0xFF;

	//本机设备类型
	buf[lbufLen++] = 1;
	//本机应用类型
	buf[lbufLen++] = Store.HostAttriInfo.present;
	//GPS状态
	buf[lbufLen++] = Store.HostStateInfo.GnssState;
	//CAN状态
	buf[lbufLen++] = Store.HostStateInfo.CanState;
	//串口状态
	buf[lbufLen++] = Store.HostStateInfo.VicState;
	//TCP服务器状态
	buf[lbufLen++] = Store.HostStateInfo.TcpServer;
	//TCP客户端状态
	buf[lbufLen++] = Store.HostStateInfo.TcpCilent;
	//UDP服务器状态
	buf[lbufLen++] = Store.HostStateInfo.UdpServer;
	//UDP客户端状态
	buf[lbufLen++] = Store.HostStateInfo.UdpCilent;
	//V2X接收状态
	buf[lbufLen++] = Store.HostComInfo.RxEnable;

	//V2X-BSM发送状态
	buf[lbufLen++] = (Store.HostComInfo.BsmTx.TxEnable >> 0) & 0xFF;
	buf[lbufLen++] = (Store.HostComInfo.BsmTx.TxEnable >> 8) & 0xFF;

	//V2X-BSM发送间隔
	buf[lbufLen++] = (Store.HostComInfo.BsmTx.TxInterval >> 0) & 0xFF;
	buf[lbufLen++] = (Store.HostComInfo.BsmTx.TxInterval >> 8) & 0xFF;

	//V2X-RSI发送状态
	buf[lbufLen++] = (Store.HostComInfo.RsiTx.TxEnable >> 0) & 0xFF;
	buf[lbufLen++] = (Store.HostComInfo.RsiTx.TxEnable >> 8) & 0xFF;

	//V2X-RSI发送间隔
	buf[lbufLen++] = (Store.HostComInfo.RsiTx.TxInterval >> 0) & 0xFF;
	buf[lbufLen++] = (Store.HostComInfo.RsiTx.TxInterval >> 8) & 0xFF;

	//V2X-RSM发送状态
	buf[lbufLen++] = (Store.HostComInfo.RsmTx.TxEnable >> 0) & 0xFF;
	buf[lbufLen++] = (Store.HostComInfo.RsmTx.TxEnable >> 8) & 0xFF;

	//V2X-RSM发送间隔
	buf[lbufLen++] = (Store.HostComInfo.RsmTx.TxInterval >> 0) & 0xFF;
	buf[lbufLen++] = (Store.HostComInfo.RsmTx.TxInterval >> 8) & 0xFF;

	//V2X-MAP发送状态
	buf[lbufLen++] = (Store.HostComInfo.MapTx.TxEnable >> 0) & 0xFF;
	buf[lbufLen++] = (Store.HostComInfo.MapTx.TxEnable >> 8) & 0xFF;
	//V2X-MAP发送间隔
	buf[lbufLen++] = (Store.HostComInfo.MapTx.TxInterval >> 0) & 0xFF;
	buf[lbufLen++] = (Store.HostComInfo.MapTx.TxInterval >> 8) & 0xFF;

	//V2X-SPAT发送状态
	buf[lbufLen++] = (Store.HostComInfo.SpatTx.TxEnable >> 0) & 0xFF;
	buf[lbufLen++] = (Store.HostComInfo.SpatTx.TxEnable >> 8) & 0xFF;

	//V2X-SPAT发送间隔
	buf[lbufLen++] = (Store.HostComInfo.SpatTx.TxInterval >> 0) & 0xFF;
	buf[lbufLen++] = (Store.HostComInfo.SpatTx.TxInterval >> 8) & 0xFF;

	//开启发送BSM帧
	if (Store.HostComInfo.BsmTx.TxEnable
			|| BSM_Message->present != MessageFrame_PR_NOTHING)
	{
#if 0
		xer_fprint(stdout, &asn_DEF_MessageFrame, BSM_Message);
#endif
		//当前时间
		memcpy(&buf[lbufLen], &(BSM_Message->choice.bsmFrame.secMark),
				sizeof(long));
		lbufLen += sizeof(long);
		//车辆位置
		memcpy(&buf[lbufLen], &(BSM_Message->choice.bsmFrame.pos.lat),
				sizeof(long));
		lbufLen += sizeof(long);

		memcpy(&buf[lbufLen], &(BSM_Message->choice.bsmFrame.pos.Long),
				sizeof(long));
		lbufLen += sizeof(long);
		if (BSM_Message->choice.bsmFrame.pos.elevation != NULL)
		{
			memcpy(&buf[lbufLen], BSM_Message->choice.bsmFrame.pos.elevation,
					sizeof(long));
			lbufLen += sizeof(long);
		}
		else
		{
			memset(&buf[lbufLen], 0x00, sizeof(long));
			lbufLen += sizeof(long);
		}
		//档位状态
		memcpy(&buf[lbufLen], &(BSM_Message->choice.bsmFrame.transmission),
				sizeof(long));
		lbufLen += sizeof(long);
		//车速
		memcpy(&buf[lbufLen], &(BSM_Message->choice.bsmFrame.speed),
				sizeof(long));
		lbufLen += sizeof(long);
		//航向角
		memcpy(&buf[lbufLen], &(BSM_Message->choice.bsmFrame.heading),
				sizeof(long));
		lbufLen += sizeof(long);
		//方向盘转角
		if (BSM_Message->choice.bsmFrame.angle != NULL)
		{
			memcpy(&buf[lbufLen], BSM_Message->choice.bsmFrame.angle,
					sizeof(long));
			lbufLen += sizeof(long);
		}
		else
		{
			memset(&buf[lbufLen], 0x00, sizeof(long));
			lbufLen += sizeof(long);
		}
		//车辆加速度
		memcpy(&buf[lbufLen], &(BSM_Message->choice.bsmFrame.accelSet.Long),
				sizeof(long));
		lbufLen += sizeof(long);
		memcpy(&buf[lbufLen], &(BSM_Message->choice.bsmFrame.accelSet.lat),
				sizeof(long));
		lbufLen += sizeof(long);
		memcpy(&buf[lbufLen], &(BSM_Message->choice.bsmFrame.accelSet.vert),
				sizeof(long));
		lbufLen += sizeof(long);
		memcpy(&buf[lbufLen], &(BSM_Message->choice.bsmFrame.accelSet.yaw),
				sizeof(long));
		lbufLen += sizeof(long);
		//刹车状态
		memset(&buf[lbufLen], 0x00, 13);
		if (BSM_Message->choice.bsmFrame.brakes.brakePadel != NULL)
		{
			memcpy(&buf[lbufLen],
					BSM_Message->choice.bsmFrame.brakes.brakePadel,
					sizeof(long));
		}
		lbufLen += sizeof(long);

		if (BSM_Message->choice.bsmFrame.brakes.wheelBrakes != NULL)
			if (BSM_Message->choice.bsmFrame.brakes.wheelBrakes->buf != NULL)
				memcpy(&buf[lbufLen],
						BSM_Message->choice.bsmFrame.brakes.wheelBrakes->buf,
						sizeof(uint8_t));
		lbufLen++;

		if (BSM_Message->choice.bsmFrame.brakes.traction != NULL)
			memcpy(&buf[lbufLen], BSM_Message->choice.bsmFrame.brakes.traction,
					sizeof(long));
		lbufLen += sizeof(long);

		if (BSM_Message->choice.bsmFrame.brakes.abs != NULL)
			memcpy(&buf[lbufLen], BSM_Message->choice.bsmFrame.brakes.abs,
					sizeof(long));
		lbufLen += sizeof(long);

		if (BSM_Message->choice.bsmFrame.brakes.scs != NULL)
			memcpy(&buf[lbufLen], BSM_Message->choice.bsmFrame.brakes.scs,
					sizeof(long));
		lbufLen += sizeof(long);

		if (BSM_Message->choice.bsmFrame.brakes.brakeBoost != NULL)
			memcpy(&buf[lbufLen],
					BSM_Message->choice.bsmFrame.brakes.brakeBoost,
					sizeof(long));
		lbufLen += sizeof(long);

		if (BSM_Message->choice.bsmFrame.brakes.auxBrakes != NULL)
			memcpy(&buf[lbufLen], BSM_Message->choice.bsmFrame.brakes.auxBrakes,
					sizeof(long));
		lbufLen += sizeof(long);

		//车身尺寸
		memcpy(&buf[lbufLen], &(BSM_Message->choice.bsmFrame.size.width),
				sizeof(long));
		lbufLen += sizeof(long);

		memcpy(&buf[lbufLen],
				&(BSM_Message->choice.bsmFrame.size.length),
				sizeof(long));
		lbufLen += sizeof(long);

		if (BSM_Message->choice.bsmFrame.size.height != NULL)
		{
			memcpy(&buf[lbufLen],
					BSM_Message->choice.bsmFrame.size.height,
					sizeof(long));
			lbufLen += sizeof(long);
		}
		else
		{
			memset(&buf[lbufLen], 0x00, sizeof(long));
			lbufLen += sizeof(long);
		}
		//车辆类型
		memcpy(&buf[lbufLen],
				&(BSM_Message->choice.bsmFrame.vehicleClass.classification),
				sizeof(long));
		lbufLen += sizeof(long);

		//车辆特殊状态
		memcpy(&buf[lbufLen],
				&(BSM_Message->choice.bsmFrame.vehicleClass.classification),
				sizeof(long));
		lbufLen += sizeof(long);

		//车灯状态
		if (BSM_Message->choice.bsmFrame.safetyExt != NULL)
		{
			if (BSM_Message->choice.bsmFrame.safetyExt->lights != NULL)
			{
				if (BSM_Message->choice.bsmFrame.safetyExt->lights->buf != NULL)
				{
					memcpy(&buf[lbufLen],
							BSM_Message->choice.bsmFrame.safetyExt->lights->buf,
							sizeof(long));
				}
			}
		}
		else
		{
			memset(&buf[lbufLen], 0x00, sizeof(long));
		}
		lbufLen += sizeof(long);
	}
	return lbufLen;
}

/****************************************************
 * 函数名称:
 * 功能描述:
 * 输入参数:
 * 输出参数:
 * 返 回 值:
 * 创建日期:
 ****************************************************/
int InitHostStatus()
{

	GetTimeStamp(&g_u64TimeStamp_Start);
	//正常行驶
//	if ((g_ReadHisDataBase.u32HisDataBaseIndex_BSM == 0)
//			&& (g_ReadHisDataBase.u32HisDataBaseIndex_RSI == 0)
//			&& (g_ReadHisDataBase.u32HisDataBaseIndex_MAP == 0)
//			&& (g_ReadHisDataBase.u32HisDataBaseIndex_RSM == 0)
//			&& (g_ReadHisDataBase.u32HisDataBaseIndex_SPAT == 0)
//			&& (g_ReadHisDataBase.u32HisDataBaseIndex_APP_ALL == 0)
//			&& (g_ReadHisDataBase.u32HisDataBaseIndex_RX_ALL == 0))
//	if (1)
//	{
//		Init_UpD12WebService();
//		pthread_create(&tidp_HostStatus, NULL, Thread_HostStatus, NULL);
//	}
//	else //读取历史数据
//	{
//	//	pthread_create(&tidp_HostStatus, NULL, Thread_HostStatus_db, NULL);
//	}
	return 0;
}

