#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>

int main(){
	int i;
	pid_t pid;

	pid=fork();
	if(pid==-1){
		perror("fork error");
	}
	else if(pid==0){

		//execlp("ls","ls","-l","-d",NULL);
		execl("./fork","./fork",NULL);
	}
	else  if(pid>0){
		sleep(7);
		printf("i am parent :%d\n",getpid());
	}



	return 0;

}
