/*
 * @Descripttion: 
 * @version: 
 * @Author: HanJingKang
 * @Date: 2022-12-15 00:09:09
 * @LastEditTime: 2022-12-15 00:34:07
 */
#include <signal.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>

void sigcatch(int signo)
{
    printf("catch sig:%d\n", signo);
}

int main(int argc, char const *argv[])
{
    struct sigaction act, oldact;

    act.sa_handler = sigcatch;//设置回调函数
    sigemptyset(&act.sa_mask);//清空屏蔽信号集
    act.sa_flags = 0;
    while (1)
    {
        int ret = sigaction(SIGINT, &act, &oldact);//act installed ,previous config saved in oldset
        ret = sigaction(SIGQUIT, &act, &oldact);
    }

    return 0;
}
