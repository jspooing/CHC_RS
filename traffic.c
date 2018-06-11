#include<stdio.h>
#include<stdlib.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<pthread.h>
#define TXPATH "/sys/class/net/enxb827eba64985/statistics/tx_bytes"

char global_txr[16]= "default";

int getTx(){

	char buf[16];
	int fd;
	int tx;
	int len;
	
	memset(buf,0x00,16);
	fd = open(TXPATH, O_RDONLY);
	read(fd, buf, sizeof(buf));
	close(fd);
	len = strlen(buf);	
	len = len -4;
	buf[len] = NULL;

	tx = atoi(buf);
	
	return tx;
}

int  getTxrate(char* buf){
	int ltx,ntx;
	int txr;

	sleep(0);
	ltx = getTx();
	sleep(1);
	ntx = getTx();
	txr = ntx - ltx;
	sprintf(buf,"%d",txr);

	return 1;

}

void* setGtxr(void *data){

	while(1){
		getTxrate(global_txr);
	}
	return;

}	
