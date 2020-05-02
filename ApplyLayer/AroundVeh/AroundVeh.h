/*
 * AroundVeh.h
 *
 *  Created on: Jun 7, 2018
 *      Author: root
 */

#ifndef __APPLYLAYER_AROUNDVEH_H_
#define __APPLYLAYER_AROUNDVEH_H_

#include "../../DataSet/DataSet.h"
#include "../DataSet/DataSet.h"
#include "../Common/Common.h"
#include "../Common/SendData2Pad.h"
#include "../HostStatus/HostStatus.h"
#include "../AroundRsi/AroundRsi.h"
#include "../Encode2Pad/Encode2Pad.h"
extern void *Thread_AroundVeh(void * arg);
int InitAroundVeh();
int AddInfo(UpD0Data *pUpD0Data, tVehData *RV, int nIsSafe);
#endif /* __APPLYLAYER_AROUNDVEH_H_ */
