#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/socket.h>

int REMOTE_PORT = 6666;                //服务器端口
char REMOTE_ADDR[100]  ;       //服务器地址

int main(int argc, char *argv[])
{
	int sockfd;
	int i = 0;
	struct sockaddr_in addr;
	char msgbuffer[256];
	//初始化
	memset (REMOTE_ADDR, 0x00, sizeof(REMOTE_ADDR)) ;
	REMOTE_PORT = 0;
	//IP地址和端口号赋值
	memcpy(REMOTE_ADDR, argv[1], strlen(argv[1]));
	REMOTE_PORT = atoi(argv[2]);

	//创建套接字
	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (sockfd >= 0)
		printf("open socket: %d\n", sockfd);

	//将服务器的地址和端口存储于套接字结构体中
	bzero(&addr, sizeof(addr));
	addr.sin_family = AF_INET;
	addr.sin_port = htons(REMOTE_PORT);
	addr.sin_addr.s_addr = inet_addr(REMOTE_ADDR);

	//向服务器发送请求34
	if (connect(sockfd, (struct sockaddr*) &addr, sizeof(addr)) >= 0)
		printf("connect successfully\n");

	memset (msgbuffer, 0x00, sizeof(msgbuffer)) ;
	//接收服务器返回的消息(注意这里程序会被阻塞,也就是说只有服务器回复信息,才会继续往下执行)
	recv(sockfd, msgbuffer, sizeof(msgbuffer), 0);
	for(i = 0;i<sizeof(msgbuffer);i++)
		printf("%02X ", msgbuffer[i]);
	printf(" \n");

	printf("%s\n", msgbuffer);

	while (1)
	{
		//将键盘输入的消息发送给服务器,并且从服务器中取得回复消息
		bzero(msgbuffer, sizeof(msgbuffer));
//		read(STDIN_FILENO, msgbuffer, sizeof(msgbuffer));
//		if (send(sockfd, msgbuffer, sizeof(msgbuffer), 0) < 0)
//			perror("ERROR");

		bzero(msgbuffer, sizeof(msgbuffer));
		recv(sockfd, msgbuffer, sizeof(msgbuffer), 0);
		printf("[receive]:%s\n", msgbuffer);

		for(i = 0;i<sizeof(msgbuffer);i++)
			printf("%02X ", msgbuffer[i]);
		printf(" \n");

		usleep(500000);
	}
}
