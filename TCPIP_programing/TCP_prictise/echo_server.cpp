/*
 * @Descripttion: 
 * @version: 
 * @Author: HanJingKang
 * @Date: 2022-11-02 14:07:22
 * @LastEditTime: 2022-12-25 16:42:06
 */

#include <iostream>
#include <string.h>
#include <stdlib.h>
#include <string>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#define BUF_SIZE 1024
using namespace std;
void error_handing(string msg);
int main(int argc, char const *argv[])
{
    int serv_sock, clnt_sock;
    char message[BUF_SIZE];
    int str_len, i;
    sockaddr_in serv_add, clnt_add;
    socklen_t clnt_adr_sz;

    if (argc != 2)
    {
        std::cout << "Usage:%s<port>" << argv[0] << std::endl;
        exit(1);
        
    }

    serv_sock = socket(PF_INET, SOCK_STREAM, 0);
    if (serv_sock == -1)
    {
        error_handing("socket_error!");
    }
    memset(&serv_add, 0, sizeof(serv_add));
    serv_add.sin_addr.s_addr = htonl(INADDR_ANY);
    serv_add.sin_family = AF_INET;
    serv_add.sin_port = htons(atoi(argv[1]));//string port-->int port-->网络字节序

    if (bind(serv_sock, (sockaddr *)&serv_add, sizeof(serv_add)) == -1)
    {
        error_handing("bind error");
    }
    if (listen(serv_sock, 5) == -1)
    {
        error_handing("listen error");
    }
    clnt_adr_sz = sizeof(clnt_add);
    for (i = 0; i < 5; i++)
    {
        
        clnt_sock = accept(serv_sock, (sockaddr *)&clnt_add, &clnt_adr_sz);
        if (clnt_sock == -1)
        {
            error_handing("accpet error");
            
        }
        else
        {
            std::cout << "connect client" << i + 1 << std::endl;
        }

        while ((str_len = read(clnt_sock, message, BUF_SIZE)) != 0)
        {
            std::cout << "receive:" << message << std::endl;
            write(clnt_sock, message, str_len);
        }
        close(clnt_sock);
        std::cout << "clnt" << i + 1 << " sock closed" << std::endl;
    }

    close(serv_sock);

    return 0;
}

void error_handing(string msg)
{
    std::cerr << msg << std::endl;
    exit(1);
}