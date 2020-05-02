#include "TcpServer.h"
#define DEBUG_PRINT_INFO 0
/*********************************************************************
 * 函数名称：nint32_t TcpSer_InitSoc(StcTcpServer *pStcTcpServer)
 * 功能描述：创建并初始化TCP服务器端口，绑定端口号，开启监听连接线程，监听连接线程中一旦有客户端连接，创建数据接收线程，等待接收数据，并将接收到的数据放到处理区。
 * 输入参数：StcTcpServer *pStcTcpServer TCP服务器结构体变量，包含TCP服务器参数配置及回调函数指针等，详见定义
 * 输出参数：//无
 * 返回结果：TCP服务器创建状态 >0代表创建成功，值为创建的socket句柄 非0 失败
 * 函数作者：魏林林
 * 创建日期：2018-05-04 12:38:34
 *********************************************************************/
int TcpSer_InitSoc(StcTcpServer *pStcTcpServer)
{
	struct sockaddr_in TcpSvr_addr;
	int sockfd;
	//建立TCP套接口
	if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1)
	{
		perror("socket");
		return -1;
	}

	int i = 0;
	for (i = 0; i < TCP_SVR_MAXCLTNUM; i++)
	{
		memset(&(pStcTcpServer->m_StcTcpCln[i].m_TcpClnSok), 0x00,
				sizeof(StcTcpCln));
	}

	pStcTcpServer->m_TcpSerSok = sockfd;
	//初始化结构体，并绑定指定端口
	TcpSvr_addr.sin_family = AF_INET;
	TcpSvr_addr.sin_port = htons(pStcTcpServer->m_TcpSerPort);
	TcpSvr_addr.sin_addr.s_addr = INADDR_ANY;
	bzero(&(TcpSvr_addr.sin_zero), 8);

	//设置该端口可复用
	socklen_t nResue_Enable = 1;
	socklen_t len = sizeof(nResue_Enable);
	setsockopt(pStcTcpServer->m_TcpSerSok, SOL_SOCKET, SO_REUSEADDR, &nResue_Enable,
			len);

	//设置 发送超时时间。
	int nNetTimeOut = 10;//10毫秒
	setsockopt(pStcTcpServer->m_TcpSerSok, SOL_SOCKET, SO_SNDTIMEO,  (char*) &nNetTimeOut, sizeof(int));

	//设置 发送小包不等待
	int nEnable = 1;
	setsockopt(pStcTcpServer->m_TcpSerSok, IPPROTO_TCP, TCP_NODELAY,  (char*) &nEnable, sizeof(nEnable));

	//绑定 套接口
	if (bind(pStcTcpServer->m_TcpSerSok, (struct sockaddr *) &TcpSvr_addr,
			sizeof(struct sockaddr)) == -1)
	{
		perror("bind socket error");
		return -2;
	}

	//创建 监听套接口
	if (listen(pStcTcpServer->m_TcpSerSok, 10) == -1)
	{
		perror("listen");
		return -3;
	}
	//开启接收线程接收tcp传来的数据
	pStcTcpServer->pThread_TcpSev_Listen = *Thread_TcpSer_Listen;

	pthread_t tidp;
	pthread_create(&tidp, NULL, pStcTcpServer->pThread_TcpSev_Listen,
			pStcTcpServer);
//	pthread_join(tidp, NULL);
	return pStcTcpServer->m_TcpSerSok;
}

/*********************************************************************
 * 函数名称：nint32_t CheckTcpLst(StcTcpCln m_StcTcpCln[], nint32_t nTcpClnSoc)
 * 功能描述：在客户端列表中查找当前空闲的客户端位置，如果有空闲的，将客户端句柄赋值给查找到的客户端句柄，并将查找到的客户端下标返回；如果查找失败，则返回-1。
 * 输入参数：StcTcpCln m_StcTcpCln[] 客户端列表 长度：10, nint32_t nTcpClnSoc 需要加入的客户端句柄
 * 输出参数：//无
 * 返回结果：查找空闲客户端位置的结果 >=0代表查找成功，此时返回的值代表当前客户端所在列表的下标位置；<0 表示查找失败或者当前没有空闲的客户端位置。
 * 函数作者：魏林林
 * 创建日期：2018-05-04 12:38:34
 *********************************************************************/
