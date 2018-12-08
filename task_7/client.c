#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/time.h>
#include <fcntl.h>
#include <sys/select.h>
#include <sys/ipc.h>
#include <sys/msg.h>

#define SIZE 1024

char* READING(){			//just reading
  	char* BUF = (char*)malloc(sizeof(char)*SIZE);
  	char a;
  	int i;
  	scanf("%c", &a);
  	BUF[0] = a;
	i = 1;
  	while (a != '\n'){
    		a = getchar();
    		BUF[i] = a;
		i++;
	}
	BUF[i-1] = '\0';
	return BUF;
}

int main()
{
  	int FLAG1 = 0;
 	char FIFO1[SIZE];
 	char FIFO2[SIZE];
 	int fd1, fd2;
	int FD = open("SERVAK", O_RDWR);
  					//REQUEST
  	while (FLAG1 == 0){
    		char buf[SIZE];
    		strcpy(buf, READING());
    		char token[SIZE];
    		char storage[SIZE];
    		strcpy(storage, buf);
    		strcpy(token, strtok(storage, " "));

    		if (strcmp(token, "REGISTER") == 0){
      			strcpy(token, strtok(NULL, " "));
      			strcpy(FIFO1, token);
    	  		strcpy(token, strtok(NULL, " "));
   	   		strcpy(FIFO2, token);

  	 	   	int res = mknod(FIFO1, S_IFIFO|0666, 0);
  		    	fd1 = open(FIFO1, O_RDWR);
  		    	res = mknod(FIFO2, S_IFIFO|0666, 0);
  	    		fd2 = open(FIFO2, O_RDWR);

  	    		int flags = fcntl(fd2, F_GETFL, 0);
      			fcntl(fd2, F_SETFL, flags | O_NONBLOCK);
      			write(FD, buf, SIZE-1);
			printf("MESSAGE IS SENT;\n");
      			FLAG1 = 1;
    		}
  	}
  
  	int count = 0;			//waiting for feedback
  	char buf[SIZE];
  	NAME:
  	if ((count = read(fd2, buf, SIZE-1)) < 0){
		goto NAME;
	}
	else{
		buf[count] = '\0';
		printf("%s\n", buf);
	}
 					 //Now, we are in
  	for (;;){
		printf("WAIT for printing\n");

 		char ALEXEY[SIZE];
		int RES;
 		strcpy(ALEXEY, READING());
		int num = strlen(ALEXEY);
		write(fd1, ALEXEY, num);
		printf("MESSAGE IS SENT;\n");
		CICL:
		if ((RES = read(fd2, ALEXEY, SIZE-1)) < 0){
    			goto CICL;
    		}
    		else{
      			ALEXEY[RES] = '\0';
      			printf("%s", ALEXEY);
    		}
  	}
	close(fd1);
	close(fd2);
	return 0;
}



