
/**
 * @ingroup Algorithm API
 * @{
 *
 * 本模块提供V2X算法接口层的基本公用函数的API接口.
 *
 * @file GetMapPoints.h
 * @brief API file.
 *
 */
#ifndef SRC_VEHLOCAL_GETMAPPOINTS_H_
#define SRC_VEHLOCAL_GETMAPPOINTS_H_

#include "../DataSet/DataSet.h"
//#include "TypeInclude.h"
#include "MapMatch.h"

/**
 * @brief 打印地图结构体.
 * @param[in] map 地图结构体.
 * @return 无.
 */
void Out_Map(tMapInfo *pMapInfo);
/**
 * @brief 从地图文件中读取MAP消息体.
 * @param[out] pSendMsg MAP消息体.
 * @param[in] pConfigFileName 地图文件名.
 * @return 无.
 */
void LoadXml(MessageFrame_t **pSendMsg, char *pConfigFileName);
/**
 * @brief 拷贝地图结构体.
 * @param[in] pSrcMapInfo 源地图结构体.
 * @param[out] pDstMapInfo 目的地图结构体.
 * @return 函数执行状态，0：拷贝成功，其他：拷贝失败.
 */
int cpyMapInfo(tMapInfo *pSrcMapInfo, tMapInfo *pDstMapInfo);
/**
 * @brief 清空地图结构体.
 * @param[in] pMapInfo  地图结构体.
 * @return 函数执行状态，0：提取成功，其他：提取失败.
 */
int ClearMapInfo(tMapInfo *pMapInfo);
/**
 * @brief 从MAP消息体中提取地图信息到地图结构体中.
 * @param[out] pMapInfo  地图结构体.
 * @param[in] pMAPMsg MAP消息体.
 * @return 提取信息的状态，0：提取成功，其他：提取失败.
 */
int GetMapInfo(tMapInfo *pMapInfo, MessageFrame_t *pMAPMsg);

#endif /* SRC_VEHLOCAL_GETMAPPOINTS_H_ */
