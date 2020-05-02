#include "UdpClient.h"

#define DEBUG_PRINT_INFO 0
/*********************************************************************
 * 函数名称：nint32_t UdpClt_InitSoc(StcUdpClient *pUdpClient)
 * 功能描述：Udp客户端创建socket
 * 输入参数：StcUdpClient *pUdpClient UDP客户端结构体变量，包含相关变量及函数指针，详见定义
 * 输出参数：无
 * 返回结果：Udp客户端创建状态 >0代表创建成功，值为创建的socket句柄 非0 失败
 * 函数作者：魏林林
 * 创建日期：2018-05-04 14:00:32
 *********************************************************************/
nint32_t UdpClt_InitSoc(StcUdpClient *pUdpClient)
{
    nint32_t sockfd;
    //建立一个UDP套接口
    if((sockfd = socket(AF_INET, SOCK_DGRAM, 0))==-1)
    {
        perror("socket");
        return -1;
    }

    if(pUdpClient->m_selfPort > 1000)
    {
        struct sockaddr_in UdpSvr_addr;
        //初始化结构体，并绑定指定端口
        memset(&UdpSvr_addr, 0, sizeof(struct sockaddr_in));
        UdpSvr_addr.sin_family = AF_INET;
        UdpSvr_addr.sin_port = htons(pUdpClient->m_selfPort);
        UdpSvr_addr.sin_addr.s_addr = INADDR_ANY;
        bzero(&(UdpSvr_addr.sin_zero), 8);

        //绑定套接口
        if (bind(sockfd, (struct sockaddr *)&UdpSvr_addr, sizeof(UdpSvr_addr)) == -1)
        {
            perror("bind socket error");
            return -2;
        }
    }
    pUdpClient->m_UdpClnSok = sockfd;
    return 0;
}

/*********************************************************************
 * 函数名称：nint32_t UdpClt_LinkSer(StcUdpClient *pUdpClient, nint8_t p_acIp[], nint32_t p_nPort)
 * 功能描述：创建并初始化UDP服务器端Addr，开启数据接收线程，等待接收数据，并将接收到的数据放到处理区
 * 输入参数：StcUdpClient *pUdpClient UDP客户端结构体变量，包含相关变量及函数指针，详见定义，nint8_t p_acIp[] 服务器IP地址, nint32_t p_nPort 服务器端口号
 * 输出参数：//无
 * 返回结果：udp客户端创建服务器状态，0 成功，非0 失败
 * 函数作者：魏林林
 * 创建日期：2018-05-04 14:05:43
 *********************************************************************/
nint32_t UdpClt_LinkSer(StcUdpClient *pUdpClient, nint8_t p_acIp[], nint32_t p_nPort)
{
    memset(&pUdpClient->m_UdpSer_addr, 0, sizeof(struct sockaddr_in));
    pUdpClient->m_UdpSer_addr.sin_family = AF_INET;
    pUdpClient->m_UdpSer_addr.sin_port = htons(p_nPort);
    pUdpClient->m_UdpSer_addr.sin_addr.s_addr = inet_addr(p_acIp);
    pUdpClient->pThread_UdpCln_Receive = *Thread_UdpClient_Receive;
    pthread_t tidp;
    pthread_create(&tidp, NULL, pUdpClient->pThread_UdpCln_Receive, pUdpClient);

    return 0;
}

/*********************************************************************
 * 函数名称：void *pThread_UdpCln_DealData(void *arg)
 * 功能描述：
 * 输入参数：
 * 输出参数：
 * 返回结果：
 * 函数作者：魏林林
 * 创建日期：2018-05-04 14:10:10
 *********************************************************************/
void *pThread_UdpCln_DealData(void *arg)
{
    StcUdpClient *pUdpClient = (StcUdpClient *)arg;

    if(pUdpClient->pCallBackDealData != NULL)
    {
        pUdpClient->pCallBackDealData(pUdpClient->m_UdpCltRecvbuf, pUdpClient->m_UdpCltRecvlen);
    }

    return NULL;
}

/*********************************************************************
 * 函数名称：void *Thread_UdpClient_Receive(void *arg)
 * 功能描述：UDP客户端的接收线程，接收数据并通过回调函数传出。
 * 输入参数：void *arg UDP客户端结构体变量指针，详见定义
 * 输出参数：无
 * 返回结果：无
 * 函数作者：魏林林
 * 创建日期：2018年05月04日14:13:05
 *********************************************************************/
