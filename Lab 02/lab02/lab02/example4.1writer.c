#include <fcntl.h> 
#include <sys/stat.h> 
#include <sys/types.h> 
#include <unistd.h> 

int main() 
{ 
    int fd; 
    char * fifo = "/tmp/fifo"; 

    mkfifo(fifo,0666); 

    fd = open(fifo, O_WRONLY); 
    write(fd, "Hi",2); 
    close(fd); 

    unlink(fifo); 

    return 0; 
}

