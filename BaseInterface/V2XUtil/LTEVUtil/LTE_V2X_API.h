#ifndef __LTE_V_API_H_
#define __LTE_V_API_H_
#include "../LTEVUtil/LTEV_Include.h"

int api_init(LTE_V2X_Options *option);
int api_write(LTE_V2X_Options *option, char *p_pcBuf, int p_nLen);
int api_read(LTE_V2X_Options *option, char *p_pcBuf);

int LTEV_GPS_OPEN();

#endif //#ifdef __LTE_V_API_H_

