/*
 * @Descripttion: 
 * @version: 
 * @Author: HanJingKang
 * @Date: 2022-12-12 15:40:25
 * @LastEditTime: 2022-12-12 17:09:48
 */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
//利用waitpid循环回收多个子进程

int main()
{
    int i;
    pid_t pid,wpid;

    for (i = 0; i < 5; i++)
    {
        pid=fork();
        if (pid==0)
        {
            sleep(i);
            printf("iam %dth child id=%d\n", i + 1,getpid());
            break;
        }
        
    }

    if (i == 5)
    {
        //sleep(5);
        printf("iam parent\n");
        while ((wpid=waitpid(-1,NULL,0))!=-1)//(wpid=waitpid(-1,NULL,WNOHANG))!=-1为非阻塞方式
        {
            if(wpid>0){
                printf("finish child %d\n",wpid);
            }
            else if(wpid==0){
                sleep(1);
                continue;
            }
        }
        
    }
    return 0;
}