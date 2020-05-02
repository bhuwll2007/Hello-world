#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
pthread_mutex_t mutex_1;

void Thread_DealData1(void *p)
{
	while (1)
	{
		pthread_mutex_lock(&mutex_1);
		printf("Thread_DealData1111111111 Begin\n");
		sleep(1);
		printf("Thread_DealData1111111111 End\n");
		pthread_mutex_unlock(&mutex_1);
		sleep(1);
	}
}
void Thread_DealData2(void *p)
{
	while (1)
	{
		pthread_mutex_lock(&mutex_1);
		printf("Thread_DealData2 Begin\n");
		sleep(1);
		printf("Thread_DealData2 End\n");
		pthread_mutex_unlock(&mutex_1);
		sleep(1);
	}
}

int main(int argc, char *argv[]) {

	int ret ;

	pthread_mutex_init(&mutex_1, NULL);

	pthread_t serialThread1;
	pthread_t serialThread2;

	//创建串口线程
	ret = pthread_create(&serialThread1, NULL,
			(void *(*)(void *)) Thread_DealData1, 0);

	//创建串口线程
	ret = pthread_create(&serialThread2, NULL,
			(void *(*)(void *)) Thread_DealData2, 0);

	if(ret == 0);
	while (1)
		;
	return 0;
}
