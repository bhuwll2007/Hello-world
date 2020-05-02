/*
 * BsmTx.c
 *
 *  Created on: Apr 3, 2018
 *      Author: root
 */

#include "BsmTx.h"

int BsmTx_Open(struct BsmTx **ppBsm, pthread_attr_t *pAttr)
{
	int Res = Send_ERR_NONE;
	struct BsmTx *pBsm;

	// Create and check the local structure
	pBsm = (struct BsmTx *) malloc(sizeof(struct BsmTx));

	if (pBsm == NULL)
	{
		Res = MSG_ERR_MALLOC;
		goto Error;
	}
	memset(pBsm, 0, sizeof(struct BsmTx));

	pBsm->ThreadAttr = *pAttr;

//	pthread_attr_t thread_attr;
//	struct sched_param schedule_param;
//
//	pthread_attr_init(&thread_attr);
//	schedule_param.sched_priority = 90;
//	pthread_attr_setinheritsched(&thread_attr, PTHREAD_EXPLICIT_SCHED); //有这行，设置优先级才会生效
//	pthread_attr_setschedpolicy(&thread_attr,SCHED_RR);
//	pthread_attr_setschedparam(&thread_attr, &schedule_param);

//	pthread_create(&tidp_AroundVeh, &thread_attr, Thread_AroundVeh, NULL);

	// Create pthread
	pBsm->ThreadState |= BSMTX_THREAD_STATE_INIT;
	Res = pthread_create(&pBsm->ThreadID, &pBsm->ThreadAttr,
			(void *) BsmTx_ThreadProc, pBsm);

	if (Res)
	{
		pBsm->ThreadState = BSMTX_THREAD_STATE_NONE;
		Res = Send_ERR_THREAD_FAIL;
		goto Error;
	}

	//Success
	*ppBsm = pBsm;
	Res = 0;
	goto Success;

	Error: *ppBsm = NULL;
	BsmTx_close(pBsm);
	Success: return Res;
}

void BsmTx_close(struct BsmTx *pBsm)
{
	// Signal thread to terminate
	pBsm->ThreadState |= BSMTX_THREAD_STATE_STOP;
	// Wait for thread termination
	if (pBsm->ThreadState & BSMTX_THREAD_STATE_INIT)
		pthread_join(pBsm->ThreadID, NULL);
	free(pBsm);
	return;
}

void BsmTx_ThreadProc(void *pArg)
{
	struct BsmTx *pBsm;
	pBsm = (struct BsmTx *) pArg;

	struct timeval delay;
	// Starting BsmTx Periodic Thread
	pBsm->ThreadState |= BSMTX_THREAD_STATE_RUN;

	//Thread loop
	while ((pBsm->ThreadState & BSMTX_THREAD_STATE_STOP) == 0)
	{
		if (Store.HostComInfo.BsmTx.TxEnable)
		{
			int Res = BsmTx_SendMsg(pBsm);
			if (Res == 0)
			{
				pBsm->Stats.Tx.Okay++;
			} else
			{
				pBsm->Stats.Tx.Fail++;
			}
			printf("BSM_Tx Count = %d\n", pBsm->Stats.Tx.Okay);
		}

//		if (pBsm->Stats.Tx.Okay == 127)
//			Store.HostComInfo.BsmTx.TxEnable = 0;
		msTimeDelay(delay, Store.HostComInfo.BsmTx.TxInterval);
	}

	(void) pthread_exit(NULL);

}

int BsmTx_SendMsg(struct BsmTx *pBsm)
{
	int Res = Send_ERR_NONE;

	int BufLen = 2028;
	char pBuf[BufLen];
	asn_enc_rval_t EncRVal; // Encoder return value

	MessageFrame_t *pSendMsg = ALLOC_OBJ(MessageFrame_t, 1);

	Res = BSM_Msg_Create(pSendMsg);
	if (Res)
	{
		Res = Send_ERR_CREATE_FAIL;
		goto Error;
	}
	pBsm->Params = *pSendMsg;
#if 0
	xer_fprint(stdout, &asn_DEF_MessageFrame, &(pBsm->Params));
	printf("\n");
#endif
	pthread_mutex_lock(&mutex_StoreBSM);
	WriteXml(&(pBsm->Params), "./TxLog/BSM_SendInfo.txt");
	pthread_mutex_unlock(&mutex_StoreBSM);
	EncRVal = Msg_UPEREncode_ToBuf(&(pBsm->Params), pBuf, BufLen);

	if (EncRVal.encoded == -1)
	{
		printf("uper_encode_to_buffer() failed!\n");
		printf("The failed name is: %s\n", EncRVal.failed_type->name);

		Res = Send_ERR_ENCODE_FAIL;
		goto Error;
	}
#if 0
	else
	{
		printf("ec.encoded:%d\n", EncRVal.encoded);
		printf("Bytes len:%d\n",
				(EncRVal.encoded / 8 + ((EncRVal.encoded % 8) ? 1 : 0)));

		printf("\n");
	}
#endif
	printf("BSM Message UPER Endoce Success!!!\n");

	BufLen = (EncRVal.encoded / 8 + ((EncRVal.encoded % 8) ? 1 : 0));
//	Res = V2X_SendData(pBuf, BufLen);
//	Res = DSMSend(pBuf, BufLen);
	tDSM_Request dsmSend_rq;
	dsmSend_rq.AID = 17;
	dsmSend_rq.Length = BufLen;
	dsmSend_rq.Data = pBuf;
	Res = SecLayer_Send(dsmSend_rq);
	if (Res)
	{
		Res = Send_ERR_CREATE_FAIL;
		goto Error;
	}

	Error:
	ASN_STRUCT_FREE(asn_DEF_MessageFrame, pSendMsg);
	return Res;
}

