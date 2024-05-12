#include <stdio.h> 
#include <fcntl.h> 
#include <sys/types.h> 
#include <sys/stat.h> 
#include <string.h> 

int main() { 
	char* banner = "This is a string written to a textfile by a C program\n"; 
	int desc = open("out.txt", O_WRONLY|O_APPEND|O_CREAT , S_IRUSR | S_IWUSR); 
	write(desc,banner,strlen(banner)); 
	close(desc); 
	return 0; 
}

