/*
 * DSMP.c
 *
 *  Created on: Aug 27, 2018
 *      Author: root
 */

#include "DSMP.h"
#include "MIBUtil.h"

pthread_mutex_t mutex_RecvNetLayer;

int DSM_Init(int RxMode, uint16_t Aid[], int nAidCnt)
{
	int res = 0;
	int Res = Init_ERR_NONE;

	memset(&g_Queue_NetRecv, 0x00, sizeof(tQueue_RevLst));
	sem_init(&g_Queue_NetRecv.m_blank_number, 0, NUM);
	sem_init(&g_Queue_NetRecv.m_product_number, 0, 0);

	pthread_mutex_init(&mutex_RecvNetLayer, NULL);
	//V2X通信初始化
	V2X_Tx_Init();
	if (Res)
	{
		Res = Init_ERR_TxINIT_FAIL;
		goto Error;
	}
	if (RxMode == 1)
	{
		V2X_Rx_Init(MsgRx_CallBack);
	}

	//启动MIB注册
	Res = DSM_MIB_Setup(Aid, nAidCnt);
	if (Res == 0)
		;
	Error: return res;
}
int DSMSend(tDSM_Request dsmSend_rq)
{
	int Res;
	char *pBuf;
	pBuf = calloc(1, dsmSend_rq.Length + 20);
	//Create DSM　Message Header
	int HdrLength = DSMPHeader_Create(dsmSend_rq, &pBuf);

	//Add DSM Data pDSMPayLoad in send pBuf
	memcpy(pBuf + HdrLength, dsmSend_rq.Data, dsmSend_rq.Length);

#if NETLAYER_PRINTLOG
	int i;
	printf("Send DSM Message Below, Length-->%d\n",
			dsmSend_rq.Length + HdrLength);
	for (i = 0; i < dsmSend_rq.Length + HdrLength; i++)
	{
		printf("%02X ", pBuf[i]);
		if (i % 16 == 0)
		{
			printf("\n");
		}
	}
	printf("\n");
#endif

	//send DSM
	Res = V2X_SendData(dsmSend_rq.AID, pBuf, dsmSend_rq.Length + HdrLength);

	free(pBuf);
	pBuf = NULL;
	return Res;
}

int DSMPHeader_Create(tDSM_Request dsmSend_rq, char **pHdr)
{
	int HdrLen = 0;
	char Hdr[100];

	//版本号
	uint8_t Version = DSMP_VERSION;

	//扩展域
	uint8_t OptionIndicator;
	if (dsmSend_rq.DsmpHeaderExtensions == NULL)
	{
		OptionIndicator = 0;
	} else
	{
		OptionIndicator = 1;
	}
	OptionIndicator = 0;
	//Adapt Layer Protocol Type(该字段为适配层协议类型，暂放在这儿)
	Hdr[HdrLen++] = 0x04;

	//DSMP Version(3 bits), Option Indicator(1 bit) and reserved(4 bits --- 0000)
	Version = (0xFF & DSMP_VERSION) << 5;
	Version |= (OptionIndicator << 4);
	Hdr[HdrLen++] = Version;

	if (OptionIndicator == 1)
	{
		tExtension Extension;

		Extension.ElementID = 15;
		Extension.ByteLength = 1;

		Extension.ExtensionInfo = calloc(1, Extension.ByteLength);
		*Extension.ExtensionInfo = 112;

		Hdr[HdrLen++] = Extension.ElementID;
		Hdr[HdrLen++] = Extension.ByteLength;
		memcpy(&Hdr[HdrLen], Extension.ExtensionInfo, Extension.ByteLength);
		HdrLen += Extension.ByteLength;

		free(Extension.ExtensionInfo);
		Extension.ExtensionInfo = NULL;
	}

	//AID Type
	if (dsmSend_rq.AID <= 0x7F)
	{
		//1 Byte
		Hdr[HdrLen++] = (char) dsmSend_rq.AID;
	} else
	{
		Hdr[HdrLen++] = (dsmSend_rq.AID + 0x7F80) >> 8;
		Hdr[HdrLen++] = (dsmSend_rq.AID + 0x7F80);
	}

	//Data Length
	short Len = htons(dsmSend_rq.Length);
	memcpy(&Hdr[HdrLen], &Len, sizeof(short));
	HdrLen += sizeof(uint16_t);

	memcpy(*pHdr, Hdr, HdrLen);

	return HdrLen;
}

