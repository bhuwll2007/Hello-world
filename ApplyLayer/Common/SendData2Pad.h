/*
 * SendData2Pad.h
 *
 *  Created on: Nov 10, 2018
 *      Author: root
 */

#ifndef SENDDATA2PAD_H_
#define SENDDATA2PAD_H_

#include "../../Include/BaseInterface_Include.h"
#include "../../DataSet/DataSet.h"
typedef struct _Msg{
	long nMsg_Cmd;
	char acMsg_Text[1024];
}tMsg;
int fun_SendData2Pad(nint8_t *pbuf, nint32_t len);
void initMsgQueue_Send2Veh();
#endif /* SENDDATA2PAD_H_ */
