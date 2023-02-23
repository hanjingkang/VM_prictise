#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
//循环创建子进程

int main(){
	int i;
	pid_t pid;

	for(i=0;i<5;i++){
		if(fork()==0){
		break;
		}
	}

	if(i==5){
		sleep(5);
		printf("iam parent\n");
	}
	else{
		sleep(i);
		printf("iam %dth child\n",i+1);

	}

	return 0;

}

//使用gdb调试父子 进程
//在fork()执行后设置set follow-fork-mode child/parent 指定跟踪父子进程
