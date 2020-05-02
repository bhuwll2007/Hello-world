/*
 * TcpClient.c
 *
 *  Created on: Jul 15, 2019
 *      Author: wanji-hxy
 */
#include"TcpClient.h"
int g_nPlatform_ClientSok = 0;


int Tcp_InitTcpClientSoc(int *pSocket)
{
    int lsockfd;
    //建立一个TCP套接口
    if((lsockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1)
    {
        perror("socket");
        return -1;
    }

	//设置 发送超时时间。
	int nNetTimeOut = 20;//10毫秒
	setsockopt(lsockfd, SOL_SOCKET, SO_SNDTIMEO,  (char*) &nNetTimeOut, sizeof(int));

	*pSocket = lsockfd;
	return lsockfd;
}

int Tcp_InitTcpCltCntSer(char p_acIp[], int p_nPort, int lsocked)
{
	struct sockaddr_in their_addr;
	//初始化结构体，连接到服务器的2323端口
	their_addr.sin_family = AF_INET;
	their_addr.sin_port = htons(p_nPort);
	// their_addr.sin_addr = *((struct in_addr *)he->h_addr);
	inet_aton(p_acIp, &their_addr.sin_addr );

	bzero(&(their_addr.sin_zero),8);
	//和服务器建立连接
	if(connect(lsocked,(struct sockaddr *)&their_addr,sizeof(struct sockaddr))==-1)
	{
		perror("connect");
		return -1;
	}
	return 0;
}

int Tcp_TcpCltSnd2Data(int sockfd, char p_abuf[], int p_nBufSize)
{
	int numbytes = 0;

	//向服务器发送数据
	if((numbytes = send(sockfd, p_abuf, p_nBufSize,0))==-1)
	{
		return -1;
	}

	return numbytes;
}

int Tcp_TcpCltRcvData(int sockfd, char p_abuf[], int *p_nBufSize)
{
	int numbytes = 0;
	//接受从服务器返回的信息
	if((numbytes = recv(sockfd,p_abuf,100,0))==-1)
	{
		perror("recv");
		return -1;
	}

	*p_nBufSize = numbytes;
	return numbytes;
}

int Tcp_ClsSoc(int sockfd)
{
    if (sockfd > 0)
    {
        close(sockfd);
    }
    return 0;
}


