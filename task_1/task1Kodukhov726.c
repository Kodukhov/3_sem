#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <ctype.h>
#include <sys/wait.h>

int main()
{

	printf("to stop the program print:11\n");
	char com[20];
	int i=0;
	for(i=0;i<20;i++){
		com[i]='@';
	}
	printf("enter a command:");
	scanf("%s", com);
	/*scanf("%c", &com[0]);
	while(com[i]!='\n'){
		i++;
		scanf("%c", &com[i]);
	}*/

	while(com[0]!='1' && com[1]!='1')
	{
		int len = strlen(com);
		//printf("lenth of command:%d\n", len);
		char* cmd = malloc(sizeof(char)*len);
		for(i=0;i<len;i++)
		{
			*(cmd+i)=com[i];
		}

		pid_t pid = fork();

		if(pid<0)
		{
			printf("fork error\n");
			return -1;
		}
		if(pid)
		{
			int status;
			waitpid(pid, &status, 0);
			printf("Ret code: %d\n", WEXITSTATUS(status));
		}
		else
		{
			//printf("doing\n");
			execlp(cmd,"-l",NULL);
		}
		

		free(cmd);
		printf("enter a command:");
		scanf("%s", com);
	}

	return 0;
}
