/*
 * RsmDeal.h
 *
 *  Created on: Sep 17, 2018
 *      Author: root
 */

#ifndef RSMDEAL_H_
#define RSMDEAL_H_

#include "../DataSet/DataSet.h"
#include "../../Include/FaciLayer_Include.h"
#include "../../Include/lib_Include.h"
#include "../Common/Common.h"
#include "YuTong_Cloud.h"

extern int FG_RsmUdpSerInit(int p_nPort);
extern void FG_RsmTxCallBackback(nint8_t *p_pcRecvBuf, uint32_t p_unRecvBufSize);
extern void FG_GetCompleteFrame(uint8_t *p_pucInBuf, int p_nInCnt, uint8_t *p_pucOutBuf, int *p_pnDestCnt, int *p_pnReadStatus, int * p_pnFirstReadFlag);
#endif /* RSMDEAL_H_ */
