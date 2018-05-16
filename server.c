#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include "unistd.h"
#include "time.h"
#include "netinet/in.h"
#include "arpa/inet.h"
#include "sys/types.h"
#include "sys/socket.h"
#include "pthread.h"

#define BUFSIZE 1024 //버퍼 사이즈 
#define PORT 9957 //서버 포트 
#define MAX_CLIENT 5 //클라이언트 최대 

void error_handling(char* message); //예외처리 함수 
void* clnt_connection(void* arg);  //클라이언트 서비스 스레드 함수 
int cntNum=0;  //현재 서비스중인 클라이언트 수 
int clnt_sock[5]; //클라이언트 소켓 

int printNs();
char* catchIp();

int main(int argc, char ** argv)
{

	int serv_sock; // 서버  소켓 
	struct sockaddr_in serv_addr;   //서버,클라이언트  소켓 주소 구조체 
	struct sockaddr_in clnt_addr;
	int clnt_addr_size;  // 클라이언트 소켓 구조체 사이즈 
	pthread_t tid[MAX_CLIENT]; // 스레드 구조체 배열 
	
	#ifdef _DEBUG
		printf("*****************Debug mode*********************\n");
		printNs();
		catchIp();
	#endif	

	if((serv_sock = socket(AF_INET, SOCK_STREAM, 0))<0)  //서버 소켓 생성 
	{
		perror("socket() error: ");
	}
	
	
	memset(&serv_addr, 0x00 , sizeof(serv_addr));  

	serv_addr.sin_family = AF_INET;   //tcp 
	serv_addr.sin_addr.s_addr = htonl(INADDR_ANY); 
	serv_addr.sin_port = htons(PORT);  //서버 포트번호 설정 
	
	if(bind(serv_sock,(struct sockaddr*)&serv_addr, sizeof(serv_addr))<0)
		error_handling("bind() error");
	

	if(listen(serv_sock,5)<0)
		error_handling("listen() error");
	
	write(1,"Waiting for client",sizeof("Waiting for client"));
	clnt_addr_size = sizeof(clnt_addr);

	while(1){
		//클라이언트 연결 
		clnt_sock[cntNum] = accept(serv_sock,(struct sockaddr*)&clnt_addr,&clnt_addr_size);
		if(clnt_sock < 0)
			error_handling("accept() error");
		#ifdef _DEBUG
			printf("cntNum = %d\nsocket=%d\n",cntNum,clnt_sock[cntNum]);
		#endif	
		
		if(pthread_create(&tid[cntNum],NULL, clnt_connection, (void *)&clnt_sock[cntNum])<0)
			printf("Thread Error!\n");

		cntNum++;
		
		
	}
	close(serv_sock);
	return 0;
}
	
void error_handling(char* message){
	fputs(message, stderr);
	fputc('\n',stderr);
	exit(1);
}

void *clnt_connection(void* arg){
	int* pSock = (int*) arg;
	int sock = *pSock;
	char* command[5];
	char* ptr;
	int str_len;
	char message[BUFSIZE];
	int i;
	#ifdef _DEBUG
		printf("Thread arg = %d\nThread socket = %d\n",*((int *)arg),sock);
	#endif 
	memset(message,0x00, sizeof(message));
	while(str_len = recv(sock,message,BUFSIZE,0)>0){
		str_len = strlen(message);
		#ifdef _DEBUG
			printf("message = %s\nstr_len = %d\n",message,str_len);
			fflush(stdout);
		#endif
		ptr = strtok(message, " ");
		i=0;
		while(ptr != NULL){
			command[i] = ptr;
			ptr = strtok(NULL, " ");
			i++;
			if(i > 4)break;
		}

		str_len = strlen(command[0]);
		#ifdef _DEBUG
			printf("command = %s\ncommand Length = %d",command[0],str_len);
			fflush(stdout);
		#endif
		
		if(!strcmp(command[0],"network"))
			write(sock,"network service!",sizeof("network service!"));
		else if(!strcmp(command[0],"dc"))
			write(sock,"disconnect service!",sizeof("disconnect service!"));
		else if(!strcmp(command[0],"log"))
			write(sock,"log service!",sizeof("log service!"));
		else if(!strcmp(command[0],"trf"))
			write(sock,"traffic service!",sizeof("traffic service!"));
		else 
			write(sock,"Undefined commnad!",sizeof("Undefined command!"));		
		
		memset(message,0x00,sizeof(message));
	}

 	
	close(sock);
	for(i=0;i<cntNum;i++){
		if(sock == clnt_sock[i]){
			#ifdef _DEBUG 
				printf("Client %d is disconnected.\n",i);
			#endif
			for(;i<cntNum-1;i++){
				clnt_sock[i]=clnt_sock[i+1];
			}
			break;
		}
	}
	
	cntNum--;
	#ifdef _DEBUG
		printf("******Replace Clients*********\n Sector : Socket\n");
		for(i=0;i < cntNum; i++)
			printf("%d : %d\n",i,clnt_sock[i]);
	#endif	
			

		
	
	return 0;
}
