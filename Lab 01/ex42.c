#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

#define MAX_COMMAND_LENGTH 1024

int main() {
    char command[MAX_COMMAND_LENGTH];
    char *args[MAX_COMMAND_LENGTH / 2];
    int status;

    while (1) {
        printf("simple-shell> ");
        fflush(stdout);

        // Read the command from the user
        if (!fgets(command, MAX_COMMAND_LENGTH, stdin)) {
            // Handle error if user input fails
            perror("fgets failed");
            exit(EXIT_FAILURE);
        }

        // Remove newline character from command
        size_t length = strlen(command);
        if (command[length - 1] == '\n') {
            command[length - 1] = '\0';
        }

        // Parse the command into arguments
        char *token = strtok(command, " ");
        int i = 0;
        while (token != NULL) {
            args[i++] = token;
            token = strtok(NULL, " ");
        }
        args[i] = NULL; // Null-terminate the arguments list

        // If the user types 'exit', terminate the shell
        if (strcmp(args[0], "exit") == 0) {
            break;
        }

        // Create a child process to run the command
        pid_t pid = fork();
        if (pid == 0) {
            // Child process
            if (execvp(args[0], args) == -1) {
                perror("execvp failed");
                exit(EXIT_FAILURE);
            }
        } else if (pid < 0) {
            // Fork failed
            perror("fork failed");
            exit(EXIT_FAILURE);
        } else {
            // Parent process waits for the child to complete
            do {
                waitpid(pid, &status, WUNTRACED);
            } while (!WIFEXITED(status) && !WIFSIGNALED(status));
        }
    }

    return 0;
}

