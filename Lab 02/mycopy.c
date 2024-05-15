// Include the necessary libraries for file control and Unix standard functions
#include <fcntl.h>
#include <unistd.h>

// Define a buffer size of 1024 bytes
#define BUF_SIZE 1024

// Main function with command line arguments
int main(int argc, char *argv[]) {

    // Declare variables for source file, target file, and read status
    int sourceFile, targetFile, readStatus;
    // Declare a buffer of size BUF_SIZE
    char buffer[BUF_SIZE];

    // Open the source file in read-only mode
    sourceFile = open(argv[1], O_RDONLY);
    // If the source file cannot be opened, return 1
    if (sourceFile == -1) {

        return 1;

    }

    // Open or create the target file in write-only mode, create if it doesn't exist, truncate it otherwise
    targetFile = open(argv[2], O_WRONLY | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR);

    // If the target file cannot be opened or created, close the source file and return 1
    if (targetFile == -1) {

        close(sourceFile);
        return 1;

    }

    // Start the copy process
    while ((readStatus = read(sourceFile, buffer, BUF_SIZE)) > 0) {

        // Write the read content to the target file
        // If the number of written bytes doesn't match the read bytes, close both files and return 1
        if (write(targetFile, buffer, readStatus) != readStatus) {
            close(sourceFile);
            close(targetFile);
            return 1;

        }
        
    }

    // After the copy process, close the source file
    close(sourceFile);
    // Close the target file
    close(targetFile);

    // If everything is successful, return 0
    return 0;
}
