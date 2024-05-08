#include <sys/types.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h> // Include for wait()

int main()
{
    pid_t pid;
    int i;

    for (i = 0; i < 3; i++)
    {
        // Fork a child process
        pid = fork();

        if (pid < 0)
        {
            // Error occurred
            fprintf(stderr, "Fork Failed!");
            return 1;
        }
        else if (pid == 0)
        {
            // Child process
            printf("child: my pid = %d, parent pid = %d\n", getpid(), getppid());
            //return 0; // Child exits
        }
        else
        {
            // Parent process
            wait(NULL); // Wait for child to exit
            printf("parent: my pid = %d, I just waited for child pid %d\n", getpid(), pid);
        }
    }
    return 0; // end main
}