int CheckTcpLst(StcTcpCln m_StcTcpCln[], nint32_t nTcpClnSoc)
{
	nint32_t i;
	for (i = 0; i < TCP_SVR_MAXCLTNUM; i++)
	{
		if (m_StcTcpCln[i].m_TcpClnSok <= 0)
		{
			m_StcTcpCln[i].m_TcpClnSok = nTcpClnSoc;
			return i;
		}
	}
	m_StcTcpCln[0].m_TcpClnSok = nTcpClnSoc;
	return 0;
}
/*********************************************************************
 * 函数名称：void *Thread_TcpSer_Listen(void * arg);
 * 功能描述：TCP服务器的监听建立连接的线程
 * 输入参数：void * arg TCP服务器结构体变量指针
 * 输出参数：//无
 * 返回结果：//void * 无实际意义
 * 函数作者：魏林林
 * 创建日期：2018-05-04 12:57:47
 *********************************************************************/
void *Thread_TcpSer_Listen(void * arg)
{
	pthread_detach(pthread_self());
	socklen_t sin_size;
	struct sockaddr_in TcpClt_addr;
	StcTcpServer * pTcpServer = (StcTcpServer *) arg;
	int nClnSoc = -1;

	sin_size = sizeof(struct sockaddr_in);
	while (1)
	{
		//如果建立连接，将产生一个全新的套接字
		if ((nClnSoc = accept(pTcpServer->m_TcpSerSok,
				(struct sockaddr *) &TcpClt_addr, &sin_size)) == -1)
		{
//			continue;
		}
		else
		{
			int nInsertId = CheckTcpLst(pTcpServer->m_StcTcpCln, nClnSoc);
			if (nInsertId >= 0)
			{
				pTcpServer->m_StcTcpCln[nInsertId].m_TcpClnSok = nClnSoc;
//				if (pTcpServer->pCallBack_Listen)
//				{
//					pTcpServer->pCallBack_Listen(
//							&(pTcpServer->m_StcTcpCln[nInsertId]), nInsertId);
//
//					pthread_t tidp_RecvTcpData;
//					pthread_create(&tidp_RecvTcpData, NULL, Thread_Client_Deal,
//							&(pTcpServer->m_StcTcpCln[nInsertId])); //开启接收线程接收tcp传来的数据
//				}
			}
			else
			{
				close(nClnSoc);
				nClnSoc = -1;
			}
		}
		usleep(1000);
	}
	close(pTcpServer->m_TcpSerSok);
#if DEBUG_PRINT_INFO
	printf("close thread.\n");
#endif
	pthread_exit(0);
}

/*********************************************************************
 * 函数名称：void *pThread_TcpSCln_DealData(void *arg)
 * 功能描述：
 * 输入参数：
 * 输出参数：
 * 返回结果：
 * 函数作者：
 * 创建日期：
 *********************************************************************/
void *pThread_TcpSCln_DealData(void *arg)
{
	pthread_detach(pthread_self());

	StcTcpCln *pTcpClient = (StcTcpCln *) arg;

	if (pTcpClient->pCallBackDealData != NULL)
	{
		pTcpClient->pCallBackDealData(pTcpClient->m_TcpClnSok,
				pTcpClient->m_TcpSvrRecvbuf, pTcpClient->m_TcpSvrRecvlen);
	}

	pthread_exit(0);
}
/*********************************************************************
 * 函数名称：void *Thread_Client_Deal(void * arg)
 * 功能描述：TCP客户端建立连接之后的数据接收线程
 * 输入参数：void * arg 建立连接的TCP客户端相关变量
 * 输出参数：//无
 * 返回结果：void * 无意义
 * 函数作者：魏林林
 * 创建日期：2018-05-04 13:05:06
 *********************************************************************/
