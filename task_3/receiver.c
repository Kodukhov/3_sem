#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <stdio.h>
#include <stdlib.h>

#define MSG_SIZE 1024

typedef struct msgbuf {
    	long    mtype;
    	char    mtext[MSG_SIZE];
} message_buf;

int main(){
	int msqid;
    	key_t key;
	message_buf rbuf;
	
	int size=400;
	char* rec_data=malloc(sizeof(char)*size);
		
	int buf_size = 4;
	printf("ENTER SIZE OF BUF:\n");
	scanf("%d", &buf_size);
	key = ftok("iput.txt",1);
	int msgflg = IPC_CREAT | 0666;
	
	 if ((msqid = msgget(key, msgflg)) < 0) {
        	perror("msgget");
       	 	exit(1);
	}
	
	int i = 0;
	int k = 0;
	int al = size/buf_size;
	for(k=0;k<(al-1);k++){
		if(msgrcv(msqid, &rbuf, buf_size, 1, 0)<0){
			perror("NO MESSEGES");
			exit(1);
		}
        	for(i=0;i<buf_size;i++){
			rec_data[buf_size*k+i]=rbuf.mtext[i];	
		}
	}	
	
	 for(k=0;k<size;k++){
		 printf("%c",rec_data[k]);
	 }
	free(rec_data);	
	return 0;
}
