/***************************************************************//**
 * @defgroup BsmTx BsmTx API
 * @ingroup FaciLayer
 * @{
 *
 * This module provides APIs to do the operate about BsmMessage.
 *
 * @file BsmTx.h
 * @brief API file.
 *
 *******************************************************************/

#ifndef BSMTX_H_
#define BSMTX_H_

#include "../MsgFrame/MessageFrame.h"
#include "../BaseInterface/V2XUtil/V2X_INTERFACE.h"
#include "../BaseInterface/GPSUtil/GPS.h"
#include "../BaseInterface/CANUtil/CAN.h"

#include "ExtFunction.h"
#include "DataSet.h"
#include "../NetLayer/DSMP.h"

/**
 * @brief 创建一条BSM消息.
 *
 * @param[in] pSendMsg  创建一条BSM消息.
 *
 * @return  0 执行成功， 非0 执行失败.
 */
int BSM_Msg_Create(MessageFrame_t *pSendMsg);

/**
 * @brief 分配BSM消息对象，配置线程、关联BSM数据结构.
 *
 * @param[in] ppBsm  指向BSM句柄的指针.
 * @param[in] pAttr  线程属性.
 *
 * @return  0 执行成功， 非0 执行失败.
 */
int BsmTx_Open(struct BsmTx **ppBsm, pthread_attr_t *pAttr);

/**
 * @brief 停止指向BSMTx，释放线程及相关资源.
 *
 * @param[in] pBsm  BSM句柄.
 */
void BsmTx_close(struct BsmTx *pBsm);

/**
 * @brief 周期执行BSM发送线程.
 *
 * @param[in] pArg  线程调用传入参数指针.
 */
void BsmTx_ThreadProc (void *pArg);

/**
 * @brief 发送一条BSM消息.
 * @param[in] pBsm  BSM句柄.
 * @return  0 执行成功， 非0 执行失败.
 */
int BsmTx_SendMsg(struct BsmTx *pBsm);

/**
 * @brief 读取设备最后发送的那一条Bsm消息.
 * @param[out] pBsmMsg BSM句柄.
 * @return 0 读取失败，1 读取成功，本消息确实为Bsm信息. 其他 ，不合法
 */
int BsmTx_ReadlstTxMsg(MessageFrame_t *pBsmMsg);

#endif /*** BSMTX_H_ */

/**
 * @}
 */
