//TCP服务端
#include"myhead.h"

struct client *head = NULL;
struct sockaddr_in saddr;
struct sockaddr_in caddr;

struct client
{
	int sock;
	struct client *next;
};

struct client *init_list()               //创建客户端队列头，为了让客户端“群聊”
{
	struct client *head = malloc(sizeof(struct client));
	head->next = NULL;
	return head;
}

int add_node(struct client*head, int new_sockfd)
{
	struct client *new = malloc(sizeof(struct client));
	struct client *p = head;

	new->sock = new_sockfd;
	new->next = NULL;

	while (p->next != NULL)
	{
		p = p->next;
	}
	p->next = new;
	new->next = NULL;
	return 0;
}

int del_node(struct client*head, int sockfd)
{
	struct client *q = head;
	struct client *p = q->next;

	while (p->sock != sockfd)
	{
		if (p->next != NULL)
		{
			p = p->next;
			q = q->next;
		}
		else if (p->next == NULL)    //cannot find the sockfd
		{
			printf("can not find sockfd to del\n");
			return -1;
		}
	}
	q->next = p->next;
	free(p);
	return 0;
}

int broadcast_client(struct client*head, int sockfd, char *buf)
{
	struct client *p = head->next;
	while (p != NULL)
	{
		if (p->sock != sockfd)
		{
			write(p->sock, buf, 50);
		}
		else if (p->sock == sockfd)
		{
			p = p->next;
			continue;
		}
		p = p->next;
	}
	return 0;
}

void* thread(void* arg)
{
	pthread_detach(pthread_self());              //设置该线程为分离状态，不需要主线程回收其系统资源。
	char buf[50];
	char ipbuf[50];
	bzero(ipbuf, 50);
	bzero(buf, 50);
	int port;
	int sockfd = *((int*) arg);
	while (1)                                     //循环接收客户端发来的信息
	{
		read(sockfd, buf, 50);
		if (strcmp(buf, "quit") == 0)            //当客户端发来的信息为 quit，则退出线程，并删除该结点
		{
			del_node(head, sockfd);
			pthread_exit(NULL);
		}
		inet_ntop(AF_INET, (void*) &caddr.sin_addr.s_addr, ipbuf, 50);//把客户端的ip信息放到ipbuf中
		port = ntohs(caddr.sin_port);                        //把客户端的端口号放到变量port中
		printf("read from ip:%s,port:%hu\n", ipbuf, port);
		printf("message:%s\n", buf);
		broadcast_client(head, sockfd, buf);               //把一个客户端发来的信息转发给其他客户端
		bzero(buf, 50);
	}
}

int main()
{

	int sockfd, new_sockfd;
	socklen_t size;
	int ret;

	pthread_t tid;

	size = sizeof(struct sockaddr_in);
	head = init_list();

	bzero(&saddr, size);
	saddr.sin_family = AF_INET;                 //绑定协议域为 IPv4
	saddr.sin_port = htons(7777);               //绑定端口号为7777
	saddr.sin_addr.s_addr = htonl(INADDR_ANY);  //绑定ip地址为本机的一个随机IP

	sockfd = socket(AF_INET, SOCK_STREAM, 0);     //创建socket套接字
	ret = bind(sockfd, (struct sockaddr*) &saddr, size); //socket套接字和sockaddr_in结构体绑定在一起，赋予socket属性
	listen(sockfd, 5);                               //开始监听有没有客户端连接
	if (ret == 0)
		;
	while (1)                   //循环监听有没有客户端连接进来，就像一个接待人员等待客人，有客人来了就服务他
	{
		new_sockfd = accept(sockfd, (struct sockaddr*) &caddr, &size); //等待客户端连接，并返回该客户端的描述符
		if (new_sockfd > 0)
		{
			add_node(head, new_sockfd);                //若有客户端连接，则把该客户端加入到客户端队列中

			pthread_create(&tid, NULL, thread, (void*) &new_sockfd); //创建一个线程服务新连接进来的客户端
		}
	}
	pthread_exit(NULL);
	return 0;
}
