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

int N;          //amount of streams
//pthread_t* tid;
pthread_t tid[2];
float* res;
int NUM = 0;

void* thread_stuff(void *arg)
{
	int i = 0;
	int k = 0;
	int max = 1000000;
	float integ = 0;
	int n = 0;
	float x, y, val;
	if(NUM<N){
		n = NUM;
		NUM++;
	}
	else
	{
		printf("ERROR In TREAD_STUFF\n");
		return NULL;
	}
	for(i=0;i<max;i++){
		x = (float)rand()/RAND_MAX;
		y = (float)rand()/RAND_MAX;
		val = x*x;
		if(y<=val){
			k++;		
		}
	}
	float cyk = 1.0/max;
	integ = k*cyk;
	//printf("{%f}\n", integ);
	res[n]=integ;
	return NULL;
}

int main(void){
	printf("ENTER THE NUMBER OF STREAMS:\n");
	scanf("%d", &N);
	//tid = malloc(sizeof(pthread_t)*N);
	res = malloc(sizeof(float)*N);
	int i = 0;
	NUM = 0;
	int err;
	while(i < N) {
        	err = pthread_create(&(tid[i]), NULL, &thread_stuff, NULL);
        	if (err != 0){
       		     printf("\ncan't create thread :[%s]", strerror(err));
		}
        	else{
            		printf("\n Thread %d created successfully with tid=%lu\n", i, (unsigned long)tid[i]);
		}
		i++;
	}
	sleep(4);
	float aver = 0;
	for(i=0;i<N;i++){
		//printf("[%f]", res[i]);
		aver = aver + res[i];
	}
	aver = aver/N;
	printf("integrate(x^2dx),[0;1]=%f\n", aver);
	free(res);
	//free(tid);
	return 0;
}
