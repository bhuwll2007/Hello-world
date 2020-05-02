/*
 * SpatDeal.h
 *
 *  Created on: Sep 17, 2018
 *      Author: root
 */

#ifndef SPATDEAL_H_
#define SPATDEAL_H_

#include "../DataSet/DataSet.h"
#include "../../Include/FaciLayer_Include.h"
#include "../../Include/BaseInterface_Include.h"
#include "../Common/Common.h"
#include "YuTong_Cloud.h"
extern int FG_SpatInit(tStateStore *p_prectApp);
extern void FG_SpatTxCallBack(char *p_pcRecvBuf, uint32_t p_nRecvBufSize);


extern void FG_SpatTxCallBack_Old(char *p_pcRecvBuf, uint32_t p_nRecvBufSize);


static void FS_YuTongSpatSendData(tSpatMsg p_rectSpatInfo);
static int FS_SpatUdpDataProcess(uint8_t *p_ucBuf, uint32_t p_unLen);

#endif /* SPATDEAL_H_ */
