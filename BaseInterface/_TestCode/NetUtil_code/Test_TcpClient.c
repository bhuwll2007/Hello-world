#include "./../../NetUtil/TcpClient.h"
//接收数据回调函数
void sFunRecvData(nint8_t *p_cRecvBuf, uint32_t pnRecvBufSize)
{
    int i = 0;
    if (pnRecvBufSize > 0)
    {
        printf("Recv %d Bytes.\n", pnRecvBufSize);
        for (i = 0; i < pnRecvBufSize; i++)
        {
            printf("%02X ", p_cRecvBuf[i]);
        }
        printf("\n");
    }
}
void Thread_RecvGPSData1(void *p)
{
//	printf("1\n");
    StcTcpClient lTcpClient;
//	printf("2\n");
    TcpClt_InitSoc(&lTcpClient);
//	printf("3\n");
    lTcpClient.pCallBackDealData = sFunRecvData;
    TcpClt_LinkSer(&lTcpClient,"192.168.20.123", 1234);
//	printf("4\n");
    while (1)
    {
        sleep(8);
    }
}

void Thread_RecvGPSData2(void *p)
{
    printf("1\n");
    StcTcpClient lTcpClient;
    printf("2\n");
    TcpClt_InitSoc(&lTcpClient);
    lTcpClient.pCallBackDealData = sFunRecvData;
    printf("3\n");
    TcpClt_LinkSer(&lTcpClient,"10.100.12.55", 8020);
    printf("4\n");
    while (1)
    {
        sleep(8);
    }
}

int main(int argc, char *argv[])
{

    int ret ;
//	struct Hello a;
//	a.sayHello = sayHello;
//	strcpy(a.Name, "11111");
    pthread_t serialThread1;

    //创建串口线程
    ret = pthread_create(&serialThread1, NULL,
                         (void *(*)(void *)) Thread_RecvGPSData1, 0);

//	struct Hello b;
//	b.sayHello = sayHello;
//	strcpy(b.Name, "2222");
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


