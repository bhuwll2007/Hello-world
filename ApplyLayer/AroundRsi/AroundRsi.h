/*
 * AroundRsi.h
 *
 *  Created on: Jun 7, 2018
 *      Author: root
 */

#ifndef __APPLYLAYER_AROUNDRSI_H_
#define __APPLYLAYER_AROUNDRSI_H_

#include "../../DataSet/DataSet.h"
#include "../DataSet/DataSet.h"
#include "../Common/Common.h"
#include "../Common/SendData2Pad.h"
#include "../HostStatus/HostStatus.h"
#include "../DataBase/DataBase.h"
#include "../Encode2Pad/Encode2Pad.h"

int InitAroundRsi();
int AddRsiInfo(UpD9Data *pUpD9Data, tRsiMsg lRsiInfo, tResult_Rsi lResult_Rsi);
int AddMapSLInfo(UpD9Data *pUpD9Data, tResult_SL lResult_SL);
#endif /* __APPLYLAYER_AROUNDRSI_H_ */
