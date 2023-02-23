/*
 * @Descripttion: 
 * @version: 
 * @Author: HanJingKang
 * @Date: 2022-11-02 14:07:22
 * @LastEditTime: 2022-12-27 15:12:31
 */

#include <iostream>
#include <string.h>
#include <stdlib.h>
#include <string>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <pthread.h>

#define BUF_SIZE 100
#define MAX_CLNT 256

using namespace std;

void *handle_clnt(void *arg);
void send_msg(char *msg, int len);
void error_handing(string msg);
int clnt_cnt = 0;
int clnt_socks[MAX_CLNT];
pthread_mutex_t mutx;

int main(int argc, char const *argv[])
{
    int serv_sock, clnt_sock;
    sockaddr_in serv_add, clnt_add;
    socklen_t clnt_adr_sz;
    pthread_t t_id;
    if (argc != 2)
    {
        std::cout << "Usage:%s<port>" << argv[0] << std::endl;
        exit(1);
    }

    pthread_mutex_init(&mutx, NULL);
    serv_sock = socket(PF_INET, SOCK_STREAM, 0);

    if (serv_sock == -1)
    {
        error_handing("socket_error!");
    }
    memset(&serv_add, 0, sizeof(serv_add));
    serv_add.sin_addr.s_addr = htonl(INADDR_ANY);
    serv_add.sin_family = AF_INET;
    serv_add.sin_port = htons(atoi(argv[1]));

    if (bind(serv_sock, (sockaddr *)&serv_add, sizeof(serv_add)) == -1)
    {
        error_handing("bind error");
    }
    if (listen(serv_sock, 5) == -1)
    {
        error_handing("listen error");
    }

    while (1)
    {
        clnt_adr_sz = sizeof(clnt_add);
        clnt_sock = accept(serv_sock, (sockaddr *)&clnt_add, &clnt_adr_sz);

        pthread_mutex_lock(&mutx);
        clnt_socks[clnt_cnt++] = clnt_sock;
        pthread_mutex_unlock(&mutx);

        pthread_create(&t_id, NULL, handle_clnt, (void *)&clnt_sock);
        pthread_detach(t_id);
        std::cout << "Connected client IP:" << inet_ntoa(clnt_add.sin_addr) << std::endl;
        
    }

    close(serv_sock);

    return 0;
}

void error_handing(string msg)
{
    std::cerr << msg << std::endl;
    exit(1);
}

void *handle_clnt(void *arg)
{
    int clnt_sock = *((int *)arg);
    int str_len = 0, i;
    char message[BUF_SIZE];

    while ((str_len = read(clnt_sock, message, sizeof(message))) != 0)
    {
        send_msg(message, str_len);
        
    }//客户端结束，退出循环，接下来close客户端fd
    pthread_mutex_lock(&mutx);
    for (size_t i = 0; i < clnt_cnt; i++)
    {
        if (clnt_sock == clnt_socks[i])
        {
            while (i++ < clnt_cnt - 1)
            {
                clnt_socks[i] = clnt_socks[i + 1];
            }
            break;
        }
    }
    clnt_cnt--;
    pthread_mutex_unlock(&mutx);
    close(clnt_sock);
    return NULL;
}
void send_msg(char *msg, int len)

{
    int i;
    pthread_mutex_lock(&mutx);
    for (size_t i = 0; i < clnt_cnt; i++)
    {
        write(clnt_socks[i], msg, len);
        
    }
    pthread_mutex_unlock(&mutx);
}