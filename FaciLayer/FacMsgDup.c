#include "FacMsgDup.h"
int AppDupBSM_rq()
{
	return DCR_OPTDUPBSM;
}

int AppDupRSI_rq()
{
	return DCR_OPTDUPRSI;
}

int AppDupMAP_rq()
{
	return DCR_OPTDUPMAP;
}

int AppDupSPAT_rq()
{
	return DCR_OPTDUPSPAT;
}

int AppDupRSM_rq()
{
	return DCR_OPTDUPRSM;
}

int AppDupBSM_rs(int Answer, MessageFrame_t *pBsmMsg)
{
	uint8_t Cnt = 0;
	int Res = MsgDup_ERR_NONE;

	//whether Send request accepted
	if (Answer)
	{
		Res = MsgDup_ERR_REJECT;
		goto Error;
	}

	do
	{
		bsmFrame.MsgData = ALLOC_OBJ(MessageFrame_t, 1);
		Msg_UPERDecode(bsmFrame.MsgData, bsmFrame.Buff, bsmFrame.BuffLen);

		memcpy(pBsmMsg, bsmFrame.MsgData, sizeof(MessageFrame_t));

		//Test wll
		ASN_STRUCT_FREE(asn_DEF_MessageFrame, bsmFrame.MsgData);
		//free(bsmFrame.MsgData);

		Cnt++;
		if (Cnt > 10)
		{
			Res = MsgDup_ERR_MSGTYPE;
			goto Error;
		}
		usleep(100);
	} while (pBsmMsg->present != MessageFrame_PR_bsmFrame);

	Error: return Res;

}

int AppDupRSI_rs(int Answer, MessageFrame_t *pRsiMsg)
{
	uint8_t Cnt = 0;
	int Res = MsgDup_ERR_NONE;

	//whether Send request accepted
	if (Answer)
	{
		Res = MsgDup_ERR_REJECT;
		goto Error;
	}

	do
	{
		rsiFrame.MsgData = ALLOC_OBJ(MessageFrame_t, 1);
		Msg_UPERDecode(rsiFrame.MsgData, rsiFrame.Buff, rsiFrame.BuffLen);

		memcpy(pRsiMsg, rsiFrame.MsgData, sizeof(MessageFrame_t));

		//Test wll
		ASN_STRUCT_FREE(asn_DEF_MessageFrame, rsiFrame.MsgData);
		//free(rsiFrame.MsgData);

		Cnt++;
		if (Cnt > 10)
		{
			Res = MsgDup_ERR_MSGTYPE;
			goto Error;
		}
		usleep(100);
	} while (pRsiMsg->present != MessageFrame_PR_rsiFrame);

	Error: return Res;
}

int AppDupMAP_rs(int Answer, MessageFrame_t *pMapMsg)
{

	uint8_t Cnt = 0;
	int Res = MsgDup_ERR_NONE;

	//whether Send request accepted
	if (Answer)
	{
		Res = MsgDup_ERR_REJECT;
		goto Error;
	}

	do
	{
		mapFrame.MsgData = ALLOC_OBJ(MessageFrame_t, 1);
		Msg_UPERDecode(mapFrame.MsgData, mapFrame.Buff, mapFrame.BuffLen);

		memcpy(pMapMsg, mapFrame.MsgData, sizeof(MessageFrame_t));

		//Test wll
		ASN_STRUCT_FREE(asn_DEF_MessageFrame, mapFrame.MsgData);
		//free(mapFrame.MsgData);

		Cnt++;
		if (Cnt > 10)
		{
			Res = MsgDup_ERR_MSGTYPE;
			goto Error;
		}
		usleep(100);
	} while (pMapMsg->present != MessageFrame_PR_mapFrame);

	Error: return Res;

}

