/*the directory, which is looked by daemon, is spelled out in main code.
 * It is more easy to test the programm.
 * Daemon sleeps one second after every iteration.
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/time.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>
#include <resolv.h>
#include <errno.h>
#include <dirent.h>
#include <string.h>
#include <fcntl.h>

void DIFF(const char *file, const char *name){
	char nam[80]="/home/alexey/backups/";
	strcat(nam, name);
	char arg[256] = "diff ";
        strcat(arg,file);
	strcat(arg," ");
        strcat(arg,nam);
        int res;
        int fd = open(nam,O_WRONLY|O_CREAT|O_TRUNC,S_IRWXU|S_IRGRP|S_IROTH);
        if(fd<0){
                printf("can't open file\n");
                exit(EXIT_FAILURE);
        }
	strcat(arg, "\n");
        close(STDOUT_FILENO);
        if(dup(fd)!=STDOUT_FILENO){
                perror("dup");
                close(fd);
                exit(EXIT_FAILURE);
        }
        close(fd);
        res = system(arg);
	return;
}

int TYPE(const char *f){
        char arg[256]="file ";
        strcat(arg, f);

        char nam[256] = "/home/alexey/classwork/STREAM.txt";
        int fd = open(nam,O_WRONLY|O_CREAT|O_TRUNC,S_IRWXU|S_IRGRP|S_IROTH);
        if(fd<0){
                printf("can't open file\n");
                exit(EXIT_FAILURE);
        }
        close(STDOUT_FILENO);
        if(dup(fd)!=STDOUT_FILENO){
                perror("dup");
                close(fd);
                exit(EXIT_FAILURE);
        }

        system(arg);
        close(fd);

        fd = open(nam,O_RDONLY);
        struct stat statbuf;
        fstat(fd, &statbuf);
        int size = statbuf.st_size;

        char buf[256];
        int num = 200;
        num = read(fd,buf,size);
        if(num!=size){
                printf("size=%d", size);
                printf("num=%d\n", num);
                printf("ERROR in read\n");
        }
        close(fd);

        if(buf[size-2]=='t' && buf[size-3]=='x' && buf[size-4]=='e' && buf[size-5]=='t'){
                return 1;
        }
        return 0;
	
}

void SearchDirectory(const char *name) {
    	DIR *dir = opendir(name);
    	if(dir) {
		char Path[PATH_MAX], *EndPtr = Path;
		struct stat info;
        	struct dirent *e;
        	strcpy(Path, name);
        	EndPtr += strlen(name);             
        	while ((e = readdir(dir)) != NULL) { 
		char add[200]="/";
		strcat(add,e->d_name);	
            	strcpy(EndPtr, add); 	
		if (!stat(Path, &info)) { 
                	if (S_ISDIR(info.st_mode)) {  
				if(e->d_name[0]!='.'){
					SearchDirectory(Path);
				}
			} 
			else{
		       		if (S_ISREG(info.st_mode)) { 
					 if(TYPE(Path)==1){
					 	DIFF(Path,e->d_name);
					 }
                		}
            		}
		}
		}
    	}
   	return;
}

void Daemon(const char *direct){
	while(1){
		SearchDirectory(direct);
		sleep(1);
	}
}

int main(){
	char direct[256]="/home/alexey/classwork/3_sem/task_6";
	pid_t parpid;
	if((parpid = fork()) < 0){
 	       perror("can't fork");
 	       exit(1);
     	}
	else if(parpid != 0){
      		  printf("backuper pid: %d\n", parpid);
       		 exit(0);
   	}
	setsid();
	Daemon(direct);
	return 0;
}
