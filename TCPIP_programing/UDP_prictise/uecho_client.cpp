/*
 * @Descripttion: 
 * @version: 
 * @Author: HanJingKang
 * @Date: 2022-11-02 14:07:22
 * @LastEditTime: 2022-11-02 23:58:53
 */

#include <iostream>
#include <string.h>
#include <stdlib.h>
#include <string>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#define BUF_SIZE 30
using namespace std;
void error_handing(string msg);
int main(int argc, char const *argv[])
{
    int sock;
    char message[BUF_SIZE];
    int str_len;
    sockaddr_in serv_add, from_add;
    socklen_t adr_sz;

    if (argc != 3)
    {
        std::cout << "Usage:%s<IP><port>" << argv[0] << std::endl;
        exit(1);
        
    }

    sock = socket(PF_INET, SOCK_DGRAM, 0);

    //创建已连接UDP套接字,注册目标IP与端口
    //connect(sock,(sockaddr*)&serv_add,sizeof(serv_add));

    if (sock == -1)
    {
        error_handing("UDP socket creat error!");
    }
    memset(&serv_add, 0, sizeof(serv_add));
    serv_add.sin_addr.s_addr = inet_addr(argv[1]);
    serv_add.sin_family = AF_INET;
    serv_add.sin_port = htons(atoi(argv[2]));


     while (1)
    {
        fputs("input message(Q to quit):",stdout);
        fgets(message,BUF_SIZE,stdin);
        if (!strcmp(message,"q\n")||!strcmp(message,"Q\n"))
        {
            
            break;
        }
        sendto(sock,message,strlen(message),0,(sockaddr*)&serv_add,sizeof(serv_add));
        adr_sz=sizeof(from_add);
        //在UDP套接字已连接时，可直接通过read,write进行通信
        //write(sock,message,strlen(message));
        //str_len=read(sock,message,sizeof(message)-1);
        str_len=recvfrom(sock,message,BUF_SIZE,0,(sockaddr*)&from_add,&adr_sz);
        message[str_len]=0;
        std::cout << "Message from server:" <<message<< std::endl; 
    }
    close(sock);

    return 0;
}

void error_handing(string msg)
{
    std::cerr << msg << std::endl;
    exit(1);
}