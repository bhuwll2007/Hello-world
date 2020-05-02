/*
 * RsiTx.c
 *
 *  Created on: Apr 3, 2018
 *      Author: root
 */
#include "RsiTx.h"
#include </usr/include/time.h>

int Rsi_CfgCnt = 0;
int FG_RsiTxOpen(struct RsiTx **p_pprectRsi, pthread_attr_t *pAttr)
{
	int Res = Send_ERR_NONE;
	struct RsiTx *pRsi;

	// Create and check the local structure
	pRsi = (struct RsiTx *) malloc(sizeof(struct RsiTx));

	if (pRsi == NULL)
	{
		Res = MSG_ERR_MALLOC;
		goto Error;
	}
	memset(pRsi, 0, sizeof(struct RsiTx));

	pRsi->ThreadAttr = *pAttr;

	// Create thread
	pRsi->ThreadState |= RSITX_THREAD_STATE_INIT;
	Res = pthread_create(&pRsi->ThreadID, &pRsi->ThreadAttr,
			(void *) FS_RsiTx_ThreadProc, pRsi);

	if (Res)
	{
		pRsi->ThreadState = RSITX_THREAD_STATE_NONE;
		Res = Send_ERR_THREAD_FAIL;
		goto Error;
	}

	//Success
	*p_pprectRsi = pRsi;
	Res = 0;
	goto Success;

	Error: *p_pprectRsi = NULL;
	FG_RsiTxclose(pRsi);
	Success: return Res;

}

void FG_RsiTxclose(struct RsiTx *p_prectRsi)
{

	// Signal thread to terminate
	p_prectRsi->ThreadState |= RSITX_THREAD_STATE_STOP;
	// Wait for thread termination
	if (p_prectRsi->ThreadState & RSITX_THREAD_STATE_INIT)
		pthread_join(p_prectRsi->ThreadID, NULL);
	free(p_prectRsi);
	return;
}

static void FS_RsiTx_ThreadProc (void *p_pvArg)
{
	struct RsiTx *pRsi;
	pRsi = (struct RsiTx *) p_pvArg;

	struct timeval delay;
	// Starting RsiTx Periodic Thread
	pRsi->ThreadState |= RSITX_THREAD_STATE_RUN;

	rectRsiPlatformToV2X_t *l_prectRsiRecvFromPlatform = NULL;
	//Thread loop
	while ((pRsi->ThreadState & RSITX_THREAD_STATE_STOP) == 0)
	{
		if (1 == Store.HostComInfo.RsiTx.TxEnable)
		{
			int Res = 0;
			if(NULL == l_prectRsiRecvFromPlatform)
			{
				l_prectRsiRecvFromPlatform = calloc(1, (sizeof(rectRsiPlatformToV2X_t)));
			}

			Res = FG_RsiRecvFromPlatformMsgGet(l_prectRsiRecvFromPlatform);
			if(0 == Res)
			{
				Res = FS_RsiTxSendMsg(pRsi, l_prectRsiRecvFromPlatform);
				if (Res == 0)
				{
					pRsi->Stats.Tx.Okay++;
				}
				else
				{
					pRsi->Stats.Tx.Fail++;
				}
				printf("RSI_Tx Count = %d\n", pRsi->Stats.Tx.Okay);
			}
		}
		if(pRsi->Stats.Tx.Okay == 5000)
		{
			pRsi->Stats.Tx.Okay = 0;
			FG_RsiRecvFromPlatformFree();
		}

		msTimeDelay(delay,
				(int) Store.HostComInfo.RsiTx.TxInterval
						/ Store.HostComInfo.RsiTx.RsiCnt);
	}

	FG_RsiRecvFromPlatformFree();
	(void) pthread_exit(NULL);
}

