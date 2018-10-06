#include<sys/types.h>
#include<stdio.h>
#include<stdlib.h>
#include<sys/stat.h>
#include<fcntl.h>
#include <time.h>
#include<sys/ipc.h>
#include<unistd.h>
#include<string.h>
#include<errno.h>
#include <sys/msg.h>


#define MSG_SIZE 1024

typedef struct msgbuf {
         long    mtype;
         char    mtext[MSG_SIZE];
} message_buf;

int main(){
	struct stat statbuf;
	int fdin;
	fdin = open("input.txt", O_RDONLY);	//open input file, file's size
  	if(fdin<0){
		  printf("can't open input\n");
		  exit(-1);
  	}
	fstat(fdin, &statbuf);
	const int size = statbuf.st_size;
	printf("size=[%d]\n",size);
	
	char* data= malloc(sizeof(char)*size);
	read(fdin,data, size);
	
	int msqid;
    	int msgflg = IPC_CREAT | 0666;
    	key_t key;
    	message_buf sbuf;
	int buf_length;
	
	key = ftok("iput.txt",1);
	
	if ((msqid = msgget(key, msgflg )) < 0) {
        	perror("msgget");
        	exit(1);
	}
	else{ 
		printf("msgget: msgget succeeded: msqid = %d\n", msqid);
	}
	
	sbuf.mtype = 1;
	
	printf("ENTER SIZE OF THE BUF:\n");
	scanf("%d", &buf_length);
	
	int al = size/buf_length;
	int i,j,k;
	
	for(i=0;i<al;i++){
		for(j=0;j<buf_length;j++){
			sbuf.mtext[j]=data[buf_length*i+j];
		}
		msgsnd(msqid, &sbuf, buf_length, IPC_NOWAIT);
		/*for(k=0;k<buf_length;k++){
			printf("%c", sbuf.mtext[k]);
		}*/
	}
	
	free(data);	
	return 0;
}
