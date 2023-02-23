/*
 * @Descripttion: 
 * @version: 
 * @Author: HanJingKang
 * @Date: 2022-12-14 23:24:05
 * @LastEditTime: 2022-12-14 23:58:04
 */
#include <signal.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>

void printset(sigset_t *set)
{
    int i;
    for (i = 0; i < 32; i++)
    {
        if (sigismember(set, i))
        {
            putchar('1');
        }
        else
        {
            putchar('0');
        }
    }
    printf("\n");
}

int main(int argc, char const *argv[])
{
    sigset_t set, oldset, pedset;
    int ret = 0;
    sigemptyset(&set);
    sigaddset(&set, SIGINT);
    ret = sigprocmask(SIG_BLOCK, &set, &oldset);
    while (1)
    {
        ret = sigpending(&pedset);
        printset(&pedset);
        sleep(1);
    }

    return 0;
}