void *Thread_Client_Deal(void * arg)
{
	pthread_detach(pthread_self());
	StcTcpCln *lstcTcpCln = (StcTcpCln *) (arg);

	nint32_t numbytes = 0;
	nint8_t buf[TCP_SVR_RECV_BUF_SIZE];

	while (1)
	{
		memset(buf, 0, sizeof(buf));
		if ((numbytes = recv(lstcTcpCln->m_TcpClnSok, buf, sizeof(buf), 0)) < 0)
		{
			perror("recv");
			printf("recv = -1\n");
			break;
		}
		else if (numbytes == 0)  // 客户端断开了连接
		{
			printf("recv = 0\n");
			break;
		}
		else
		{
			// TODO:处理收到的数据
			memcpy(lstcTcpCln->m_TcpSvrRecvbuf, buf, numbytes);
			lstcTcpCln->m_TcpSvrRecvlen = numbytes;
#if DEBUG_PRINT_INFO
			printf("%d Recv： %s\n",lstcTcpCln->m_TcpClnSok, lstcTcpCln->m_TcpSvrRecvbuf);
#endif
#if 0
			lstcTcpCln->pThread_TcpSCln_DealData = *pThread_TcpSCln_DealData;
			//创建接收线程
			pthread_t ptid;
			pthread_create(&ptid, 0, lstcTcpCln->pThread_TcpSCln_DealData, lstcTcpCln);
#endif
			if (lstcTcpCln->pCallBackDealData != NULL)
			{
				lstcTcpCln->pCallBackDealData(lstcTcpCln->m_TcpClnSok,
						lstcTcpCln->m_TcpSvrRecvbuf,
						lstcTcpCln->m_TcpSvrRecvlen);
			}
		}
		usleep(1000);
	}

	// 接收失败，关闭fd
	close(lstcTcpCln->m_TcpClnSok);
	lstcTcpCln->m_TcpClnSok = -1;

	pthread_exit(0);
}

/*********************************************************************
 * 函数名称：nint32_t TcpSer_Send(uint32_t sockfd, nint8_t buf[], uint32_t nbuflen)
 * 功能描述：TCP服务器端向客户端发送数据，并返回发送状态；
 * 输入参数：uint32_t sokfd TCP客户端端口, nint8_t buf[] 发送数据的buf, uint32_t nbuflen 发送数据的长度
 * 输出参数：//无
 * 返回结果：nint32_t 发送数据的状态 <0 表示发送失败，>0表示发送成功，此时值代表发送数据长度
 * 函数作者：魏林林
 * 创建日期：2018-05-04 13:14:32
 *********************************************************************/
//int TcpSer_Send(uint32_t sockfd, nint8_t buf[], uint32_t nbuflen)
//{
//	int send_ret = 0;
//	int pos = 0, count = 0;
//	int ret = 0;
//
//	if (sockfd <= 0)
//	{
//		ret = -1;
//		return ret;
//	}
//
//	nint8_t buf1[10000];
//	Fun_Xor((uint8_t*) buf, nbuflen);
//	uint32_t len = 0;
//	len = pkt_code((uint8_t*) buf, (uint8_t*) buf1, nbuflen);
//	nbuflen = len;
//	send_ret = send(sockfd, buf1, nbuflen, MSG_NOSIGNAL);
//	if (send_ret == -1)
//	{
//		perror("Tcp send error");
//		ret = -2;
//		return ret;
//	}
//	while (send_ret < nbuflen)
//	{
//		pos += send_ret;
//		nbuflen -= send_ret;
//		send_ret = send(sockfd, &buf1[pos], nbuflen, MSG_NOSIGNAL);
//
//		count++;
//		if (count >= 3)
//		{
//			ret = -3;
//			break;
//		}
//	}
//	if (ret == -1)
//		return ret;
//	else
//		return send_ret;
//}
//

