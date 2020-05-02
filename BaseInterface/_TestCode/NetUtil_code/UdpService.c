#include "UdpService.h"

#define DEBUG_PRINT_INFO 0
/*********************************************************************
 * 函数名称：nint32_t UdpSrv_InitSoc(StcUdpService *pUdpService)
 * 功能描述：Udp服务器创建socket
 * 输入参数：StcUdpService *pUdpService Udp服务器结构体变量，包含相关变量及函数指针，详见定义
 * 输出参数：无
 * 返回结果：Udp服务器创建状态 >0代表创建成功，值为创建的socket句柄 非0 失败
 * 函数作者：魏林林
 * 创建日期：2018-05-04 14:00:32
 *********************************************************************/
nint32_t UdpSrv_InitSoc(StcUdpService *pUdpService)
{
	nint32_t sockfd;
	//建立一个UDP套接口
	if((sockfd = socket(AF_INET, SOCK_DGRAM, 0))==-1)
	{
		perror("socket");
		return -1;
	}
//	const int opt = -1;
//	int nb = 0;
//	nb = setsockopt(sockfd, SOL_SOCKET, SO_BROADCAST, (char*) &opt, sizeof(opt)); //设置套接字类型
//	if (nb == -1)
//	{
//		printf("set socket error...\n");
//		return -1;
//	}
	if(pUdpService->m_selfPort > 1000)
	{
		struct sockaddr_in UdpSvr_addr;
		//初始化结构体，并绑定指定端口
		memset(&UdpSvr_addr, 0, sizeof(struct sockaddr_in));
		UdpSvr_addr.sin_family = AF_INET;
		UdpSvr_addr.sin_port = htons(pUdpService->m_selfPort);
		UdpSvr_addr.sin_addr.s_addr = htonl(INADDR_BROADCAST)/*INADDR_ANY*/;
		bzero(&(UdpSvr_addr.sin_zero), 8);

		//绑定套接口
		if (bind(sockfd, (struct sockaddr *)&UdpSvr_addr, sizeof(UdpSvr_addr)) == -1)
		{
			perror("bind socket error");
			return -2;
		}
	}
	pUdpService->m_UdpSrvSok = sockfd;
	return 0;
}

/*********************************************************************
 * 函数名称：nint32_t UdpSrv_LinkSer(StcUdpService *pUdpService, nint8_t p_acIp[], nint32_t p_nPort)
 * 功能描述：创建并初始化UDP服务器端Addr，开启数据接收线程，等待接收数据，并将接收到的数据放到处理区
 * 输入参数：StcUdpService *pUdpService Udp服务器结构体变量，包含相关变量及函数指针，详见定义，nint8_t p_acIp[] 服务器IP地址, nint32_t p_nPort 服务器端口号
 * 输出参数：//无
 * 返回结果：Udp服务器创建服务器状态，0 成功，非0 失败
 * 函数作者：魏林林
 * 创建日期：2018-05-04 14:05:43
 *********************************************************************/
nint32_t UdpSrv_LinkSer(StcUdpService *pUdpService/*, nint8_t p_acIp[], nint32_t p_nPort*/)
{
//	memset(&pUdpService->m_UdpSer_addr, 0, sizeof(struct sockaddr_in));
//	pUdpService->m_UdpSer_addr.sin_family = AF_INET;
//	pUdpService->m_UdpSer_addr.sin_port = htons(p_nPort);
//	pUdpService->m_UdpSer_addr.sin_addr.s_addr = inet_addr(p_acIp);
	pUdpService->pThread_UdpSrv_Receive = *Thread_UdpService_Receive;
	pthread_t tidp;
	pthread_create(&tidp, NULL, pUdpService->pThread_UdpSrv_Receive, pUdpService);

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
void *pThread_UdpSrv_DealData(void *arg)
{
	StcUdpService *pUdpService = (StcUdpService *)arg;

	if(pUdpService->pCallBackDealData != NULL)
	{
		pUdpService->pCallBackDealData(pUdpService->m_UdpSrvRecvbuf, pUdpService->m_UdpSrvRecvlen);
	}

	return NULL;
}

/*********************************************************************
 * 函数名称：void *Thread_UdpService_Receive(void *arg)
 * 功能描述：Udp服务器的接收线程，接收数据并通过回调函数传出。
 * 输入参数：void *arg Udp服务器结构体变量指针，详见定义
 * 输出参数：无
 * 返回结果：无
 * 函数作者：魏林林
 * 创建日期：2018年05月04日14:13:05
 *********************************************************************/
void *Thread_UdpService_Receive(void *arg)
{
	StcUdpService *pUdpService= (StcUdpService *)arg;
	socklen_t len;
	nint32_t numbytes = 0;
	nint8_t buf[UDPSRV_RECVBUF_SIZE] = {0};
	struct sockaddr_in lsockaddr_in;
	len = sizeof(struct sockaddr_in);

//	struct sockaddr_in lsockaddr_in;
//	memset(&lsockaddr_in, 0, sizeof(struct sockaddr_in));
//	lsockaddr_in.sin_family = AF_INET;
//	lsockaddr_in.sin_port = htons(pUdpService->m_selfPort + 1);
//	lsockaddr_in.sin_addr.s_addr = inet_addr("192.168.20.255");

	while(1)
	{
		memset(buf, 0, sizeof(buf));
		if ((numbytes = recvfrom(pUdpService->m_UdpSrvSok, buf, sizeof(buf), 0, (struct sockaddr *)&lsockaddr_in, &len)) == -1)
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
			memset(pUdpService->m_UdpSrvRecvbuf, 0x00,sizeof(pUdpService->m_UdpSrvRecvbuf));
			memcpy(pUdpService->m_UdpSrvRecvbuf, buf, numbytes);
			pUdpService->m_UdpSrvRecvlen = numbytes;
#if DEBUG_PRINT_INFO
			printf("%d Recv： %s\n",pUdpService->m_UdpSrvSok, pUdpService->m_UdpSrvRecvbuf);
#endif
#if 0
			pUdpService->pThread_UdpCln_DealData = *pThread_UdpCln_DealData;
			//创建接收线程
			pthread_t ptid;
			pthread_create(&ptid, 0, pUdpService->pThread_UdpCln_DealData, pUdpService);
#endif

			if(pUdpService->pCallBackDealData != NULL)
			{
				pUdpService->pCallBackDealData(pUdpService->m_UdpSrvRecvbuf, pUdpService->m_UdpSrvRecvlen);
			}
		}
	}
	return NULL;
}

