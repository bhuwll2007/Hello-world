#include "TcpServer.h"

int sockfd = -1;
int g_nSockClint = -1;

int g_nSendBufLen2PC = 0;
char g_nSendBuf2PC[256];
uint8_t SendFlag = 1;
#define __PRINT_DEBUGLOG_ 0
/****************************************************
 * 函数名称: int InitTcpSerSoc(int pPort)
 * 功能描述: 创建并初始化TCP服务器参数
 * 输入参数: TCP服务器端口号
 * 输出参数: //无
 * 返 回 值: //TCP服务器创建状态
 * 函数作者: 魏林林
 * 创建日期: 2017-01-16 08:56:11
 ****************************************************/
int Rsm_TcpSer_Init(int pPort)
{
	struct sockaddr_in my_addr;
	//建立TCP套接口
	if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
		printf("create socket error");
		perror("socket");
		return -1;
	}
	//初始化结构体，并绑定指定端口
	my_addr.sin_family = AF_INET;
	my_addr.sin_port = htons(pPort);
	my_addr.sin_addr.s_addr = INADDR_ANY;

	//解决端口被占用
	unsigned int value = 0x1;
	setsockopt(sockfd,SOL_SOCKET,SO_REUSEADDR,(void *)&value,sizeof(value));

	bzero(&(my_addr.sin_zero), 8);
	//绑定套接口
	if (bind(sockfd, (struct sockaddr *) &my_addr, sizeof(struct sockaddr))
			== -1) {
		perror("bind socket error");
		return -2;
	}
	//创建监听套接口
	if (listen(sockfd, 10) == -1) {
		perror("listen");
		return -3;
	}
	socklen_t sendbuflen = 0;
	socklen_t len = sizeof(sendbuflen);
	setsockopt(sockfd, SOL_SOCKET, SO_SNDTIMEO, &sendbuflen, len);


	pthread_t tidp;
	pthread_create(&tidp, NULL, Tcp_Listen, NULL);			//开启接收线程接收tcp传来的数据
	return sockfd;
}
/****************************************************
 * 函数名称: void *Tcp_Listen(void * arg)
 * 功能描述: TCP服务器的监听建立连接的线程
 * 输入参数: void * arg 无实际意义
 * 输出参数: //无
 * 返 回 值: //无
 * 函数作者: 魏林林
 * 创建日期: 2017-01-16 09:01:44
 ****************************************************/
void *Tcp_Listen(void * arg) {
	socklen_t sin_size;
	struct sockaddr_in their_addr;
	pthread_t tidp_RecvTcpData;
	int nSoc = -1;
	void *a1;
	while (1) {

		sin_size = sizeof(struct sockaddr_in);
		//如果建立连接，将产生一个全新的套接字
		if ((nSoc = accept(sockfd, (struct sockaddr *) &their_addr, &sin_size))
				== -1) {
			continue ;
		}
		else
		{
			pthread_create(&tidp_RecvTcpData, NULL, Tcp_Thread_function, &nSoc);			//开启接收线程接收tcp传来的数据
			pthread_join(tidp_RecvTcpData, &a1);
			printf("%s\n",(char*)a1);
		}
	}

	close(sockfd);
	printf("close thred!!\n");
	return NULL;
}

//
//int SocketConnected(int sock)
//{
//	if(sock<=0)
//		return 0;
//	struct tcp_info info;
//	int len=sizeof(info);
//	getsockopt(sock, IPPROTO_TCP, TCP_INFO, &info, (socklen_t *)&len);
//	if((info.tcpi_state==TCP_ESTABLISHED))
//	{
////		printf("socket connected\n");
//		return 1;
//	}
//	else
//	{
//		printf("socket disconnected\n");
//		return 0;
//	}
//}



/****************************************************
 * 函数名称: void *Tcp_Thread_function(void * arg)
 * 功能描述: 建立连接之后的数据接收线程
 * 输入参数: void * arg 建立连接的socket
 * 输出参数: //无
 * 返 回 值: //无
 * 函数作者: 魏林林
 * 创建日期: 2017-01-16 09:42:20
 ****************************************************/
