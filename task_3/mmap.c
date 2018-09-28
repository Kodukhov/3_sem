#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <time.h>

int main() {
  	int fdin;
	struct stat statbuf;

	fdin = open("XL.txt", O_RDONLY);	//open input file, file's size
  	if(fdin<0){
		  printf("can't open input\n");
		  exit(-1);
  	}
	fstat(fdin, &statbuf);
	int size = statbuf.st_size;
	printf("size=[%d]\n",size);
	
	void *addr = mmap(0, size, PROT_READ, MAP_SHARED, fdin, 0);	//mmap
	close(fdin);
	printf("Mapped at %p\n", addr);
	/*int *shared = addr;
  	printf("%c_%c\n", *shared, shared[1]);*/
	
	pid_t pid = fork();		//fork
	if(pid<0){
		printf("FORK FAILED\n");
	}
	
	if (pid>0) {
		//printf("I'm parent and i have already done all work)\n");
	} 
	else {
    		int fdout;							//create output file
		fdout = open("out.txt", O_RDWR | O_CREAT | O_TRUNC);
		if(fdout<0){
                  printf("can't open or create output\n");
		  exit(-1);
	        }
		if(lseek(fdout, size - 1, SEEK_SET)==-1){
			printf("err in lseek\n");
			exit(-1);
		}
		write(fdout, "", 1);

    		void *addrout = mmap(0, size, PROT_READ | PROT_WRITE, MAP_SHARED, fdout, 0);
		printf("Mapped at %p\n", addrout);

		//int *sh=addrout;
				
		memcpy(addrout, addr, size);		//copy from addr to addrout
		
		int t2 = clock();
		printf("time = %d\n", t2);

		//printf("%c_%c\n", *sh, sh[1]);
		
		close(fdout);
		munmap(addrout, size);
		//close(fdout);
	}
	
	munmap(addr,size);
	return 0;
}