void DSMPHeader_Decode(char *pBuf, int *pnRecvBufSize, tDSMPHdr *pDsmpHdr)
{
	int nIndex = 1;

	//get DSMP Version and Indicator
	uint8_t Temp_MultInfo = pBuf[nIndex++];
	pDsmpHdr->Version = (Temp_MultInfo & 0xFF) >> 5;
	pDsmpHdr->DSMP_OptionIdr = (Temp_MultInfo & 0x10) >> 4;

	if (pDsmpHdr->DSMP_OptionIdr == 1)
	{
		pDsmpHdr->pExtension = calloc(1, sizeof(tExtension));

		pDsmpHdr->pExtension->ElementID = pBuf[nIndex++];
		pDsmpHdr->pExtension->ByteLength = pBuf[nIndex++];
		pDsmpHdr->pExtension->ExtensionInfo = calloc(1,
				pDsmpHdr->pExtension->ByteLength + 1);
		//*pDsmpHdr->pExtension->ExtensionInfo = pBuf[nIndex++];
		memcpy(pDsmpHdr->pExtension->ExtensionInfo, &pBuf[nIndex],
				pDsmpHdr->pExtension->ByteLength);
		nIndex += pDsmpHdr->pExtension->ByteLength;
	}

	//get AID Type
	uint8_t Temp_AIDJudege = pBuf[nIndex++];
	if (Temp_AIDJudege <= 0x7F)
	{
		pDsmpHdr->AID = Temp_AIDJudege;
	} else
	{
//		pDsmpHdr->AID = ((pBuf[nIndex + 1 ]  & 0xFF) << 8) + Temp_AIDJudege;
//		nIndex += 1;
		pDsmpHdr->AID = (pBuf[nIndex++] + (Temp_AIDJudege << 8)) - 0x7F80;
	}

	//get Data Length
	pDsmpHdr->Length = ((pBuf[nIndex] & 0xFF) << 8) + pBuf[nIndex + 1];
	nIndex += 2;

	//get Data Buffer
	pDsmpHdr->Data = calloc(1, pDsmpHdr->Length);
	memcpy(pDsmpHdr->Data, &pBuf[nIndex], pDsmpHdr->Length);

#if NETLAYER_PRINTLOG
	printf("DSM Version:%d,\nDSMP_OptionIdr:%d,\nDSM AID:%d,\nLength:%d,\n",
			pDsmpHdr->Version, pDsmpHdr->DSMP_OptionIdr, pDsmpHdr->AID,
			pDsmpHdr->Length);
#endif
}

