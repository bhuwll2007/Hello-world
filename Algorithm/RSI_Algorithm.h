/**
 * @ingroup Algorithm API
 * @{
 *
 * 本模块提供V2X算法接口层-路侧信息提醒的API接口.
 *
 * @file RSI_Algorithm.h
 * @brief API file.
 *
 */
#ifndef ALGORITHM_RSI_ALGORITHM_H_
#define ALGORITHM_RSI_ALGORITHM_H_

#include "Appconfigure.h"
#include "ComputeTool.h"
#include "DataSet/DataSet.h"
#include "Type.h"
tRsiDscpInfo GetRsiDscpInfo(char *JsonStr);
/**
 * @brief 读取xml文件，获取MessageFrame消息.
 * @param[in] pConfigFileName  xml文件所在位置.
 * @param[out] pSendMsg  获取到的MessageFrame类型结构体.
 */
extern void ReadXml(MessageFrame_t **pSendMsg, char *pConfigFileName);
void ClearRsiInfo(tRsiMsg_Arry *pRsiInfo);
/**
 * @brief 将存储Rsi消息的MessageFrame结构体中的有效内容提取.
 * @param[in] pRsiMsg  用于提取Rsi算法相关变量值的MessageFrame结构体.
 * @param[out] pRsiInfo  提取到的与Rsi算法相关的结构体.
 */
int GetRsiInfo(MessageFrame_t *pRsiMsg, tRsiMsg_Arry *pRsiInfo);
uint8_t GetPos(double p_dLatObj, double p_dlngObj, double p_dLatTag,
		double p_dlngTag, double p_dCObj);
/**
 * @brief 车内标牌算法.
 * @param[in] pHV  用于算法的自车信息结构体.
 * @param[in] pRsiUsage  用于算法的Rsi信息结构体.
 * @return  Rsi算法返回结构体.
 */
tResult_Rsi IVS(tVehData *pHV, tRsiMsg *pRsiUsage);

/**
 * @brief 道路限速算法.
 * @param[in] pHV  用于算法的自车信息结构体.
 * @param[in] pRsiUsage  用于算法的Rsi信息结构体.
 * @param[in] pMapInfo  用于算法的Map信息结构体.
 * @return  Rsi算法返回结构体.
 */
tResult_SL SL(tVehData *pHV, tRsiMsg *pRsiUsage, tMapInfo *pMapInfo);
/**
 * @brief 地图信息限速算法.
 * @param[in] pHV  用于算法的自车信息结构体.
 * @param[in] pMapInfo  用于算法的Map信息结构体.
 * @return  Rsi算法返回结构体.
 */
tResult_SL MapSL(tVehData *pHV, tMapInfo *pMapInfo);

/**
 * @brief 路侧事件地图匹配算法.
 * @param[in] pHV  用于算法的自车信息结构体.
 * @param[in] pRsiUsage  用于算法的Rsi信息结构体.
 * @param[in] pMapInfo  用于算法的Map信息结构体.
 * @return  Rsi算法返回结构体.
 */
tResult_Rsi IVS_MapMatch(tVehData *pHV, tRsiMsg *pRsiUsage, tMapInfo pMapInfo);

/**
 * @brief 获取图标当前需要旋转的角度
 * @param[in] pRsiUsage  用于算法的Rsi信息结构体.
 * @param[in] pMapInfo  用于算法的Map信息结构体.
 * @param[in] ForwardNode  下游节点.
 * @param[in] BackNode   上游节点.
 * @return 图标需要旋转的角度
 */
double Get_IconAngle(tRsiMsg *pRsiUsage, tMapInfo pMapInfo,
		uint32_t ForwardNode, uint32_t BackNode);
/**
 * @brief 路侧事件处理函数接口.
 * @param[in] pHV  用于算法的自车信息结构体.
 * @param[in] pRsiUsage  用于算法的Rsi信息结构体.
 * @param[in] pMapInfo  用于算法的Map信息结构体.
 * @return  Rsi算法返回结构体.
 */
tResult_Rsi RSI_EventProcess(tVehData *pHV, tRsiMsg *pRsiUsage,
		tMapInfo pMapInfo);

#endif /* ALGORITHM_RSI_ALOGORITHM_H_ */

/**
 * @}
 */
