#include "GPS.h"

int InitGPS()
{
	int nRet = 0;
	nRet = GPS_Open();
	return nRet;
}

int CloseGPS()
{
	int nRet = 0;
	nRet = GPS_Close();
	return nRet;
}
