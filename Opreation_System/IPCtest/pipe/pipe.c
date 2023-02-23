/*
 * @Descripttion: 
 * @version: 
 * @Author: HanJingKang
 * @Date: 2022-12-12 21:19:40
 * @LastEditTime: 2022-12-12 21:34:58
 */
#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <string.h>
//进程间管道通信

int main(){
	int fd[2];
    pid_t  pid;
    int ret;
    char *msg="helloworld";
    char buf[1024];
    pipe(fd);

    pid=fork();
    if (pid==0)
    {

        write(fd[1],msg,strlen(msg));
        printf("write %s in child,lenth:%d\n",msg,strlen(msg));

        
    }
    else{

        ret=read(fd[0],buf,sizeof(buf));
        printf("read %s in parent,lenth:%d\n",buf,ret);
    }
    

    return 0;
}
