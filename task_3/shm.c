#include<sys/types.h>
#include<stdio.h>
#include<stdlib.h>
#include<sys/stat.h>
#include<fcntl.h>
#include <time.h>
#include<sys/ipc.h>
#include<sys/shm.h>
#include<unistd.h>
#include<string.h>
#include<errno.h>

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
	

	int shmid, sh_size,i,j,l,k;
	printf("ENTER SIZE OF SHARED MEMORY:\n");
	scanf("%d", &sh_size);
	char* shared;
	char* data;
	data = malloc(sizeof(char)*size);
	char* drc = malloc(sizeof(char)*size);

	read(fdin,data, size);
	
	int al = size/sh_size;	
	shmid = shmget(IPC_PRIVATE, sh_size, IPC_CREAT|0666);
	pid_t pid = fork();
	if(pid<0){
		printf("FORK ERROR\n");
		exit(-1);
	}

	if(pid){
		shared = shmat(shmid, (void*)0,0);
		sleep(1);
		for(l=0;l<al;l++){
			//printf("I'm writting\n");
			for(j=0;j<sh_size;j++){
				shared[j]=data[l*sh_size+j];
			}
			sleep(1);
		}
	}
	else{
		char* rec = shmat(shmid, (void*)0,0);
		for(k=0;k<al;k++){
			sleep(1);
			//printf("I'm reading\n");
			for(i=0;i<sh_size;i++){
				drc[k*sh_size+i]=rec[i];
			}
			//printf("recived:%c%c%c\n",drc[k*sh_size],drc[k*sh_size+1],drc[k*sh_size+2]);
		}

		for(k=0;k<size;k++){
			printf("%c",drc[k]);
		}
		shmctl(shmid,IPC_RMID,0);
	}
	free(drc);
	free(data);
	return 0;
}
