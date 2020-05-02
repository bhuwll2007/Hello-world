/*
 * Encode2Pad.h
 *
 *  Created on: Jan 6, 2020
 *      Author: wanijv2x
 */

#ifndef APPLYLAYER_ENCODE2PAD_ENCODE2PAD_H_
#define APPLYLAYER_ENCODE2PAD_ENCODE2PAD_H_

#include "Encode2Pad_V1.h"
#include "Encode2Pad_V2.h"

/****************************************************
 * 函数名称:
 * 功能描述:
 * 输入参数:
 * 输出参数:
 * 返 回 值:
 * 创建日期:
 ****************************************************/
int EncodeD0(UpD0Data pUpD0Data, uint8_t *desbuf, uint32_t *deslen);

/****************************************************
 * 函数名称:
 * 功能描述:
 * 输入参数:
 * 输出参数:
 * 返 回 值:
 * 创建日期:
 ****************************************************/
int EncodeD3(tVehData HV, tVehData RV, tResult_Bsm lRes, uint8_t *desbuf,
		uint32_t *deslen);

/****************************************************
 * 函数名称:
 * 功能描述:
 * 输入参数:
 * 输出参数:
 * 返 回 值:
 * 创建日期:
 ****************************************************/
int EncodeD4(tVehData HV, tRsiMsg lRsiInfo, tResult_Rsi lResult_Rsi,
		uint8_t *desbuf, uint32_t *deslen);

/****************************************************
 * 函数名称:
 * 功能描述:
 * 输入参数:
 * 输出参数:
 * 返 回 值:
 * 创建日期:
 ****************************************************/
int EncodeD5(double Laser_Lat, double Laser_Long, tVehData HV, tVehData *RV,
		uint32_t nRVCont, uint8_t *desbuf, uint32_t *deslen);

/****************************************************
 * 函数名称:
 * 功能描述:
 * 输入参数:
 * 输出参数:
 * 返 回 值:
 * 创建日期:
 ****************************************************/
int EncodeD6(tVehData HV, tSpeedGudie_Input pSpeedGudie_Input, tResult_Spat Res,
		uint8_t *desbuf, uint32_t *deslen);

/****************************************************
 * 函数名称:
 * 功能描述:
 * 输入参数:
 * 输出参数:
 * 返 回 值:
 * 创建日期:
 ****************************************************/
int EncodeD7(tMapInfo MapInfo, uint8_t *desbuf, uint32_t *deslen);

/****************************************************
 * 函数名称:
 * 功能描述:
 * 输入参数:
 * 输出参数:
 * 返 回 值:
 * 创建日期:
 ****************************************************/
int EncodeD9(UpD9Data pUpD9Data, uint8_t *desbuf, uint32_t *deslen);

#endif /* APPLYLAYER_ENCODE2PAD_ENCODE2PAD_H_ */
