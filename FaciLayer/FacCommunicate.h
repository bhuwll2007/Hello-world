/***************************************************************//**
 * @defgroup FaciLayer FaciLayer API
 * @{
 *
 *  This module is the FaciLayer API.
 *
 * @}
 *
 *******************************************************************/

/***************************************************************//**
 * @defgroup FacCommunicate FacCommunicate API
 * @ingroup FaciLayer
 * @{
 *
 *  This module provides APIs to do the operate about Message Communicate.
 *
 * @file FacCommunicate.h
 * @brief API file.
 *
 *
 *******************************************************************/
#ifndef __FACCOMMUNICATE_H__
#define __FACCOMMUNICATE_H__

//------------------------------------------------------------------------------
// Included headers
//------------------------------------------------------------------------------
#include "../BaseInterface/V2XUtil/V2X_INTERFACE.h"

#include "ExtFunction.h"
#include "DataSet.h"
#include "../SecLayer/SecLayer_Deal.h"

//------------------------------------------------------------------------------
// Function Prototypes
//------------------------------------------------------------------------------

/**
 * @brief DSM消息初始化请求.，获取相关设置参数
 * @return  0 接收请求成功，非0拒绝请求
 */
int AppDSMInit_rq();

/**
 * @brief DSM消息初始化请求的应答.
 * @param[in] Answer  DSM消息初始化请求时返回的结果.
 * @return  执行结果（Init_ErrorCode）.
 */
int AppDSMInit_rs();

/**
 * @brief 消息发送请求，对指定消息类型进行封装
 * @return 执行状态，0 执行成功，非0 执行失败
 */
int AppSendDSMMsg_rq();

/**
 * @brief 开启不同DSM消息发送接口.
 * @param[in] Answer  DSM消息发送请求时返回的结果.
 * @param[in] pStore  状态参数数据.
 * @return  执行结果（SEND_ErrorCode）.
 */
int AppSendDSMMsg_rs(int Answer, tStateStore *pStore);

/**
 * @brief DSM消息终止请求.
 *
 * 请求终止通信
 *
 * @return  0 接受，1 拒绝.
 */
int AppDSMTerminate_rq();

/**
 * @brief DSM执行消息终止.
 * @param[in] Answer  DSM消息终止请求时返回的结果.
 * @return  执行结果（Init_ErrorCode）.
 */
int AppDSMTerminate_rs(int Res);
#endif

/**
 * @}
 */
