#include "./../../GPSUtil/GPS.h"
int OutData(GpsLocation fix)
{
	printf("fix.latitude = %d \n",fix.latitude);
	printf("fix.longitude = %d \n",fix.longitude);
	printf("fix.altitude = %d \n",fix.altitude);
	printf("fix.speed = %f \n",fix.speed);
	printf("fix.bearing = %d \n",fix.bearing);
	printf("fix.accuracy = %d \n",fix.accuracy);
	//printf("fix.timestamp = %d \n",(int)fix.timestamp);
	return 0;
}
int main()
{
	GpsLocation fix;
	int nRet = 0;
	sleep(2);
	nRet = GPS_Open();

	while(1)
	{
		usleep(20*1000);
		nRet = GPS_GetData(&fix);
		if(nRet == 0)
		{
			OutData(fix);
		}
	}
	GPS_Close();
}