void *Tcp_Thread_function(void * arg) {
	int new_fd = *(int *)(arg);
	g_nSockClint = new_fd;

	int Bufflen;
	char buff[1024] = {0};


	while (1) {
		Bufflen = TcpServer_recv(new_fd, buff);

		if(Bufflen > 0)
			RsmTx_CallBack_back(buff, Bufflen);

//		if(SocketConnected(new_fd))
		if (1)
			usleep(1000);
		else
			break;	
	}
	close(g_nSockClint);
	g_nSockClint = -1;
	pthread_exit("Thread Finished!");
}



int TcpServer_recv(int sokfd, char *buf){
	int nbufflen = 0;
	
	if((nbufflen = recv(sokfd, buf, 1024, 0))==-1)
	{
		perror("recv");
		return -1;
	}

	return nbufflen;	
}



int TcpServer_send(int sokfd, char *buf, int nbuflen) {
	if (sokfd > 0)
		;
	else
		return -1;

	int send_ret = 0;

	send_ret = send(sokfd, buf, nbuflen, MSG_NOSIGNAL);

	return send_ret;
}






/**
 * 函数名  	：PktDecode
 * 描述    	：进行数据包的解码工作，去头去尾，通过校验码判断数据包数据是否正确
 * 输入    	: ibuf：待解码的原始帧内容
 *		  	: ilen: 原始帧内容的长度
 * 输出    	: r_u8FrmNo：解码后的帧序号
 *			：obuf：解码后的帧内容，仅包括帧内容，不包括帧序号和校验位
 *         	: olen：解码后的帧内容的长度
 * 返回值  	: -0：解码成功
 *		  	: -1：解码错误
 */
uint32_t Pkt_Decode(uint8_t *src_buf, uint8_t *target_buf, uint32_t len)
//uint8 PktDecodeNEW(uint8_t *buf, uint32_t ilen, uint8_t *obuf, uint32_t *olen)
{
	uint32_t i = 0;
	uint32_t j = 0;
	uint8_t l_u8Chk = 0;
	uint32_t l_acufLen = 0;
	//csk 增加2个ff 开头或者1个ff开头，甘肃使用
	switch(src_buf[1])
	{
		case 0xFF:
			// 原始帧长度至少为5：2个字节帧头，1 个字节帧序号，1个字节校验位，1个字节帧尾
			if ((len < 5) || (len > 255))
			{
				return -1;
			}

			// 确保帧头和帧尾正确
			if ((0xFF != src_buf[0]) || (0xFF != src_buf[1]) || (0xFF != src_buf[len - 1]))
			{
				return -2;
			}

			i = 2;	// 跳过两个字节的帧头
			target_buf[j++] = src_buf[i++]; //取帧序号
			break;
		default:
			// 原始帧长度至少为4：1个字节帧头，1 个字节帧序号，1个字节校验位，1个字节帧尾
			if ((len < 4) || (len > 255))
			{
				return -1;
			}

			// 确保帧头和帧尾正确
			if ((0xFF != src_buf[0]) || (0xFF != src_buf[len - 1]))
			{
				return -2;
			}

			i = 1;	// 跳过一个字节的帧头
			target_buf[j++] = src_buf[i++]; //取帧序号
			break;
	}

	do {
		if (0xFF == src_buf[i]) // 是否到尾
		{
			break;
		}
		else
		{
			target_buf[j] = src_buf[i];
			if (0xFE == src_buf[i])
			{
				target_buf[j] |= src_buf[i+1];
				i++;
			}
			l_u8Chk ^= target_buf[j];
			i++;
			j++;
		}
	} while (1);

	l_u8Chk ^= target_buf[0];

	// 校验正确
	if ((j > 1) && (0 == l_u8Chk))
	{
		l_acufLen = j - 1; // 去掉校验位
		return l_acufLen;
	}
	else
	{
		l_acufLen = 0;
		return -3; // 校验出错
	}
}


