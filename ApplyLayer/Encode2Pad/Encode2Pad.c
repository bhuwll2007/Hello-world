/*
 * Encode2Pad.c
 *
 *  Created on: Jan 6, 2020
 *      Author: wanijv2x
 */

#include "Encode2Pad.h"

#define MODE_PROTOCOL2PAD_V1 0
#define MODE_PROTOCOL2PAD_V2 1
/****************************************************
 * 函数名称:
 * 功能描述:
 * 输入参数:
 * 输出参数:
 * 返 回 值:
 * 创建日期:
 ****************************************************/
int EncodeD0(UpD0Data pUpD0Data, uint8_t *desbuf, uint32_t *deslen)
{
#if MODE_PROTOCOL2PAD_V1
	EncodeD0_V1(pUpD0Data, desbuf, deslen);
#endif
#if MODE_PROTOCOL2PAD_V2
	EncodeD0_V2(pUpD0Data, desbuf, deslen);
#endif
	return 0;
}
/****************************************************
 * 函数名称:
 * 功能描述:
 * 输入参数:
 * 输出参数:
 * 返 回 值:
 * 创建日期:
 ****************************************************/
int EncodeD3(tVehData HV, tVehData RV, tResult_Bsm lRes, uint8_t *desbuf,
		uint32_t *deslen) {
#if MODE_PROTOCOL2PAD_V1
	EncodeD3_V1(HV, RV, lRes, desbuf, deslen);
#endif
#if MODE_PROTOCOL2PAD_V2
	EncodeD3_V2(HV, RV, lRes, desbuf, deslen);
#endif
	return 0;
}
/****************************************************
 * 函数名称:
 * 功能描述:
 * 输入参数:
 * 输出参数:
 * 返 回 值:
 * 创建日期:
 ****************************************************/
int EncodeD4(tVehData HV, tRsiMsg lRsiInfo, tResult_Rsi lResult_Rsi,
		uint8_t *desbuf, uint32_t *deslen) {
#if MODE_PROTOCOL2PAD_V1
	EncodeD4_V1(HV, lRsiInfo, lResult_Rsi, desbuf, deslen);
#endif
#if MODE_PROTOCOL2PAD_V2
	EncodeD4_V2(HV, lResult_Rsi, desbuf, deslen);
#endif
	return 0;
}

/****************************************************
 * 函数名称:
 * 功能描述:
 * 输入参数:
 * 输出参数:
 * 返 回 值:
 * 创建日期:
 ****************************************************/
int EncodeD5(double Laser_Lat, double Laser_Long, tVehData HV, tVehData *RV,
		uint32_t nRVCont, uint8_t *desbuf, uint32_t *deslen) {

#if MODE_PROTOCOL2PAD_V1
	EncodeD5_V1(Laser_Lat, Laser_Long, HV, RV,
			nRVCont, desbuf, deslen);
#endif
#if MODE_PROTOCOL2PAD_V2
	EncodeD5_V2(Laser_Lat, Laser_Long, HV, RV, nRVCont, desbuf,
			deslen);
#endif
	return 0;
}

/****************************************************
 * 函数名称:
 * 功能描述:
 * 输入参数:
 * 输出参数:
 * 返 回 值:
 * 创建日期:
 ****************************************************/
int EncodeD6(tVehData HV, tSpeedGudie_Input pSpeedGudie_Input, tResult_Spat Res,
		uint8_t *desbuf, uint32_t *deslen) {
#if MODE_PROTOCOL2PAD_V1
	EncodeD6_V1(HV, pSpeedGudie_Input, Res, desbuf, deslen);
#endif
#if MODE_PROTOCOL2PAD_V2
	EncodeD6_V2(HV, pSpeedGudie_Input, Res, desbuf, deslen);
#endif
	return 0;
}
/****************************************************
 * 函数名称:
 * 功能描述:
 * 输入参数:
 * 输出参数:
 * 返 回 值:
 * 创建日期:
 ****************************************************/
int EncodeD7(tMapInfo MapInfo, uint8_t *desbuf, uint32_t *deslen) {

#if MODE_PROTOCOL2PAD_V1
	EncodeD7_V1(MapInfo, desbuf, deslen);
#endif
#if MODE_PROTOCOL2PAD_V2
	EncodeD7_V2(MapInfo, desbuf, deslen);
#endif
	return 0;
}

/****************************************************
 * 函数名称:
 * 功能描述:
 * 输入参数:
 * 输出参数:
 * 返 回 值:
 * 创建日期:
 ****************************************************/
int EncodeD9(UpD9Data pUpD9Data, uint8_t *desbuf, uint32_t *deslen)
{
#if MODE_PROTOCOL2PAD_V1
	EncodeD9_V1(pUpD9Data, desbuf, deslen);
#endif
#if MODE_PROTOCOL2PAD_V2
	EncodeD9_V2(pUpD9Data, desbuf, deslen);
#endif
	return 0;
}