void *Thread_UdpClient_Receive(void *arg)
{
    StcUdpClient *pUdpClient= (StcUdpClient *)arg;
    socklen_t len;
    nint32_t numbytes = 0;
    nint8_t buf[UDP_CLT_RECV_BUF_SIZE] = {0};

    len = sizeof(struct sockaddr_in);

    while(1)
    {
        memset(buf, 0, sizeof(buf));
        if ((numbytes = recvfrom(pUdpClient->m_UdpClnSok, buf, sizeof(buf), 0, (struct sockaddr *)&(pUdpClient->m_UdpSer_addr), &len)) == -1)
        {
            perror("recv");
            break;
        }
        if (numbytes <= 0)
        {
            break;
        }
        else
        {
            //TODO:处理收到的数据
            memset(pUdpClient->m_UdpCltRecvbuf, 0x00,sizeof(pUdpClient->m_UdpCltRecvbuf));
            memcpy(pUdpClient->m_UdpCltRecvbuf, buf, numbytes);
            pUdpClient->m_UdpCltRecvlen = numbytes;
#if DEBUG_PRINT_INFO
            printf("%d Recv： %s\n",pUdpClient->m_UdpClnSok, pUdpClient->m_UdpCltRecvbuf);
#endif
#if 0
            pUdpClient->pThread_UdpCln_DealData = *pThread_UdpCln_DealData;
            //创建接收线程
            pthread_t ptid;
            pthread_create(&ptid, 0, pUdpClient->pThread_UdpCln_DealData, pUdpClient);
#endif

            if(pUdpClient->pCallBackDealData != NULL)
            {
                pUdpClient->pCallBackDealData(pUdpClient->m_UdpCltRecvbuf, pUdpClient->m_UdpCltRecvlen);
            }
        }
    }
    return NULL;
}

/*********************************************************************
 * 函数名称：nint32_t UdpClt_Send(StcUdpClient pUdpClient, nint8_t p_abuf[], nint32_t p_nBufSize)
 * 功能描述：UDP客户端向UDP服务器端发送数据，并返回发送状态；
 * 输入参数：StcUdpClient pUdpClient UDP客户端结构体变量, nint8_t p_abuf[] 发送数据的buf, nint32_t p_nBufSize 发送数据的长度
 * 输出参数：无
 * 返回结果：nint32_t 发送数据的状态 >0 代表发送的数据字节数， <=0 代表发送失败
 * 函数作者：魏林林
 * 创建日期：2018-05-04 14:17:51
 *********************************************************************/
nint32_t UdpClt_Send(StcUdpClient pUdpClient, nint8_t p_abuf[], nint32_t p_nBufSize)
{
    socklen_t len;
    nint32_t send_ret = 0;
    nint32_t pos = 0, count = 0;
    nint32_t ret = 0;

    len = sizeof(struct sockaddr_in);

    send_ret = sendto(pUdpClient.m_UdpClnSok, p_abuf, p_nBufSize, 0, (struct sockaddr *)&pUdpClient.m_UdpSer_addr, len);
    while (send_ret < p_nBufSize)
    {
        pos += send_ret;
        p_nBufSize -= send_ret;
        send_ret = sendto(pUdpClient.m_UdpClnSok, p_abuf, p_nBufSize, 0, (struct sockaddr *)&pUdpClient.m_UdpSer_addr, len);

        count++;
        if (count >= 3)
        {
            ret = -1;
            return ret;
//			pUdpClient.m_UdpSerSok = 0;
//			break;
        }
    }

    return send_ret;
}

/*********************************************************************
 * 函数名称：nint32_t UdpClt_ClsSoc(StcUdpClient pUdpClient)
 * 功能描述：UDP客户端关闭SOCKET，停止接收数据，清空网络通讯相关变量
 * 输入参数：StcUdpClient pUdpClient UDP客户端结构体变量
 * 输出参数：//无
 * 返回结果：关闭状态，0成功 非0失败
 * 函数作者：魏林林
 * 创建日期：2018-05-04 14:24:30
 *********************************************************************/
nint32_t UdpClt_ClsSoc(StcUdpClient pUdpClient)
{
    if (socket >= 0)
    {
        close(pUdpClient.m_UdpClnSok);
    }
    memset(&(pUdpClient.m_UdpClnSok), 0x00, sizeof(StcUdpClient));
    return 0;
}

