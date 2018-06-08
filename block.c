#include<stdio.h>
#include<stdlib.h>
#include<sys/types.h>
#include<fcntl.h>
#include<string.h>

#define BLACKPATH "/home/chc/server/log/black.txt"

void addBlack(char *bIP){

	char buf[20];
	int fd;
	int len;

	fd = open(BLACKPATH,O_APPEND|O_WRONLY|O_CREAT,0666);

	if(fd == -1)
		printf("File open err..\n");
	
	sprintf(buf,"%s ",bIP);
	
	len = strlen(buf);

	write(fd,buf,len);

	return;


	

}

void delBlack(char *bIP){
	char command[100];

	sprintf(command,"sed -i 's/%s //g' %s",bIP,BLACKPATH);
	system(command);
	printf("%s\n",command);
	return;
}

int getBlack(char arg[][16]){
	
	FILE *fp;
	int cnt=0;
	int i;
	
	fp = fopen(BLACKPATH,"r");

	while(fscanf(fp,"%s",arg[cnt])>0){
		cnt++;
		if(cnt == 10)break;
	}



	return cnt;


}