int AppDupSPAT_rs(int Answer, MessageFrame_t *pSpatMsg)
{

	uint8_t Cnt = 0;
	int Res = MsgDup_ERR_NONE;

	//whether Send request accepted
	if (Answer)
	{
		Res = MsgDup_ERR_REJECT;
		goto Error;
	}

	do
	{
		spatFrame.MsgData = ALLOC_OBJ(MessageFrame_t, 1);
		Msg_UPERDecode(spatFrame.MsgData, spatFrame.Buff, spatFrame.BuffLen);

		memcpy(pSpatMsg, spatFrame.MsgData, sizeof(MessageFrame_t));

		//Test wll
		ASN_STRUCT_FREE(asn_DEF_MessageFrame, spatFrame.MsgData);
		//free(spatFrame.MsgData);

		Cnt++;
		if (Cnt > 10)
		{
			Res = MsgDup_ERR_MSGTYPE;
			goto Error;
		}
		usleep(100);
	} while (pSpatMsg->present != MessageFrame_PR_spatFrame);

	Error: return Res;

}

int AppDupRSM_rs(int Answer, MessageFrame_t *pRsmMsg)
{

	uint8_t Cnt = 0;
	int Res = MsgDup_ERR_NONE;

	//whether Send request accepted
	if (Answer)
	{
		Res = MsgDup_ERR_REJECT;
		goto Error;
	}

	do
	{
		rsmFrame.MsgData = ALLOC_OBJ(MessageFrame_t, 1);
		Msg_UPERDecode(rsmFrame.MsgData, rsmFrame.Buff, rsmFrame.BuffLen);

		memcpy(pRsmMsg, rsmFrame.MsgData, sizeof(MessageFrame_t));

		//Test wll
		ASN_STRUCT_FREE(asn_DEF_MessageFrame, rsmFrame.MsgData);
		//free(rsmFrame.MsgData);
		Cnt++;
		if (Cnt > 10)
		{
			Res = MsgDup_ERR_MSGTYPE;
			goto Error;
		}
		usleep(100);
	} while (pRsmMsg->present != MessageFrame_PR_rsmFrame);

	Error: return Res;

}

int AppDupBSM_id()
{
	int Res = MsgDup_ERR_NONE;
	pthread_mutex_init(&mutex_BSM, NULL);
	return Res;
}

int AppDupRSI_id()
{
	int Res = MsgDup_ERR_NONE;
	pthread_mutex_init(&mutex_RSI, NULL);
	return Res;
}

int AppDupMAP_id()
{
	int Res = MsgDup_ERR_NONE;
	pthread_mutex_init(&mutex_MAP, NULL);
	return Res;
}

int AppDupSPAT_id()
{
	int Res = MsgDup_ERR_NONE;
	pthread_mutex_init(&mutex_SPAT, NULL);
	return Res;
}

int AppDupRSM_id()
{
	int Res = MsgDup_ERR_NONE;
	pthread_mutex_init(&mutex_RSM, NULL);
	return Res;
}

int GetLstBsmData(int *TxAid, MessageFrame_t *BsmData)
{
	pthread_mutex_lock(&mutex_BSM);
	if (g_Queue_Recv_bsm.m_nQIndx_Out == g_Queue_Recv_bsm.m_nQIndx_In)
	{
		BsmData->present = MessageFrame_PR_NOTHING;
		pthread_mutex_unlock(&mutex_BSM);
		return 1;
	}
	asn_dec_rval_t Rval;
	Rval =
			Msg_UPERDecode(BsmData,
					g_Queue_Recv_bsm.m_stcqueue[g_Queue_Recv_bsm.m_nQIndx_Out].macBuff,
					g_Queue_Recv_bsm.m_stcqueue[g_Queue_Recv_bsm.m_nQIndx_Out].mnBuffLen);
	if (Rval.code != RC_OK)
	{
		printf("bsm_uper_decode() failed!\n");
		printf("bsm_Broken decoding at byte %ld\n", (long) Rval.consumed);

		g_Queue_Recv_bsm.m_nQIndx_Out++;
		g_Queue_Recv_bsm.m_nQIndx_Out %= NUM;
		sem_post(&g_Queue_Recv_bsm.m_blank_number);
		pthread_mutex_unlock(&mutex_BSM);
		return -1;
	} else
	{
		*TxAid =
				g_Queue_Recv_bsm.m_stcqueue[g_Queue_Recv_bsm.m_nQIndx_Out].mnTxAid;

		g_Queue_Recv_bsm.m_nQIndx_Out++;
		g_Queue_Recv_bsm.m_nQIndx_Out %= NUM;
		sem_post(&g_Queue_Recv_bsm.m_blank_number);
		pthread_mutex_unlock(&mutex_BSM);
		return 0;
	}

}