static int FS_RsiTxSendMsg(struct RsiTx *p_prectRsi, rectRsiPlatformToV2X_t *p_prectRsiPlatformToV2Xt)
{
	int Res = Send_ERR_NONE;

	int BufLen = 2028;
	char pBuf[BufLen];
	asn_enc_rval_t EncRVal; // Encoder return value

	//Declare a Send Message Pointer
	MessageFrame_t *pSendMsg;
	Res = FS_RsiMsgCreateFromPlatform(&pSendMsg, p_prectRsiPlatformToV2Xt);
	if (Res)
	{
		Res = Send_ERR_CREATE_FAIL;
		goto Error;
	}
	p_prectRsi->Params = *pSendMsg;

#if FACILAYER_PRINTLOG
	xer_fprint(stdout, &asn_DEF_MessageFrame, &(pRsi->Params));
	printf("\n");
#endif
	pthread_mutex_lock(&mutex_StoreRSI);
	WriteXml(&(p_prectRsi->Params), "./TxLog/RSI_SendInfo.txt");
	pthread_mutex_unlock(&mutex_StoreRSI);

#if FACILAYER_PRINTLOG
	printf("====================================================\n");
	xer_fprint(stdout, &asn_DEF_MessageFrame, pSendMsg);
#endif
	EncRVal = Msg_UPEREncode_ToBuf(&(p_prectRsi->Params), pBuf, BufLen);

	if (EncRVal.encoded == -1)
	{
		printf("uper_encode_to_buffer() failed!\n");
		printf("The failed name is: %s\n", EncRVal.failed_type->name);

		Res = Send_ERR_ENCODE_FAIL;
		goto Error;

	}

#if FACILAYER_PRINTLOG
	else
	{
		printf(" RSI Message UPER Endoce Success!!!\n");
		printf("ec.encoded:%d\n", EncRVal.encoded);
		printf("Bytes len:%d\n",
				(EncRVal.encoded / 8 + ((EncRVal.encoded % 8) ? 1 : 0)));
	}
#endif

	BufLen = (EncRVal.encoded / 8 + ((EncRVal.encoded % 8) ? 1 : 0));

	tDSM_Request dsmSend_rq;
	dsmSend_rq.AID = 17;
	dsmSend_rq.Length = BufLen;
	dsmSend_rq.Data = pBuf;
	Res = SecLayer_Send(dsmSend_rq);
	if (Res)
	{
		Res = Send_ERR_SEND_FAIL;
		goto Error;
	}

	Error:
	ASN_STRUCT_FREE(asn_DEF_MessageFrame, pSendMsg);
	return Res;
}

