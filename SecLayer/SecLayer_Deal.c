/*
 * SecLayer_Deal.c
 *
 *  Created on: Aug 28, 2019
 *      Author: wanji
 */
#include "SecLayer_Deal.h"

pthread_mutex_t mutex_RecvSecLayer;
#define ALLOC_OBJ(type, size) \
  (type *)calloc(1, (sizeof(type)*size))

void SecLayer_Recv_ThreadProc(void *pArg)
{
	char l_acRecvBuf[2048];
	int l_nRecvBufSize;

	int nRet = 0;
	//Thread loop
	int nAid = 0;
	while (1)
	{
		usleep(1000);
		sem_wait(&g_Queue_NetRecv.m_product_number);
		memset(l_acRecvBuf, 0x00, sizeof(l_acRecvBuf));
		l_nRecvBufSize = 0;
		nRet = GetLstNetRecvData(&nAid, l_acRecvBuf, &l_nRecvBufSize);
		if (nRet == 0)
		{
			nRet = SecurityVerify_SecBuf((uint8_t *)l_acRecvBuf, l_nRecvBufSize, nAid);
			if (nRet == 0)
			{
				pthread_mutex_lock(&mutex_RecvSecLayer);
				nRet = sem_trywait(&g_Queue_SecRecv.m_blank_number);
				if ((nRet < 0) || 1)
				{
					g_Queue_SecRecv.m_stcqueue[g_Queue_SecRecv.m_nQIndx_In].mnTxAid =
							nAid;
					g_Queue_SecRecv.m_stcqueue[g_Queue_SecRecv.m_nQIndx_In].mnBuffLen =
							l_nRecvBufSize;
					memcpy(
							g_Queue_SecRecv.m_stcqueue[g_Queue_SecRecv.m_nQIndx_In].macBuff,
							l_acRecvBuf,
							l_nRecvBufSize);
					g_Queue_SecRecv.m_nQIndx_In++;
					g_Queue_SecRecv.m_nQIndx_In %= NUM;
					if ( g_Queue_SecRecv.m_nQIndx_In == g_Queue_SecRecv.m_nQIndx_Out)
					{
						g_Queue_SecRecv.m_nQIndx_Out ++;
						g_Queue_SecRecv.m_nQIndx_Out %= NUM;
					}
				}

//				printf("g_Queue_SecRecv.m_nQIndx_Out = %d\n",g_Queue_SecRecv.m_nQIndx_Out );
//				printf("g_Queue_SecRecv.m_nQIndx_In = %d\n",g_Queue_SecRecv.m_nQIndx_In );

				sem_post(&g_Queue_SecRecv.m_product_number);
				pthread_mutex_unlock(&mutex_RecvSecLayer);
			}
			else
			{
				printf("SecurityVerify_SecBuf failed nRet = %d\n",nRet);
			}
		}
	}

	(void) pthread_exit(NULL);
}

