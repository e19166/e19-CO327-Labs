#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>

int main()
{
	int out;
	char* banner = "String to write and read";
	size_t size = strlen(banner);

	out = open("out.txt", O_WRONLY | O_APPEND | O_CREAT , S_IRUSR | S_IWUSR ); /*second parameter is a flag and third a mode. Find out what they do */
	
	if(out == -1)
	{
		/* the exact error could be found out by looking at the variable errno. We do not cover it here */
		fprintf(stderr,"Couldn't open the file for writing\n");
		return 1;
	}
	printf("Writing data to file...\n");

	size_t ret = write(out,banner,size);
	
	if(ret == -1)
	{
		fprintf(stderr,"Error writing to file\n");
		return 1;
	}
	printf("Done with writing.Closing the file.\n");

	ret = close(out);

	if(ret == -1)
	{
		fprintf(stderr,"Error closing the file after writing.\n");
		return 1;
	}

	printf("Reopening the file to read back the string written...\n");

	out = open("out.txt", O_RDONLY);
	
	if(out == -1)
	{
		fprintf(stderr,"Couldn't open the file\n");
		return 1;
	} 

	printf("Reading data back from the file...\n");

	char* buffer = malloc(size+1); /* size of a char is 1. This buffer holds the read back value. */

	ret = read(out,buffer,size);

	if(ret == -1)
	{
		fprintf(stderr,"Error reading from file\n");
		return 1;
	}
	buffer[ret] = '\0'; /* we have to null terminate the string ourselves. */
	
	printf("The string read back is: %s\n",buffer);
	/* In case there was something already written in the file, the text read back might not be the same as what was written */
	
	printf("Done with reading. Closing the file...\n");
	
	free(buffer);

	ret = close(out);

	if(ret == -1)
	{
		fprintf(stderr,"Error closing the file after reading.\n");
		return 1;
	}

	return 0;
}
