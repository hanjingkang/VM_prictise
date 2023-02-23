/*
 * @Descripttion: 
 * @version: 
 * @Author: HanJingKang
 * @Date: 2022-11-02 14:07:22
 * @LastEditTime: 2022-11-04 21:46:48
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
void read_routine(int sock, char *message);
void write_routine(int sock, char *message);

int main(int argc, char const *argv[])
{
    int sock;
    char message[BUF_SIZE];
    sockaddr_in serv_add;
    pid_t pid;

    if (argc != 3)
    {
        std::cout << "Usage:%s<ip><port>" << argv[0] << std::endl;
        exit(1);
    }

    sock = socket(PF_INET, SOCK_STREAM, 0);
    if (sock == -1)
    {
        error_handing("socket_error!");
    }
    memset(&serv_add, 0, sizeof(serv_add));
    serv_add.sin_addr.s_addr = inet_addr(argv[1]);
    serv_add.sin_family = AF_INET;
    serv_add.sin_port = htons(atoi(argv[2]));

    if (connect(sock, (sockaddr *)&serv_add, sizeof(serv_add)) == -1)
    {

        error_handing("connect error");
    }
    else
    {
        std::cout << "connected!" << std::endl;
    }

    pid = fork();
    if (pid == 0)
    {
        write_routine(sock, message);
        
    }
    else
    {
        read_routine(sock, message);
        close(sock);
        return 0;
    }
}

void read_routine(int sock, char *message)
{
    while (1)
    {
        int str_len = read(sock, message, BUF_SIZE);
        
        if (str_len == 0)
        {
            
            return ;
        }
        message[str_len] = 0;
        std::cout << "Message from server:" << message << std::endl;
    }
}

void write_routine(int sock, char *message)
{
    while (1)
    {
        fputs("input message(Q to quit):", stdout);
        fgets(message, BUF_SIZE, stdin);
        std::cout << "message:" << message << std::endl;
        if (!strcmp(message, "q\n") || !strcmp(message, "Q\n"))
        {
            shutdown(sock, SHUT_WR);
            return;
        }
        write(sock, message, strlen(message));
    }
}

void error_handing(string msg)
{
    std::cerr << msg << std::endl;
    exit(1);
}