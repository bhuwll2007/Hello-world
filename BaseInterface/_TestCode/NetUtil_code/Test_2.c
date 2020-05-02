//TCP服务端
#include "myhead.h"

void* thread(void* arg)
{
    pthread_detach(pthread_self());              //设置该线程为分离状态，不需要主线程回收其系统资源。
    char buf[50];
    char ipbuf[50];
    bzero(ipbuf,50);
    bzero(buf,50);
    while(1)                                     //循环接收客户端发来的信息
    {
        usleep(500*1000);
        break;
    }
    return NULL;
}

int main()
{
    pthread_t tid;

    while(1)                   //循环监听有没有客户端连接进来，就像一个接待人员等待客人，有客人来了就服务他
    {
        if(pthread_create(&tid, NULL, thread, NULL) == 0)            //创建一个线程服务新连接进来的客户端
        {
            printf("pthread_create OK! tid = %d.\n",(int )tid);
        }
        else
        {
            printf("pthread_create NOOOOO!\n");
        }
        usleep(200*1000);
    }
    return 0;
}
