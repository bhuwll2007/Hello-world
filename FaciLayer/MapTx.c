/*
 * MapTx.c
 *
 *  Created on: Apr 4, 2018
 *      Author: duser
 */
#include "MapTx.h"
#include </usr/include/time.h>

int MapTx_Open(struct MapTx **ppMap, pthread_attr_t *pAttr)
{

	int Res = Send_ERR_NONE;
	struct MapTx *pMap;

	// Create and check the local structure
	pMap = (struct MapTx *) malloc(sizeof(struct MapTx));

	if (pMap == NULL)
	{
		Res = MSG_ERR_MALLOC;
		goto Error;
	}
	memset(pMap, 0, sizeof(struct MapTx));

	pMap->ThreadAttr = *pAttr;

	// Create thread
	pMap->ThreadState |= MAPTX_THREAD_STATE_INIT;
	Res = pthread_create(&pMap->ThreadID, &pMap->ThreadAttr,
			(void *) MapTx_ThreadProc, pMap);

	if (Res)
	{
		pMap->ThreadState = MAPTX_THREAD_STATE_NONE;
		Res = Send_ERR_THREAD_FAIL;
		goto Error;
	}

	//Success
	*ppMap = pMap;
	Res = 0;
	goto Success;

	Error: *ppMap = NULL;
	MapTx_close(pMap);
	Success: return Res;

}

void MapTx_close(struct MapTx *pMap)
{

	// Signal thread to terminate
	pMap->ThreadState |= MAPTX_THREAD_STATE_STOP;
	// Wait for thread termination
	if (pMap->ThreadState & MAPTX_THREAD_STATE_INIT)
		pthread_join(pMap->ThreadID, NULL);
	free(pMap);
	return;
}

void MapTx_ThreadProc(void *pArg)
{

	struct MapTx *pMap;
	pMap = (struct MapTx *) pArg;

	struct timeval delay;
	// Starting MapTx Periodic Thread
	pMap->ThreadState |= MAPTX_THREAD_STATE_RUN;

	//Thread loop
	while ((pMap->ThreadState & MAPTX_THREAD_STATE_STOP) == 0)
	{

		if (Store.HostComInfo.MapTx.TxEnable)
		{
			int Res = MapTx_SendMsg(pMap);

			if (Res == 0)
			{
				pMap->Stats.Tx.Okay++;
			}
			else
			{
				pMap->Stats.Tx.Fail++;
			}

			printf("Map_Tx Count = %d\n", pMap->Stats.Tx.Okay);
		}
//		if(pMap->Stats.Tx.Okay == 5000)
//			Store.HostComInfo.MapTx.TxEnable = 0;

		msTimeDelay(delay, Store.HostComInfo.MapTx.TxInterval);
	}

	(void) pthread_exit(NULL);

}

int MapTx_SendMsg(struct MapTx *pMap)
{
	int Res = Send_ERR_NONE;

	int BufLen = 2028;
	char pBuf[BufLen];
	asn_enc_rval_t EncRVal; // Encoder return value

	//Declare a Send Message Pointer
	MessageFrame_t *pSendMsg;

	Res = Map_Msg_Create(&pSendMsg);
	if (Res)
	{
		Res = Send_ERR_CREATE_FAIL;
		goto Error;
	}
	pMap->Params = *pSendMsg;
#if FACILAYER_PRINTLOG
	xer_fprint(stdout, &asn_DEF_MessageFrame, &(pMap->Params));
	printf("\n");
#endif
	pthread_mutex_lock(&mutex_StoreMAP);
	WriteXml(&(pMap->Params), "./TxLog/Map_SendInfo.txt");
	pthread_mutex_unlock(&mutex_StoreMAP);

	EncRVal = Msg_UPEREncode_ToBuf(&(pMap->Params), pBuf, BufLen);

	if (EncRVal.encoded == -1)
	{
		printf("uper_encode_to_buffer() failed!\n");
		printf("The failed name is: %s\n", EncRVal.failed_type->name);

		Res = Send_ERR_ENCODE_FAIL;
		goto Error;

	}
#if FACILAYER_PRINTLOG
	else {
		printf("ec.encoded:%d\n", EncRVal.encoded);
		printf("Bytes len:%d\n",
				(EncRVal.encoded / 8 + ((EncRVal.encoded % 8) ? 1 : 0)));

		printf("\n");
	}

	printf("Map Mesage UPER Endoce Success!!!\n");
#endif
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
		Res = Send_ERR_SEND_FAIL;
		goto Error;
	}

	Error:
	ASN_STRUCT_FREE(asn_DEF_MessageFrame, pSendMsg);
	return Res;

}

int MapTx_ReadlstTxMsg(MessageFrame_t *pMapMsg)
{
	pthread_mutex_lock(&mutex_StoreMAP);
	ReadXml(&pMapMsg, "./TxLog/Map_SendInfo.txt");
	pthread_mutex_unlock(&mutex_StoreMAP);
	return pMapMsg->present;
}

