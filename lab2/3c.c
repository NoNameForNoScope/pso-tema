#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>

#define BUFSIZE 16

ssize_t full_read(int fd, void *buf, size_t count)
{
	size_t done = 0;
	ssize_t n;

	while (done < count) {
		n = read(fd, buf + done, count - done);
		if (n < 0)
			return -1;
		if (n == 0)
			break;
		done += n;
	}
	return done;
}

ssize_t full_write(int fd, const void *buf, size_t count)
{
	size_t done = 0;
	ssize_t n;

	while (done < count) {
		n = write(fd, buf + done, count - done);
		if (n < 0)
			return -1;
		done += n;
	}
	return done;
}

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
		readBytes = full_read(fdin, buffer, BUFSIZE);
		full_write(STDOUT_FILENO, buffer, readBytes);
	} while(readBytes > 0);

	if (readBytes < 0)
		printf("Error on read!");

	return 0;
}