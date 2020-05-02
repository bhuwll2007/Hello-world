
/**
 * @ingroup NetLayer API
 * @{
 *
 * 本模块V2X网络层的API接口.
 *
 * @file FunCommun.h
 * @brief API file.
 *
 */
#ifndef NETLAYER_FUNCOMMUN_H_
#define NETLAYER_FUNCOMMUN_H_
#include "DSMP.h"

/**
 * @brief 设置系统定时休眠.
 * @param[in] delay  时间结构体.
 * @param[in] usTxInterval     休眠时间（us）.
 */
void msTimeDelay(struct timeval delay, int usTxInterval);
/**
 * @brief 网络层接收初始化.
 * @param[in] pWAVEConfig  时间结构体.
 * @param[in] usTxInterval     休眠时间（us）.
 */
int NetLayer_InitRx(tWAVEConfig pWAVEConfig, int RecvMode);

#endif /* NETLAYER_FUNCOMMUN_H_ */
