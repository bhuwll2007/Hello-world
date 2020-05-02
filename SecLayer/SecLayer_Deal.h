
/**
 * @ingroup SecLayeer API
 * @{
 *
 * 本模块提供提供安全层的API接口-安全数据处理接口.
 *
 * @file SecLayer_Deal.h
 * @brief API file.
 *
 */

#ifndef SECLAYER_SECLAYER_DEAL_H_
#define SECLAYER_SECLAYER_DEAL_H_

#include "Encode_SecMsg.h"
#include "Verify_SecMsg.h"
#include "../NetLayer/FunCommun.h"
/**
 * @brief 安全层接收线程.
 *
 * @param[in] pArg  线程调用传入参数指针.
 */
void SecLayer_Recv_ThreadProc(void *pArg);

int SecLayer_InitRx(tWAVEConfig pWAVEConfig, int RecvMode);
int SecLayer_Init(int RxMode, uint16_t Aid[], int nAidCnt);
/**
 * @brief 发送一条安全消息.
 * @param[in] dsmSend_rq  tDSM_Request句柄.
 * @return  0 执行成功， 非0 失败标识.
 */
int SecLayer_Send(tDSM_Request dsmSend_rq);

int SecHeader_Encode();
void SecHeader_Decode();

// Recv V2X Data Callback function
int GetLstSecRecvData(int *TxAid, SecuredMessage_t *pSecMsg);
/**
 * @brief Recv V2X Data Callback function.
 * @param[in] nAid 安全层Aid.
 * @param[in] buf  安全层接收数据.
 * @param[in] buflen 安全层接收数据长度.
 * @return  0 执行成功， 非0 失败标识.
 */
int GetLstSecRecvBufData(int *nAid, char buf[], int *buflen);
void SecHeader_free();

#endif /* SECLAYER_SECLAYER_DEAL_H_ */
