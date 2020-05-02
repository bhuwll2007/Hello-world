/**
 * @ingroup Algorithm API
 * @{
 *
 * 本模块提供V2X算法接口层-红绿灯相关应用的API接口.
 *
 * @file SPAT_Algorithm.h
 * @brief API file.
 *
 */

#ifndef ALGORITHM_SPAT_ALGORITHM_H_
#define ALGORITHM_SPAT_ALGORITHM_H_

#include "Appconfigure.h"
#include "ComputeTool.h"
#include "DataSet/DataSet.h"
#include "Type.h"

/**
 * @brief 将存储Spat消息的MessageFrame结构体中的有效内容提取.
 * @param[in] pSpatMsg  用于提取Spat算法相关变量值的MessageFrame结构体.
 * @param[out] pSpatInfo  提取到的与Spat算法相关的结构体.
 */
void ClearSpatInfo(tSpatMsg *pSpatInfo);

/**
 * @brief 将存储Spat消息的MessageFrame结构体中的有效内容提取.
 * @param[in] pSpatMsg  用于提取Spat算法相关变量值的MessageFrame结构体.
 * @param[out] pSpatInfo  提取到的与Spat算法相关的结构体.
 */
void GetSpatInfo(MessageFrame_t *pSpatMsg, tSpatMsg *pSpatInfo);

/**
 * @brief 获取自车前方的信号灯相关信息.
 * @param[in] pHV  自车信息结构体.
 * @param[in] pSpatInfo  用于提取前方信号灯信息的Spat结构体.
 * @param[in] pMapInfo   用于提取前方信号灯信息的Map结构体.
 * @param[out] pSpeedGudie  提取到的自车前方信号灯信息的结构体.
 */
void GetForward_Intersection(tVehData *pHV, tSpatMsg *pSpatInfo, tMapInfo *pMapInfo, tSpeedGudie_Input *pSpeedGudie_Input);

/**
 * @brief 绿波车速引导 + 闯红灯预警 算法.
 * @param[in] pHV  用于算法的自车信息结构体.
 * @param[in] pSpeedGudie_Input  用于算法的自车前方信号灯结构体.
 * @return  Spat算法返回结构体.
 */
tResult_Spat SpeedGuide(tVehData *pHV, tSpeedGudie_Input *pSpeedGudie_Input);

#endif /* ALGORITHM_SPAT_ALGORITHM_H_ */

/**
 * @}
 */
