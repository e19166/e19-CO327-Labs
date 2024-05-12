#include <fcntl.h> 
#include <stdio.h> 
#include <sys/stat.h> 
#include <unistd.h> 

#define MAX_SIZE 1024 

int main() 
{ 
    int fd; 
    char* fifo = "/tmp/fifo"; 
    char buf[MAX_SIZE]; 

    mkfifo(fifo,0666);

    fd = open(fifo, O_RDONLY); 
    read(fd, buf, MAX_SIZE); 
    printf("message read = %s\n", buf); 
    close(fd); 

    return 0; 
}

