/**
 * @defgroup SerialUtil SerialUtil API
 * @ingroup BaseInterface API
 * @{
 *
 * 本模块提供对SERIAL数据进行操作的API接口.
 *
 * @file Serial.h
 * @brief API file.
 *
 */

#ifndef __SERIAL_H
#define __SERIAL_H

#include "./../DataSet.h"
#include "./../ConfUtil/Config.h"

#define SERIAL_RECV_BUF_SIZE 512

/**
 * @brief Serial结构体
 */
typedef struct __Serial
{
	/**
	 * 串口名称
	 */
	nint8_t SerialName[30];
	/**
	 * 波特率
	 */
	nint32_t baudrate;
//	nint32_t flow_ctrl;
//	nint32_t databits;
//	nint32_t stopbits;
//	nint32_t parity;
	/**
	 * 串口句柄
	 */
	nint32_t fd;
	/**
	 * 串口接收数据缓存
	 */
	nint8_t m_SerialRecvbuf[SERIAL_RECV_BUF_SIZE];// = {0};
	/**
	 * 串口接收数据缓存大小
	 */
	uint32_t m_SerialRecvlen;
	/**
	 * @brief 回调函数数据处理函数 ----调用方需要按照格式实现
	 * @param p_cDealBuf 串口接收数据缓存
	 * @param pnDealBufSize 串口接收数据缓存大小
	 */
	void (*pCallBackDealData)(nint8_t *p_cDealBuf,uint32_t pnDealBufSize);
}StcSerial;

/**
 * @brief 根据串口相关参数，执行串口打开，并清空串口缓存的操作.
 *
 * @param[in] pStcSerial  串口操作相关结构体变量，包含串口参数配置及回调函数指针.@see StcSerial
 *
 * @return  >0--成功 代表操作的串口句柄，<0 --失败
 */
int Serial_Open(StcSerial *pStcSerial);

/**
 * @brief 开启串口数据接收
 *
 * 创建接收数据的线程，等待接收数据，并将接收到的数据区存放至处理区；
 *
 * @param[in] pStcSerial   串口操作变量，包含串口参数配置及回调函数指针，详见定义. @see StcSerial
 *
 * @return  0--成功，非0--失败
 */
int Serial_CreatThread(StcSerial *pStcSerial);

 /**
 * @brief 提供主动向对方发送数据的函数接口，并返回发送状态.
 *
 * @param[in] fd       串口句柄.
 * @param[in] pBuf     发送数据的buf.
 * @param[in] nBufLen  发送数据的长度.
 *
 * @return  发送数据的状态 >0 代表发送的数据字节数， <=0 代表发送失败..
 */
nint32_t Serial_Send(nint32_t fd, nint8_t *pBuf, nint32_t nBufLen);

 /**
 * @brief 关闭串口操作，
 *
 * 即关闭串口，停止串口数据接收线程，清空相关变量内容.
 *
 * @param[in] pStcSerial  串口操作相关结构体. @see StcSerial
 *
 * @return  0--成功，非0--失败.
 */
int Serial_Close(StcSerial *pStcSerial);

#endif  //_SERIAL_H

/**
 * @}
 */
