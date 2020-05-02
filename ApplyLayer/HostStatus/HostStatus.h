/*
 * HostStatus.h
 *
 *  Created on: May 25, 2018
 *      Author: root
 */

#ifndef HOSTSTATUS_H_
#define HOSTSTATUS_H_

#include "../../Include/lib_Include.h"
#include "../../DataSet/DataSet.h"
#include "../Common/Common.h"
#include "../Common/SendData2Pad.h"

extern int InitHostStatus();
extern int getHvInfo(tVehData *HV);

#endif /* HOSTSTATUS_H_ */