int GetLstRsiData(int *TxAid, MessageFrame_t *RsiData)
{
	pthread_mutex_lock(&mutex_RSI);

	if (g_Queue_Recv_rsi.m_nQIndx_Out == g_Queue_Recv_rsi.m_nQIndx_In)
	{
		RsiData->present = MessageFrame_PR_NOTHING;
		pthread_mutex_unlock(&mutex_RSI);
		return 1;
	}
	asn_dec_rval_t Rval;

	Rval =
			Msg_UPERDecode(RsiData,
					g_Queue_Recv_rsi.m_stcqueue[g_Queue_Recv_rsi.m_nQIndx_Out].macBuff,
					g_Queue_Recv_rsi.m_stcqueue[g_Queue_Recv_rsi.m_nQIndx_Out].mnBuffLen);
	if (Rval.code != RC_OK)
	{
		printf("rsi_uper_decode() failed!\n");
		printf("rsi_Broken decoding at byte %ld\n", (long) Rval.consumed);
	}
	else
	{

	}
	*TxAid = g_Queue_Recv_rsi.m_stcqueue[g_Queue_Recv_rsi.m_nQIndx_Out].mnTxAid;
	g_Queue_Recv_rsi.m_nQIndx_Out++;
	g_Queue_Recv_rsi.m_nQIndx_Out %= NUM;
	sem_post(&g_Queue_Recv_rsi.m_blank_number);

	pthread_mutex_unlock(&mutex_RSI);

	return Rval.code;
}
int GetLstMapData(int *TxAid, MessageFrame_t *MapData)
{
	pthread_mutex_lock(&mutex_MAP);

	if (g_Queue_Recv_map.m_nQIndx_Out == g_Queue_Recv_map.m_nQIndx_In)
	{
		MapData->present = MessageFrame_PR_NOTHING;
		pthread_mutex_unlock(&mutex_MAP);
		return 1;
	}
	asn_dec_rval_t Rval;

	Rval =
			Msg_UPERDecode(MapData,
					g_Queue_Recv_map.m_stcqueue[g_Queue_Recv_map.m_nQIndx_Out].macBuff,
					g_Queue_Recv_map.m_stcqueue[g_Queue_Recv_map.m_nQIndx_Out].mnBuffLen);
	if (Rval.code != RC_OK)
	{
		printf("map_uper_decode() failed!\n");
		printf("map_Broken decoding at byte %ld\n", (long) Rval.consumed);
		*TxAid = 0;
	}
	*TxAid = g_Queue_Recv_map.m_stcqueue[g_Queue_Recv_map.m_nQIndx_Out].mnTxAid;
	g_Queue_Recv_map.m_nQIndx_Out++;
	g_Queue_Recv_map.m_nQIndx_Out %= NUM;
	sem_post(&g_Queue_Recv_map.m_blank_number);

	pthread_mutex_unlock(&mutex_MAP);
	return Rval.code;
}
int GetLstSpatData(int *TxAid, MessageFrame_t *SpatData)
{
	asn_dec_rval_t Rval;

	pthread_mutex_lock(&mutex_SPAT);

	if (g_Queue_Recv_spat.m_nQIndx_Out == g_Queue_Recv_spat.m_nQIndx_In)
	{
		SpatData->present = MessageFrame_PR_NOTHING;
		pthread_mutex_unlock(&mutex_SPAT);
		return 1;
	}
	Rval =
			Msg_UPERDecode(SpatData,
					g_Queue_Recv_spat.m_stcqueue[g_Queue_Recv_spat.m_nQIndx_Out].macBuff,
					g_Queue_Recv_spat.m_stcqueue[g_Queue_Recv_spat.m_nQIndx_Out].mnBuffLen);
	if (Rval.code != RC_OK)
	{
		printf("spat_uper_decode() failed!\n");
		printf("spat_Broken decoding at byte %ld\n", (long) Rval.consumed);
		*TxAid = 0;
	}
	*TxAid =
			g_Queue_Recv_spat.m_stcqueue[g_Queue_Recv_spat.m_nQIndx_Out].mnTxAid;
	g_Queue_Recv_spat.m_nQIndx_Out++;
	g_Queue_Recv_spat.m_nQIndx_Out %= NUM;
	sem_post(&g_Queue_Recv_spat.m_blank_number);

	pthread_mutex_unlock(&mutex_SPAT);
	return Rval.code;
}
int GetLstRsmData(int *TxAid, MessageFrame_t *RsmData)
{
	//int nRet = 0;
	pthread_mutex_lock(&mutex_RSM);

	if (g_Queue_Recv_rsm.m_nQIndx_Out == g_Queue_Recv_rsm.m_nQIndx_In)
	{
		RsmData->present = MessageFrame_PR_NOTHING;
		pthread_mutex_unlock(&mutex_RSM);
		return 1;
	}
	asn_dec_rval_t Rval;
	Rval =
			Msg_UPERDecode(RsmData,
					g_Queue_Recv_rsm.m_stcqueue[g_Queue_Recv_rsm.m_nQIndx_Out].macBuff,
					g_Queue_Recv_rsm.m_stcqueue[g_Queue_Recv_rsm.m_nQIndx_Out].mnBuffLen);
	if (Rval.code != RC_OK)
	{
		printf("rsm_uper_decode() failed!\n");
		printf("rsm_Broken decoding at byte %ld\n", (long) Rval.consumed);
	}
	*TxAid = g_Queue_Recv_rsm.m_stcqueue[g_Queue_Recv_rsm.m_nQIndx_Out].mnTxAid;
	g_Queue_Recv_rsm.m_nQIndx_Out++;
	g_Queue_Recv_rsm.m_nQIndx_Out %= NUM;
	sem_post(&g_Queue_Recv_rsm.m_blank_number);

	pthread_mutex_unlock(&mutex_RSM);
	return Rval.code;
}
void AppInfo_Process(int nAid, char *p_cRecvBuf, int *pnRecvBufSize)
{
	asn_dec_rval_t Rval;
	int nRet = 0;
#if FACILAYER_PRINTLOG
	printf("Receive AppInfo_Process Message Below, Length-->%d\n",
			*pnRecvBufSize);
	int i;
	for (i = 0; i < *pnRecvBufSize; i ++)
	{
		printf("%02X ", p_cRecvBuf[i]);
	}
	printf("\n");
#endif
	//Allocate Memory
	MessageFrame_t *pRecvMsg;
	pRecvMsg = ALLOC_OBJ(MessageFrame_t, 1);
	//Recv Msg UPER Decode
	Rval = Msg_UPERDecode(pRecvMsg, p_cRecvBuf, *pnRecvBufSize);
	if (Rval.code != RC_OK)
	{
#if FACILAYER_PRINTLOG
		printf("AppInfo_Process_uper_decode() failed!\n");
		printf("AppInfo_Process_Broken encoding at byte %ld\n",
				(long) Rval.consumed);
#endif
		ASN_STRUCT_FREE(asn_DEF_MessageFrame, pRecvMsg);
	} else
	{
		//uper_decode Success
#if FACILAYER_PRINTLOG

		printf("CallBack Information:\n");
		printf("uper_decode() Success! Recv Message Type is %d\n",
				pRecvMsg->present);

		xer_fprint(stdout, &asn_DEF_MessageFrame, pRecvMsg);
		printf("\n");
		asn_fprint(stdout, &asn_DEF_MessageFrame, pRecvMsg);
		printf("\n");

#endif
		int *pChangeFlag = NULL;
		switch (pRecvMsg->present)
		{
		case MessageFrame_PR_bsmFrame:

			pthread_mutex_lock(&mutex_BSM);
			nRet = sem_trywait(&g_Queue_Recv_bsm.m_blank_number);
//			if (nRet < 0)
//			{
//				g_Queue_Recv_bsm.m_stcqueue[g_Queue_Recv_bsm.m_nQIndx_Out].mnTxAid =
//						nAid;
//				g_Queue_Recv_bsm.m_stcqueue[g_Queue_Recv_bsm.m_nQIndx_Out].mnBuffLen =
//						*pnRecvBufSize;
//				memcpy(
//						g_Queue_Recv_bsm.m_stcqueue[g_Queue_Recv_bsm.m_nQIndx_Out].macBuff,
//						p_cRecvBuf,
//						g_Queue_Recv_bsm.m_stcqueue[g_Queue_Recv_bsm.m_nQIndx_Out].mnBuffLen);
//				g_Queue_Recv_bsm.m_nQIndx_Out ++;
//				g_Queue_Recv_bsm.m_nQIndx_Out %= NUM;
//			} else
//			{
//				g_Queue_Recv_bsm.m_stcqueue[g_Queue_Recv_bsm.m_nQIndx_In].mnTxAid =
//						nAid;
//				g_Queue_Recv_bsm.m_stcqueue[g_Queue_Recv_bsm.m_nQIndx_In].mnBuffLen =
//						*pnRecvBufSize;
//				memcpy(
//						g_Queue_Recv_bsm.m_stcqueue[g_Queue_Recv_bsm.m_nQIndx_In].macBuff,
//						p_cRecvBuf,
//						g_Queue_Recv_bsm.m_stcqueue[g_Queue_Recv_bsm.m_nQIndx_In].mnBuffLen);
//				g_Queue_Recv_bsm.m_nQIndx_In++;
//				g_Queue_Recv_bsm.m_nQIndx_In %= NUM;
//			}
			if (nRet < 0)
				pChangeFlag = &g_Queue_Recv_bsm.m_nQIndx_Out;
			else
				pChangeFlag = &g_Queue_Recv_bsm.m_nQIndx_In;

			g_Queue_Recv_bsm.m_stcqueue[*pChangeFlag].mnTxAid = nAid;
			g_Queue_Recv_bsm.m_stcqueue[*pChangeFlag].mnBuffLen =
					*pnRecvBufSize;
			memcpy(g_Queue_Recv_bsm.m_stcqueue[*pChangeFlag].macBuff,
					p_cRecvBuf,
					g_Queue_Recv_bsm.m_stcqueue[*pChangeFlag].mnBuffLen);
			(*pChangeFlag)++;
			(*pChangeFlag) %= NUM;

			sem_post(&g_Queue_Recv_bsm.m_product_number);
			pthread_mutex_unlock(&mutex_BSM);

			ASN_STRUCT_FREE(asn_DEF_MessageFrame, pRecvMsg);
			break;

		case MessageFrame_PR_rsiFrame:

			pthread_mutex_lock(&mutex_RSI);
			nRet = sem_trywait(&g_Queue_Recv_rsi.m_blank_number);
			if (nRet < 0)
			{

				g_Queue_Recv_rsi.m_stcqueue[g_Queue_Recv_rsi.m_nQIndx_Out].mnTxAid =
						nAid;
				g_Queue_Recv_rsi.m_stcqueue[g_Queue_Recv_rsi.m_nQIndx_Out].mnBuffLen =
						*pnRecvBufSize;
				memcpy(
						g_Queue_Recv_rsi.m_stcqueue[g_Queue_Recv_rsi.m_nQIndx_Out].macBuff,
						p_cRecvBuf,
						g_Queue_Recv_rsi.m_stcqueue[g_Queue_Recv_rsi.m_nQIndx_Out].mnBuffLen);
				g_Queue_Recv_rsi.m_nQIndx_Out++;
				g_Queue_Recv_rsi.m_nQIndx_Out %= NUM;
			} else
			{
				g_Queue_Recv_rsi.m_stcqueue[g_Queue_Recv_rsi.m_nQIndx_In].mnTxAid =
						nAid;
				g_Queue_Recv_rsi.m_stcqueue[g_Queue_Recv_rsi.m_nQIndx_In].mnBuffLen =
						*pnRecvBufSize;
				memcpy(
						g_Queue_Recv_rsi.m_stcqueue[g_Queue_Recv_rsi.m_nQIndx_In].macBuff,
						p_cRecvBuf,
						g_Queue_Recv_rsi.m_stcqueue[g_Queue_Recv_rsi.m_nQIndx_In].mnBuffLen);
				g_Queue_Recv_rsi.m_nQIndx_In++;
				g_Queue_Recv_rsi.m_nQIndx_In %= NUM;
			}
			sem_post(&g_Queue_Recv_rsi.m_product_number);

			pthread_mutex_unlock(&mutex_RSI);
			ASN_STRUCT_FREE(asn_DEF_MessageFrame, pRecvMsg);
			break;
		case MessageFrame_PR_mapFrame:

			pthread_mutex_lock(&mutex_MAP);
			nRet = sem_trywait(&g_Queue_Recv_map.m_blank_number);
			if (nRet < 0)
			{
				g_Queue_Recv_map.m_stcqueue[g_Queue_Recv_map.m_nQIndx_Out].mnTxAid =
						nAid;
				g_Queue_Recv_map.m_stcqueue[g_Queue_Recv_map.m_nQIndx_Out].mnBuffLen =
						*pnRecvBufSize;
				memcpy(
						g_Queue_Recv_map.m_stcqueue[g_Queue_Recv_map.m_nQIndx_Out].macBuff,
						p_cRecvBuf,
						g_Queue_Recv_map.m_stcqueue[g_Queue_Recv_map.m_nQIndx_Out].mnBuffLen);
				g_Queue_Recv_map.m_nQIndx_Out++;
				g_Queue_Recv_map.m_nQIndx_Out %= NUM;
			} else
			{
				g_Queue_Recv_map.m_stcqueue[g_Queue_Recv_map.m_nQIndx_In].mnTxAid =
						nAid;
				g_Queue_Recv_map.m_stcqueue[g_Queue_Recv_map.m_nQIndx_In].mnBuffLen =
						*pnRecvBufSize;
				memcpy(
						g_Queue_Recv_map.m_stcqueue[g_Queue_Recv_map.m_nQIndx_In].macBuff,
						p_cRecvBuf,
						g_Queue_Recv_map.m_stcqueue[g_Queue_Recv_map.m_nQIndx_In].mnBuffLen);
				g_Queue_Recv_map.m_nQIndx_In++;
				g_Queue_Recv_map.m_nQIndx_In %= NUM;
			}
			sem_post(&g_Queue_Recv_map.m_product_number);

			pthread_mutex_unlock(&mutex_MAP);
			ASN_STRUCT_FREE(asn_DEF_MessageFrame, pRecvMsg);
			break;
		case MessageFrame_PR_spatFrame:
			pthread_mutex_lock(&mutex_SPAT);
			nRet = sem_trywait(&g_Queue_Recv_spat.m_blank_number);
			if (nRet < 0)
			{
				g_Queue_Recv_spat.m_stcqueue[g_Queue_Recv_spat.m_nQIndx_Out].mnTxAid =
						nAid;
				g_Queue_Recv_spat.m_stcqueue[g_Queue_Recv_spat.m_nQIndx_Out].mnBuffLen =
						*pnRecvBufSize;
				memcpy(
						g_Queue_Recv_spat.m_stcqueue[g_Queue_Recv_spat.m_nQIndx_Out].macBuff,
						p_cRecvBuf,
						g_Queue_Recv_spat.m_stcqueue[g_Queue_Recv_spat.m_nQIndx_Out].mnBuffLen);
				g_Queue_Recv_spat.m_nQIndx_Out++;
				g_Queue_Recv_spat.m_nQIndx_Out %= NUM;

			} else
			{
				g_Queue_Recv_spat.m_stcqueue[g_Queue_Recv_spat.m_nQIndx_In].mnTxAid =
						nAid;
				g_Queue_Recv_spat.m_stcqueue[g_Queue_Recv_spat.m_nQIndx_In].mnBuffLen =
						*pnRecvBufSize;
				memcpy(
						g_Queue_Recv_spat.m_stcqueue[g_Queue_Recv_spat.m_nQIndx_In].macBuff,
						p_cRecvBuf,
						g_Queue_Recv_spat.m_stcqueue[g_Queue_Recv_spat.m_nQIndx_In].mnBuffLen);
				g_Queue_Recv_spat.m_nQIndx_In++;
				g_Queue_Recv_spat.m_nQIndx_In %= NUM;
			}
			sem_post(&g_Queue_Recv_spat.m_product_number);

			pthread_mutex_unlock(&mutex_SPAT);
			ASN_STRUCT_FREE(asn_DEF_MessageFrame, pRecvMsg);
			break;
		case MessageFrame_PR_rsmFrame:

			pthread_mutex_lock(&mutex_RSM);

			nRet = sem_trywait(&g_Queue_Recv_rsm.m_blank_number);
			if (nRet < 0)
			{

				g_Queue_Recv_rsm.m_stcqueue[g_Queue_Recv_rsm.m_nQIndx_Out].mnTxAid =
						nAid;
				g_Queue_Recv_rsm.m_stcqueue[g_Queue_Recv_rsm.m_nQIndx_Out].mnBuffLen =
						*pnRecvBufSize;
				memcpy(
						g_Queue_Recv_rsm.m_stcqueue[g_Queue_Recv_rsm.m_nQIndx_Out].macBuff,
						p_cRecvBuf,
						g_Queue_Recv_rsm.m_stcqueue[g_Queue_Recv_rsm.m_nQIndx_Out].mnBuffLen);
				g_Queue_Recv_rsm.m_nQIndx_Out++;
				g_Queue_Recv_rsm.m_nQIndx_Out %= NUM;
			} else
			{
				g_Queue_Recv_rsm.m_stcqueue[g_Queue_Recv_rsm.m_nQIndx_In].mnTxAid =
						nAid;
				g_Queue_Recv_rsm.m_stcqueue[g_Queue_Recv_rsm.m_nQIndx_In].mnBuffLen =
						*pnRecvBufSize;
				memcpy(
						g_Queue_Recv_rsm.m_stcqueue[g_Queue_Recv_rsm.m_nQIndx_In].macBuff,
						p_cRecvBuf,
						g_Queue_Recv_rsm.m_stcqueue[g_Queue_Recv_rsm.m_nQIndx_In].mnBuffLen);
				g_Queue_Recv_rsm.m_nQIndx_In++;
				g_Queue_Recv_rsm.m_nQIndx_In %= NUM;
			}
			sem_post(&g_Queue_Recv_rsm.m_product_number);

			pthread_mutex_unlock(&mutex_RSM);
			ASN_STRUCT_FREE(asn_DEF_MessageFrame, pRecvMsg);
			break;

		default:
			ASN_STRUCT_FREE(asn_DEF_MessageFrame, pRecvMsg);
			break;
		}
	}
}

