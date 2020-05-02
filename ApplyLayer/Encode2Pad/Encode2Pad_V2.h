/*
 * Encode2Pad_V2.h
 *
 *  Created on: Jan 6, 2020
 *      Author: wanijv2x
 */

#ifndef APPLYLAYER_ENCODE2PAD_ENCODE2PAD_V2_H_
#define APPLYLAYER_ENCODE2PAD_ENCODE2PAD_V2_H_

#include "../DataSet/DataSet.h"
#include "../../Include/lib_Include.h"

#include "../Common/Common.h"

//*****************处理结果结构体***************//
typedef struct _Result_Rsi_V2
{
	uint8_t wStr; /**< V2I应用场景. */

	///消息类型
	uint8_t MsgType;
	///参考位置
	nint32_t Latitude; /**< 纬度值. */
	nint32_t Longitude; /**< 经度值. */
	uint16_t AlertPosDir; /**< 图标基于路网的角度. */

	///从Rsi的Description中提取出的事件相关信息
	uint16_t TagVal;
	uint8_t AssisTag;
	uint8_t AssisVal;
}__attribute__ ((packed)) tResult_Rsi_V2;

typedef struct __Result_Rsm_V2
{
	uint8_t nAnalysisCount; /**< 识别物个数. */
	uint8_t AnalysisInfoId[8]; /** 识别物ID. */
	nint32_t Latitude; /**< 纬度 度. */
	nint32_t Longitude; /**< 经度 度. */
	uint16_t Speed; /**< 速度 米每秒. */
	uint16_t Heading;/**< 航向角 度. */
	nint8_t Location;/**< 相对方位 . */
}__attribute__ ((packed)) tResult_Rsm_V2;


//*****************处理结果结构体***************//
typedef struct _UpPhaseInfo_V2
{
	uint8_t Status; /**< 灯色状态. */
	uint8_t TimeLeft; /**< 剩余时间. */
	uint8_t Direction; /**< 行车方向. */
	uint8_t SuggSd_H; /**< 最高建议时速. */
	uint8_t SuggSd_L; /**< 最低建议时速. */
	uint8_t SuggSd; /**< 建议时速. */
}__attribute__ ((packed)) UpPhaseInfo_V2;

typedef struct _SpeedGudie_Input_V2
{
	uint16_t HVLight; /**< 转向灯状态 详见定义. */
	nint32_t Latitude; /**< 信号灯纬度. */
	nint32_t Longitude; /**< 信号灯经度. */
//	uint8_t wType; /**< 应用类型. */
	uint8_t wStr; /**< 应用场景. */
	uint8_t wPrio; /**< 优先级. */
//	uint16_t Cycle; /**< 信号灯周期. */
//	nint16_t Distance; /**< 到红绿灯距离. */
	nint16_t RelBearing; /**< 红绿灯方位角. */
	uint8_t PhaseCnt; /**< 前方相位个数. */
	UpPhaseInfo_V2 PhaseInfo_V2[PHASEINFO_MAX_NUM]; /**< 包含相位信息结构体. */
}__attribute__ ((packed)) tResult_Spat_V2;


/****************************************************
 * 函数名称:int EncodeD0(UpD0Data pUpD0Data, uint8_t *desbuf, uint32_t *deslen)
 * 功能描述:将UpD0Data结构体的数据按照D0帧协议帧格式组帧，并存放到缓存desbuf中，缓存长度为deslen
 * 输入参数:UpD0Data pUpD0Data：周围车辆信息结构体变量,
 * 输出参数:uint8_t *desbuf：D0帧内容, uint32_t *deslen：D0帧内容长度
 * 返 回 值:
 * 创建日期:
 ****************************************************/
int EncodeD0_V2(UpD0Data pUpD0Data, uint8_t *desbuf, uint32_t *deslen);
/****************************************************
 * 函数名称:
 * 功能描述:
 * 输入参数:
 * 输出参数:
 * 返 回 值:
 * 创建日期:
 ****************************************************/
int EncodeD3_V2(tVehData HV, tVehData RV, tResult_Bsm lRes, uint8_t *desbuf,
		uint32_t *deslen);

/****************************************************
 * 函数名称:
 * 功能描述:
 * 输入参数:
 * 输出参数:
 * 返 回 值:
 * 创建日期:
 ****************************************************/
int EncodeD4_V2(tVehData HV, tResult_Rsi lResult_Rsi,
		uint8_t *desbuf, uint32_t *deslen);

/****************************************************
 * 函数名称:
 * 功能描述:
 * 输入参数:
 * 输出参数:
 * 返 回 值:
 * 创建日期:
 ****************************************************/
int EncodeD5_V2(double Laser_Lat, double Laser_Long, tVehData HV, tVehData *RV,
		uint32_t nRVCont, uint8_t *desbuf, uint32_t *deslen);
/****************************************************
 * 函数名称:
 * 功能描述:
 * 输入参数:
 * 输出参数:
 * 返 回 值:
 * 创建日期:
 ****************************************************/
int EncodeD6_V2(tVehData HV, tSpeedGudie_Input pSpeedGudie_Input,
		tResult_Spat Res, uint8_t *desbuf, uint32_t *deslen);
/****************************************************
 * 函数名称:
 * 功能描述:
 * 输入参数:
 * 输出参数:
 * 返 回 值:
 * 创建日期:
 ****************************************************/
int EncodeD7_V2(tMapInfo MapInfo, uint8_t *desbuf, uint32_t *deslen);
/****************************************************
 * 函数名称:
 * 功能描述:
 * 输入参数:
 * 输出参数:
 * 返 回 值:
 * 创建日期:
 ****************************************************/
int EncodeD9_V2(UpD9Data pUpD9Data, uint8_t *desbuf, uint32_t *deslen);
#endif /* APPLYLAYER_ENCODE2PAD_ENCODE2PAD_V2_H_ */
