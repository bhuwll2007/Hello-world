/**
 * @defgroup LTEV-V2XUtil LTEV-V2XUtil API
 * @ingroup V2X_INTERFACE API
 * @{
 *
 * 本模块提供对V2X设备-LTEV设备 进行操作的API接口.
 *
 * @file LTEV_V2X.h
 * @brief API file.
 *
 */
#ifndef __LTE_V2X_H_
#define __LTE_V2X_H_
#include "LTEVUtil/LTE_V2X_API.h"
#include "LTEVUtil/LTE_V2X_IP.h"
#include "LTEVUtil/LTEV_Include.h"

/**
 * @brief LTEV通讯初始化
 *
 * @return  执行状态，0 执行成功，非0 执行失败.
 */
int LTE_V2X_CommunInit();

/**
 * @brief 初始化LTEV发送数据的互斥信号量
 *
 * @return  无.
 */
void ParamInit_ltev_send();

/**
 * @brief LTEV通讯参数读取
 *
 * 按照用户定义的信道配置参数读取配置数据库中的信道配置参数，
 *
 * @param[out] pLTE_Option LTEV通讯参数
 *
 * @return  执行状态，0 执行成功，非0 执行失败.
 */
int LTE_V2X_GetParam(LTEV2X_Option *pLTE_Option);

/**
 * @brief LTEV通讯参数配置
 *
 * 按照用户定义的信道配置参数修改配置数据库中的信道配置参数，并重新读取配置参数至三个全局变量；
 *
 * @param[in] pLTE_Option 用户新定义的LTEV通讯参数。
 *
 * @return  执行状态，0 执行成功，非0 执行失败.
 */
int LTE_V2X_SetParam(LTEV2X_Option *pLTE_Option);

/**
 * @brief LTEV通讯参数初始化
 *
 * @param 无
 *
 * @return  执行状态，0 执行成功，非0 执行失败.
 */
int LTE_V2X_ParamInit();

/**
 * @brief LTEV通讯 发送指定数据
 *
 * 将指定的数据按照当前的发送参数发送出去
 *
 * @param[in] p_pSendData 发送数据指针
 * @param[in] nDataLen 发送数据长度
 *
 * @return  执行状态，>0 执行成功，(成功发送数据的长度（字节数）) <0 执行失败.
 */
int LTE_V2X_Tx_SendData(char* p_pSendData, int nDataLen);

/**
 * @brief 开启接收数据
 *
 * 将当前接收参数配置到设备中，进入接收数据状态，如果接收到数据，则进行数据处理.
 *
 * @param[in] pFunRecvData  接收数据的回调函数.
 *
 * @return  执行状态，0 执行成功，非0 执行失败.
 */
int LTE_V2X_Rx_SetDevParam(
		void (*pFunRecvData)(char *p_cRecvBuf, int *pnRecvBufSize));

#endif //#ifndef __LTE_V2X_H_

/**
 * @}
 */

