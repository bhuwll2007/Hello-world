/**
 * @defgroup NETUtil NETUtil API
 * @ingroup BaseInterface API
 * @{
 *
 * 本模块提供对TCP服务器通讯进行操作的API接口.
 *
 * @file TcpServer.h
 * @brief API file.
 *
 */
#ifndef __TCPSERVER_H_
#define __TCPSERVER_H_
#include "./../DataSet.h"
#include "../BaseFunct/BaseFunct.h"

/**
 * TCP服务区接收的最大缓存大小
 */
#define TCP_SVR_RECV_BUF_SIZE 512

/**
 * TCP服务器支持的可被连接的最大数 3
 */
#define TCP_SVR_MAXCLTNUM 3

/**
 * @brief TcpCln相关操作结构体.
 */
typedef struct __TcpCln
{
    /**
     * TCP客户端句柄
     */
    nint32_t m_TcpClnSok;
    /**
     * TCP客户端接收数据缓存
     */
    nint8_t m_TcpSvrRecvbuf[TCP_SVR_RECV_BUF_SIZE];
    /**
     * TCP客户端接收数据缓存长度
     */
    uint32_t m_TcpSvrRecvlen;

    /**
     * @brief 网络接收数据线程 ----调用方不要使用
     * @param arg 无意义
     */
    void *(*pThread_TcpSev_Receive)(void *arg);

    /**
     * @brief 网络接收数据后处理线程 ----调用方不要使用
     * @param arg 无意义
     */
    void *(*pThread_TcpSCln_DealData)(void *arg);

    /**
     * @brief 回调函数数据处理函数 --	调用方需要按照格式实现
     *
     * @param fd 				--	接收方网络通讯句柄
     * @param p_cDealBuf 		-- 	接收数据缓存
     * @param pnDealBufSize 	-- 	接收数据缓存长度
     */
    void (*pCallBackDealData)(uint32_t fd, nint8_t *p_cDealBuf,
                              uint32_t pnDealBufSize);
} StcTcpCln;

/**
 * @brief TcpServer相关操作结构体.
 */
typedef struct __TcpServer
{
    /**
     * TCP服务器端端口号
     */
    nint32_t m_TcpSerPort;
    /**
     * TCP服务器端操作句柄
     */
    nint32_t m_TcpSerSok;

    /**
     * TCP服务器目前控制的TCP客户端数组信息
     */
    StcTcpCln m_StcTcpCln[TCP_SVR_MAXCLTNUM];	/// @see StcTcpCln

    /**
     * @brief 网络接收数据线程 ----调用方不要使用
     * @param arg 无意义
     */
    void *(*pThread_TcpSev_Listen)(void *arg);

    /**
     * @brief 回调函数数据处理函数 ----调用方需要按照格式实现
     *
     * @param pstcTcpCln TCP客户端句柄
     *
     * @param nIndex 序号
     */
    void (*pCallBack_Listen)(StcTcpCln *pstcTcpCln, int nIndex);
} StcTcpServer;

/**
 * @brief TcpSer_InitSoc TCP 服务器端初始化，
 *
 * 创建并初始化TCP服务器端口，绑定端口号，开启监听连接线程，监听连接线程中一旦有客户端连接，创建数据接收线程，等待接收数据，并将接收到的数据放到处理区.
 *
 * @param[in] pStcTcpServer  TcpServer相关操作结构体.包含TCP服务器参数配置及回调函数指针等，详见定义 @see StcTcpServer
 *
 * @return  TCP服务器创建状态 >0代表创建成功，值为创建的socket句柄 非0失败.
 */
nint32_t TcpSer_InitSoc(StcTcpServer *pStcTcpServer);

/**
 * @brief Thread_TcpSer_Listen TCP服务器的监听建立连接的线程
 *
 * @param[in] void * arg TCP服务器结构体变量指针
 *
 * @return  void * 无实际意义
 */
void *Thread_TcpSer_Listen(void * arg);

/**
 * @brief 建立连接之后的数据接收线程.
 *
 * @param[in] arg  建立连接的socket.
 */
void *Thread_Client_Deal(void * arg);

/**
 * @brief TCP服务器端向客户端发送数据，并返回发送状态.
 *
 * @param[in] sokfd     TCP客户端句柄.
 * @param[in] buf       存储发送数据的buf.
 * @param[in] nbuflen   发送数据的长度.
 *
 * @return  TCP发送结果 >0代表发送成功，<0代表发送失败.
 */
nint32_t TcpSer_Send(uint32_t sockfd, nint8_t buf[], uint32_t nbuflen);

/**
 * @brief TCP服务器端向所有客户端发送数据，并返回发送状态.
 *
 * @param[in] pStcTcpServer  TcpServer相关操作结构体.@see StcTcpServer
 * @param[in] buf            存储发送数据的buf.
 * @param[in] nbuflen        发送数据的长度.
 *
 * @return  0 发送结束.
 */
nint32_t TcpSer_SendAll(StcTcpServer *pStcTcpServer, nint8_t buf[],
                        uint32_t nbuflen);

/**
 * @brief 关闭Tcp服务器Socket，停止接收数据线程和监听线程，清空相关变量.
 *
 * @param[in] pStcTcpServer  TcpServer相关操作结构体.@see StcTcpServer
 *
 * @return  0关闭结束.
 */
nint32_t TcpSer_ClsSoc(StcTcpServer *pStcTcpServer);

#endif

/**
 * @}
 */

