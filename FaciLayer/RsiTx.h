/***************************************************************//**
 * @defgroup RsiTx RsiTx API
 * @ingroup FaciLayer
 * @{
 *
 *  This module provides APIs to do the operate about RsiMessage.
 *
 * @file RsiTx.h
 * @brief API file.
 *
 *
 *******************************************************************/

#ifndef RSITX_H_
#define RSITX_H_

//------------------------------------------------------------------------------
// Included headers
//------------------------------------------------------------------------------
#include "../MsgFrame/MessageFrame.h"
#include "../BaseInterface/V2XUtil/V2X_INTERFACE.h"
#include "../BaseInterface/ConfUtil/Config.h"

#include "ExtFunction.h"
#include "DataSet.h"
#include "../Algorithm/MercatorProj.h"
#include "../ApplyLayer/DataSet/DataSet.h"
#include "RsiConfig.h"
//------------------------------------------------------------------------------
// Function Prototypes
//------------------------------------------------------------------------------
/**
 * @brief 分配RSI消息对象，配置线程、关联RSI数据结构.
 * @param[in] ppRsi    指向RSI句柄的指针.
 * @param[in] pAttr    线程属性.
 * @return  0 执行成功， 非0 失败标识.
 */
extern int FG_RsiTxOpen(struct RsiTx **p_pprectRsi, pthread_attr_t *pAttr);

/**
 * @brief 停止执行RSITx，释放线程及相关资源.
 * @param[in] pRsi  RSI句柄.
 */
extern void FG_RsiTxclose(struct RsiTx *p_prectRsi);

/**
 * @brief 周期执行RSI发送线程.
 * @param[in] pArg  线程调用传入参数指针.
 */
static void FS_RsiTx_ThreadProc (void *p_pvArg);

/**
 * @brief 发送一条RSI消息.
 * @param[in] pRsi  RSI句柄.
 * @return  0 执行成功， 非0 失败标识.
 */
static int FS_RsiTxSendMsg(struct RsiTx *p_prectRsi, rectRsiPlatformToV2X_t *p_prectRsiPlatformToV2Xt);

/**
 * @brief 读取设备最后发送的那一条Rsi消息.
 * @param[out] pRsiMsg RSI句柄.
 * @return 0 读取失败，5 读取成功，本消息确实为Rsi信息. 其他 ，不合法
 */
static int FS_RsiTxReadlstTxMsg(MessageFrame_t *p_prectRsiMsg);

/**
 * @brief 创建一条RSI消息.
 * @param[in] pSendMsg  指向RSI发送句柄的指针.
 * @return  0 执行成功， 非0 失败标识.
 */
static int FS_RsiMsgCreate(MessageFrame_t **p_pprectSendMsg);

static int FS_RsiMsgCreateFromPlatform(MessageFrame_t **p_pprectSendMsg, rectRsiPlatformToV2X_t *p_prectRsiPlatformToV2Xt);

#endif /* RSITX_H_ */

/**
 * @}
 */
