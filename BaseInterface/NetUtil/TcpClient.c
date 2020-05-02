/*
 * TcpClient.c
 *
 *  Created on: Jun 28, 2017
 *      Author: root
 */
#include "TcpClient.h"

#define DEBUG_PRINT_INFO 0
/****************************************************
 * 函数名称: int TcpClt_InitSoc()
 * 功能描述: 创建并初始化TCP客户端口。
 * 输入参数: TCP服务器端口号
 * 输出参数: //无
 * 返 回 值: TCP服务器创建状态 >0代表创建成功，值为创建的socket句柄 非0 失败
 * 函数作者: 魏林林
 * 创建日期: 2017-01-16 08:56:11
 ****************************************************/
int TcpClt_InitSoc(StcTcpClient *p_TcpClient)
{
    int lsockfd;
    //建立一个TCP套接口
    if((lsockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1)
    {
        perror("socket");
        return -1;
    }
    p_TcpClient->m_TcpClnSok = lsockfd;

	//设置 发送超时时间。
	int nNetTimeOut = 20;//10毫秒
	setsockopt(p_TcpClient->m_TcpClnSok, SOL_SOCKET, SO_SNDTIMEO,  (char*) &nNetTimeOut, sizeof(int));

    return 0;
}

/****************************************************
 * 函数名称: void *Thread_Client_Receive(void *arg)
 * 功能描述: TCP服务器的接收线程
 * 输入参数: TCP客户端socket句柄
 * 输出参数: //无
 * 返 回 值: //无
 * 函数作者: 魏林林
 * 创建日期: 2017-01-16 08:56:11
 ****************************************************/
void *pThread_TcpCln_DealData(void *arg)
{
    StcTcpClient *pTcpClient = (StcTcpClient *)arg;

    if(pTcpClient->pCallBackDealData != NULL)
    {
        pTcpClient->pCallBackDealData(pTcpClient->m_TcpCltRecvbuf, pTcpClient->m_TcpCltRecvlen);
    }

    return NULL;
}

/****************************************************
 * 函数名称: void *Thread_Client_Receive(void *arg)
 * 功能描述: TCP服务器的接收线程
 * 输入参数: TCP客户端socket句柄
 * 输出参数: //无
 * 返 回 值: //无
 * 函数作者: 魏林林
 * 创建日期: 2017-01-16 08:56:11
 ****************************************************/
void *Thread_TcpCln_Receive(void *arg)
{
    StcTcpClient *pTcpClient = (StcTcpClient *)arg;
    int numbytes = 0;
    char buf[512] = {0};

    while (1)
    {
        memset(buf, 0, sizeof(buf));
        //接受从服务器返回的信息
        if((numbytes = recv(pTcpClient->m_TcpClnSok, buf, sizeof(buf), 0)) == -1)
        {
            pTcpClient->is_Connect = 0;
            perror("recv");
            return NULL;
        }
        else if(numbytes == 0)
        {
            pTcpClient->is_Connect = 0;
            printf("Server closed is_Connect = %d\n", pTcpClient->is_Connect);
            return NULL;
        }
        else
        {
            //TODO:处理接收到的数据
            memset(pTcpClient->m_TcpCltRecvbuf, 0x00,sizeof(pTcpClient->m_TcpCltRecvbuf));
            memcpy(pTcpClient->m_TcpCltRecvbuf, buf, numbytes);
            pTcpClient->m_TcpCltRecvlen = numbytes;
#if DEBUG_PRINT_INFO
            printf("%d Recv： %s\n",pTcpClient->m_TcpClnSok, pTcpClient->m_TcpCltRecvbuf);
#endif
            if(pTcpClient->pCallBackDealData != NULL)
            {
                pTcpClient->pCallBackDealData(pTcpClient->m_TcpCltRecvbuf, pTcpClient->m_TcpCltRecvlen);
            }
#if 0
            pTcpClient->pThread_TcpCln_DealData = *pThread_TcpCln_DealData;
            //创建接收线程
            pthread_t ptid;
            pthread_create(&ptid, 0, pTcpClient->pThread_TcpCln_DealData, pTcpClient);
#endif
        }
    }

    return NULL;
}

/****************************************************
 * 函数名称: int TcpClt_LinkSer(char p_acIp[], int p_nPort)
 * 功能描述: TCP客户端连接服务器，成功后创建数据接收线程，等待接收数据，并将接收到的数据放到处理区
 * 输入参数: char p_acIp[] 服务器IP地址, int p_nPort 服务器端口号
 * 输出参数: //无
 * 返 回 值: 客户端连接服务器状态，0 成功，非0 失败
 * 函数作者: 魏林林
 * 创建日期: 2017-01-16 08:56:11
 ****************************************************/
int TcpClt_LinkSer(StcTcpClient *p_TcpClient, char p_acIp[], int p_nPort)
{
    struct sockaddr_in TcpSvr_addr;
    bzero(&TcpSvr_addr,sizeof(struct sockaddr_in));
    //初始化结构体，连接到服务器端口
    TcpSvr_addr.sin_family = AF_INET;
    TcpSvr_addr.sin_port = htons(p_nPort);
    inet_aton(p_acIp, &TcpSvr_addr.sin_addr);

    //和服务器建立连接
    if(connect(p_TcpClient->m_TcpClnSok, (struct sockaddr *)&TcpSvr_addr, sizeof(struct sockaddr)) == -1)
    {
        perror("connect");
        return -1;
    }
    p_TcpClient->pThread_TcpCln_Receive = *Thread_TcpCln_Receive;
    //创建接收线程
    pthread_t ptid;
    pthread_create(&ptid, 0, p_TcpClient->pThread_TcpCln_Receive, p_TcpClient);

    return 0;
}

/****************************************************
 * 函数名称: int TcpClt_Send(int sockfd, char p_abuf[], int p_nBufSize)
 * 功能描述: TCP客户端向服务器端发送数据，并返回发送状态；
 * 输入参数: int sokfd TCP服务器端口句柄, char p_abuf[] 发送数据的buf, int p_nBufSize 发送数据的长度
 * 输出参数: //无
 * 返 回 值: //int 发送数据的状态 >0 代表发送的数据字节数， <=0 代表发送失败
 * 函数作者: 魏林林
 * 创建日期: 2017-01-16 08:56:11
 ****************************************************/
int TcpClt_Send(StcTcpClient *p_TcpClient, char p_abuf[], int p_nBufSize)
{
    if(p_TcpClient->is_Connect == 0)
	return 0;
    int send_ret = 0;
    int pos = 0, count = 0;
    int ret = 0;
    uint8_t buf1[4000];
    uint32_t len = 0;

    Fun_Xor((uint8_t*)p_abuf, (uint32_t)p_nBufSize);
    len = pkt_code((uint8_t*)p_abuf, buf1, (uint32_t)p_nBufSize);
    p_nBufSize = len;
    if ((send_ret = send(p_TcpClient->m_TcpClnSok, buf1, p_nBufSize,0)) == -1)
    {
        ret = -1;
        return -1;
    }
    while (send_ret < p_nBufSize)
    {
        pos += send_ret;
        p_nBufSize -= send_ret;
        send_ret = send(p_TcpClient->m_TcpClnSok, &buf1[pos], p_nBufSize, 0);

        count++;
        if (count >= 3)
        {
            ret = -2;
            break;
        }
    }

    if(ret == 0)
        return send_ret;
    else
        return ret;
}

/****************************************************
 * 函数名称: int TcpClt_ClsSoc()
 * 功能描述: TCP客户端关闭SOCKET，停止接收数据，清空网络通讯相关变量
 * 输入参数: TCP客户端socket句柄
 * 输出参数: //无
 * 返 回 值: 关闭状态，0成功 非0失败
 * 函数作者: 魏林林
 * 创建日期: 2017-01-16 08:56:11
 ****************************************************/
int TcpClt_ClsSoc(StcTcpClient *p_TcpClient)
{
    if (socket > 0)
    {
        close(p_TcpClient->m_TcpClnSok);
    }
    return 0;
}
