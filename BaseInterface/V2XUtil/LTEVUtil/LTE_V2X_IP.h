#ifndef __LTE_V_OPRTS_H_
#define __LTE_V_OPRTS_H_
#include "../LTEVUtil/LTEV_Include.h"

int ip_init(LTE_V2X_Options *option);
int ip_write(LTE_V2X_Options *option, char *p_pcBuf, int p_nLen);
int ip_read(LTE_V2X_Options *option, char *p_pPlayLoad);


int LTEV_GPS_OPEN();


#endif

