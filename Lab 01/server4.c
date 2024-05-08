#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <errno.h>
#include <fcntl.h>

#define BUFFER_SIZE 1024

void handle_client(int newsockfd) {
    char buffer[BUFFER_SIZE];
    int n;

    // Clear the buffer
    bzero(buffer, BUFFER_SIZE);

    // Read the file path from the client
    n = read(newsockfd, buffer, BUFFER_SIZE - 1);
    if (n < 0) {
        perror("ERROR reading from socket");
        close(newsockfd);
        return;
    }

    // Open the file requested by the client
    int filefd = open(buffer, O_RDONLY);
    if (filefd < 0) {
        perror("ERROR opening file");
        close(newsockfd);
        return;
    }

    // Send the contents of the file back to the client
    while ((n = read(filefd, buffer, BUFFER_SIZE - 1)) > 0) {
        write(newsockfd, buffer, n);
    }

    // Close the file and the client's socket
    close(filefd);
    close(newsockfd);
}

int main(int argc, char *argv[]) {
    int sockfd, newsockfd, portno = 12345;
    socklen_t clilen;
    struct sockaddr_in serv_addr, cli_addr;
    int pid;

    // Create a socket
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) {
        perror("ERROR opening socket");
        exit(1);
    }

    // Initialize socket structure
    bzero((char *) &serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port = htons(portno);

    // Bind the host address
    if (bind(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0) {
        perror("ERROR on binding");
        exit(1);
    }

    // Start listening for the clients
    listen(sockfd, 5);
    clilen = sizeof(cli_addr);

    // Accept actual connection from the client
    while (1) {
        newsockfd = accept(sockfd, (struct sockaddr *) &cli_addr, &clilen);
        if (newsockfd < 0) {
            if (errno == EINTR) continue; // Ignore interrupted system calls
            perror("ERROR on accept");
            exit(1);
        }

        // Fork a new process
        pid = fork();
        if (pid < 0) {
            perror("ERROR on fork");
            exit(1);
        }

        if (pid == 0) { // In the child process
            close(sockfd); // Close the original socket
            handle_client(newsockfd); // Handle the client's request
            exit(0);
        } else { // In the parent process
            close(newsockfd); // Close the new socket
        }
    }

    return 0; // We never get here
}

