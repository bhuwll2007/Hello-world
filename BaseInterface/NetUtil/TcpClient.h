/**
 * @defgroup NETUtil NETUtil API
 * @ingroup BaseInterface API
 * @{
 *
 * 本模块提供对TCP客户端通讯进行操作的API接口.
 *
 * @file TcpClient.h
 * @brief API file.
 *
 */
#ifndef __TCPCLIENT_H_
#define __TCPCLIENT_H_
#include "./../DataSet.h"
#include "./../BaseFunct/BaseFunct.h"

#define TCP_CLT_RECV_BUF_SIZE 512

/**
 * @brief TcpClient相关操作结构体.
 */
typedef struct __TcpClient
{
	/**
	 * TCP客户端句柄
	 */
	uint32_t m_TcpClnSok;
	/**
	 * TCP客户端与服务器连接状态
	 */
	uint32_t is_Connect;
	/**
	 * TCP客户端接收数据缓存
	 */
	nint8_t m_TcpCltRecvbuf[TCP_CLT_RECV_BUF_SIZE];
	/**
	 * TCP客户端接收数据缓存长度
	 */
	uint32_t m_TcpCltRecvlen;
	/**
	 * @brief 网络接收数据线程 ----调用方不要使用
	 * @param arg 无意义
	 */
	void *(*pThread_TcpCln_Receive)(void *arg);

	/**
	 * @brief 网络接收数据后处理线程 ----调用方不要使用
	 * @param arg 无意义
	 */
	void *(*pThread_TcpCln_DealData)(void *arg);

	/**
	 * @brief 回调函数数据处理函数 --	调用方需要按照格式实现
	 *
	 * @param p_cDealBuf 		-- 	接收数据缓存
	 * @param pnDealBufSize 	-- 	接收数据缓存长度
	 */
	void (*pCallBackDealData)(nint8_t *p_cDealBuf,uint32_t pnDealBufSize);
}StcTcpClient;

/**
 * @brief TcpClt_InitSoc 创建并初始化TCP客户端Socket.
 *
 * @param[in] p_StcTcpClient  TcpClient相关操作结构体. @see StcTcpClient
 *
 * @return  TCP服务器创建状态 >0代表创建成功，值为创建的socket句柄 非0失败.
 */
int TcpClt_InitSoc(StcTcpClient *p_StcTcpClient);

/**
 * @brief Thread_TcpCln_Receive TCP客户端的接收线程
 *
 * @param[in] arg TCP客户端结构体变量，包含相关变量及函数指针，详见定义
 *
 * @return  无意义
 */
void *Thread_TcpCln_Receive(void *arg);

/**
 * @brief TCP客户端连接服务器，成功后创建数据接收线程，等待接收数据，并将接收到的数据放到处理区.
 *
 * @param[in] p_StcTcpClient  TcpClient相关操作结构体. @see StcTcpClient
 * @param[in] p_acIp          服务器IP地址.
 * @param[in] p_nPort         服务器端口号.
 *
 * @return  客户端连接服务器状态，0成功，非0失败.
 */
int TcpClt_LinkSer(StcTcpClient *p_StcTcpClient, nint8_t p_acIp[], nint32_t p_nPort);

/**
 * @brief TCP客户端向服务器端发送数据，并返回发送状态.
 *
 * @param[in] p_StcTcpClient  TcpClient相关操作结构体. @see StcTcpClient
 * @param[in] p_abuf          发送数据的buf.
 * @param[in] p_nBufSize      发送数据的长度.
 *
 * @return  发送数据的状态 >0代表发送的数据字节数， <=0代表发送失败.
 */
int TcpClt_Send(StcTcpClient *p_StcTcpClient, char p_abuf[], int p_nBufSize);

/**
 * @brief TCP客户端关闭SOCKET，停止接收数据，清空网络通讯相关变量.
 *
 * @param[in] p_StcTcpClient  TcpClient相关操作结构体. @see StcTcpClient
 *
 * @return  关闭状态，0成功 非0失败.
 */
int TcpClt_ClsSoc(StcTcpClient *p_StcTcpClient);

#endif /* __TCPCLIENT_H_ */

/**
 * @}
 */
