/*
 * C.c
 *
 *  Created on: May 30, 2018
 *      Author: root
 */

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

#define REMOTE_PORT 6666
#define REMOTE_ADDR "127.0.0.1"

int main()
{
	int sockfd;
	struct sockaddr_in addr;
	char msgbuffer[256];

	//
	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (sockfd >= 0)
		printf("open socket: %d\n", sockfd);

	//
	bzero(&addr, sizeof(addr));
	addr.sin_family = AF_INET;
	addr.sin_port = htons(REMOTE_PORT);
	addr.sin_addr.s_addr = inet_addr(REMOTE_ADDR);

	//
	if (connect(sockfd, (struct sockaddr*) &addr, sizeof(addr)) >= 0)
		printf("connect successfully\n");

	//
	recv(sockfd, msgbuffer, sizeof(msgbuffer), 0);
	printf("%s\n", msgbuffer);

	while (1)
	{
		//
		bzero(msgbuffer, sizeof(msgbuffer));
		read(STDIN_FILENO, msgbuffer, sizeof(msgbuffer));
		if (send(sockfd, msgbuffer, sizeof(msgbuffer), 0) < 0)
			perror("ERROR");

		bzero(msgbuffer, sizeof(msgbuffer));
		recv(sockfd, msgbuffer, sizeof(msgbuffer), 0);
		printf("[receive]:%s\n", msgbuffer);

		usleep(500000);
	}
}