nint8_t g_an8buf[20000];
int TcpSer_Send(uint32_t sockfd, nint8_t buf[], uint32_t nbuflen)
{
	int send_ret = 0;
	int pos = 0, count = 0;
	int ret = 0;

	if (sockfd <= 0)
	{
		ret = -1;
		return ret;
	}

	memset(g_an8buf, 0x00, sizeof(g_an8buf));
	Fun_Xor((uint8_t*) buf, nbuflen);

	uint32_t len = 0;
	len = pkt_code((uint8_t*) buf, (uint8_t*) g_an8buf, nbuflen);
	nbuflen = len;
	send_ret = send(sockfd, g_an8buf, nbuflen, MSG_NOSIGNAL);
	if (send_ret == -1)
	{
		perror("Tcp send error");
		ret = -2;
		return ret;
	}
	while (send_ret < nbuflen)
	{
		pos += send_ret;
		nbuflen -= send_ret;
		send_ret = send(sockfd, &g_an8buf[pos], nbuflen, MSG_NOSIGNAL);

		count++;
		if (count >= 3)
		{
			ret = -3;
			break;
		}
	}
	if (ret == -1)
		return ret;
	else
		return send_ret;
}

/*********************************************************************
 * 函数名称：nint32_t TcpSer_SendAll(StcTcpServer *pStcTcpServer, nint8_t buf[], uint32_t nbuflen)
 * 功能描述：TCP服务器端向所有正在连接着的客户端发送数据；
 * 输入参数：StcTcpServer *pStcTcpServer TCP服务器结构体变量，包含TCP服务器参数配置及回调函数指针等，详见定义, nint8_t buf[] 发送数据的buf, uint32_t nbuflen 发送数据的长度
 * 输出参数：无
 * 返回结果：nint32_t 发送数据的状态 此处默认返回0
 * 函数作者：魏林林
 * 创建日期：2018-05-04 13:27:22
 *********************************************************************/
nint32_t TcpSer_SendAll(StcTcpServer *pStcTcpServer, nint8_t buf[],
		uint32_t nbuflen)
{
	nint32_t i = 0;
	nint32_t nRet = 0;
	for (i = 0; i < TCP_SVR_MAXCLTNUM; i++)
	{
		if (pStcTcpServer->m_StcTcpCln[i].m_TcpClnSok > 0)
		{
			nRet = TcpSer_Send(pStcTcpServer->m_StcTcpCln[i].m_TcpClnSok, buf,
					nbuflen);
			if (nRet < 0)
			{
				close(pStcTcpServer->m_StcTcpCln[i].m_TcpClnSok);
				pStcTcpServer->m_StcTcpCln[i].m_TcpClnSok = 0;
			}
		}
		else
		{
			pStcTcpServer->m_StcTcpCln[i].m_TcpClnSok = 0;
		}
	}
	return 0;
}
/*********************************************************************
 * 函数名称：nint32_t TcpSer_ClsSoc(StcTcpServer *pStcTcpServer)
 * 功能描述：关闭Tcp服务器Socket，停止接收数据线程和监听线程，清空相关变量
 * 输入参数：StcTcpServer *pStcTcpServer TCP服务器结构体变量，包含TCP服务器参数配置及回调函数指针等，详见定义,
 * 输出参数：无
 * 返回结果：关闭状态，0成功 非0失败 此处默认0
 * 函数作者：魏林林
 * 创建日期：2018-05-04 13:27:27
 *********************************************************************/
int TcpSer_ClsSoc(StcTcpServer *pStcTcpServer)
{
	if (pStcTcpServer->m_TcpSerSok > 0)
	{
		close(pStcTcpServer->m_TcpSerSok);
		pStcTcpServer->m_TcpSerSok = 0;
	}

	memset(pStcTcpServer, 0x00, sizeof(StcTcpServer));

	pStcTcpServer->m_TcpSerSok = 0;
	pStcTcpServer->pCallBack_Listen = NULL;
	pStcTcpServer->pThread_TcpSev_Listen = NULL;

	uint32_t i = 0;

	for (i = 0; i < TCP_SVR_MAXCLTNUM; i++)
	{
		memset(&(pStcTcpServer->m_StcTcpCln[i].m_TcpClnSok), 0x00,
				sizeof(StcTcpCln));
	}

	return 0;
}
