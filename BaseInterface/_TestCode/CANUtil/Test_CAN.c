#include "./../../CANUtil/CAN.h"

void OutData(CANData fix)
{
	printf("===================================\n");
	printf("lat = %d\n", fix.lat);
	printf("lng = %d\n", fix.lng);
	printf("alt = %d\n", fix.alt);
	printf("===================================\n");

	printf("transmission = %d\n", fix.transmission);
	printf("Release1 = %d\n", fix.Release1);
	printf("speed = %d\n", fix.speed);
	printf("===================================\n");

	printf("heading = %d\n", fix.heading);
	printf("angle = %d\n", fix.angle);
	printf("Release2 = %d\n", fix.Release2);
	printf("vehicleClass = %d\n", fix.vehicleClass);
	printf("===================================\n");

	printf("motionCfd.speedCfd = %d\n", fix.motionCfd.speedCfd);
	printf("motionCfd.headingCfd = %d\n", fix.motionCfd.headingCfd);
	printf("motionCfd.steerCfd = %d\n", fix.motionCfd.steerCfd);
	printf("motionCfd.Release1 = %d\n", fix.motionCfd.Release1);
	printf("===================================\n");

	printf("accelSet.Long = %d\n", fix.accelSet.Long);
	printf("accelSet.lat = %d\n", fix.accelSet.lat);
	printf("accelSet.vert = %d\n", fix.accelSet.vert);
	printf("accelSet.Release1 = %d\n", fix.accelSet.Release1);
	printf("accelSet.yaw = %d\n", fix.accelSet.yaw);
	printf("===================================\n");

	printf("brakes.brakePadel = %d\n", fix.brakes.brakePadel);
	printf("brakes.wheelBrakes = %d\n", fix.brakes.wheelBrakes);
	printf("brakes.traction = %d\n", fix.brakes.traction);
	printf("brakes.abs = %d\n", fix.brakes.abs);
	printf("brakes.scs = %d\n", fix.brakes.scs);
	printf("brakes.brakeBoost = %d\n", fix.brakes.brakeBoost);
	printf("brakes.auxBrakes = %d\n", fix.brakes.auxBrakes);
	printf("brakes.Release1 = %d\n", fix.brakes.Release1);
	printf("===================================\n");

	printf("size.width = %d\n", fix.size.width);
	printf("size.length = %d\n", fix.size.length);
	printf("size.height = %d\n", fix.size.height);
	printf("size.Release1[0] = %d\n", fix.size.Release1[0]);
	printf("size.Release1[1] = %d\n", fix.size.Release1[1]);
	printf("size.Release1[2] = %d\n", fix.size.Release1[2]);
	printf("===================================\n");

	printf("vehicleClass = %d\n", fix.vehicleClass);
	printf("vehicleLight = %d\n", fix.vehicleLight);
	int i = 0;
	printf("Release3:\n");
	for (i = 0; i < 10; i++)
		printf("[%d] = %d ", i, fix.size.Release1[i]);
	printf("\n===================================\n");

}
int main()
{
	char str[30];
//	CANData fix;
	int nRet;
	int sock = CAN_Open("can0");
	while(1)
	{
		sleep(2);
//		nRet = CAN_GetData(&fix);
//		if(nRet == 0)
//		{
//			OutData(fix);
//		}

		printf("Please Input string to send:");
		nRet = scanf("%s", str);
		if(nRet > 0);
		if(strlen(str)>0)
		{
			CAN_SendData(sock,str,strlen(str));
		}
	}
	close(sock);

	return 0;
}




