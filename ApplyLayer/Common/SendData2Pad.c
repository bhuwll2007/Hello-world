/*
 * SendData2Pad.c
 *
 *  Created on: Nov 10, 2018
 *      Author: root
 */
#include "SendData2Pad.h"

pthread_mutex_t g_mutex_TcpSendData;
int g_nMode = 0;

void initMsgQueue_Send2Veh()
{
	//创建消息队列
	while (1)
	{
//		g_nMsgId_Send2VehTransefer = MsgQueue_New(10052, IPC_CREAT);
		g_nMsgId_Send2VehTransefer = msgget(10052, IPC_CREAT);
		if (g_nMsgId_Send2VehTransefer < 0)
		{
			printf("msq not existed! errno=%d [%s]\n", errno, strerror(errno));
			sleep(2);
			continue;
		} else
		{
			printf("msgget(nMsg_Key, IPC_EXCL) = %d\n", g_nMsgId_Send2VehTransefer);
			break;
		}
	}
}

int fun_SendData2Pad(nint8_t *pbuf, nint32_t len)
{
#if UPMODE==TCPMODE
	if(g_nMode == 0)
	{
		g_nMode = 1;
		//初始化互斥信号量
		pthread_mutex_init(&g_mutex_TcpSendData, NULL);
	}

	pthread_mutex_lock(&g_mutex_TcpSendData);
//
//	tMsg msgs;
//	int ret_value = 0;
//
////	int i = 0;
////	for(i = 0;i<len;i++)
////	{
////		printf("%02X ",pbuf[i]);
////	}
////	printf("\n");
//	msgs.nMsg_Cmd = 0x01;;
//	memcpy(&msgs.acMsg_Text[0], pbuf, len);
//
//	/* 发送消息队列 */
////	ret_value = msgsnd(g_nMsgId_Send2VehTransefer, (void *)&msgs, sizeof(msgs.acMsg_Text)- sizeof(msgs.nMsg_Cmd),IPC_NOWAIT);
//	ret_value = msgsnd(g_nMsgId_Send2VehTransefer, (void *)&msgs, len, IPC_NOWAIT);
//
////	ret_value = MsgQueue_Send(g_nMsgId_Send2VehTransefer, msgs);
//	if(ret_value > 0)
//	{
//		printf("Send %s\n", msgs.acMsg_Text);
//	}

	if (g_StcTcpServer.m_TcpSerSok > 0)
	{
		TcpSer_SendAll(&g_StcTcpServer, pbuf, len);
	}
	pthread_mutex_unlock(&g_mutex_TcpSendData);
#if 0
	if (len > 0)
	{
		int i = 0;
		struct timeval tv1;
		gettimeofday(&tv1, NULL);
		printf("Time TcpSend: tv.tv_sec = %ld,tv.tv_usec = %ld\n", tv1.tv_sec,
				tv1.tv_usec);

		printf("TcpService :Send %d Bytes.\n", len);
		for (i = 0; i < len; i++)
		{
			printf("%02X ", pbuf[i]);
		}
		printf("\n");
	}
#endif
#else
	UdpSrv_Send(g_StcUdpService, pbuf, len);
#endif
	return 0;
}

