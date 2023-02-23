/*
 * @Descripttion: 
 * @version: 
 * @Author: HanJingKang
 * @Date: 2023-01-07 22:10:25
 * @LastEditTime: 2023-01-07 22:15:47
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
#define CLIE_ADDR "clie.socket"

int main(int argc, char const *argv[])
{
    int cfd, len;
    sockaddr_un servaddr, cliaddr;
    char buf[4096];

    cfd = socket(AF_UNIX, SOCK_STREAM, 0);

    bzero(&cliaddr, sizeof(cliaddr));
    cliaddr.sun_family = AF_UNIX;
    strcpy(cliaddr.sun_path, CLIE_ADDR);

    len = offsetof(sockaddr_un, sun_path) + strlen(cliaddr.sun_path);

    unlink(CLIE_ADDR);
    bind(cfd, (sockaddr *)&cliaddr, len);

    /////////////////////////
    bzero(&servaddr, sizeof(servaddr));
    servaddr.sun_family = AF_UNIX;
    strcpy(servaddr.sun_path, SERV_ADDR);

    len = offsetof(sockaddr_un, sun_path) + strlen(servaddr.sun_path);
    

    connect(cfd,(sockaddr*)&cliaddr,len);

    



    while (fgets(buf,sizeof(buf),stdin)!=NULL)
    {
        write(cfd,buf,strlen(buf));
        len=read(cfd,buf,sizeof(buf));;
        write(STDOUT_FILENO,buf,len);
       
    }
    close(cfd);
    return 0;
}
