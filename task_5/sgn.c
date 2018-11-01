#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <pthread.h>
#include <semaphore.h>
#include <sys/shm.h>
#include <string.h>
#include <signal.h>
#include <time.h>

int size;
char* rec;
int j = 0;

void sig_handler(int signo)
{
	printf("I'M HERE\n");
	switch(signo){
		case SIGUSR1:
			rec[j]='0';
			j++;
			break;
		case SIGUSR2:
			rec[j]='1';
			j++;
			break;
	}
	printf("I'M HERE\n");
}

int main(){
	int i = 0;
	int fdin;
	struct stat statbuf;
	fdin = open("in.txt", O_RDONLY);	//open input file, file's size
  	if(fdin<0){
		  printf("can't open input\n");
		  exit(-1);
  	}
	fstat(fdin, &statbuf);
	size = statbuf.st_size;
	printf("size=[%d]\n",size);
	
	char* data;				//data for sending
	data = malloc(sizeof(char)*size);
	read(fdin, data, size);
	close(fdin);

	rec = malloc(sizeof(char)*size);	//received data

	pid_t pid = fork();
	if(pid<0){
		printf("FORK ERR\n");
		exit(-1);
	}
	if(pid>0)
	{					//parent=sender
		for(i = 0; i < size; i++)
		{
            		kill(pid, data[i] == '1' ? SIGUSR1 : SIGUSR2);
			printf("SEND\n");
			//sleep(1);
		}	
	}
	else
	{					//child=reciever
		printf("\nFUCKING CHILD\n");
		if(signal(SIGUSR1, sig_handler) == SIG_ERR){
            		printf("\ncan't catch a SIGUSR1\n");
		}
        	if(signal(SIGUSR2, sig_handler) == SIG_ERR){
			printf("\ncan't catch a SIGUSR2\n");
		}
		for(i=0;i<size;i++){
			printf("%c\n", rec[i]);
		}
		free(rec);
	}
	free(data);
	return 0;
}
