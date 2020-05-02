/**
 * @defgroup V2X_INTERFACE V2X_INTERFACE API
 * @ingroup BaseInterface API
 * @{
 *
 * 本模块提供对GPS数据进行操作的API接口.
 *
 * @file V2X_INTERFACE.h
 * @brief API file.
 *
 */

#ifndef V2X_INTERFACE_H_
#define V2X_INTERFACE_H_
#include "./../DataSet.h"
#include "LTE_V2X.h"

#if MODE_LTEV_API
#include "LTEVUtil/LTE_V2X_API.h"
#endif
#if MODE_LTEV_IP
#include "LTEVUtil/LTE_V2X_IP.h"
#endif
#if MODE_LTEV_RAW
#include "LTEVUtil/LTE_V2X_RAW.h"
#endif

/**
 * @brief 错误类型
 */
typedef enum
{
    Process_ERR_NONE = 0,
    Process_ERR_REJECT = 1,
    Process_ERR_GetChnCfg = 2,
    Process_ERR_GetTxCfg = 3,
    Process_ERR_GetRxCfg = 4,
    Process_ERR_SetChnCfg = 5,
    Process_ERR_SetTxCfg = 6,
    Process_ERR_SetRxCfg = 7,
} HostProcess_ErrorCode;

/**
 * @brief DSRC类型设备参数结构体
 */
typedef struct DevParam_DSRC
{
//    tTxOpts m_TxOpts;
//    tRxOpts m_RxOpts;
//    tCHOpts m_CHOpts;
} DevParam_DSRC_t;

/**
 * @brief LTEV类型设备参数结构体
 */
typedef struct DevParam_LTEV
{
    LTE_V2X_Options m_V2XOptions;
} DevParam_LTEV_t;
/**
 * @brief DevParam_PR 设备参数类型选择
 */
typedef enum DevParam_PR
{
    DevParam_PR_NOTHING, 	///无意义
    DevParam_PR_DSRC, 		///DSRC设备参数
    DevParam_PR_LTEV,		///LTEV设备参数
} DevParam_PR;

/**
 * @brief DevParam 设备参数结构体.
 */
typedef struct _DevParam
{
    /**
     * 设备参数选择
     */
    DevParam_PR present;
    union DevParam_u
    {
        /**
         * DSRC设备参数
         */
        DevParam_DSRC_t DevParam_DSRC;
        /**
         * LTEV设备参数
         */
        DevParam_LTEV_t DevParam_LTEV;
    } choice;
} DevParam_t;
/**
 * 接收数据处理接口
 */
typedef void (*pFunRecvData)(char *,int *);

/**
* @brief 初始化V2X通讯
*
* 初始化V2X通讯
*
* @param 无
*
* @return  执行状态，0 执行成功，非0 执行失败.
*/
int V2X_Tx_Init();

/**
* @brief 初始化V2X通讯_接收模块
*
* 初始化V2X通讯_接收模块
*
* @param[in] MsgRx_CallBack 接收数据回调函数 @see pFunRecvData
*
* @return  执行状态，0 执行成功，非0 执行失败.
*/
int V2X_Rx_Init(pFunRecvData MsgRx_CallBack);

/**
* @brief 设备参数读取
*
* 用户获取设备当前运行参数，并通过函数输出参数传出；
*
* @param[out] pDevParam 设备当前运行参数。
*
* @return  执行状态，0 执行成功，非0 执行失败.
*/
int V2X_GetParam(DevParam_t *pDevParam);

/**
* @brief 设备参数配置
*
* 按照用户定义的设备参数修改配置数据库中的配置参数，并重新配置参数；
*
* @param[in] pDevParam 用户想配置的新参数。
*
* @return  执行状态，0 执行成功，非0 执行失败.
*/
int V2X_SetParam(DevParam_t *pDevParam);

/**
* @brief 发送指定数据
*
* 将指定的数据按照当前的发送参数发送出去
*
* @param[in] char* p_pSendData 发送数据指针，
* @param[in] int nDataLen 发送数据长度
*
* @return >0 成功发送数据的长度（字节数） <0 表示发送失败，返回结果代表错误类型
*/
int V2X_SendData(int nAid, char* p_pSendData, int nDataLen);

/**
 * @brief 关闭V2X通讯
 *
 * 关闭V2X通讯
 *
 * @param 无
 *
 * @return 执行状态，0 执行成功，非0 执行失败
 */
int V2X_Close();

#endif /* V2X_INTERFACE_H_ */

/**
 * @}
 */
