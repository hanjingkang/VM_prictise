/*
 * @Descripttion: 
 * @version: 
 * @Author: HanJingKang
 * @Date: 2022-12-12 15:40:25
 * @LastEditTime: 2022-12-12 15:43:29
 */
#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
//利用wait回收子进程,防止变成孤儿或僵尸

int main(){
	int i;
	pid_t pid;
	pid_t wpid;
	int status; 	
	pid=fork();
	if(pid==0){
		printf("iam child:%d ,parent is %d\n",getpid(),getppid());
		sleep(1);
		return 55;
	}
	else if (pid>0){
		wpid=wait(&status); //阻塞回收子进程，返回子进程pid
		if(wpid==-1){
			perror("wait error");
		}
		if(WIFEXITED(status)){//利用宏函数判断子进程终止原因，查看man手册
			printf("child exit with %d\n",WEXITSTATUS(status));
			}
			
		printf("finish child:%d\n",wpid);
			
		

	}

	return 0;

}
