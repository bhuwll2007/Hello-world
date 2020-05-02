#include "./../../NetUtil/UdpService.h"
//接收数据回调函数
//void sFunRecvData(nint8_t *p_cRecvBuf, uint32_t pnRecvBufSize) {
//	int i = 0;
//	if (pnRecvBufSize > 0) {
//		printf("Recv %d Bytes.\n", pnRecvBufSize);
//		for (i = 0; i < pnRecvBufSize; i++) {
//			printf("%02X ", p_cRecvBuf[i]);
//		}
//		printf("\n");
//	}
//}
//void Thread_RecvGPSData1(void *p)
//{
//	StcTcpClient lTcpClient;
//	TcpClt_InitSoc(&lTcpClient);
//	lTcpClient.pCallBackDealData = sFunRecvData;
//	TcpClt_LinkSer(&lTcpClient,"10.100.12.55", 8010);
//	while (1)
//	{
//		sleep(8);
//	}
//}
//
//void Thread_RecvGPSData2(void *p)
//{
//	StcTcpClient lTcpClient;
//	TcpClt_InitSoc(&lTcpClient);
//	lTcpClient.pCallBackDealData = sFunRecvData;
//	TcpClt_LinkSer(&lTcpClient,"10.100.12.55", 8020);
//	while (1)
//	{
//		sleep(8);
//	}
//}

int main(int argc, char *argv[]) {

//	int ret ;
//	pthread_t serialThread1;
//	//创建串口线程
//	ret = pthread_create(&serialThread1, NULL,
//			(void *(*)(void *)) Thread_RecvGPSData1, 0);
//	if (ret == 0)
//		;
//
//	pthread_t serialThread2;
//	//创建串口线程
//	ret = pthread_create(&serialThread2, NULL,
//			(void *(*)(void *)) Thread_RecvGPSData2, 0);
//	if (ret == 0)
//		;

	while (1)
		;

	return 0;
}


