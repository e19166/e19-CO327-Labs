#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/wait.h>

void handle_client(int newsockfd) {
    char buffer[256];
    ssize_t n;

    // Clear the buffer
    bzero(buffer, 256);

    // Read message from the client
    n = read(newsockfd, buffer, 255);
    if (n < 0) perror("ERROR reading from socket");

    printf("Client: %s\n", buffer);

    // Send a response back to the client
    n = write(newsockfd, "I got your message", 18);
    if (n < 0) perror("ERROR writing to socket");

    // Close the client's socket
    close(newsockfd);
}

int main(int argc, char *argv[]) {
    int sockfd, newsockfd, portno = 12345;
    socklen_t clilen;
    struct sockaddr_in serv_addr, cli_addr;
    pid_t pid;

    // Create a socket
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) {
        perror("ERROR opening socket");
        exit(EXIT_FAILURE);
    }

    // Initialize socket structure
    bzero((char *) &serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port = htons(portno);

    // Bind the host address
    if (bind(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0) {
        perror("ERROR on binding");
        exit(EXIT_FAILURE);
    }

    // Start listening for the clients
    listen(sockfd, 5);
    clilen = sizeof(cli_addr);

    // Accept actual connection from the client
    while (1) {
        newsockfd = accept(sockfd, (struct sockaddr *) &cli_addr, &clilen);
        if (newsockfd < 0) {
            perror("ERROR on accept");
            continue;
        }

        // Fork a new process
        pid = fork();
        if (pid < 0) {
            perror("ERROR on fork");
            exit(EXIT_FAILURE);
        }

        if (pid == 0) { // In the child process
            close(sockfd); // Close the original socket
            handle_client(newsockfd); // Handle the client's request
            exit(EXIT_SUCCESS);
        } else { // In the parent process
            wait(NULL); // Wait for the child process to terminate
            close(newsockfd); // Close the new socket
        }
    }

    // Close the server socket
    close(sockfd);
    return 0; // We never get here
}

