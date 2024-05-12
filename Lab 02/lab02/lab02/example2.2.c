#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

#define READ_END 0
#define WRITE_END 1

const char banner [] = "hello there\n";

int main()
{
	int pipe_ends[2];
	pid_t pid;
	if(pipe(pipe_ends)) 
	{
		perror("Pipe creation");
		return -1;
	}
	pid = fork();
	if(pid < 0) 
	{
		perror("Fork");
		return -1;
	}
	if(pid > 0) 
	{ /* parent */
		int i;
		close(pipe_ends[READ_END]);
		for(i=0; i<10; i++) 
		{
			printf("Parent Writing [%d]...\n",i);
			write(pipe_ends[WRITE_END], banner, strlen(banner));
			sleep(1);
		}
		exit(EXIT_SUCCESS);
	}
	if(pid == 0) 
	{ /* child */
		char buff[128];
		int count;
		close(pipe_ends[WRITE_END]);
		while((count = read(pipe_ends[READ_END], buff, 128)) > 0) {
			write(STDOUT_FILENO, &buff, count);
			sleep(2);
		}
	}
	return 0;
}
