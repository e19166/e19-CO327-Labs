#include <stdio.h>
#include <unistd.h>

int main(int argc, char *argv[])
{
    // Check if a path argument is provided
    if (argc != 2) {
        printf("Usage: %s <path>\n", argv[0]);
        return 1;
    }

    // Execute the 'ls' command with the provided path
    execl("/bin/ls", "ls", "-l", argv[1], NULL);

    // This line will not be executed if 'execl' is successful
    puts("Program ls has terminated");

    return 0;
}

