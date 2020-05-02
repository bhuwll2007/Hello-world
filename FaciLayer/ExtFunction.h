/***************************************************************//**
 * @defgroup ExtFunction ExtFunction API
 * @ingroup FaciLayer
 * @{
 *
 *  This module provides APIs to do the operate about Message.
 *
 * @file ExtFunction.h
 * @brief API file.
 *
 *
 *******************************************************************/

#ifndef EXTFUNCTION_H_
#define EXTFUNCTION_H_

//------------------------------------------------------------------------------
// Included headers
//------------------------------------------------------------------------------
#include "../DataSet/DataSet.h"
#include "../BaseInterface/NetUtil/TcpClient.h"
#include "../BaseInterface/ConfUtil/Config.h"
#include "../MsgFrame/MessageFrame.h"

#include "BsmTx.h"
/**
 * @brief DSM消息UPER编码.
 * @param[in] pSendMsg  指向用于编码的MessageFrame_t指针.
 * @param[in] pBuf      编码后数据指针.
 * @param[in] Size      编码后数据长度.
 * @return  编码信息.
 */
asn_enc_rval_t Msg_UPEREncode_ToBuf(MessageFrame_t *pSendMsg, char *pBuf,
		size_t Size);

/**
 * @brief DSM消息UPER解码.
 * @param[in] pRecvMsg  指向解码后的MessageFrame_t指针.
 * @param[in] pBuf      用于解码的数据指针.
 * @param[in] Size      用于解码的数据长度.
 * @return  编码信息.
 */
asn_dec_rval_t Msg_UPERDecode(MessageFrame_t *pRecvMsg, char *pBuf,
		size_t Size);

/**
 * @brief 写入XMl文件
 * @param[in] pRecvMsg  指向解码后的MessageFrame_t指针.
 * @param[in] pBuf      用于解码的数据指针.
 * @param[in] Size      用于解码的数据长度.
 * @return  编码信息.
 */
void WriteXml(MessageFrame_t *pSendMsg, char *pConfigFileName);

/**
 * @brief 读取XML文件
 *
 * @param[in]  **pSendMsg V2X消息 @see  MessageFrame_t
 * @param[in] char *pConfigFileName
 *
 * @return  编码信息.
 */
void ReadXml(MessageFrame_t **pSendMsg, char *pConfigFileName);

/**
 * @brief 打印数据.
 * @param[in] pRecvMsg        要打印的数据指针.
 * @param[in] pnRecvBufSize   要打印的数据长度指针.
 */
void sFunRecvData(char *p_cRecvBuf, int *pnRecvBufSize);

/**
 * @brief Attempts to receive GNSS location info via handle into the fix.
 *        If no data available, the receive would block.
 * @param[in] handle  GNSS handle.
 * @param[in] fix     Point to the location info.
 * @return  The number of bytes received on success or -1 on error.
 */
void *Thread_GetTcpData(void * arg);

/**
 * @brief 进行TCP客户端连接，执行回调函数.
 * @param[in] pFunRecvData    指向回调函数指针.
 * @param[in] p_acIp          服务器IP.
 * @param[in] p_nPort         服务器端口号.
 * @param[in] lStcTcpClient   接口层TCP连接信息.
 * @return  <0 执行失败， >=0 执行成功.
 */
int ExtNet_CallbackRegister(
		void (*pFunRecvData)(char *p_cRecvBuf, uint32_t pnRecvBufSize),
		char p_acIp[], int p_nPort, StcTcpClient **lStcTcpClient);

/**
 * @brief 计算当前一分钟内的毫秒数.
 * @return  一分钟内的毫秒数值.
 */
long GetDSecond();

/**
 * @brief 计算当前时间在当年内的分钟数.
 * @return  当年内的分钟数值.
 */
long GetMinuteOFYear();

/**
 * @brief 设置系统定时休眠.
 * @param[in] delay  时间结构体.
 * @param[in] usTxInterval     休眠时间（us）.
 */
void msTimeDelay(struct timeval delay, int usTxInterval);

/**
 * @brief 读取配置文件信息，存入系统信息.
 * @param[in] pStore            状态参数数据.
 * @param[in] pConfigFileName   配置文件路径.
 */
void HostInfo_LoadConfig(tStateStore *pStore, char *pConfigFileName);

/**
 * @brief 读取XML配置文件信息，获取消息数据.
 * @param[in] pSendMsg          指向用于编码的MessageFrame_t指针的指针.
 * @param[in] pConfigFileName   配置文件路径.
 * @return  执行结果（MGS_ErrorCode）.
 */
void SimCANInfo_LoadCfg(CanData *pCanData, char *pConfigFileName);

int MessageFrame_Memcpy(MessageFrame_t *psrcMsg, MessageFrame_t *pdesMsg);

int MessageFrame_Memcpy1(MessageFrame_t *psrcMsg, char pBuf[], int *BufLen1);
#endif /* EXTFUNCTION_H_ */

/**
 * @}
 */
