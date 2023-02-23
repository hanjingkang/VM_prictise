/*
 * @Descripttion: 
 * @version: 
 * @Author: HanJingKang
 * @Date: 2022-10-28 20:42:30
 * @LastEditTime: 2022-11-04 21:27:27
 */
#include<iostream>
#include<sys/socket.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include <netdb.h>

using namespace std;

int main(int argc, char const *argv[])
{
    int sock;
    /* cout<<hex<<htonl(0x12345678)<<endl;
    cout<<hex<<inet_addr("1.2.3.4")<<endl;
    sockaddr_in ad1;
    ad1.sin_addr.s_addr=htonl(0x1020304);
    cout<<hex<<inet_ntoa(ad1.sin_addr)<<endl; */

/*     hostent *host;
    host=gethostbyname(argv[1]);
    std::cout << "official name:" <<host->h_name<< std::endl;
    std::cout << "address type:" <<host->h_addrtype<< std::endl;
    for (size_t i = 0; host->h_aliases[i]; i++)
    {
        std::cout << "h_aliases name:" <<host->h_aliases[i]<< std::endl;
    }
    for (size_t i = 0; host->h_addr_list[i]; i++)
    {
        std::cout << "h_addr_list mem:" <<inet_ntoa(*((in_addr*)host->h_addr_list[i]))<< std::endl;
    } */

    cin>>sock;
    std::cout << "input:" <<sock<< std::endl;
    
    return 0;
}
