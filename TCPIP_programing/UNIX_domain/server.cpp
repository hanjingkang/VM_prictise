/*
 * @Descripttion: 
 * @version: 
 * @Author: HanJingKang
 * @Date: 2023-01-07 21:52:18
 * @LastEditTime: 2023-01-07 22:09:43
 */
#include <iostream>
#include <string.h>
#include <stdlib.h>
#include <string>
#include <unistd.h>
#include <sys/wait.h>
#include <arpa/inet.h>
#include <signal.h>
#include <sys/socket.h>
#include <sys/select.h>
#include <sys/epoll.h>
#include <time.h>
#include <fcntl.h>
#include <sys/un.h>
#include <errno.h>

#define SERV_ADDR "serv.socket"

int main(int argc, char const *argv[])
{
    int lfd, cfd, len, size, i;
    sockaddr_un servaddr, cliaddr;
    char buf[4096];

    lfd = socket(AF_UNIX, SOCK_STREAM, 0);

    bzero(&servaddr, sizeof(servaddr));
    servaddr.sun_family = AF_UNIX;
    strcpy(servaddr.sun_path, SERV_ADDR);

    len = offsetof(sockaddr_un, sun_path) + strlen(servaddr.sun_path);

    unlink(SERV_ADDR);
    bind(lfd, (sockaddr *)&servaddr, len);
    listen(lfd, 20);
    printf("Accept...\n");

    while (1)
    {
        len = sizeof(cliaddr);
        cfd = accept(lfd, (sockaddr *)&cliaddr, (socklen_t *)len);

        len -= offsetof(sockaddr_un, sun_path);
        cliaddr.sun_path[len] = '\0';

        printf("client bind filename %s\n", cliaddr.sun_path);

        while ((size = read(cfd, buf, sizeof(buf))) > 0)
        {
            for (i = 0; i < size; i++)
            {
                buf[i] = toupper(buf[i]);
                write(cfd, buf, size);
            }
        }
        close(cfd);
    }
    close(lfd);
    return 0;
}
