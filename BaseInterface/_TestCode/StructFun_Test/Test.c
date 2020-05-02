#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <poll.h>
#include <fcntl.h>
#include <math.h>
#include <sys/types.h>
#include <sys/stat.h>
//#include <sys/shm.h>
#include <signal.h>
#include <assert.h>
#include <inttypes.h>

#include <sys/time.h> // for rate control
#include <sys/socket.h>
#include <netpacket/packet.h>
#include <net/ethernet.h>
#include <netinet/in.h>
#include <net/if.h>
#include <pthread.h>
//#include <sys/ipc.h>
//#include <sys/sem.h>

#include <getopt.h>
#include <linux/if_ether.h>
//#include "TxOpts.h"

#include <sys/ioctl.h>
#include <netinet/if_ether.h>
#include <linux/sockios.h>

#include <stdint.h>
#include <stdbool.h>
#include <sys/select.h>
#include <linux/filter.h>
struct Hello
{
	char Name[100];
	void (*sayHello)(char *name);
};
void sayHello(char *Name)
{
	int n = 5;
	while (n--)
	{
		printf("你好，%s\n", Name);
		sleep(1);
	}/* end while(i_serial_open) */
}
void Thread_RecvGPSData1(void *p)
{
	struct Hello *a = (struct Hello *)p;
	while (1)
	{
		a->sayHello(a->Name);

		sleep(8);
	}
}

void Thread_RecvGPSData2(void *p)
{
	struct Hello *b = (struct Hello *)p;
	while (1)
	{
		b->sayHello(b->Name);
		sleep(8);
	}
}

int main()
{
	int ret;


	struct Hello a;
	a.sayHello = sayHello;
	strcpy(a.Name, "11111");
	pthread_t serialThread1;

	//创建串口线程
	ret = pthread_create(&serialThread1, NULL,
			(void *(*)(void *)) Thread_RecvGPSData1, (void *)&a);

	struct Hello b;
	b.sayHello = sayHello;
	strcpy(b.Name, "2222");
	pthread_t serialThread2;
	if (ret == 0)
		;
	//创建串口线程
	ret = pthread_create(&serialThread2, NULL,
			(void *(*)(void *)) Thread_RecvGPSData2, (void *)&b);
	if (ret == 0)
		;
	while (1)
		;
	return 0;
}
