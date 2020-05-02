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

#include <arpa/inet.h>
#include <sys/ioctl.h>
#include <netinet/if_ether.h>
#include <linux/sockios.h>

#include <stdint.h>
#include <stdbool.h>
#include <sys/select.h>
#include <linux/filter.h>

/*线程一*/
void thread_1(void)
{
	int i = 0;
	for (i = 0; i <= 6; i++)
	{
		printf("This is a pthread_1.\n");
		if (i == 2)
			pthread_exit(0); //用pthread_exit()来调用线程的返回值，用来退出线程，但是退出线程所占用的资源不会随着线程的终止而得到释放
		sleep(1);
	}
}

/*线程二*/
void thread_2(void)
{
	int i;
	for (i = 0; i < 3; i++)
		printf("This is a pthread_2.\n");
	pthread_exit(0); //用pthread_exit()来调用线程的返回值，用来退出线程，但是退出线程所占用的资源不会随着线程的终止而得到释放
}

int main(void)
{
	pthread_t id_1 = NULL, id_2 = NULL;
	int ret;
	/*创建线程一*/
	ret = pthread_create(&id_1, NULL, (void *) thread_1, NULL);
	if (ret != 0)
	{
		printf("Create pthread error!\n");
		return -1;
	}
	/*创建线程二*/
	ret = pthread_create(&id_2, NULL, (void *) thread_2, NULL);
	if (ret != 0)
	{
		printf("Create pthread error!\n");
		return -1;
	}
	printf("id_1:%d id_2:%d \n",(int)id_1,(int)id_2);
	printf("id_1:%d id_2:%d \n",*(int*)(id_1),*(int*)(id_2));
	/*等待线程结束*/
	pthread_join(id_2, NULL);
	printf("4id_1:%d id_2:%d \n",(int)id_1,(int)id_2);
	pthread_join(id_1, NULL);
	printf("3id_1:%d id_2:%d \n",(int)id_1,(int)id_2);
	return 0;
}