int BsmTx_ReadlstTxMsg(MessageFrame_t *pBsmMsg)
{
	pthread_mutex_lock(&mutex_StoreBSM);
	ReadXml(&pBsmMsg, "./TxLog/BSM_SendInfo.txt");
	pthread_mutex_unlock(&mutex_StoreBSM);
	return pBsmMsg->present;
}
int BSM_Msg_Create(MessageFrame_t *pSendMsg)
{
	int RetVal = MSG_ERR_NONE;
	static long BSM_MsgCnt = 0;

	//Get GPS Information
	GpsLocation *pGpsData = NULL;
	CANData *pCanData = NULL;
	pGpsData = ALLOC_OBJ(GpsLocation, 1);

	RetVal = GPS_GetData(pGpsData);
	if (RetVal)
	{
		RetVal = MSG_ERR_INTERFACE;
		goto Error;
	}

	/*
	 * 获取CAN数据
	 */
	pCanData = ALLOC_OBJ(CANData, 1);
	RetVal = CAN_GetData(pCanData);

	//allocate memory for MessageFrame
//	(*pSendMsg) = ALLOC_OBJ(MessageFrame_t, 1);
	if (!(pSendMsg))
	{
		RetVal = MSG_ERR_MALLOC;
		goto Error;
	}

	//Set Message Type
	(pSendMsg)->present = MessageFrame_PR_bsmFrame;

	BSM_MsgCnt++;
	if (BSM_MsgCnt > 127)
		BSM_MsgCnt = 0;
	// Update MsgCnt
	(pSendMsg)->choice.bsmFrame.msgCnt = BSM_MsgCnt;

	// Vehicle ID
	(pSendMsg)->choice.bsmFrame.id.buf = ALLOC_OBJ(uint8_t, 8);
	if (!(pSendMsg)->choice.bsmFrame.id.buf)
	{
		RetVal = MSG_ERR_MALLOC;
		goto Error;
	}
	(pSendMsg)->choice.bsmFrame.id.size = 8;
	memcpy((pSendMsg)->choice.bsmFrame.id.buf, &Store.HostAttriInfo.ID[0],
			sizeof(uint8_t) * 8);

	//Current Time
	(pSendMsg)->choice.bsmFrame.secMark = GetDSecond();

	//TimeConfidence_t	*timeConfidence	/* OPTIONAL */;
	(pSendMsg)->choice.bsmFrame.timeConfidence = NULL;

	//Position  and Heading
	(pSendMsg)->choice.bsmFrame.pos.elevation = ALLOC_OBJ(Elevation_t, 1);
	if (!(pSendMsg)->choice.bsmFrame.pos.elevation)
	{
		RetVal = MSG_ERR_MALLOC;
		goto Error;
	}

	if ((pCanData) && (pCanData->lat > 100000))
	{
		(pSendMsg)->choice.bsmFrame.pos.lat = pCanData->lat;
		(pSendMsg)->choice.bsmFrame.pos.Long = pCanData->lng;
		*(pSendMsg)->choice.bsmFrame.pos.elevation = pCanData->alt;
	} else if (pGpsData)
	{
		(pSendMsg)->choice.bsmFrame.pos.lat = pGpsData->latitude;
		(pSendMsg)->choice.bsmFrame.pos.Long = pGpsData->longitude;
		*(pSendMsg)->choice.bsmFrame.pos.elevation = 10 * pGpsData->altitude;
	} else
	{
		//Set Unuseful Value
		(pSendMsg)->choice.bsmFrame.pos.lat = 900000001;
		(pSendMsg)->choice.bsmFrame.pos.Long = 1800000001;
		*(pSendMsg)->choice.bsmFrame.pos.elevation = 0;
	}

	if ((pCanData) && (pCanData->heading != 0))
	{
		(pSendMsg)->choice.bsmFrame.heading = pCanData->heading;
	} else if (pGpsData)
	{
		(pSendMsg)->choice.bsmFrame.heading = 80 * pGpsData->bearing;
	} else
	{
		(pSendMsg)->choice.bsmFrame.heading = 0;
	}
	if ((*(pSendMsg)->choice.bsmFrame.pos.elevation < -4096)
			|| (*(pSendMsg)->choice.bsmFrame.pos.elevation >= 61439))
	{
//		printf("*(pSendMsg)->choice.bsmFrame.pos.elevation = %d\n",*(pSendMsg)->choice.bsmFrame.pos.elevation);
		*(pSendMsg)->choice.bsmFrame.pos.elevation = 0;
	}

//	struct PositionalAccuracy	*posAccuracy	/* OPTIONAL */;
//	struct PositionConfidenceSet	*posConfidence	/* OPTIONAL */;
	(pSendMsg)->choice.bsmFrame.posAccuracy = NULL;
	(pSendMsg)->choice.bsmFrame.posConfidence = NULL;

	//Speed
	if ((pCanData) && (pCanData->speed > 0))
	{
		(pSendMsg)->choice.bsmFrame.speed = pCanData->speed;
	} else if (pGpsData)
	{
		(pSendMsg)->choice.bsmFrame.speed = 50 * pGpsData->speed;
	} else
	{
		//Set Unuseful Value
		(pSendMsg)->choice.bsmFrame.speed = 8191;
	}

	//transmission
	if (pCanData)
	{
		(pSendMsg)->choice.bsmFrame.transmission = pCanData->transmission;
	}

	//Acceleration
	/*
	 * 这些值暂时没有
	 */
	(pSendMsg)->choice.bsmFrame.accelSet.lat = 2001;
	(pSendMsg)->choice.bsmFrame.accelSet.Long = 2001;
	(pSendMsg)->choice.bsmFrame.accelSet.vert = -127;
	(pSendMsg)->choice.bsmFrame.accelSet.yaw = 0;

	//BreakSystem
	(pSendMsg)->choice.bsmFrame.brakes.brakePadel = NULL;
	(pSendMsg)->choice.bsmFrame.brakes.brakePadel = ALLOC_OBJ(
			BrakePedalStatus_t, 1);
	memcpy((pSendMsg)->choice.bsmFrame.brakes.brakePadel,
			&pCanData->brakes.brakePadel, sizeof(uint8_t));
//	printf("(pSendMsg)->choice.bsmFrame.brakes.brakePadel = %d\n",
//			*(pSendMsg)->choice.bsmFrame.brakes.brakePadel);
//	printf("pCanData->brakes.brakePadel = %d\n", pCanData->brakes.brakePadel);
	(pSendMsg)->choice.bsmFrame.brakes.abs = ALLOC_OBJ(AntiLockBrakeStatus_t,
			1);
	memcpy((pSendMsg)->choice.bsmFrame.brakes.abs, &pCanData->brakes.abs,
			sizeof(uint8_t));
//	printf("(pSendMsg)->choice.bsmFrame.brakes.abs = %d\n",
//			*(pSendMsg)->choice.bsmFrame.brakes.abs);
//	printf("pCanData->brakes.abs = %d\n", pCanData->brakes.abs);

	//Vehicle Size
	memcpy(&(pSendMsg)->choice.bsmFrame.size.width,
			&Store.HostAttriInfo.choice.OBUInfo.VehWidth,
			sizeof(VehicleWidth_t));
	memcpy(&(pSendMsg)->choice.bsmFrame.size.length,
			&Store.HostAttriInfo.choice.OBUInfo.VehLenght,
			sizeof(VehicleLength_t));

	(pSendMsg)->choice.bsmFrame.size.height = ALLOC_OBJ(VehicleHeight_t, 1);
	if (!(pSendMsg)->choice.bsmFrame.size.height)
	{
		RetVal = MSG_ERR_MALLOC;
		goto Error;
	}
	memcpy((pSendMsg)->choice.bsmFrame.size.height,
			&Store.HostAttriInfo.choice.OBUInfo.VehHeight,
			sizeof(VehicleHeight_t));

	//Vehicle Classification
	memcpy(&(pSendMsg)->choice.bsmFrame.vehicleClass.classification,
			&Store.HostAttriInfo.choice.OBUInfo.VehClassification,
			sizeof(BasicVehicleClass_t));

	//VehicleSafetyExtensions
	(pSendMsg)->choice.bsmFrame.safetyExt = ALLOC_OBJ(
			struct VehicleSafetyExtensions, 1);
	//ExteriorLights
	(pSendMsg)->choice.bsmFrame.safetyExt->lights = ALLOC_OBJ(ExteriorLights_t,
			1);

	(pSendMsg)->choice.bsmFrame.safetyExt->lights->buf = ALLOC_OBJ(uint8_t, 2);
//	printf("pCanData->vehicleLight0 = %d\n", pCanData->vehicleLight[0]);
//	printf("pCanData->vehicleLight1 = %d\n", pCanData->vehicleLight[1]);

	//pCanData->vehicleLight = pCanData->vehicleLight<<7;

	memcpy((pSendMsg)->choice.bsmFrame.safetyExt->lights->buf,
			pCanData->vehicleLight, 2);
	(pSendMsg)->choice.bsmFrame.safetyExt->lights->size = 2;
	(pSendMsg)->choice.bsmFrame.safetyExt->lights->bits_unused = 7;
#if 0
	xer_fprint(stdout, &asn_DEF_MessageFrame, pSendMsg);
	printf("\n");
#endif
	Error: if (pGpsData != NULL)
		free(pGpsData);
	if (pCanData != NULL)
		free(pCanData);
	return RetVal;
}
