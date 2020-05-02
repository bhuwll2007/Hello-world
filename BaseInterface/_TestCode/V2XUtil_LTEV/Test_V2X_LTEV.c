#include "./../../V2XUtil/V2X_INTERFACE.h"
//接收数据回调函数
void MsgRx_CallBack(char *p_cRecvBuf, int *pnRecvBufSize) {
	int i = 0;
	if (*pnRecvBufSize > 0) {
		printf("Recv %d Bytes.\n", *pnRecvBufSize);
		for (i = 0; i < *pnRecvBufSize; i++) {
			printf("%02X ", p_cRecvBuf[i]);
		}
		printf("\n");
	}
}
int main() {
	//char ss[2000];
	int Res = 0;
	Res = V2X_Tx_Init();
	if (Res)
	{
		Res = -1;
		goto Error;
	}

	Res = V2X_Rx_Init(MsgRx_CallBack);
	if (Res)
	{
		Res = -2;
		goto Error;
	}

	while (1)
	{
		sleep(5);
	}
	Error:
	return 0;
}
