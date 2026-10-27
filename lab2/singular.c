#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/file.h> /* flock */
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h> /* errno */

#define LOCK_FILE "/tmp/my_lock_file"

static int fdlock = -1;

static void do_stuff(void)
{
	sleep(10);
}

static void check_lock(void)
{
	int rc;

	fdlock = open(LOCK_FILE, O_RDWR | O_CREAT, 0644);
	if (fdlock < 0) {
		perror("open");
		exit(-1);
	}

	rc = flock(fdlock, LOCK_EX | LOCK_NB);
	if (rc < 0) {
		if (errno == EWOULDBLOCK)
			printf("Another instance is running\n");
		else
			perror("flock");
		exit(-1);
	}

	printf("\nGot Lock\n\n");
}

static void clean_up(void)
{
	int rc;

	rc = flock(fdlock, LOCK_UN);
	if (rc < 0)
		perror("unlock");

	rc = close(fdlock);
	if (rc < 0)
		perror("close");

	rc = unlink(LOCK_FILE);
	if (rc < 0)
		perror("unlink");
}

int main(void)
{
	check_lock();
	do_stuff();
	clean_up();
	return 0;
}