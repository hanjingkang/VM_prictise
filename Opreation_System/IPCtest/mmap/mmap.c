/*
 * @Descripttion: 
 * @version: 
 * @Author: HanJingKang
 * @Date: 2022-12-12 21:19:40
 * @LastEditTime: 2022-12-14 23:23:23
 */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/mman.h>
#include <string.h>
//兄弟进程间通信实现ls | wc-l
//for循环创建子进程,父进程回收俩子进程

int main()
{
    char *p;
    pid_t pid;
    int ret;
    int fd;
    fd = open("txtOfmmap", O_RDWR | O_CREAT | O_TRUNC, 0644);
    if (fd == -1)
    {
        perror("open file fail\n");
    }
    ftruncate(fd, 20);
    p = (char *)mmap(NULL, 20, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    //p = (int *)mmap(NULL, 4, PROT_READ | PROT_WRITE, MAP_PRIVATE, fd, 0);
    if (p == MAP_FAILED)
    {
        perror("mmap error\n");
        exit(1);
    }
    close(fd);
    pid = fork();
    if (pid == -1)
    {
        perror("fork error\n");
    }
    if (pid == 0)
    {
        strcpy(p,"hello");
        printf("after child write: p=%s\n", p);
    }
    else if (pid > 0)
    {
        sleep(1);
        printf("read p=%s in parent\n",p);
        wait(NULL);
    }
    return 0;
}