int Map_Msg_Create(MessageFrame_t **pSendMsg)
{

	int RetVal = MSG_ERR_NONE;
	static long MAP_MsgCnt = 0;

	uint8_t *xer_MAPbuf = NULL;
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
	(*pSendMsg) = ALLOC_OBJ(MessageFrame_t, 1);
	if (!(*pSendMsg))
	{
		RetVal = MSG_ERR_MALLOC;
		goto Error;
	}

	FILE *pMAPxerfp = fopen(Store.HostComInfo.MapTx.CfgFileName, "a+");

	//read all bytes in Map config file
	fseek(pMAPxerfp, 0, SEEK_END);
	fsize = ftell(pMAPxerfp);
	xer_MAPbuf = ALLOC_OBJ(uint8_t, fsize);
	if (!xer_MAPbuf)
	{
		RetVal = MSG_ERR_MALLOC;
		goto Error;
	}
	rewind(pMAPxerfp);
	fread(xer_MAPbuf, fsize, 1, pMAPxerfp);
	fclose(pMAPxerfp);

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
	rval = xer_decode(0, &asn_DEF_MessageFrame, (void **) pSendMsg, xer_MAPbuf,
			fsize);
//	assert(rval.code == RC_OK);
	if (rval.code != RC_OK)
		printf("Read Xml File error!");
	free(xer_MAPbuf);

//	printf("sfss:%d\n", *(*pSendMsg)->choice.mapFrame.nodes.list.array[1]->id.region);  // 测试是否读出来了

	//=========================== 添加list类型的数据 ===============================
//	int res = 0;
//	Node_t *Node;
//	Node = calloc(1, sizeof(Node_t));
//	assert(Node);
//
//	Node->id.id = 111;
//	Node->id.region = calloc(1, sizeof(RoadRegulatorID_t));
//	*Node->id.region = 121;
//	Node->refPos.elevation = calloc(1, sizeof(Elevation_t));
//	Node->refPos.lat = 0;
//	Node->refPos.Long = 0;
//	*Node->refPos.elevation = 0;
//	res = ASN_SEQUENCE_ADD(&(*pSendMsg)->choice.mapFrame.nodes, Node);
//	assert(res == 0);
//
//
//	Node = calloc(1, sizeof(Node_t));
//	assert(Node);
//
//	Node->id.id = 222;
//	Node->id.region = calloc(1, sizeof(RoadRegulatorID_t));
//	*Node->id.region = 232;
//	Node->refPos.elevation = calloc(1, sizeof(Elevation_t));
//	Node->refPos.lat = 0;
//	Node->refPos.Long = 0;
//	*Node->refPos.elevation = 0;
//	res = ASN_SEQUENCE_ADD(&(*pSendMsg)->choice.mapFrame.nodes, Node);
//	assert(res == 0);
//
//
//	Node = calloc(1, sizeof(Node_t));
//	assert(Node);
//
//	Node->id.id = 333;
//	Node->id.region = calloc(1, sizeof(RoadRegulatorID_t));
//	*Node->id.region = 343;
//	Node->refPos.elevation = calloc(1, sizeof(Elevation_t));
//	Node->refPos.lat = 0;
//	Node->refPos.Long = 0;
//	*Node->refPos.elevation = 0;
//	res = ASN_SEQUENCE_ADD(&(*pSendMsg)->choice.mapFrame.nodes, Node);
//	assert(res == 0);

	//===========================================================================

	//Set Message Type
//	(*pSendMsg)->present = MessageFrame_PR_mapFrame;

	MAP_MsgCnt++;
	if (MAP_MsgCnt > 127)
		MAP_MsgCnt = 0;
	// Update MsgCnt
	(*pSendMsg)->choice.mapFrame.msgCnt = MAP_MsgCnt;

	// timeStamp
	(*pSendMsg)->choice.mapFrame.timeStamp = ALLOC_OBJ(MinuteOfTheYear_t, 1);
	if (!(*pSendMsg)->choice.mapFrame.timeStamp)
	{
		RetVal = MSG_ERR_MALLOC;
		goto Error;
	}
	ttime = time(NULL);
	ptm = gmtime(&ttime);
	*(*pSendMsg)->choice.mapFrame.timeStamp = ptm->tm_yday * 1440
			+ ptm->tm_hour * 60 + ptm->tm_min;

//  将map数据写入到作为配置文件的文件中，实际使用时将会使用其他小工具将此配置文件的内容填充
//	FILE *pMAPMsgfp = fopen("../xml_MAPMsg.txt", "w+");
//	res = xer_fprint(pMAPMsgfp, &asn_DEF_MessageFrame, *pSendMsg);
//
//	assert(res == 0);
//	fclose(pMAPMsgfp);

	Error: return RetVal;
}