int SecLayer_InitRx(tWAVEConfig pWAVEConfig, int RecvMode)
{
	return NetLayer_InitRx(pWAVEConfig, RecvMode);
}
int SecLayer_Init(int RxMode, uint16_t Aid[], int nAidCnt)
{
	pthread_t ThreadID;

	pthread_attr_t Attr;
	pthread_attr_init(&Attr);

	memset(&g_Queue_SecRecv, 0x00, sizeof(tQueue_RevLst));
	sem_init(&g_Queue_SecRecv.m_blank_number, 0, NUM);
	sem_init(&g_Queue_SecRecv.m_product_number, 0, 0);

	pthread_mutex_init(&mutex_RecvSecLayer, NULL);

	int nRet = DSM_Init(RxMode, Aid, nAidCnt);

	pthread_create(&ThreadID, &Attr, (void *) SecLayer_Recv_ThreadProc, NULL);
	return nRet;
}
//int DSMSend(tDSM_Request dsmSend_rq)
int SecLayer_Send(tDSM_Request dsmSend_rq)
{
	int nSecuredMsgLen = 3000;
	unsigned char pbSecuredMsg[3000] =
	{ 0 };
	int nRet = 0;
	nRet = Encode_SecuredMessage(1, 1, dsmSend_rq.AID,
			(uint8_t *) dsmSend_rq.Data, dsmSend_rq.Length, pbSecuredMsg,
			&nSecuredMsgLen);
	if(nRet == 0)
	{
//		printf("Encode_SecuredMessage Ok , nRet = %d,nSecuredMsgLen = %d\n", nRet,
//				nSecuredMsgLen);
	}
	else
	{
		printf("Encode_SecuredMessage Failed, nRet = %d\n", nRet);
		return 0;
	}
//	//测试延签
//	nRet = SecurityVerify_SecBuf(pbSecuredMsg,nSecuredMsgLen, dsmSend_rq.AID);
//	if(nRet == 0)
//	{
//		printf("!!!SecurityVerify_SecBuf, nRet = %d \n", nRet);
//
//#if Test_PRINTF_TIME_20191015
//	gettimeofday(&tv3, NULL);
//	printf("Verify OK tv1tv_sec = %ld,tv.tv_usec = %ld ",
//			tv1.tv_sec, tv1.tv_usec);
//	printf("tv2tv_sec = %ld,tv.tv_usec = %ld ",
//			tv2.tv_sec, tv2.tv_usec);
//	printf("tv3tv_sec = %ld,tv.tv_usec = %ld\n",
//			tv3.tv_sec, tv3.tv_usec);
//#endif
//	}
//	else
//	{
//
//		printf("***SecurityVerify_SecBuf, nRet = %d \n", nRet);
//#if !Test_PRINTF_TIME_20191015
//	gettimeofday(&tv3, NULL);
//	printf("Verify Bad tv1tv_sec = %ld,tv.tv_usec = %ld ",
//			tv1.tv_sec, tv1.tv_usec);
//	printf("tv2tv_sec = %ld,tv.tv_usec = %ld ",
//			tv2.tv_sec, tv2.tv_usec);
//	printf("tv3tv_sec = %ld,tv.tv_usec = %ld\n",
//			tv3.tv_sec, tv3.tv_usec);
//#endif
//	}

	dsmSend_rq.Length = nSecuredMsgLen;
	dsmSend_rq.Data = (nint8_t *) pbSecuredMsg;

	nRet = DSMSend(dsmSend_rq);

	dsmSend_rq.Data = NULL;
//	ASN_STRUCT_FREE(asn_DEF_SecuredMessage, pSecuredMessage);
	return nRet;
}
// Recv V2X Data Callback function
int GetLstSecRecvData(int *TxAid, SecuredMessage_t *pSecMsg)
{
	pthread_mutex_lock(&mutex_RecvSecLayer);
	asn_dec_rval_t Rval;
	Rval = Sec_OerDecode(pSecMsg,
			g_Queue_SecRecv.m_stcqueue[g_Queue_SecRecv.m_nQIndx_Out].macBuff,
			g_Queue_SecRecv.m_stcqueue[g_Queue_SecRecv.m_nQIndx_Out].mnBuffLen);
	if (Rval.code != RC_OK)
	{
		printf("sec_oer_decode() failed!\n");
		printf("sec_Broken decoding at byte %ld\n", (long) Rval.consumed);
	}
	*TxAid = g_Queue_SecRecv.m_stcqueue[g_Queue_SecRecv.m_nQIndx_Out].mnTxAid;
	g_Queue_SecRecv.m_nQIndx_Out++;
	g_Queue_SecRecv.m_nQIndx_Out %= NUM;
	sem_post(&g_Queue_SecRecv.m_blank_number);
	pthread_mutex_unlock(&mutex_RecvSecLayer);

	return 0;
}

// Recv V2X Data Callback function
int GetLstSecRecvBufData(int *nAid, char buf[], int *buflen)
{
	pthread_mutex_lock(&mutex_RecvSecLayer);
	if (g_Queue_SecRecv.m_nQIndx_Out != g_Queue_SecRecv.m_nQIndx_In)
	{
		asn_dec_rval_t Rval;
		SecuredMessage_t *pSecMsg = ALLOC_OBJ(SecuredMessage_t, 1);
		Rval =
				Sec_OerDecode(pSecMsg,
						g_Queue_SecRecv.m_stcqueue[g_Queue_SecRecv.m_nQIndx_Out].macBuff,
						g_Queue_SecRecv.m_stcqueue[g_Queue_SecRecv.m_nQIndx_Out].mnBuffLen);
		if (Rval.code != RC_OK)
		{
			printf("===sec_oer_decode() failed!\n");
			printf("---sec_Broken decoding at byte %ld\n", (long) Rval.consumed);
		} else
		{
#if SECLAYER_PRINTLOG
			asn_fprint(stdout, &asn_DEF_SecuredMessage, pSecMsg);
			printf("\n");
#endif
			*nAid =
					g_Queue_SecRecv.m_stcqueue[g_Queue_SecRecv.m_nQIndx_Out].mnTxAid;
			if(pSecMsg->payload.choice.signedData.tbs.data != NULL)
			{
				*buflen = pSecMsg->payload.choice.signedData.tbs.data->size;
				memcpy(buf, pSecMsg->payload.choice.signedData.tbs.data->buf,
						pSecMsg->payload.choice.signedData.tbs.data->size);
			}
			else
			{
				*buflen = 0;
			}
		}
		g_Queue_SecRecv.m_nQIndx_Out++;
		g_Queue_SecRecv.m_nQIndx_Out %= NUM;
		sem_post(&g_Queue_SecRecv.m_blank_number);
		ASN_STRUCT_FREE(asn_DEF_SecuredMessage, pSecMsg);
		pthread_mutex_unlock(&mutex_RecvSecLayer);
	} else
	{
		pthread_mutex_unlock(&mutex_RecvSecLayer);
		return -1;
	}
	return 0;
}

void SecHeader_free()
{

}
