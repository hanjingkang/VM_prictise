/*
 * @Descripttion: 
 * @version: 
 * @Author: HanJingKang
 * @Date: 2022-11-02 14:07:22
 * @LastEditTime: 2022-11-15 16:40:50
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
#define NAME_SIZE 20

using namespace std;

void *recv_msg(void *arg);
void *send_msg(void *arg);
void error_handing(string msg);

char name[NAME_SIZE] = "[DEFAULT]";
char msg[BUF_SIZE];

int main(int argc, char const *argv[])
{
    int sock;
    sockaddr_in serv_add, clnt_add;

    pthread_t snd_t, rcv_t;
    void *thread_return;
    if (argc != 4)
    {
        std::cout << "Usage:%s<IP><port><name>" << argv[0] << std::endl;
        exit(1);
    }

    sprintf(name, "[%s]", argv[3]);
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

    pthread_create(&snd_t, NULL, send_msg, (void *)&sock);
    pthread_create(&rcv_t, NULL, recv_msg, (void *)&sock);
    pthread_join(snd_t,&thread_return);
    pthread_join(rcv_t,&thread_return);
    close(sock);
    return 0;
}

void error_handing(string msg)
{
    std::cerr << msg << std::endl;
    exit(1);
}

void *recv_msg(void *arg)
{
    int sock=*((int*)arg);
    char name_msg[NAME_SIZE+BUF_SIZE];
    int str_len;
    while (1)
    {
        str_len=read(sock,name_msg,NAME_SIZE+BUF_SIZE-1);
        
        if (str_len==-1)
        {
            return (void*)-1;
            
        }
        name_msg[str_len]=0;
        fputs(name_msg,stdout);
        

    }
    return NULL;
    
}
void *send_msg(void *arg)
{
    int sock=*((int*)arg);
    char name_msg[NAME_SIZE+BUF_SIZE];
    while (1)
    {
        fgets(msg,BUF_SIZE,stdin);
         if (!strcmp(msg,"q\n")||!strcmp(msg,"Q\n"))
        {
            close(sock);    
            break;
        }
        sprintf(name_msg,"%s %s",name,msg);
        write(sock,name_msg,strlen(name_msg));

    }
    return NULL;
    
}