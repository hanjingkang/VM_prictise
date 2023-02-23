/*
 * @Descripttion: 
 * @version: 
 * @Author: HanJingKang
 * @Date: 2022-12-12 21:19:40
 * @LastEditTime: 2022-12-13 14:19:04
 */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <string.h>
//兄弟进程间通信实现ls | wc-l
//for循环创建子进程,父进程回收俩子进程

int main()
{
    int fd[2], i;
    pid_t pid;
    int ret;
    pipe(fd);
    for (i = 0; i < 2; i++)
    {
        pid = fork();
        if (pid == -1)
        {
            perror("fork error\n");
        }
        else if (pid == 0)
        {
            break;
        }
    }
    switch (i)
    {
        //父进程，创建管道后尽量保持一个读端一个写端（理论上 多个端也是允许）
        //因此，为了让子进程使用读端写端，在父进程里关闭两端口，并阻塞等待回收子进程
    case 2:
        close(fd[0]);
        close(fd[1]);
        wait(NULL);
        wait(NULL);
        break;
    case 1: 
        //兄弟1，关闭读端，将标准输出重定向到写端，在执行ls，因此文件结果集会保存到pipe写端
        close(fd[0]);
        dup2(fd[1], STDOUT_FILENO);
        execlp("ls", "ls", NULL);
        break;
    case 0: //兄弟2，关闭写端，将标准输入重定向到读端，在执行wc，因此wc命令会从pipe读端读取文件集
        close(fd[1]);
        dup2(fd[0], STDIN_FILENO);
        execlp("wc", "wc", "-l", NULL);
        break;
    default:
        break;
    }

    return 0;
}
