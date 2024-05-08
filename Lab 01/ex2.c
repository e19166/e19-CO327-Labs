#include <sys/types.h>
#include <stdio.h>
#include <unistd.h>

int main()
{
    pid_t pid, mpid, ppid, cpid;
    int i;

    for (i = 0; i < 3; i++)
    {
        // Fork a child process
        pid = fork();
        mpid = getpid();
        ppid = getppid();

        if (pid < 0)
        {
            // Error occurred
            fprintf(stderr, "Fork Failed!");
            return 1;
        }
        else if (pid == 0)
        {
            // Child process
            printf("child: my pid = %d, parent pid = %d\n", mpid, ppid);
        }
        else
        {
            // Parent process
            printf("parent: my pid = %d, parent pid = %d ", mpid, ppid);
            printf(" -- > I just spawned a child with pid %d\n", pid);
        }
    } // end for

    wait(NULL); // Wait for all child processes to finish
    return 0; // end main
}

