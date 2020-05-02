/***************************************************************//**
 * @defgroup MapTx MapTx API
 * @ingroup FaciLayer
 * @{
 *
 * This module provides APIs to do the operate about MapMessage.
 *
 * @file MapTx.h
 * @brief API file.
 *
 *
 *******************************************************************/

#ifndef MAPTX_H_
#define MAPTX_H_

//------------------------------------------------------------------------------
// Included headers
//------------------------------------------------------------------------------
#include "../MsgFrame/MessageFrame.h"
#include "../BaseInterface/V2XUtil/V2X_INTERFACE.h"
#include "../BaseInterface/ConfUtil/Config.h"

#include "ExtFunction.h"
#include "DataSet.h"
//------------------------------------------------------------------------------
// Macros & Constants
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Function Prototypes
//------------------------------------------------------------------------------


/**
 * @brief 分配Map消息对象，配置线程、关联MAP数据结构.
 * @param[in] ppMap  指向Map句柄的指针.
 * @param[in] pAttr  线程属性.
 * @return 0 执行成功， 非0 失败标识.
 */
int MapTx_Open(struct MapTx **ppMap, pthread_attr_t *pAttr);


/**
 * @brief 停止执行MapTx，释放线程及相关资源.
 * @param[in] pMap Map句柄.
 */
void MapTx_close(struct MapTx *pMap);


/**
 * @brief 周期执行Map发送线程.
 * @param[in] pArg 线程调用传入参数指针.
 */
void MapTx_ThreadProc (void *pArg);


/**
 * @brief 发送一条Map消息.
 * @param[in] pMap MAP句柄.
 * @return 0 执行成功， 非0 失败标识.
 */
int MapTx_SendMsg(struct MapTx *pMap);

/**
 * @brief 读取设备最后发送的那一条Map消息.
 * @param[out] pMapMsg MAP句柄.
 * @return 0 读取失败，2 读取成功，本消息确实为MAP信息. 其他 ，不合法
 */
int MapTx_ReadlstTxMsg(MessageFrame_t *pMapMsg);
/**
 * @brief 创建一条Map消息.
 * @param[in] pSendMsg 指向MaP发送句柄的指针.
 * @return 0 执行成功， 非0 失败标识.
 */
int Map_Msg_Create(MessageFrame_t **pSendMsg);



#endif /* MAPTX_H_ */

/**
 * @}
 */
