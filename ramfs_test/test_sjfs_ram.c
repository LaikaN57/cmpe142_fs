#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>

#define FILENAME        "/mnt/test/me"

int main (int argc, char **argv) {
	int fd, len;
	char buf[256];

	fd = open(FILENAME, O_RDWR | O_CREAT, 0666);

	len = sprintf(buf, "fd=%d\n", fd);

	write(fd, buf, len);

	close(fd);

	return 0;
}