void FaciLayer_Recv_ThreadProc(void *pArg)
{
	char l_acRecvBuf[2048];
	int l_nRecvBufSize;

	int nRet = 0;
	//Thread loop
	int nAid = 0;
	while (1)
	{
		usleep(1000);
//		printf("0sem_wait(&g_Queue_SecRecv.m_product_number);\n");
		sem_wait(&g_Queue_SecRecv.m_product_number);
//		printf("1sem_wait(&g_Queue_SecRecv.m_product_number);\n");
		memset(l_acRecvBuf, 0x00, sizeof(l_acRecvBuf));
		l_nRecvBufSize = 0;
		nRet = GetLstSecRecvBufData(&nAid, l_acRecvBuf, &l_nRecvBufSize);
		if (nRet == 0)
		{
			AppInfo_Process(nAid, l_acRecvBuf, &l_nRecvBufSize);
		}
	}

	(void) pthread_exit(NULL);
}

int FaciLayer_Recv()
{
	pthread_t ThreadID;

	pthread_attr_t Attr;
	pthread_attr_init(&Attr);

	int Res = 0;

	// Create thread
	Res = pthread_create(&ThreadID, &Attr, (void *) FaciLayer_Recv_ThreadProc,
			NULL);

	//Success
	Res = 0;
	return Res;

}
