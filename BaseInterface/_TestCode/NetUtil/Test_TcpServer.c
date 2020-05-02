#include "./../../NetUtil/TcpServer.h"
StcTcpServer lStcTcpServer;
//接收数据回调函数
void sFunRecvData(uint32_t fd, nint8_t *p_cRecvBuf, uint32_t pnRecvBufSize)
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
    printf("33333-----\n");
    TcpSer_Send(fd, p_cRecvBuf, pnRecvBufSize);
}
void CallBack_Listen(StcTcpCln *pstcTcpCln, int nIndex)
{
    pstcTcpCln->pCallBackDealData = sFunRecvData;
    return ;
}
int main(int argc, char *argv[])
{

    lStcTcpServer.m_TcpSerPort = atoi(argv[1]);
    lStcTcpServer.pCallBack_Listen = CallBack_Listen;
    TcpSer_InitSoc(&lStcTcpServer);
    while (1)
    {
        sleep(1);
    }
    return 0;
}


