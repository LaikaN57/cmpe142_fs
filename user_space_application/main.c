#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <string.h>
#include <unistd.h>

// for protection against multiple running instances
#include <sys/file.h>
#include <errno.h>

// netlink socket headers
#include <asm/types.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <sys/poll.h>

#include <linux/netlink.h>
#include <linux/rtnetlink.h>

#include <arpa/inet.h>

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <time.h>
#include <getopt.h>

#include <linux/connector.h>

// our headers
#include "../include/ipc.h"
#include "../include/blocks.h"

#define NETLINK_CONNECTOR       11

/* Hopefully your userspace connector.h matches this kernel */
#define CN_TEST_IDX             CN_NETLINK_USERS + 3
#define CN_TEST_VAL             0x456

// TODO: SIGPIPE handler for socket?

// used to kill the main while loop
static int need_exit;

int format(char * filename, int size) {
	int ret;
	superblock_t * sb;
	FILE * diskFile;
	int bytes;
	unsigned char * zeroBlock;
	int i;

	// innitially set the return value to no error
	ret = 1;
	
	// init superblock
	// clears out a superblock structure
	// if this fails we can not format the disk so we exit
	sb = (superblock_t *) calloc(1, sizeof(superblock_t));
	if(sb == NULL) {
		ret = 0;
		goto out;
	}
	
	// open the disk file in write only binary mode
	// if we can not open the file, throw an error and return
	diskFile = fopen(filename, "wb"); // create or overwrite
	if(diskFile == NULL) {
		ret = 0;
		goto out;
	}
	
	// write superblock out to file
	// this will take up the first BLOCKSIZE bytes of the file
	// the superblock should have reserved bytes to make it BLOCKSIZE
	bytes = fwrite((const void *) sb, sizeof(unsigned char), sizeof(sb), diskFile);
	if(bytes != sizeof(sb)) {
		ret = 0;
		fclose(diskFile);
		goto out;
	}
	
	// writes out zero blocks up until size in KB
	// clears a block of memory that is BLOCKSIZE
	// writes multiple zeroBlocks to file up to size
	zeroBlock = (unsigned char *) calloc(BLOCK_SIZE, sizeof(unsigned char));
	if(zeroBlock == NULL) {
		ret = 0;
		fclose(diskFile);
		goto out;
	}
	for(i = BLOCK_SIZE; i < size; i += BLOCK_SIZE) {
		bytes = fwrite(zeroBlock, sizeof(unsigned char), sizeof(zeroBlock), diskFile);
	}

	// closes the file and errors out because we are just going to open this file again in a bit
	if(fclose(diskFile)) {
		ret = 0;
	}

out:
	return ret;
}

static int netlink_send(int s, struct cn_msg *msg) {
	static uint32_t seq;
	struct nlmsghdr *nlh;
	unsigned int size;
	int err;
	char buf[2048];
	struct cn_msg *m;

	/* call was this
	char buf[1024];
	int len;
	struct cn_msg *data;

	memset(buf, 0, sizeof(buf));

	data = (struct cn_msg *)buf;

	data->id.idx = CN_TEST_IDX;
	data->id.val = CN_TEST_VAL;
	data->seq = seq++;
	data->ack = 0;
	data->len = 0;

	len = netlink_send(s, data);
	*/

	// this was actually the ucon fuction part

	size = NLMSG_SPACE(sizeof(struct cn_msg) + msg->len);

	nlh = (struct nlmsghdr *)buf;
	nlh->nlmsg_seq = seq++;
	nlh->nlmsg_pid = getpid();
	nlh->nlmsg_type = NLMSG_DONE;
	nlh->nlmsg_len = size;
	nlh->nlmsg_flags = 0;

	m = NLMSG_DATA(nlh);
	memcpy(m, msg, sizeof(*m) + msg->len);

	err = send(s, nlh, size, 0);
	if (err == -1)
		printf("Failed to send: %s [%d].\n", strerror(errno), errno);

	return err;
}

int do_work(unsigned char * buffer) {
	// seek address
	if(fseek(diskFile, address * BLOCK_SIZE, SEEK_SET) != 0) {
		return -1;
	}

	// decode opcode to decide what work we should perform
	// the opcode is the firt byte in the buffer
	switch(opcode) {
		case OPCODE_READ:
			// get data from disk
			//netlink_send(s, block);
			break;
		case OPCODE_WRITE:
			// fwrite buffer to diskFile
			break;
		case OPCODE_NOTIFY:
			printf("Info sjfs said: %s\n", buffer);
			break;
		default:
			printf("Warn unknown opcode.\n");
			break;
	}

	// TODO: make this function actually do something
	return 0;
}

