#include <stdio.h> 
#include <unistd.h> 

int main() 
{ 
	int pipe_ends[2]; 
	pid_t pid; 
	int status;

	if(pipe(pipe_ends)) 
	{ 
		perror("Create pipe"); 
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
		close(pipe_ends[0]); 
		write(pipe_ends[1],"Hello!\n",8);
		write(pipe_ends[1],"hello!\n",8);
		close(pipe_ends[1]);
	}
	if(pid == 0) 
	{ /* child */ 
		char buff[128]; 
		close(pipe_ends[1]);
		sleep(5); 
		int count = read(pipe_ends[0], buff, 128);
		buff[count] = '\0';
		printf("%s",buff);
	} 
	
	wait(&status);
	return 0;
}