// Recv V2X Data Callback function
void MsgRx_CallBack(char *p_cRecvBuf, int *pnRecvBufSize)
{
	char *pBuf = NULL;
	pBuf = p_cRecvBuf;
//	g_RecvNetMsgCnt++;
#if NETLAYER_PRINTLOG
	printf("Receive DSM Message Below, Length-->%d\n", *pnRecvBufSize);
	int i;
	for (i = 0; i < *pnRecvBufSize; i++)
	{
		printf("%02X ", p_cRecvBuf[i]);
	}
	printf("\n");
#endif

	//判断是否存在网络层数据
//	if (g_nIsHavNetLayer == 0) //没有网络层
//	{
//		sem_wait(&g_Queue_NetRecv.m_blank_number);
//		g_Queue_NetRecv.m_stcqueue[g_Queue_NetRecv.m_nQIndx_In].mnBuffLen =
//				*pnRecvBufSize;
//		memcpy(g_Queue_NetRecv.m_stcqueue[g_Queue_NetRecv.m_nQIndx_In].macBuff,
//				p_cRecvBuf,
//				g_Queue_NetRecv.m_stcqueue[g_Queue_NetRecv.m_nQIndx_In].mnBuffLen);
//		g_Queue_NetRecv.m_nQIndx_In ++;
//		g_Queue_NetRecv.m_nQIndx_In %= NUM;
//		sem_post(&g_Queue_NetRecv.m_product_number);
//		return;
//	}
	int nRet = 0;
	switch (pBuf[0])
	{
	case ADAPT_LAYER_DSMP:
	{
		tDSMPHdr *pDsmpHdr;
		pDsmpHdr = calloc(1, sizeof(tDSMPHdr));

		//DSM帧头解析
		DSMPHeader_Decode(pBuf, pnRecvBufSize, pDsmpHdr);

		//注册AID匹配
		if (DSM_AID_Check(pDsmpHdr->AID) == 0)
		{
			if (pDsmpHdr->Data[0] != 0x02)
			{
				DSMPHeader_free(pDsmpHdr);
				break;
			}
			if (pDsmpHdr->Length > MAXSIZE_MSGBUF)
			{
				DSMPHeader_free(pDsmpHdr);
				break;
			}
			//用于上层处理回调函数
			pthread_mutex_lock(&mutex_RecvNetLayer);
			nRet = sem_trywait(&g_Queue_NetRecv.m_blank_number);
			if (nRet == 0){}
			{
				g_Queue_NetRecv.m_stcqueue[g_Queue_NetRecv.m_nQIndx_In].mnTxAid =
						pDsmpHdr->AID;
				g_Queue_NetRecv.m_stcqueue[g_Queue_NetRecv.m_nQIndx_In].mnBuffLen =
						pDsmpHdr->Length;
				memcpy(
						g_Queue_NetRecv.m_stcqueue[g_Queue_NetRecv.m_nQIndx_In].macBuff,
						pDsmpHdr->Data,
						g_Queue_NetRecv.m_stcqueue[g_Queue_NetRecv.m_nQIndx_In].mnBuffLen);
				g_Queue_NetRecv.m_nQIndx_In++;
				g_Queue_NetRecv.m_nQIndx_In %= NUM;

				if (g_Queue_NetRecv.m_nQIndx_In == g_Queue_NetRecv.m_nQIndx_Out)
				{
					g_Queue_NetRecv.m_nQIndx_Out++;
					g_Queue_NetRecv.m_nQIndx_Out %= NUM;
				}
#if NETLAYER_PRINTLOG
				printf("MsgRx_CallBack(In, Out) = (%d %d)\n",g_Queue_NetRecv.m_nQIndx_In,g_Queue_NetRecv.m_nQIndx_Out );
#endif
			}
			sem_post(&g_Queue_NetRecv.m_product_number);
			pthread_mutex_unlock(&mutex_RecvNetLayer);
		}
		DSMPHeader_free(pDsmpHdr);
	}
		break;
	case ADAPT_LAYER_IPV6:
		break;
	case ADAPT_LAYER_IPV4:
		break;
	}
}
int GetLstNetRecvData(int *nAid, char buf[], int *buflen)
{
	pthread_mutex_lock(&mutex_RecvNetLayer);
#if NETLAYER_PRINTLOG
	printf("\t\t\t\t\t\t\tGetLstNetRecvData(In, Out) = (%d %d)\n",g_Queue_NetRecv.m_nQIndx_In,g_Queue_NetRecv.m_nQIndx_Out );
#endif
	if (g_Queue_NetRecv.m_nQIndx_Out != g_Queue_NetRecv.m_nQIndx_In)
	{
		*nAid =
				g_Queue_NetRecv.m_stcqueue[g_Queue_NetRecv.m_nQIndx_Out].mnTxAid;
		*buflen =
				g_Queue_NetRecv.m_stcqueue[g_Queue_NetRecv.m_nQIndx_Out].mnBuffLen;
		memcpy(buf,
				g_Queue_NetRecv.m_stcqueue[g_Queue_NetRecv.m_nQIndx_Out].macBuff,
				g_Queue_NetRecv.m_stcqueue[g_Queue_NetRecv.m_nQIndx_Out].mnBuffLen);
		g_Queue_NetRecv.m_nQIndx_Out++;
		g_Queue_NetRecv.m_nQIndx_Out %= NUM;
		sem_post(&g_Queue_NetRecv.m_blank_number);
		pthread_mutex_unlock(&mutex_RecvNetLayer);
		return 0;
	} else
	{
		pthread_mutex_unlock(&mutex_RecvNetLayer);
		return -1;
	}

}

void DSMPHeader_free(tDSMPHdr *pDsmpHdr)
{
	if (pDsmpHdr->Data != NULL)
	{
		free(pDsmpHdr->Data);
		pDsmpHdr->Data = NULL;
	}

	if (pDsmpHdr->pExtension != NULL)
	{
		if (pDsmpHdr->pExtension->ExtensionInfo != NULL)
		{
			free(pDsmpHdr->pExtension->ExtensionInfo);
			pDsmpHdr->pExtension->ExtensionInfo = NULL;
		}
		free(pDsmpHdr->pExtension);
		pDsmpHdr->pExtension = NULL;
	}

	if (pDsmpHdr != NULL)
	{
		free(pDsmpHdr);
		pDsmpHdr = NULL;
	}
}