static int FS_RsiTxReadlstTxMsg(MessageFrame_t *p_prectRsiMsg)
{
	pthread_mutex_lock(&mutex_StoreRSI);
	ReadXml(&p_prectRsiMsg, "./TxLog/RSI_SendInfo.txt");
	pthread_mutex_unlock(&mutex_StoreRSI);
	return p_prectRsiMsg->present;
}
static int FS_RsiMsgCreate(MessageFrame_t **p_pprectSendMsg)
{
	int RetVal = MSG_ERR_NONE;
	static long RSI_MsgCnt = 0;
	//Get GPS Information
	GpsLocation *l_prectGpsData;
	l_prectGpsData = ALLOC_OBJ(GpsLocation, 1);
	RetVal = GPS_GetData(l_prectGpsData);
	uint8_t *xer_RSIbuf = NULL;
	int fsize = 0;
	time_t ttime =
	{ 0 };
	struct tm *ptm = NULL;

	if (RetVal)
	{
		RetVal = MSG_ERR_INTERFACE;
		goto Error;
	}

	//allocate memory for MessageFrame
	(*p_pprectSendMsg) = ALLOC_OBJ(MessageFrame_t, 1);
	if (!(*p_pprectSendMsg))
	{
		RetVal = MSG_ERR_MALLOC;
		goto Error;
	}

	FILE *pRSIxerfp;
	pRSIxerfp = fopen(Store.HostComInfo.RsiTx.CfgFileName[Rsi_CfgCnt], "a+");
//	assert(pRSIxerfp != NULL);

	Rsi_CfgCnt++;
	if (Rsi_CfgCnt >= Store.HostComInfo.RsiTx.RsiCnt)
		Rsi_CfgCnt = 0;

	//read all bytes in rsi config file
	fseek(pRSIxerfp, 0, SEEK_END);
	fsize = ftell(pRSIxerfp);
	xer_RSIbuf = ALLOC_OBJ(uint8_t, fsize);
	if (!xer_RSIbuf)
	{
		RetVal = MSG_ERR_MALLOC;
		goto Error;
	}
	rewind(pRSIxerfp);
	fread(xer_RSIbuf, fsize, 1, pRSIxerfp);
	fclose(pRSIxerfp);

	/*
	 * Decode the XER buffer.
	 * 此处直接将读取到的数据解码到原始的结构体pSendMsg中
	 * 问题在于：如果申请一个新的MessageFrame_t类型结构体并将读到的xml解码到其中，再将list部分复制到原始的结构体中
	 * 会有有深浅拷贝问题，同时在原始结构体的list一侧需要为每一个list中的成员各申请一块空间，复制list成员时还需要
	 * 考虑成员中的可选参数Option问题，比较复杂
	 * 因此直接将xml信息解码到原始结构体pSendMsg中，注意，xml信息必须不包含可选项，尤其不要包含二级的可选项
	 * 否则对原始结构体中的数据进行UPER编码时，若代码编写不当，会导致重复为可选项分配空间，造成垃圾空间，导致程序崩溃
	 * 另一种方法是为可选项分配空间时，先判断其值是否是NULL，若不是NULL，就表示在xer_decode时已为其分配空间
	 * 同时需要检查此可选项中的二级可选项是否为NULL，这种方式也比较麻烦
	 */
	asn_dec_rval_t rval;
	rval = xer_decode(0, &asn_DEF_MessageFrame, (void **) p_pprectSendMsg, xer_RSIbuf,
			fsize);
//	assert(rval.code == RC_OK);
	if (rval.code != RC_OK)
	{
		printf("Rsi xer_decode rval.code != RC_OK\n");
	}
	free(xer_RSIbuf);

	//=========================== 添加list类型的数据 ===============================
//	int res = 0;
//	PositionOffsetLLV_t *pPositionOffsetIN;
//	pPositionOffsetIN = calloc(1, sizeof *pPositionOffsetIN);
//	assert(pPositionOffsetIN);
//
//	pPositionOffsetIN->offsetLL.present = PositionOffsetLL_PR_position_LL1;
//	pPositionOffsetIN->offsetLL.choice.position_LL1.lat = 2047;
//	pPositionOffsetIN->offsetLL.choice.position_LL1.lon = 2046;
//	res = ASN_SEQUENCE_ADD(&(*pSendMsg)->choice.rsiFrame.alertPath, pPositionOffsetIN);
//	assert(res == 0);
//
//	pPositionOffsetIN = calloc(1, sizeof *pPositionOffsetIN);
//	assert(pPositionOffsetIN);
//	pPositionOffsetIN->offsetLL.present = PositionOffsetLL_PR_position_LL2;
//	pPositionOffsetIN->offsetLL.choice.position_LL2.lat = 8191;
//	pPositionOffsetIN->offsetLL.choice.position_LL2.lon = 8190;
//	res = ASN_SEQUENCE_ADD(&(*pSendMsg)->choice.rsiFrame.alertPath, pPositionOffsetIN);
//	assert(res == 0);
//
//	pPositionOffsetIN = calloc(1, sizeof *pPositionOffsetIN);
//	assert(pPositionOffsetIN);
//	pPositionOffsetIN->offsetLL.present = PositionOffsetLL_PR_position_LL3;
//	pPositionOffsetIN->offsetLL.choice.position_LL3.lat = 32767;
//	pPositionOffsetIN->offsetLL.choice.position_LL1.lon = 32766;
//	res = ASN_SEQUENCE_ADD(&(*pSendMsg)->choice.rsiFrame.alertPath, pPositionOffsetIN);
//	assert(res == 0);
//
//	FILE *pRSIMsgfp = fopen("../xml_RSIMsg.txt", "w+");
//	xer_fprint(pRSIMsgfp, &asn_DEF_MessageFrame, *pSendMsg);
//	fclose(pRSIMsgfp);
	//===========================================================================

	//Set Message Type
	(*p_pprectSendMsg)->present = MessageFrame_PR_rsiFrame;

	RSI_MsgCnt++;
	if (RSI_MsgCnt > 127)
		RSI_MsgCnt = 0;
	// Update MsgCnt
	(*p_pprectSendMsg)->choice.rsiFrame.msgCnt = RSI_MsgCnt;

	// timeStamp
	(*p_pprectSendMsg)->choice.rsiFrame.moy = ALLOC_OBJ(MinuteOfTheYear_t, 1);
	if (!(*p_pprectSendMsg)->choice.rsiFrame.moy)
	{
		RetVal = MSG_ERR_MALLOC;
		goto Error;
	}
	ttime = time(NULL);
	ptm = gmtime(&ttime);
	*(*p_pprectSendMsg)->choice.rsiFrame.moy = ptm->tm_yday * 1440 + ptm->tm_hour * 60
			+ ptm->tm_min;

	// id
	(*p_pprectSendMsg)->choice.rsiFrame.id.buf = ALLOC_OBJ(uint8_t, 8);
	if (!(*p_pprectSendMsg)->choice.rsiFrame.id.buf)
	{
		RetVal = MSG_ERR_MALLOC;
		goto Error;
	}
	(*p_pprectSendMsg)->choice.rsiFrame.id.size = 8;
	memcpy((*p_pprectSendMsg)->choice.rsiFrame.id.buf, &Store.HostAttriInfo.ID[0],
			sizeof(uint8_t) * 8);

	//xer_fprint(stdout, &asn_DEF_MessageFrame, *pSendMsg);
	Error: if (l_prectGpsData)
	{
		free(l_prectGpsData);
	}
	l_prectGpsData = NULL;
	return RetVal;
}


