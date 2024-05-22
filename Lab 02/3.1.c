#include <stdio.h>
#include <unistd.h>
#include <ctype.h>
#include <string.h>
#include <sys/wait.h>

void capitalize(char *str) {
    for (int i = 0; str[i] != '\0'; i++) {
        str[i] = toupper((unsigned char) str[i]);
    }
}

int main() {
    int pipe_parent_to_child[2];
    int pipe_child_to_parent[2];
    pid_t pid;
    int status;

    // Create pipes
    if (pipe(pipe_parent_to_child) == -1) {
        perror("pipe");
        return 1;
    }
    if (pipe(pipe_child_to_parent) == -1) {
        perror("pipe");
        return 1;
    }

    pid = fork();
    if (pid < 0) {
        perror("fork");
        return 1;
    }

    if (pid > 0) { // Parent process
        close(pipe_parent_to_child[0]); // Close read end of parent-to-child pipe
        close(pipe_child_to_parent[1]); // Close write end of child-to-parent pipe

        char input_str[128];
        printf("Enter a string: ");
        fgets(input_str, 128, stdin);
        // Remove the newline character from the input string
        input_str[strcspn(input_str, "\n")] = 0;

        // Send the string to the child
        write(pipe_parent_to_child[1], input_str, strlen(input_str) + 1);
        close(pipe_parent_to_child[1]); // Close write end of parent-to-child pipe

        // Read the capitalized string from the child
        char output_str[128];
        int count = read(pipe_child_to_parent[0], output_str, 128);
        output_str[count] = '\0';
        close(pipe_child_to_parent[0]); // Close read end of child-to-parent pipe

        printf("Capitalized string: %s\n", output_str);

        wait(&status); // Wait for the child process to finish
    } else { // Child process
        close(pipe_parent_to_child[1]); // Close write end of parent-to-child pipe
        close(pipe_child_to_parent[0]); // Close read end of child-to-parent pipe

        char buff[128];
        int count = read(pipe_parent_to_child[0], buff, 128);
        buff[count] = '\0';
        close(pipe_parent_to_child[0]); // Close read end of parent-to-child pipe

        // Capitalize the string
        capitalize(buff);

        // Send the capitalized string back to the parent
        write(pipe_child_to_parent[1], buff, strlen(buff) + 1);
        close(pipe_child_to_parent[1]); // Close write end of child-to-parent pipe

        return 0; // Child process exits
    }

    return 0; // Parent process exits
}
