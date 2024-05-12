#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>

/**
 * Executes the command "cat fixtures | grep <search_term> | cut -b 1-9".
 */

int main(int argc, char **argv)
{
	int status;
	int i;

	// arguments for commands
	char *cat_args[] = ** insert your code here ** 
	char *grep_args[] = ** insert your code here **
	char *cut_args[] = ** insert your code here **

	// make 2 pipes (cat to grep and grep to cut); each has 2 fds

	int pipes[4];
	// sets up 1st pipe
	// TODO: ** insert your code here **

	// sets up 2nd pipe
	// TODO: ** insert your code here **

	// we now have 4 fds:
	// pipes[0] = read end of cat->grep pipe (read by grep)
	// pipes[1] = write end of cat->grep pipe (written by cat)
	// pipes[2] = read end of grep->cut pipe (read by cut)
	// pipes[3] = write end of grep->cut pipe (written by grep)

	// fork the first child (to execute cat)

	if (fork() == 0)
	{
		// replace cat's stdout with write part of 1st pipe
		// TODO: ** insert your code here **
		
		// close all pipes (very important!); end we're using was safely copied
		// TODO: ** insert your code here **

		// Execute the cat command
		// TODO: ** insert your code here **
	}
	else
	{
		// fork second child (to execute grep)

		if (fork() == 0)
		{
			// replace grep's stdin with read end of 1st pipe
			// TODO: ** insert your code here **

			// replace grep's stdout with write end of 2nd pipe
			// TODO: ** insert your code here **

			// close all ends of pipes
			// TODO: ** insert your code here **

			// execute grep command
			// TODO: ** insert your code here **
		}
		else
		{
			// fork third child (to execute cut)

			if (fork() == 0)
			{
				// replace cut's stdin with input read of 2nd pipe
				// TODO: ** insert your code here **

				// close all ends of pipes
				// TODO: ** insert your code here **

				// execute cut command
				// TODO: ** insert your code here **
			}
		}
	}

	// only the parent gets here and waits for 3 children to finish
	// It's a good idea to close all your pipes (the parent needs none!)
	// before waiting for your children! 

	// TODO: ** insert your code here **

	// TODO: ** insert your code here **
}