int main(int argc, char *argv[]) {
	// for protection
	int pid_file;
	int rc;

	// for getopts and file
	int c;
	int size;
	char * filename;
	FILE * diskFile;

	// for netlink socket
	int s;
	unsigned char buf[2048];
	int len;
	struct nlmsghdr *reply;
	struct sockaddr_nl l_local;
	struct cn_msg *data;
	struct pollfd pfd;

	// protection
	// ---------------------------------------------------------------------------------------------
	// only allow a single instance of this program to be run at any given time

	pid_file = open("/var/run/sjfs_helper.pid", O_CREAT | O_RDWR, 0666);
	rc = flock(pid_file, LOCK_EX | LOCK_NB);
	if(rc) {
		if(EWOULDBLOCK == errno) {
			exit(EXIT_FAILURE);
		}
	}

	// parse command line options
	// ---------------------------------------------------------------------------------------------
	// Usage: sjfs_helper [-n <size>] <filename>
	size = 0;

	while((c = getopt(argc, argv, "n:")) != -1) {
		switch(c) {
			case 'n':
				size = strtol(optarg, &optarg, 10);
				if(errno == ERANGE || size < MIN_BLOCKS) {
					printf("Usage: sjfs_helper [-n <size>] <filename>\n");
					printf("         -n <size>  create a new disk of size in KB");
					exit(EXIT_FAILURE);
				}
				break;
			default:
				printf("Usage: sjfs_helper [-n <size>] <filename>\n");
				printf("         -n <size>  create a new disk of size in KB");
				exit(EXIT_FAILURE);
				break;
		}
	}
	
	if(optind >= argc) {
		printf("Usage: sjfs_helper [-n <size>] <filename>\n");
		printf("         -n <size>  create a new disk of size in KB");
		exit(EXIT_FAILURE);
	}
	
	filename = argv[optind];
	
	// Format if creating new disk
	// ---------------------------------------------------------------------------------------------
	if(size) {
		if(format(filename, size) == 0) {
			perror("Could not format disk.");
			exit(EXIT_FAILURE);
		}
	}
	
	// init fd
	diskFile = fopen(filename, "a+b"); // TODO: Find the right mode for this.
	if(diskFile == NULL) {
		perror("Error opening file");
		exit(EXIT_FAILURE);
	}

	// create socket
	// ---------------------------------------------------------------------------------------------
	memset(buf, 0, sizeof(buf));

	s = socket(PF_NETLINK, SOCK_DGRAM, NETLINK_CONNECTOR);
	if (s == -1) {
		perror("socket");
		fclose(diskFile);
		exit(EXIT_FAILURE);
	}

	l_local.nl_family = AF_NETLINK;
	l_local.nl_groups = -1; /* bitmask of requested groups */
	l_local.nl_pid = 0;

	printf("subscribing to %u.%u\n", CN_TEST_IDX, CN_TEST_VAL);

	if (bind(s, (struct sockaddr *)&l_local, sizeof(struct sockaddr_nl)) == -1) {
		perror("bind");
		close(s);
		fclose(diskFile);
		exit(EXIT_FAILURE);
	}

	pfd.fd = s;

	// LOOP
	// ---------------------------------------------------------------------------------------------
	while(!need_exit) {
		pfd.events = POLLIN;
		pfd.revents = 0;
		switch (poll(&pfd, 1, -1)) {
			case 0:
				need_exit = 1;
				break;
			case -1:
				if (errno != EINTR) {
					need_exit = 1;
					break;
				}
				continue;
		}
		if (need_exit)
			break;

		memset(buf, 0, sizeof(buf));
		len = recv(s, buf, sizeof(buf), 0);
		if (len == -1) {
			perror("recv buf");
			continue; // none fatal error, we just skip is packet
		}
		reply = (struct nlmsghdr *)buf;

		switch (reply->nlmsg_type) {
			case NLMSG_ERROR:
				printf("Warn message received.\n");
				break;
			case NLMSG_DONE:
				data = (struct cn_msg *)NLMSG_DATA(reply);

				printf("[%x.%x] [%08u.%08u].\n", data->id.idx, data->id.val, data->seq, data->ack);

				// do something with data
				// TODO: fix this call to only pass the payload
				if(do_work(data) < 0) {
					printf("Warn failed to do_work.");
				}
				break;
			default:
				printf("Warn unknown nlmsg_type.\n");
				break;
		}
	} // LOOP END

	// cleanup (unreachable?)
	// ---------------------------------------------------------------------------------------------
	if(close(s) == -1) { // TODO: check return cond
		perror("s close");
	}

	fclose(diskFile);

	return EXIT_SUCCESS;
}

