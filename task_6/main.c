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

void SearchDirectory(const char *name) {
    DIR *dir = opendir(name);               //Assuming absolute pathname here.
    if(dir) {
	int num = 0;
	int res;
	char bu[7]="backups";
        char Path[PATH_MAX], *EndPtr = Path;
        struct stat info;                //Helps us know about stuff
        struct dirent *e;
        strcpy(Path, name);                  //Copies the current path to the 'Path' variable.
        EndPtr += strlen(name);              //Moves the EndPtr to the ending position.
        while ((e = readdir(dir)) != NULL) {  //Iterates through the entire directory.
            strcpy(EndPtr, e -> d_name);       //Copies the current filename to the end of the path, overwriting it with each loop.
            if (!stat(Path, &info)) {         //stat returns zero on success.
                if (S_ISDIR(info.st_mode)) {  //Are we dealing with a directory?
                    //Make corresponding directory in the target folder here.
                    //printf("directory: %s/\n", Path);
			num = strcmp(Path, bu);
			if(num!=0){
				SearchDirectory(Path);	//Calls this function AGAIN, this time with the sub-name.
			}
		} else if (S_ISREG(info.st_mode)) { //Or did we find a regular file?;
                    //Run Copy routine
                    //printf("reg_file: %s\n", Path);
			res = system("diff ");
                }
            }
        }
    }
    return;
}

int Daemon(void){
	
}

int main(){
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
	Daemon();
	return 0;
}
