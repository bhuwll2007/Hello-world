/***************************************************************//**
 * @defgroup FacHostProcess FacHostProcess API
 * @ingroup FaciLayer
 * @{
 *
 *  This module provides APIs to do the operate about Host equipment.
 *
 * @file FacHostProcess.h
 * @brief API file.
 *
 *
 *******************************************************************/
#ifndef _FACHOSTPROCESS_H_
#define _FACHOSTPROCESS_H_

//------------------------------------------------------------------------------
// Included headers
//------------------------------------------------------------------------------

#include "../BaseInterface/CANUtil/CAN.h"
#include "../BaseInterface/V2XUtil/V2X_INTERFACE.h"
#include "../BaseInterface/GPSUtil/GPS.h"
#include "ExtFunction.h"
#include "DataSet.h"

//------------------------------------------------------------------------------
/**
 * @brief 获取主机属性信息请求.
 * @return  0 接受，1 拒绝.
 */
int AppGetHostInfo_rq();

/**
 * @brief 返回设备的基本属性数据ID， 如查询OBU的GNSS数据、或者RSU的消息发送类型.
 * @param[in] Answer      获取主机属性信息请求时返回的结果.
 * @param[in] pHostInfo   本机数据请求信息结构体.
 * @return  执行结果（HostProcess_ErrorCode）.
 */
int AppGetHostInfo_rs(int Answer, tHostInfo *pHostInfo);

/**
 * 函数名 :  AppGetHostStatus_rq()
 * 功能描述:	执行获取设备状态信息查询请求
 * 输入参数:
 * 输出参数:
 * 返回值 :0 接收请求， 其他 拒绝请求
 **/
//int AppGetHostStatus_rq();
/**
 * 函数名 :  AppGetHostStatus_rs(int Res, struct HostStatus)
 * 功能描述:	查询当前设备各模块是否正常工作
 * 输入参数:  Res 请求结果标识位
 * 输出参数:  HostStatus 主机状态结构体
 * 返回值 : 0 查询成功， 其他 失败理由
 **/
//int AppGetHostStatus_rs(int Res, struct HostStatus);
/**
 * @brief 获取主机通信状态信息请求.
 * @return  0 接受，1 拒绝.
 */
int AppGetCommStatus_rq();

/**
 * @brief 获取设备通信状态属性及状态信息.
 * @param[in] Answer         获取主机通信状态信息请求时返回的结果.
 * @param[in] pCommStatus    本机通信状态信息结构体.
 * @return  执行结果（HostProcess_ErrorCode）.
 */
int AppGetCommStatus_rs(int Answer, tCommStatus *pCommStatus);

/**
 * @brief 配置主机通信参数请求.
 * @return  0 接受，1 拒绝.
 */
int AppSetCommCfg_rq();

/**
 * @brief 获取设备通信状态属性及状态信息.
 * @param[in] Res         配置主机通信参数请求时返回的结果.
 * @param[in] pCommCfg    通信设置参数指针.
 * @return  执行结果（HostProcess_ErrorCode）.
 */
int AppSetCommCfg_rs(int Res, char *pCommCfg);

#endif

/**
 * @}
 */
