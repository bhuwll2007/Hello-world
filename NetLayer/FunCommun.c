/*
 * FunCommun.c
 *
 *  Created on: Aug 11, 2019
 *      Author: wanji
 */
#include "FunCommun.h"
void msTimeDelay(struct timeval delay, int usTxInterval)
{
	delay.tv_sec = 0;
	delay.tv_usec = usTxInterval * 1000;
	select(0, NULL, NULL, NULL, &delay);
}

int NetLayer_InitRx(tWAVEConfig pWAVEConfig, int RecvMode)
{
	//memcpy(&g_WAVEConfig ,&pWAVEConfig ,sizeof(tWAVEConfig));
	//RxEnable = RecvMode;
	return 0;
}
