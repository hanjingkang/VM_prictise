/*
 * @Descripttion: 
 * @version: 
 * @Author: HanJingKang
 * @Date: 2022-12-14 23:24:05
 * @LastEditTime: 2022-12-15 00:07:47
 */
#include <signal.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>

void sigcatch(int signo)
{
    printf("catch sig:%d\n",signo);
}

int main(int argc, char const *argv[])
{
    while (1)
    {
        signal(SIGINT,sigcatch);
    }
    

    return 0;
}
