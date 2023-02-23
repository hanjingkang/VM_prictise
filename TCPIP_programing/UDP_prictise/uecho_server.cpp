/*
 * @Descripttion: 
 * @version: 
 * @Author: HanJingKang
 * @Date: 2022-11-02 14:07:22
 * @LastEditTime: 2022-11-02 23:30:04
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
    int serv_sock;
    char message[BUF_SIZE];
    int str_len;
    sockaddr_in serv_add, clnt_add;
    socklen_t clnt_adr_sz;
    if (argc != 2)
    {
        std::cout << "Usage:%s<port>" << argv[0] << std::endl;
        exit(1);
        
    }
    serv_sock = socket(PF_INET, SOCK_DGRAM, 0);
    if (serv_sock == -1)
    {
        error_handing("UDP socket creat error!");
    }
    memset(&serv_add, 0, sizeof(serv_add));
    serv_add.sin_addr.s_addr = htonl(INADDR_ANY);
    serv_add.sin_family = AF_INET;
    serv_add.sin_port = htons(atoi(argv[1]));
    if (bind(serv_sock, (sockaddr *)&serv_add, sizeof(serv_add)) == -1)
    {
        error_handing("bind error");
    }
    while (1)
    {
        
        clnt_adr_sz=sizeof(clnt_add);
        str_len=recvfrom(serv_sock,message,BUF_SIZE,0,(sockaddr*)&clnt_add,&clnt_adr_sz);
        std::cout << "recieve:" <<message<<std::endl;
        if (!strcmp(message,"close\n"))
        {
            std::cout << "server closed" << std::endl;
            close(serv_sock);
        }
        sendto(serv_sock,message,str_len,0,(sockaddr*)&clnt_add,clnt_adr_sz);
    }
    return 0;
}


void error_handing(string msg)
{
    std::cerr << msg << std::endl;
    exit(1);
}

