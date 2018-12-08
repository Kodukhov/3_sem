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

typedef struct _fifoinfo{
	char FIFO1[SIZE];
  	char FIFO2[SIZE];
} fifoinfo;

typedef struct _fdinfo{
	int fd1;
  	int fd2;
} fdinfo;

int main()
{
  						//LIST OF FILES ON THE SERVER
	char file[10][SIZE];
 	strcpy(file[0], "NUMBER1\n");
	strcpy(file[1], "NUMBER2\n");
  	strcpy(file[2], "NUMBER3\n");
  	strcpy(file[3], "NUMBER4\n");
  	strcpy(file[4], "NUMBER5\n");
  	strcpy(file[5], "NUMBER6\n");
  	strcpy(file[6], "NUMBER7\n");
  	strcpy(file[7], "NUMBER8\n");
  	strcpy(file[8], "NUMBER9\n");
  	strcpy(file[9], "NUMBER10\n");

  	int variable = mknod("SERVAK", S_IFIFO|0666, 0);
  	int FD = open("SERVAK", O_RDWR);
  	int flags = fcntl(FD, F_GETFL, 0);
  	fcntl(FD, F_SETFL, flags | O_NONBLOCK);
	
  	//list of fifo
  	fifoinfo record[128];
	
  	//list of file's handles
  	fdinfo registered[128];
  	int client_number = 0;
  	fd_set for_descriptors;
  	struct timeval tv;
  	while(1){
    		char minibuf[SIZE];
    		int pot = read(FD, minibuf, SIZE-1);
    		if (pot != -1){
      			printf("RECEIVED  MESSAGE.\n");		//SERVAK GOT A MESSAGE ABOUT REGISTRATION
      			char token[SIZE];
      			char storage[SIZE];
      			strcpy(storage, minibuf);
      			strcpy(token, strtok(storage, " "));
      			strcpy(record[client_number].FIFO1, strtok(NULL, " "));
			
      			strcpy(record[client_number].FIFO2, strtok(NULL, " "));
			
      			registered[client_number].fd1 = open(record[client_number].FIFO1, O_RDWR);

      			int flags = fcntl(registered[client_number].fd1, F_GETFL, 0);
      			fcntl(registered[client_number].fd1, F_SETFL, flags | O_NONBLOCK);
      			registered[client_number].fd2 = open(record[client_number].FIFO2, O_RDWR);
      			
      			char revers[SIZE];
      			strcpy(revers, "YOU ARE REGISTRATED\nFiles on server: from 0 to 9\n");
      			write(registered[client_number].fd2, revers, SIZE-1);
      			client_number++;
      			printf("NUMBER OF CLIENTS =  %d\n", client_number);
    		}
    		
    		if (client_number > 0){
      			int i;
      			tv.tv_sec = 0;
      			tv.tv_usec = 0;    
      			FD_ZERO(&for_descriptors);
			
      			for (i = 0; i < client_number; i++){
        			FD_SET(registered[i].fd1, &for_descriptors);
      			}
			
      			int volume = 0;
      			for (i = 0; i < client_number; i++){
        			volume += registered[i].fd1;
      			}
      			int retval = select(volume+1, &for_descriptors, NULL, NULL, &tv);
      			
     			if (retval > 0){
        			for (i = 0; i < client_number; i++){
          				if (FD_ISSET(registered[i].fd1, &for_descriptors)){
            					int KODUKHOV;
            					char buffer[SIZE];
            					KODUKHOV:
            					if ((KODUKHOV = read(registered[i].fd1, buffer, SIZE-1)) > 0){
              						char tok[SIZE];
	      						char BUFF[SIZE];
              						strcpy(BUFF, buffer);
     	      						strcpy(tok, strtok(BUFF, " "));
              						if (strcmp(tok, "GET") == 0){
	        						strcpy(tok, strtok(NULL, " "));
	        						write(registered[i].fd2, file[(int)(tok[0]) - 48], strlen(file[(int)(tok[0]) - 48]));
	      						}
            					}
            					else{
              						goto KODUKHOV;
            					}
          				}
        			}
      			}
    		} 
  	} 
	close(FD);
	return 0;
}
