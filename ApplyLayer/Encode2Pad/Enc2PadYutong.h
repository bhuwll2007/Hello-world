#ifndef _APPLYLAYER_ENCODE2PAD_ENC2PAD_YUTONG_H_
#define _APPLYLAYER_ENCODE2PAD_ENC2PAD_YUTONG_H_

#include "../DataSet/DataSet.h"
#include "../../Include/lib_Include.h"
#include "../Common/Common.h"


int EncodeD4_YuTong(tVehData HV, tRsiMsg lRsiInfo, tResult_Rsi lResult_Rsi,
		uint8_t *desbuf, uint32_t *deslen);

int EncodeD6_2VehYuTong(tVehData HV, tSpeedGudie_Input pSpeedGudie_Input,
		tResult_Spat Res, uint8_t *desbuf, uint32_t *deslen);

int EncodeD6_YuTong(tVehData HV, tSpeedGudie_Input pSpeedGudie_Input,
		tResult_Spat Res, uint8_t *desbuf, uint32_t *deslen);

int EncodeD5_YuTong(double Laser_Lat, double Laser_Long, tVehData HV,
		tVehData *RV, uint32_t nRVCont, uint8_t *desbuf, uint32_t *deslen);

int EncodeD5_2VehYuTong(double Laser_Lat, double Laser_Long, tVehData HV,
		tVehData *RV, uint32_t nRVCont, uint8_t *desbuf, uint32_t *deslen);

int EncodeD3_YuTong(tVehData HV, tVehData RV, tResult_Bsm lRes, uint8_t *desbuf,
		uint32_t *deslen);

int EncodeD0_2VehYuTong(UpD0Data pUpD0Data, uint8_t *desbuf, uint32_t *deslen);
int EncodeD0_YuTong(UpD0Data pUpD0Data, uint8_t *desbuf, uint32_t *deslen);
#endif