/***************************
 * 20191009
 * platform
 ***************************/
#if 1
static int FS_RsiMsgCreateFromPlatform(MessageFrame_t **p_pprectSendMsg, rectRsiPlatformToV2X_t *p_prectRsiPlatformToV2Xt)
{
	int RetVal = MSG_ERR_NONE;
	static long RSI_MsgCnt = 0;
	//Get GPS Information
	GpsLocation *l_prectGpsData;
	l_prectGpsData = ALLOC_OBJ(GpsLocation, 1);
	RetVal = GPS_GetData(l_prectGpsData);

	time_t ttime ={ 0 };
	struct tm *ptm = NULL;
	if (RetVal)
	{
		RetVal = MSG_ERR_INTERFACE;
		goto Error;
	}

	//allocate memory for MessageFrame
	(*p_pprectSendMsg) = ALLOC_OBJ(MessageFrame_t, 1);
	if (!(*p_pprectSendMsg))
	{
		RetVal = MSG_ERR_MALLOC;
		goto Error;
	}

	//Set Message Type
	(*p_pprectSendMsg)->present = MessageFrame_PR_rsiFrame;

	RSI_MsgCnt++;
	if (RSI_MsgCnt > 127)
		RSI_MsgCnt = 0;

// Update MsgCnt
	(*p_pprectSendMsg)->choice.rsiFrame.msgCnt = RSI_MsgCnt;

// timeStamp
	(*p_pprectSendMsg)->choice.rsiFrame.moy = ALLOC_OBJ(MinuteOfTheYear_t, 1);
	if (!(*p_pprectSendMsg)->choice.rsiFrame.moy)
	{
		RetVal = MSG_ERR_MALLOC;
		goto Error;
	}

	ttime = time(NULL);
	ptm = gmtime(&ttime);
	*(*p_pprectSendMsg)->choice.rsiFrame.moy = ptm->tm_yday * 1440
			+ ptm->tm_hour * 60 + ptm->tm_min;

// id
	(*p_pprectSendMsg)->choice.rsiFrame.id.buf = ALLOC_OBJ(uint8_t, 8);
	if (!(*p_pprectSendMsg)->choice.rsiFrame.id.buf)
	{
		RetVal = MSG_ERR_MALLOC;
		goto Error;
	}
	(*p_pprectSendMsg)->choice.rsiFrame.id.size = 8;
	memcpy((*p_pprectSendMsg)->choice.rsiFrame.id.buf, &Store.HostAttriInfo.ID[0],
			sizeof(uint8_t) * 8);

	RTSList_t* pRsiRTSList = NULL;
	RTSData_t *pRsiRTSData = NULL;
	pRsiRTSList = calloc(1,sizeof(RTSList_t));
	(*p_pprectSendMsg)->choice.rsiFrame.rtss = pRsiRTSList;
	pRsiRTSData = calloc(1,sizeof(RTSData_t));
	ASN_SEQUENCE_ADD(pRsiRTSList,pRsiRTSData);

	pRsiRTSData->rtsId = p_prectRsiPlatformToV2Xt->m_unRsiID;
	pRsiRTSData->signType = p_prectRsiPlatformToV2Xt->m_usRsiType;
	pRsiRTSData->description = calloc(1,sizeof(Description_t));
	pRsiRTSData->description->present = Description_PR_textString;
	pRsiRTSData->description->choice.textString.size = 32;
	pRsiRTSData->description->choice.textString.buf = calloc(32,sizeof(uint8_t));
	memcpy(pRsiRTSData->description->choice.textString.buf, p_prectRsiPlatformToV2Xt->m_ucRSIDescrib, 32);

	ReferencePathList_t *p_ReferencePathList_t = NULL;
	ReferencePath_t *p_ReferencePath_t = NULL;
	p_ReferencePathList_t = calloc(1,sizeof(ReferencePathList_t));
	pRsiRTSData->referencePaths = p_ReferencePathList_t;
	p_ReferencePath_t = calloc(1,sizeof(ReferencePath_t));
	ASN_SEQUENCE_ADD(p_ReferencePathList_t,p_ReferencePath_t);

	PositionOffsetLLV_t *pPositionOffsetIN = NULL;
	PositionOffsetLLV_t *pPositionOffsetIN1 = NULL;
	pPositionOffsetIN = calloc(1, sizeof *pPositionOffsetIN);
	pPositionOffsetIN1 = calloc(1, sizeof *pPositionOffsetIN);
	ASN_SEQUENCE_ADD(&(p_ReferencePath_t->activePath),pPositionOffsetIN);
	ASN_SEQUENCE_ADD(&(p_ReferencePath_t->activePath),pPositionOffsetIN1);

	pPositionOffsetIN->offsetLL.present = PositionOffsetLL_PR_position_LL1;
	pPositionOffsetIN->offsetLL.choice.position_LL1.lat = p_prectRsiPlatformToV2Xt->m_unRsiPoint1lat;
	pPositionOffsetIN->offsetLL.choice.position_LL1.lon = p_prectRsiPlatformToV2Xt->m_unRsiPoint1long;

	pPositionOffsetIN1->offsetLL.present = PositionOffsetLL_PR_position_LL2;
	pPositionOffsetIN1->offsetLL.choice.position_LL3.lat = p_prectRsiPlatformToV2Xt->m_unRsiPoint2lat;
	pPositionOffsetIN1->offsetLL.choice.position_LL3.lon = p_prectRsiPlatformToV2Xt->m_unRsiPoint2long;

//	(*pSendMsg)->choice.rsiFrame.description->buf = ALLOC_OBJ(uint8_t, 32);
//	memcpy(&(*pSendMsg)->choice.rsiFrame.description->buf, &rRsidata.RSI_describ, 8);
//	(*pSendMsg)->choice.rsiFrame.description->buf = rRsidata.RSI_describ;

	(*p_pprectSendMsg)->choice.rsiFrame.refPos.lat = p_prectRsiPlatformToV2Xt->m_unRsiLat;
	(*p_pprectSendMsg)->choice.rsiFrame.refPos.Long = p_prectRsiPlatformToV2Xt->m_unRsiLong;
	(*p_pprectSendMsg)->choice.rsiFrame.refPos.elevation = ALLOC_OBJ(Elevation_t, 1);
	*((*p_pprectSendMsg)->choice.rsiFrame.refPos.elevation) = p_prectRsiPlatformToV2Xt->m_usRsiAlt;

	xer_fprint(stdout, &asn_DEF_MessageFrame, *p_pprectSendMsg);
	Error:
	if (l_prectGpsData)
	{
		free(l_prectGpsData);
	}
	l_prectGpsData = NULL;
	return RetVal;
}
#endif

