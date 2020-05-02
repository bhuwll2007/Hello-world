/*
 * TcpClient.c
 *
 *  Created on: Jun 28, 2017
 *      Author: root
 */
#include "TcpClient.h"
//接收数据回调函数
void CallBack_TcpClientRecvData(nint8_t *p_cRecvBuf, uint32_t pnRecvBufSize) {
	int i = 0;
	if (pnRecvBufSize > 0) {
		printf("Recv %d Bytes.\n", pnRecvBufSize);
		for (i = 0; i < pnRecvBufSize; i++) {
			printf("%02X ", p_cRecvBuf[i]);
		}
		printf("\n");
	}
}
int InitTcpClient(StcTcpClient *pTcpClient, char *pIP, int Port) {

	int nRet = 0;
	nRet = TcpClt_InitSoc(pTcpClient);
	if(nRet < 0)
	{
		return -1;
	}
	pTcpClient->pCallBackDealData = &CallBack_TcpClientRecvData;
	nRet = TcpClt_LinkSer(pTcpClient,pIP, Port);
	if(nRet < 0)
	{
		return -2;
	}
	return 0;
}
