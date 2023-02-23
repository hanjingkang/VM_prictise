#include<stdio.h>
#include<dirent.h>
#include<sys/stat.h>
#include<fcntl.h>

int main(int argc ,char* argv[]){
	int fd1=open(argv[1],O_RDWR);
	printf("fd1=%d\n",fd1);
	int newfd=fcntl(fd1,F_DUPFD,0);
	printf("newfd=%d\n",newfd);

	int newfd2=fcntl(fd1,F_DUPFD,7);
	printf("newfd2=%d\n",newfd2);


	return 0;
}
