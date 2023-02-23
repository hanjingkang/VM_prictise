/*
 * @Descripttion: 
 * @version: 
 * @Author: HanJingKang
 * @Date: 2022-11-02 14:07:22
 * @LastEditTime: 2022-11-06 21:33:24
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
#define BUF_SIZE 1024
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
    int str_len, fd_num, i, fd_max;
    sockaddr_in serv_add, clnt_add;
    socklen_t clnt_adr_sz;
    timeval timeout;
    fd_set reads, cpy_reads;

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

    FD_ZERO(&reads);
    FD_SET(serv_sock, &reads);
    fd_max = serv_sock;

    while (1)
    {
        cpy_reads = reads;
        timeout.tv_sec = 5;
        timeout.tv_usec = 5000;

        if ((fd_num = select(fd_max + 1, &cpy_reads, 0, 0, &timeout)) == -1)
        {
            break;
        }
        if (fd_num == 0)
        {
            continue;
        }

        for (i = 0; i < fd_max + 1; i++)
        {
            if (FD_ISSET(i, &cpy_reads))
            {
                if (i == serv_sock)
                {
                    clnt_adr_sz = sizeof(clnt_add);
                    clnt_sock = accept(serv_sock, (sockaddr *)&clnt_add, &clnt_adr_sz);
                    FD_SET(clnt_sock, &reads);
                    if (fd_max < clnt_sock)
                    {
                        fd_max = clnt_sock;
                    }
                    std::cout << "connected client:" << clnt_sock << std::endl;
                }
                else
                {
                    str_len = read(i, message, BUF_SIZE);
                    if (str_len == 0)
                    {
                        FD_CLR(i, &reads);
                        close(i);
                        std::cout << "closed client:" << i << std::endl;
                    }
                    else
                    {
                        write(i, message, str_len);
                    }
                }
            }
        }
    }
    close(serv_sock);
    return 0;
}
