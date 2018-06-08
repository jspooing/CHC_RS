#include<stdio.h>
#include<stdlib.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<string.h>
#include<pthread.h>
#include<time.h>


#define BUFSIZE 1024

int catchIp(char*);
int getLogTime(char*);


void* t_wLog(void *data)
{
	int fd;
	char buf[BUFSIZE];
	int str_len;
	char ip[10][16];
	char time[25];
	int i,cnt;

	fd = open("/home/chc/server/log/log.txt",O_APPEND|O_WRONLY|O_CREAT,0666);

	if(fd == -1)
		printf("FILE open err...\n");
	else	
	while(1){
		memset(buf,0x00,sizeof(buf));
		cnt = getIp(ip);


		for(i=0; i<cnt; i++){
			getLogTime(time);
			sprintf(buf,"%s%s\t%s\n",buf,ip[i],time);
		}

		str_len = strlen(buf);
		write(fd,buf,str_len);

		#ifdef _DEBUG	
			printf("log :\n%s\n",buf);
		#endif
		sleep(60);
	}

	close(fd);
	return 1;
}

int getLogTime(char *tb){

	time_t timer;
	struct tm *t;

	timer = time(NULL);
	t = localtime(&timer);

	sprintf(tb,"%d/%2d/%2d %d:%d:%d",t->tm_year+1900,t->tm_mon+1,t->tm_mday,t->tm_hour,t->tm_min,t->tm_sec);

	return 1;
}

int getLog(int sock,char* buf,int buf_size){
	
	char chkbuf;
	int fd;
	int str_len;

	fd = open("/home/chc/server/log/log.txt",O_RDONLY);
	if(fd == -1)
		printf("File Open err .. \n");
	while(read(fd,buf,buf_size)){
		str_len = strlen(buf);
		write(sock,buf,str_len);
		printf("%s",buf);
	}

	return 0;
}

