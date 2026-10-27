#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>

#define BUFSIZE 16

int main(int argc, char **argv, char **envp)
{
	if (argc < 2)
	{
		printf("Not enough arguments");
		exit(-1);
	}

	int   fdin      = open(argv[1], O_RDONLY);

	if (fdin < 0) {
		perror("read at open!");
		exit(-1);
	}

	if (argc > 2) {
		int fdout = open(argv[2], O_WRONLY | O_CREAT | O_TRUNC, 0644);
		if (fdout < 0) {
			perror("write at open!");
			exit(-1);
		}
		dup2(fdout, STDOUT_FILENO);
	}

	char *buffer    = (char*) malloc (BUFSIZE);
	ssize_t readBytes = 0;


	do {
		readBytes = read(fdin, buffer, BUFSIZE);
		write(STDOUT_FILENO, buffer, readBytes);
	} while(readBytes > 0);

	if (readBytes < 0)
		printf("Error on read!");

	return 0;
}