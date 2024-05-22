#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>

int main() {
    int pipe_fd[2];
    pid_t pid;

    // Create the pipe
    if (pipe(pipe_fd) == -1) {
        perror("pipe");
        exit(EXIT_FAILURE);
    }

    // Fork the process
    pid = fork();
    if (pid == -1) {
        perror("fork");
        exit(EXIT_FAILURE);
    }

    if (pid == 0) { // Child process
        // Close the read end of the pipe
        close(pipe_fd[0]);
        
        // Redirect standard output to the write end of the pipe
        dup2(pipe_fd[1], STDOUT_FILENO);
        
        // Close the write end of the pipe after duplicating
        close(pipe_fd[1]);

        // Execute the ls command
        execlp("ls", "ls", (char *) NULL);

        // If execlp fails
        perror("execlp");
        exit(EXIT_FAILURE);
    } else { // Parent process
        // Close the write end of the pipe
        close(pipe_fd[1]);
        
        // Read from the read end of the pipe
        char buffer[1024];
        ssize_t count;
        
        // Wait for the child process to complete
        wait(NULL);

        // Read the output from the pipe and print it
        while ((count = read(pipe_fd[0], buffer, sizeof(buffer)-1)) > 0) {
            buffer[count] = '\0'; // Null-terminate the string
            printf("%s", buffer);
        }

        // Close the read end of the pipe
        close(pipe_fd[0]);
    }

    return 0;
}
