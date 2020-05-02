#include "./../../SerialUtil/Serial.h"
int main(int argc, char *argv[])
{
	StcSerial lpStcSerial;
	int nRet;
	memcpy(lpStcSerial.SerialName, "/dev/ttymxc0", sizeof("/dev/ttymxc0"));
	nRet = Serial_Open(&lpStcSerial);
	if (nRet <= 0)
	{
		printf("open succeed!\n");
	}
	Serial_Send(lpStcSerial.fd, "Test  ", 6);
	Serial_CreatThread(&lpStcSerial);
	while (1)
	{
		sleep(1);
		Serial_Send(lpStcSerial.fd, "Test  aa\n", 6);
	}
	return 0;
}

