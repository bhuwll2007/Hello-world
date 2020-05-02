/***************************************************************//**
 * @defgroup FacMsgDup FacMsgDup API
 * @ingroup FaciLayer
 * @{
 *
 *  This module provides APIs to do the operate about Message.
 *
 * @file FacMsgDup.h
 * @brief API file.
 *
 *
 *******************************************************************/
#ifndef _FACMSGDUP_H_
#define _FACMSGDUP_H_
//------------------------------------------------------------------------------
// Included headers
//------------------------------------------------------------------------------

#include "../MsgFrame/MessageFrame.h"
#include "../BaseInterface/V2XUtil/V2X_INTERFACE.h"
#include "../SecLayer/SecLayer_Deal.h"
#include "DataSet.h"
#include "ExtFunction.h"

//------------------------------------------------------------------------------
// Function Prototypes
//------------------------------------------------------------------------------
/**
 * @brief 用于接收数据线程内执行的消息处理回调函数.
 * @param[in] p_cRecvBuf  接收到的DSM数据缓存.
 * @param[in] pnRecvBufSize     接收到的DSM数据长度.
 * @return  0 执行成功， 非0 失败标识.
 */
void AppInfo_Process(int nAid, char *p_cRecvBuf, int *pnRecvBufSize);

int FaciLayer_Recv();
/**
 * @brief BSM消息请求.
 * @return  0 接受，1 拒绝.
 */
int AppDupBSM_rq();

/**
 * @brief 应答一条BSM消息内容.
 * @param[in] Res        BSM消息请求时返回的结果.
 * @param[in] pBsmMsg    指向接收到BSM消息地址的指针.
 * @return  执行结果（MsgDup_ErrorCode）.
 */
int AppDupBSM_rs(int Res, MessageFrame_t *pBsmMsg);

/**
 * @brief 执行BSM消息通知，开启线程循环获取BSM消息内容.
 * @return  0 执行成功， 非0 失败标识.
 */
int AppDupBSM_id(/*int **MsgCnt, MessageFrame_t **pRecvMsg*/);

/**
 * @brief 获取BSM接收缓存中最新的一条数据.
 *
 * 该函数在执行了GetRcvBsmCount()之后，返回结果>0时，用户选择执行，执行了此函数后，BSM接收缓存清空
 *
 * @param[out] BsmData  BSM数据.
 * @return  0 执行成功， 非0 失败标识.
 */
int GetLstBsmData(int *TxAid, MessageFrame_t *BsmData);

/**
 * @brief RSM消息请求.
 * @return  0 接受，1 拒绝.
 */
int AppDupRSM_rq();

/**
 * @brief 应答一条RSM消息内容.
 * @param[in] Res         RSM消息请求时返回的结果.
 * @param[in] pRecvMsg    指向接收到BSM消息的指针.
 * @return  执行结果（MsgDup_ErrorCode）.
 */
int AppDupRSM_rs(int Res, MessageFrame_t *pRecvMsg);

/**
 * @brief 执行RSM消息通知，开启线程循环获取RSM消息内容.
 * @return  0 执行成功， 非0 失败标识.
 */
int AppDupRSM_id(/*int **MsgCnt, MessageFrame_t **pRsmMsg*/);

/**
 * @brief 获取RSM接收缓存中最新的一条数据.
 *
 * 该函数在执行了GetRcvRsmCount()之后，返回结果>0时，用户选择执行，执行了此函数后，RSM接收缓存清空
 *
 * @param[out] RsmData  RSM数据.
 * @return  0 执行成功， 非0 失败标识.
 */
int GetLstRsmData(int *TxAid, MessageFrame_t *RsmData);

/**
 * @brief RSI消息请求.
 * @return  0 接受，1 拒绝.
 */
int AppDupRSI_rq();

/**
 * @brief 应答一条RSI消息内容.
 * @param[in] Res         RSI消息请求时返回的结果.
 * @param[in] pRecvMsg    指向接收到RSI消息的指针.
 * @return  0 执行成功， 非0 失败标识.
 */
int AppDupRSI_rs(int Res, MessageFrame_t *pRecvMsg);

/**
 * @brief 执行RSI消息通知，开启线程循环获取RSI消息内容.
 * @return  0 执行成功， 非0 失败标识.
 */
int AppDupRSI_id(/*int **MsgCnt, MessageFrame_t **pRsiMsg*/);

/**
 * @brief 获取RSM接收缓存中最新的一条数据.
 *
 * 该函数在执行了GetRcvRsiCount()之后，返回结果>0时，用户选择执行，执行了此函数后，RSI接收缓存清空
 *
 * @param[out] RsiData  RSI数据.
 * @return  0 执行成功， 非0 失败标识.
 */
int GetLstRsiData(int *TxAid, MessageFrame_t *RsiData);

/**
 * @brief MAP消息请求.
 * @return  0 接受，1 拒绝..
 */
int AppDupMAP_rq();

/**
 * @brief 应答一条MAP消息内容.
 * @param[in] Res        MAP消息请求时返回的结果.
 * @param[in] pRecvMsg   指向接收到RSI消息的指针.
 * @return  执行结果（MsgDup_ErrorCode）.
 */
int AppDupMAP_rs(int Res, MessageFrame_t *pRecvMsg);

/**
 * @brief 执行MAP消息通知，开启线程循环获取MAP消息内容.
 * @return  0 执行成功， 非0 失败标识.
 */
int AppDupMAP_id(/*int **MsgCnt, MessageFrame_t **pMapMsg*/);

/**
 * @brief 获取MAP接收缓存中最新的一条数据.
 *
 * 该函数在执行了GetRcvMapCount()之后，返回结果>0时，用户选择执行，执行了此函数后，MAP接收缓存清空
 *
 * @param[out] MapData  MAP数据.
 * @return  0 执行成功， 非0 失败标识.
 */
int GetLstMapData(int *TxAid, MessageFrame_t *MapData);

/**
 * @brief SPAT消息请求.
 * @return  0 接受，1 拒绝.
 */
int AppDupSPAT_rq();

/**
 * @brief 应答一条SPAT消息内容.
 * @param[in] Res         SPAT消息请求时返回的结果.
 * @param[in] pRecvMsg    指向接收到MAP消息的指针.
 * @return  执行结果（MsgDup_ErrorCode）.
 */
int AppDupSPAT_rs(int Res, MessageFrame_t *pRecvMsg);

/**
 * @brief 执行SPAT消息通知，开启线程循环获取SPAT消息内容.
 * @return  0 执行成功， 非0 失败标识.
 */
int AppDupSPAT_id(/*int **MsgCnt, MessageFrame_t **pSpatMsg*/);


/**
 * @brief 获取RSM接收缓存中最新的一条数据.
 *
 * 该函数在执行了GetRcvSpatCount()之后，返回结果>0时，用户选择执行，执行了此函数后，SPAT接收缓存清空
 *
 * @param[out] SpatData  SPAT数据.
 * @return  0 执行成功， 非0 失败标识.
 */
int GetLstSpatData(int *TxAid, MessageFrame_t *SpatData);

#endif

/**
 * @}
 */
