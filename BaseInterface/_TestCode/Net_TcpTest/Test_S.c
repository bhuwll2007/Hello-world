/*
 * S.c
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
#include <sys/types.h>
#include <sys/socket.h>

#define LOCAL_PORT 6666		//ط˿
#define MAX 5	//

int main()
{
    int sockfd, connfd, fd, is_connected[MAX];
    struct sockaddr_in addr;
    int addr_len = sizeof(struct sockaddr_in);
    char msgbuffer[256];
    char msgsend[] = "Welcome To Demon Server";
    fd_set fds;

    //
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd >= 0)
        printf("open socket: %d\n", sockfd);

    //
    bzero(&addr, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_port = htons(LOCAL_PORT);
    addr.sin_addr.s_addr = htonl(INADDR_ANY);   //INADDR_ANYʾַ0.0.0.0 0.0.0.0

    //
    if (bind(sockfd, (struct sockaddr*) &addr, sizeof(addr)) >= 0)
        printf("bind the port: %d\n", LOCAL_PORT);

    if (listen(sockfd, 3) >= 0)
        printf("begin listenning...\n");

    //
    for (fd = 0; fd < MAX; fd++)
        is_connected[fd] = 0;

    while (1)
    {

        FD_ZERO(&fds);
        FD_SET(sockfd, &fds);

        for (fd = 0; fd < MAX; fd++)
            if (is_connected[fd])
                FD_SET(fd, &fds);

        if (!select(MAX, &fds, NULL, NULL, NULL))
            continue;

        for (fd = 0; fd < MAX; fd++)
        {
            if (FD_ISSET(fd, &fds))
            {
                if (fd == sockfd)
                {
                    connfd = accept(sockfd, (struct sockaddr*) &addr,
                                    &addr_len);
                    write(connfd, msgsend, sizeof(msgsend));
                    is_connected[connfd] = 1;
                    printf("connected from %s\n", inet_ntoa(addr.sin_addr));
                }
                else
                {
                    //
                    if (read(fd, msgbuffer, sizeof(msgbuffer)) > 0)
                    {
                        write(fd, msgbuffer, sizeof(msgbuffer));
                        printf("[read]: %s\n", msgbuffer);
                    }
                    else
                    {
                        is_connected[fd] = 0;
                        close(fd);
                        printf("close connected\n");
                    }
                }
            }
        }
    }
}
