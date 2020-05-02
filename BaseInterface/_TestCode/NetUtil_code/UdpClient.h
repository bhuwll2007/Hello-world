/**
 * @defgroup NETUtil NETUtil API
 * @ingroup BaseInterface API
 * @{
 *
 * 本模块提供对UDP客户端通讯进行操作的API接口.
 *
 * @file Udpclient.h
 * @brief API file.
 *
 */
#ifndef __UDPCLIENT_H_
#define __UDPCLIENT_H_
#include "./DataSet.h"

/**
 * UDP客户端接收数据缓存大小
 */
#define UDP_CLT_RECV_BUF_SIZE 512
/**
 * @brief UdpClient相关操作结构体.
 */
typedef struct __UdpClient
{
	/**
	 * UDP客户端操作句柄
	 */
	uint32_t m_UdpClnSok;
	/**
	 * UDP客户端端口号
	 */
	uint16_t m_selfPort;
	/**
	 * UDP客户端发送的对方socket信息
	 */
	struct sockaddr_in m_UdpSer_addr;
	/**
	 * UDP客户端接收数据缓存
	 */
	nint8_t m_UdpCltRecvbuf[UDP_CLT_RECV_BUF_SIZE];
	/**
	 * UDP客户端接收数据的缓存的大小
	 */
	uint32_t m_UdpCltRecvlen;

	/**
	 * @brief 网络接收数据线程 ----调用方不要使用
	 * @param arg 无意义
	 */
	void *(*pThread_UdpCln_Receive)(void *arg);

	/**
	 * @brief 网络接收数据后处理线程 ----调用方不要使用
	 * @param arg 无意义
	 */
	void *(*pThread_UdpCln_DealData)(void *arg);
	/**
	 * @brief 回调函数数据处理函数 ----调用方需要按照格式实现
	 *
	 * @param p_cDealBuf 接收数据缓存
	 * @param pnDealBufSize 接收数据缓存大小
	 */
	void (*pCallBackDealData)(nint8_t *p_cDealBuf, uint32_t pnDealBufSize);
} StcUdpClient;

/**
 * @brief UdpClt_InitSoc Udp客户端创建socket
 *
 * Udp客户端创建socket
 *
 * @param[in] pUdpClient UDP客户端结构体变量，包含相关变量及函数指针，详见定义， @see StcUdpClient
 *
 * @return  Udp客户端创建状态 >0代表创建成功，值为创建的socket句柄 非0 失败
 */
nint32_t UdpClt_InitSoc(StcUdpClient *pUdpClient);

/**
 * @brief UdpClt_LinkSer udp客户端配置服务器，开启接收数据线程
 *
 * 创建并初始化UDP服务器端Addr，开启数据接收线程，等待接收数据，并将接收到的数据放到处理区
 *
 * @param[in] pUdpClient UDP客户端结构体变量，包含相关变量及函数指针，详见定义， @see StcUdpClient
 * @param[in] p_acIp[] 服务器IP地址,
 * @param[in] p_nPort 服务器端口号
 *
 * @return  udp客户端配置服务器状态，0 成功，非0 失败
 */
nint32_t UdpClt_LinkSer(StcUdpClient *pUdpClient, nint8_t p_acIp[],
		nint32_t p_nPort);

/**
 * @brief pThread_UdpCln_DealData 关闭UDP
 *
 * UDP客户端的数据处理线程。
 *
 * @param[in] arg 无意义
 *
 * @return  无意义
 */
void *pThread_UdpCln_DealData(void *arg);

/**
 * @brief Thread_UdpClient_Receive 关闭UDP
 *
 * UDP客户端的接收线程，接收数据并通过回调函数传出。
 *
 * @param[in] arg 无意义
 *
 * @return  无意义
 */
void *Thread_UdpClient_Receive(void *arg);

/**
 * @brief UdpClt_Send 关闭UDP
 *
 * UDP客户端向UDP服务器端发送数据，并返回发送状态；
 *
 * @param[in] pUdpClient UDP客户端结构体变量 @see StcUdpClient
 * @param[in] p_abuf[] 发送数据的buf
 * @param[in] p_nBufSize 发送数据的长度
 *
 * @return  发送数据的状态 >0 代表发送的数据字节数， <=0 代表发送失败
 */
nint32_t UdpClt_Send(StcUdpClient pUdpClient, nint8_t p_abuf[],
		nint32_t p_nBufSize);

/**
 * @brief UdpClt_ClsSoc 关闭UDP
 *
 * UDP客户端关闭SOCKET，停止接收数据，清空网络通讯相关变量
 *
 * @param[in] pUdpClient 	UDP客户端结构体变量
 *
 * @return  关闭状态，0成功 非0失败
 */
nint32_t UdpClt_ClsSoc(StcUdpClient pUdpClient);

#endif /* __UdpCLIENT_H_ */

/**
 * @}
 */
