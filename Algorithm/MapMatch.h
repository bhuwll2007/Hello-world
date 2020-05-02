/**
 * @ingroup Algorithm API
 * @{
 *
 * 本模块提供V2X算法接口层的地图匹配相关的API接口.
 *
 * @file MapMatch.h
 * @brief API file.
 *
 */

#ifndef MAPMATCH_H_
#define MAPMATCH_H_

//------------------------------------------------------------------------------
// Included headers
//------------------------------------------------------------------------------
#include "Appconfigure.h"
#include "ComputeTool.h"
#include "BSM_Algorithm.h"

//------------------------------------------------------------------------------
// Type definitions
//------------------------------------------------------------------------------

/**
 * @brief 可调用地图匹配函数接口.
 * @param[in] pMapInfo  地图信息结构体指针.
 * @param[in] pVehInfo  车辆信息结构体指针.
 * @return  0--匹配成功， 1--匹配失败.
 */
double Get_CompareValue(double ForwardLat, double ForwardLng, double BehindLat, double BehindLng, tVehData *pVehInfo);

int MapMatch_Pojcet(tMapInfo pMapInfo, tVehData *pVehInfo);
int MapMatch_Pojcet_test(tMapInfo pMapInfo, tVehData *pVehInfo);
int MapMatch_RoadPints(tRoadPoint *proadPoints, int pointsCnt, tVehData *pVehInfo);
#endif /* MAPMATCH_H_ */

/**
 * @}
 */
