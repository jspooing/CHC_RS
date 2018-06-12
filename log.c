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
int checkLogout(char ip[16],char Lip[][16],int lcnt);
int checkLogin(char ip[16],char Lip[][16],int Ncnt);

void* t_wLog(void *data)
{
	int fd;
	char buf[BUFSIZE];
	int str_len;
	char Lip[10][16];
	char Nip[10][16];
	char time[25];
	int i;
	int l_cnt=0,n_cnt=0;
	fd = open("/home/chc/server/log/log.txt",O_APPEND|O_WRONLY|O_CREAT,0666);

	if(fd == -1)
		printf("LogFILE open err...\n");
	else

		
		n_cnt = getIp(Nip);
	while(1){
		memset(buf,0x00,sizeof(buf));
			l_cnt = n_cnt;
			for(i=0; i < n_cnt ; i++){	
				sprintf(Lip[i],"%s",Nip[i]);
			}
			n_cnt = getIp(Nip);

			for(i=0 ; i < n_cnt;i++){
				if(checkLogin(Nip[i],Lip,l_cnt)==1){
					getLogTime(time);
					sprintf(buf,"%s%s-%s#Login,",buf,Nip[i],time);
				}
			}

			printf("l_cnt = %d\n",l_cnt);
			for(i=0 ; i < l_cnt;i++){
				printf("%s : %d\n",Lip[i],checkLogout(Lip[i],Nip,l_cnt));
				if(checkLogout(Lip[i],Nip,n_cnt)==1){
					getLogTime(time);
					sprintf(buf,"%s%s-%s#Logout,",buf,Lip[i],time);
				}
			}



		str_len = strlen(buf);
		write(fd,buf,str_len);

		#ifdef _DEBUG	
			printf("log :\n%s\n",buf);
		#endif
		sleep(1);
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
		buf[str_len++] = '\n';
		buf[str_len] = '\0';
		write(sock,buf,str_len);
		printf("%s",buf);
	}
	close(fd);

	return 0;
}

int checkLogin(char ip[16],char Lip[][16],int lcnt){
	int i; 
	for(i=0; i < lcnt; i ++ ){
		if(strcmp(ip,Lip[i])==0)
			break;	
	}
	if(i == lcnt)
		return 1;
	else return 0;
}



int checkLogout(char ip[16],char Nip[][16],int Ncnt){
	int i;
	for(i=0; i < Ncnt; i ++ ){
		if(strcmp(ip,Nip[i])==0)
			break;	
	}
	if(i == Ncnt )
		return 1;
	else return 0;
}
