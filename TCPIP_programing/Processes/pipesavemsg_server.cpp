/*
 * @Descripttion: 
 * @version: 
 * @Author: HanJingKang
 * @Date: 2022-11-02 14:07:22
 * @LastEditTime: 2022-11-04 21:48:39
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
#define BUF_SIZE 1024
using namespace std;

void error_handing(string msg)
{
    std::cerr << msg << std::endl;
    exit(1);
}

void read_childProc(int sig)
{
    int status;
    pid_t id = waitpid(-1, &status, WNOHANG);
    if (WIFEXITED(status))
    {
        std::cout << "remove proc:" << id << std::endl;

        std::cout << "child sent:" << WEXITSTATUS(status) << std::endl;
    }
}

int main(int argc, char const *argv[])
{
    int serv_sock, clnt_sock;
    int fds[2];
    char message[BUF_SIZE];
    int str_len, state;
    sockaddr_in serv_add, clnt_add;
    socklen_t clnt_adr_sz;

    memset(&serv_add, 0, sizeof(serv_add));
    serv_add.sin_addr.s_addr = htonl(INADDR_ANY);
    serv_add.sin_family = AF_INET;
    serv_add.sin_port = htons(atoi(argv[1]));
    serv_sock = socket(PF_INET, SOCK_STREAM, 0);

    pid_t pid;
    struct sigaction act;
    act.sa_handler = read_childProc;
    sigemptyset(&act.sa_mask);
    act.sa_flags = 0;
    state = sigaction(SIGCHLD, &act, 0);

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


    pipe(fds);
    pid=fork();
    if (pid==0)
    {
        FILE*fp=fopen("echomsg.txt","wt");
        char msgbuf[BUF_SIZE];
        int i,len;
        for (size_t i = 0; i < 10; i++)
        {
            len=read(fds[0],msgbuf,BUF_SIZE);
            std::cout << "write:" <<msgbuf<< std::endl;
            fwrite((void*)msgbuf,1,len,fp);
            
        }
        fclose(fp);
        return 0;
        
    }
    
    while (1)
    {
        clnt_adr_sz = sizeof(clnt_add);
        clnt_sock = accept(serv_sock, (sockaddr *)&clnt_add, &clnt_adr_sz);
        if (clnt_sock == -1)
        {
            continue;
        }
        else
        {
            std::cout << "new client connected" << std::endl;
        }

        pid = fork();
        if (pid == 0)
        {

            close(serv_sock);
            while ((str_len = read(clnt_sock, message, BUF_SIZE)) != 0)
            {
                std::cout << "receive:" << message << std::endl;
                write(clnt_sock, message, str_len);
                write(fds[1],message,str_len);
            }
            close(clnt_sock);
            std::cout << "client disconnected" << std::endl;

            return 0;
        }
        else
        {
            close(clnt_sock);
        }
    }
    close(serv_sock);
    return 0;
}
