#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

typedef struct {
  int txd[2]; 
  int rxd[2]; 
} dpipe_t;

int main()
{
  char buf[4096];
  buf[0]='\0';
  char bufc[4096];
  int size;
  dpipe_t fd;
  
  if (pipe(fd.txd) < 0) 
  {
	printf("Pipe creation is failed!\n");
	return -1;
  }
  
  if(pipe(fd.rxd)<0)
  {
	  printf("Pipe creation is failed!\n");
	  return -2;
  }

  const pid_t pid = fork();
  if (pid < 0) {
    	printf("fork failed!\n");
    	return -1;
  }
//===========================================================================
  if (pid) {
    	close(fd.txd[0]);
    	close(fd.rxd[1]);
	while((size = read(0, buf, sizeof(buf)-1)) > 0) 
        {
                buf[size] = 0;
                write(fd.txd[1], buf, size);
                printf("Parent:Send to child: %s\n", buf);

		read(fd.rxd[0], bufc, sizeof(bufc)-1);
		bufc[size]='\0';
		printf("Parent:Received from child: %s\n", bufc);
        }	
  }
  else {
	close(fd.txd[1]);
    	close(fd.rxd[0]);
    	while((size = read(fd.txd[0], buf, sizeof(buf)-1)) > 0) 
    	{
      		buf[size] = '\0'; // the text string data is expected
      		printf("Child:Received from parent: %s\n", buf);
		
		read(0, bufc, sizeof(bufc) -1);
		bufc[size]=0;
		write(fd.rxd[1], bufc, size);
		printf("Child:Send to parent: %s\n", bufc);
    	}
  }

  return 0;
}
