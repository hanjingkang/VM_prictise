/*
 * @Descripttion: 
 * @version: 
 * @Author: HanJingKang
 * @Date: 2022-11-02 14:07:22
 * @LastEditTime: 2022-11-14 15:54:08
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

    FILE *readfp, *writefp;

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
    serv_add.sin_port = htons(atoi(argv[1]));

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
        //调用基于字符串的gets(),fputs()函数，可以通过额外提供的缓存大大提高数据传输速率
        //fllush(stdout)用于清空缓存区向输出设备传输数据。
        readfp=fdopen(clnt_sock,"r");
        writefp=fdopen(clnt_sock,"w");

        while (!feof(readfp))
        {
            fgets(message,BUF_SIZE,readfp);
            std::cout << "receive:" << message << std::endl;
            fputs(message,writefp);
            fflush(writefp);
        }
        fclose(readfp);
        fclose(writefp);
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