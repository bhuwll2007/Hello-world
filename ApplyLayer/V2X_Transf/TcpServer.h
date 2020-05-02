#ifndef __RSM_TCPSERVER_H_
#define __RSM_TCPSERVER_H_

#include "RsmDeal.h"
extern int g_nSendBufLen2PC;
extern char g_nSendBuf2PC[256];


/****************************************************
 * 函数名称: int InitTcpSerSoc(int pPort)
 * 功能描述: 创建并初始化TCP服务器参数
 * 输入参数: TCP服务器端口号
 * 函数作者: 魏林林
 * 创建日期: 2017-01-16 08:56:11
int TcpServer_recv(int sokfd, char *buf);

 ****************************************************/
int Rsm_TcpSer_Init(int pPort);


/****************************************************
 * 函数名称: int Tcp_send(int sokfd, char buf[], int nbuflen)
 * 输入参数: int sokfd TCP客户端端口, char buf[] 发送数据的buf, int nbuflen 发送数据的长度
 * 输出参数: //无
 * 返 回 值: //int 发送数据的状态
 * 函数作者: 魏林林
 * 创建日期: 2017-01-16 08:56:11
 ****************************************************/
int Tcp_send(int sokfd, uint8_t buf[], int nbuflen);

/****************************************************
 * 函数名称: void *Tcp_Listen(void * arg)
 * 功能描述: TCP服务器的监听建立连接的线程
 * 输入参数: void * arg 无实际意义
 * 输出参数: //无
 * 返 回 值: //无
 * 函数作者: 魏林林
 * 创建日期: 2017-01-16 09:01:44
 ****************************************************/
void *Tcp_Listen(void * arg);
int TcpServer_recv(int sokfd, char *buf);
/****************************************************
 * 函数名称: void *Tcp_Thread_function(void * arg)
 * 功能描述: 建立连接之后的数据接收线程
 * 输入参数: void * arg 建立连接的socket
 * 输出参数: //无
 * 返 回 值: //无
 * 函数作者: 魏林林
 * 创建日期: 2017-01-16 09:42:20
 ****************************************************/
void *Tcp_Thread_function(void * arg) ;

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
uint32_t Pkt_Decode(uint8_t *src_buf, uint8_t *target_buf, uint32_t len);
#endif
