/**
 * @defgroup NETUtil NETUtil API
 * @ingroup BaseInterface API
 * @{
 *
 * 本模块提供对UDP服务器通讯进行操作的API接口.
 *
 * @file UdpService.h
 * @brief API file.
 *
 */
#ifndef __UDPSERVICE_H_
#define __UDPSERVICE_H_
#include "./../DataSet.h"

/**
 * UDP服务器端接收数据缓存大小
 */
#define UDPSRV_RECVBUF_SIZE 512
/**
 * @brief UdpService相关操作结构体.
 */
typedef struct __UdpService
{
	/**
	 * UDP客户端操作句柄
	 */
	uint32_t m_UdpSrvSok;
	/**
	 * UDP客户端端口号
	 */
	uint16_t m_selfPort;
	/**
	 * UDP客户端接收数据缓存
	 */
	nint8_t m_UdpSrvRecvbuf[UDPSRV_RECVBUF_SIZE];
	/**
	 * UDP客户端接收数据的缓存的大小
	 */
	uint32_t m_UdpSrvRecvlen;
	/**
	 * UDP客户端发送的对方socket信息
	 */
	struct sockaddr_in m_UdpSer_addr;
	/**
	 * @brief 网络接收数据线程 ----调用方不要使用
	 * @param arg 无意义
	 */
	void *(*pThread_UdpSrv_Receive)(void *arg);
	/**
	 * @brief 网络接收数据后处理线程 ----调用方不要使用
	 * @param arg 无意义
	 */
	void *(*pThread_UdpSrv_DealData)(void *arg);

	/**
	 * @brief 回调函数数据处理函数 ----调用方需要按照格式实现
	 * @param p_cDealBuf 接收数据缓存
	 * @param pnDealBufSize 接收数据缓存大小
	 */
	void (*pCallBackDealData)(nint8_t *p_cDealBuf,uint32_t pnDealBufSize);
}StcUdpService;

/**
 * @brief UdpSrv_InitSoc Udp服务器创建socket
 *
 * Udp服务器创建socket
 *
 * @param[in] pUdpClient UDP服务器结构体变量，包含相关变量及函数指针，详见定义， @see StcUdpService
 *
 * @return  Udp服务器创建状态 >0代表创建成功，值为创建的socket句柄 非0 失败
 */
nint32_t UdpSrv_InitSoc(StcUdpService *pUdpService);

/**
 * @brief UdpSrv_LinkSer Udp服务器开启接收数据线程
 *
 * 创建并初始化UDP服务器端Addr，开启数据接收线程，等待接收数据，并将接收到的数据放到处理区
 *
 * @param[in] pUdpService UDP服务器结构体变量，包含相关变量及函数指针，详见定义， @see StcUdpService
 *
 * @return 线程创建成功状态
 */
nint32_t UdpSrv_LinkSer(StcUdpService *pUdpService);


/**
 * @brief Thread_UdpClient_Receive 关闭UDP
 *
 * UDP服务器的接收线程，接收数据并通过回调函数传出。
 *
 * @param[in] arg 无意义
 *
 * @return  无意义
 */
void *Thread_UdpService_Receive(void *arg);

/**
 * @brief UdpSrv_Send UDP服务器端发送数据
 *
 * UDP服务器端向UDP客户端发送数据，并返回发送状态；
 *
 * @param[in] pUdpService UDP服务器结构体变量 @see StcUdpService
 * @param[in] p_abuf[] 发送数据的buf
 * @param[in] p_nBufSize 发送数据的长度
 *
 * @return  发送数据的状态 >0 代表发送的数据字节数， <=0 代表发送失败
 */
nint32_t UdpSrv_Send(StcUdpService pUdpService, nint8_t p_abuf[], nint32_t p_nBufSize);

/**
 * @brief UdpSrv_Send UDP服务器端关闭
 *
 * UDP服务器关闭SOCKET，停止接收数据，清空网络通讯相关变量
 *
 * @param[in] pUdpService UDP服务器结构体变量 @see StcUdpService
 *
 * @return  关闭状态，0成功 非0失败
 */
nint32_t UdpSrv_ClsSoc(StcUdpService pUdpService);

#endif /* __UDPSERVICE_H_ */

/**
 * @}
 */
