//通过目录操作函数实现递归指定遍历目录

#include<stdio.h>
#include<dirent.h>
#include<sys/stat.h>



void Isfile(char* name);

void read(char *dir){

	char path[256];
	DIR* dp;
	struct dirent *sdp;
	dp=opendir(dir);
	if(dp==NULL)
	{
		perror("opendir error");
		return;
	}
	while((sdp=readdir(dp))!=NULL){
		if(strcmp(sdp->d_name,".")==0 ||strcmp(sdp->d_name,"..")==0){
			continue;
		}
		sprintf(path,"%s/%s",dir,sdp->d_name);
		Isfile(path);
		
		}
	closedir(dp);
	



}



void Isfile(char* name)
{
	int ret=0;
	struct stat info;

	ret=stat(name,&info);
	if(ret==-1)
	{
		perror("stat error");
		return ;
	}


	if(S_ISDIR(info.st_mode))
	{
		read(name);

	}
	else{
		printf("size:%ld %5s\n",info.st_size,name);
	}
	return;
	


}







int main(int argc,char *argv[]){
	if(argc==1)
	{
		Isfile(".");
		exit(1);
	}
	if(argc>1)
	{
		Isfile(argv[1]);
		exit(1);
	}
	return 0;


}