/*********************************************************************
 * 函数名称：nint32_t UdpSrv_Send(StcUdpService pUdpService, nint8_t p_abuf[], nint32_t p_nBufSize)
 * 功能描述：Udp服务器向UDP服务器端发送数据，并返回发送状态；
 * 输入参数：StcUdpService pUdpService Udp服务器结构体变量, nint8_t p_abuf[] 发送数据的buf, nint32_t p_nBufSize 发送数据的长度
 * 输出参数：无
 * 返回结果：nint32_t 发送数据的状态 >0 代表发送的数据字节数， <=0 代表发送失败
 * 函数作者：魏林林
 * 创建日期：2018-05-04 14:17:51
 *********************************************************************/
nint32_t UdpSrv_Send(StcUdpService pUdpService, nint8_t p_abuf[], nint32_t p_nBufSize)
{
//	socklen_t len;
	nint32_t send_ret = 0;
	nint32_t pos = 0, count = 0;
	nint32_t ret = 0;
/*
	//struct sockaddr_in lUdpSer_addr;
	memset(&pUdpService.m_UdpSer_addr, 0, sizeof(struct sockaddr_in));
	pUdpService.m_UdpSer_addr.sin_family = AF_INET;
	pUdpService.m_UdpSer_addr.sin_port = htons(pUdpService.m_selfPort + 1);
	//pUdpService.m_UdpSer_addr.sin_addr.s_addr = htonl(INADDR_BROADCAST); //套接字地址为广播地址 //inet_addr("192.168.123.255");
	pUdpService.m_UdpSer_addr.sin_addr.s_addr = inet_addr("192.168.123.255") ; //套接字地址为广播地址

	printf("lUdpSer_addr.sin_port = %d\n",pUdpService.m_UdpSer_addr.sin_port);
	printf("lUdpSer_addr.sin_addr.s_addr = %X\n",pUdpService.m_UdpSer_addr.sin_addr.s_addr);
	len = sizeof(struct sockaddr_in);

	send_ret = sendto(pUdpService.m_UdpSrvSok, p_abuf, p_nBufSize, 0, (struct sockaddr *)&pUdpService.m_UdpSer_addr, len);
	while (send_ret < p_nBufSize) {
		pos += send_ret;
		p_nBufSize -= send_ret;
		send_ret = sendto(pUdpService.m_UdpSrvSok, p_abuf, p_nBufSize, 0, (struct sockaddr *)&pUdpService.m_UdpSer_addr, len);

		count++;
		if (count >= 3) {
			ret = -1;
			return ret;
		}
	}
*/
	struct sockaddr_in addrto[20];
	int i = 0;
	char buf[20];
	for(i = 0;i<10;i++)
	{
		memset(buf, 0x00, sizeof(buf));
		sprintf(buf,"192.168.123.%d",i+20);
		bzero(&addrto[i], sizeof(struct sockaddr_in));
		addrto[i].sin_family = AF_INET;
		addrto[i].sin_addr.s_addr = inet_addr(buf) ; //套接字地址为广播地址
		addrto[i].sin_port = htons(pUdpService.m_selfPort + 1); //套接字广播端口号为6000
	}

	for(i = 0; i < 10; i ++)
	{
		count = 0;
		send_ret = 0;
		int p_nBufSizetmp = p_nBufSize;
//		printf("%x %d %d\n",addrto[i].sin_addr.s_addr,addrto[i].sin_port,p_nBufSizetmp);
		while (send_ret < p_nBufSizetmp) {
			pos += send_ret;
			p_nBufSizetmp -= send_ret;
			send_ret = sendto(pUdpService.m_UdpSrvSok, p_abuf, p_nBufSizetmp, 0, (struct sockaddr *)&addrto[i], sizeof(struct sockaddr_in));
//			printf("%d\n",send_ret);

			count++;
			if (count >= 3) {
				ret = -1;
				break;
			}
		}
	}
	return ret;
}

/*********************************************************************
 * 函数名称：nint32_t UdpSrv_ClsSoc(StcUdpService pUdpService)
 * 功能描述：Udp服务器关闭SOCKET，停止接收数据，清空网络通讯相关变量
 * 输入参数：StcUdpService pUdpService Udp服务器结构体变量
 * 输出参数：//无
 * 返回结果：关闭状态，0成功 非0失败
 * 函数作者：魏林林
 * 创建日期：2018-05-04 14:24:30
 *********************************************************************/
nint32_t UdpSrv_ClsSoc(StcUdpService pUdpService)
{
	if (socket >= 0)
	{
		close(pUdpService.m_UdpSrvSok);
	}
	memset(&(pUdpService.m_UdpSrvSok), 0x00, sizeof(StcUdpService));
	return 0;
}

