/*
 * V2X_Init.h
 *
 *  Created on: May 31, 2018
 *      Author: root
 */

#ifndef V2X_INIT_H_
#define V2X_INIT_H_

#include "../../Include/BaseInterface_Include.h"
#include "../../Include/FaciLayer_Include.h"

#include "../V2X_Transf/RsmDeal.h"
#include "../V2X_Transf/SpatDeal.h"
//#include "../V2X_Transf/TcpServer.h"

#include "V2X_RecvBSM.h"
#include "V2X_RecvRSM.h"
#include "V2X_RecvRSI.h"
#include "V2X_RecvMAP.h"
#include "V2X_RecvSPAT.h"

int V2X_Init(tStateStore *pApp);

#endif /* V2X_INIT_H_ */
