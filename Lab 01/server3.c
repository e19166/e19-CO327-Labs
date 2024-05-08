#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

int sockfd;

void signal_handler(int sig) {
    // Close the server socket
    close(sockfd);
    printf("Server terminated properly.\n");
    exit(0);
}

int main(int argc, char *argv[]) {
    struct sockaddr_in serv_addr;
    int portno = 12345;
    int reuse = 1;

    // Create a socket
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) {
        perror("ERROR opening socket");
        exit(EXIT_FAILURE);
    }

    // Set the SO_REUSEADDR socket option
    if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof(int)) < 0) {
        perror("setsockopt(SO_REUSEADDR) failed");
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

    // Install the signal handler
    signal(SIGINT, signal_handler);

    // Start listening for the clients
    listen(sockfd, 5);

    // Server loop...
    // Accept connections, handle clients, etc.

    return 0; // We never get here
}

