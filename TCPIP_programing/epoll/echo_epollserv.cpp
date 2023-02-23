/*
 * @Descripttion: 
 * @version: 
 * @Author: HanJingKang
 * @Date: 2022-11-02 14:07:22
 * @LastEditTime: 2022-11-14 20:25:00
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

#define EPOLL_SIZE 50
#define BUF_SIZE 100
using namespace std;

void error_handing(string msg)
{
    std::cerr << msg << std::endl;
    exit(1);
}

int main(int argc, char const *argv[])
{
    int serv_sock, clnt_sock;
    char message[BUF_SIZE];
    int str_len, i;
    sockaddr_in serv_add, clnt_add;
    socklen_t clnt_adr_sz;
    //ep_event用于保存发生事件的文件描述符集合
    epoll_event ep_event[EPOLL_SIZE], event;
    int epfd, event_cnt;

    memset(&serv_add, 0, sizeof(serv_add));
    serv_add.sin_addr.s_addr = htonl(INADDR_ANY);
    serv_add.sin_family = AF_INET;
    serv_add.sin_port = htons(atoi(argv[1]));
    serv_sock = socket(PF_INET, SOCK_STREAM, 0);

    if (argc != 2)
    {
        std::cout << "Usage:%s<port>" << argv[0] << std::endl;
        exit(1);
    }

    if (serv_sock == -1)
    {
        error_handing("socket_error!");
    }

    if (bind(serv_sock, (sockaddr *)&serv_add, sizeof(serv_add)) == -1)
    {
        error_handing("bind error");
    }
    if (listen(serv_sock, 5) == -1)
    {
        error_handing("listen error");
    }
    //开辟空间保存epoll例程
    epfd = epoll_create(EPOLL_SIZE);
    //监视事件类型为接受数据，监视对象为serv_sock
    event.events = EPOLLIN;
    event.data.fd = serv_sock;
    //将serv_sock描述符注册到epoll例程epfd中
    epoll_ctl(epfd, EPOLL_CTL_ADD, serv_sock, &event);

    while (1)
    {
        //通过等待监听，将epfd范围内发生事件的文件描述符保存在地址ep_event处,-1表示一直等待
        //返回发生时间的描述符 个数
        event_cnt = epoll_wait(epfd, ep_event, EPOLL_SIZE, -1);

        if (event_cnt == -1)
        {
            std::cout << "epoll_wait() error" << std::endl;
            break;
        }

        for (i = 0; i < event_cnt; i++)
        {
            //如果服务端套接字发生变化，则受理新的连接请求，将新的客户端fd注册进epoll例程中
            if (ep_event[i].data.fd == serv_sock)
            {
                clnt_adr_sz=sizeof(clnt_add);
                clnt_sock=accept(serv_sock,(sockaddr*)&clnt_add,&clnt_adr_sz);
                event.events=EPOLLIN;
                event.data.fd=clnt_sock;
                epoll_ctl(epfd,EPOLL_CTL_ADD,clnt_sock,&event);
                std::cout << "connected client:" << clnt_sock << std::endl;
            }
            else
            {
                str_len = read(ep_event[i].data.fd, message, BUF_SIZE);
                if (str_len == 0)
                {
                    epoll_ctl(epfd,EPOLL_CTL_DEL,ep_event[i].data.fd,NULL);
                    close(ep_event[i].data.fd);
                    std::cout << "closed client:" << ep_event[i].data.fd << std::endl;
                }
                else
                {
                    write(ep_event[i].data.fd, message, str_len);
                }
            }
        }
    }
    close(serv_sock);
    close(epfd);
    return 0;
}
