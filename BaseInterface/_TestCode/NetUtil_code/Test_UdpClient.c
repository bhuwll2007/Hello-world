/*
 * test.c
 *
 *  Created on: Mar 20, 2018
 *      Author: root
 */

#include "./UdpClient.h"
//接收数据回调函数
void sFunRecvData(nint8_t *p_cRecvBuf, uint32_t pnRecvBufSize) {
	int i = 0;
	if (pnRecvBufSize > 0) {
		printf("Recv %d Bytes.\n", pnRecvBufSize);
		for (i = 0; i < pnRecvBufSize; i++) {
			printf("%02X ", p_cRecvBuf[i]);
		}
		printf("\n");
	}
}
void Thread_RecvGPSData1(void *p)
{
	char buf[10];
	int i = 0, nRet = 0;
	for(i = 0; i < 10; i ++)
		buf[i] = i * 1;

	StcUdpClient lUdpClient;
	lUdpClient.m_selfPort = 2001;
	UdpClt_InitSoc(&lUdpClient);

	lUdpClient.pCallBackDealData = sFunRecvData;
	UdpClt_LinkSer(&lUdpClient,"10.100.12.55", 8010);

	while (1)
	{
		nRet = UdpClt_Send(lUdpClient, buf, 10);
		printf("1nRet = %d\n",nRet);
		sleep(1);
	}
}

void Thread_RecvGPSData2(void *p)
{
	char buf[10];
	int i = 0, nRet = 0;
	for(i = 0; i < 10; i ++)
		buf[i] = i * 2;
	StcUdpClient lUdpClient;
	lUdpClient.m_selfPort = 2002;
	UdpClt_InitSoc(&lUdpClient);
	lUdpClient.pCallBackDealData = sFunRecvData;
	UdpClt_LinkSer(&lUdpClient,"10.100.12.55", 8020);
	while (1)
	{
		nRet = UdpClt_Send(lUdpClient, buf, 10);
		printf("2nRet = %d\n",nRet);
		sleep(1);
	}
}

int main(int argc, char *argv[]) {
	int ret ;
	pthread_t serialThread1;

	//创建串口线程
	ret = pthread_create(&serialThread1, NULL,
			(void *(*)(void *)) Thread_RecvGPSData1, 0);

	pthread_t serialThread2;
	if (ret == 0)
		;
	//创建串口线程
	ret = pthread_create(&serialThread2, NULL,
			(void *(*)(void *)) Thread_RecvGPSData2, 0);
	if (ret == 0)
		;
	while (1)
		;
	return 0;
}


