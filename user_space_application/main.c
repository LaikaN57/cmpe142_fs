// man 7 unix
// man 2 bind

#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <string.h>
#include <unistd.h>

#include "../include/ipc.h"

#include "blocks.h"

#define MY_SOCK_PATH	"/tmp/sjfs"
#define LISTEN_BACKLOG	50

// TODO: SIGPIPE handler for socket?

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
	zeroBlock = (unsigned char *) calloc(BLOCKSIZE, sizeof(unsigned char));
	if(zeroBlock == NULL) {
		ret = 0;
		fclose(diskFile);
		goto out;
	}
	for(i = BLOCKSIZE; i < size; i += BLOCKSIZE) {
		byres = fwrite(zeroBlock, sizeof(unsigned char), sizeof(zeroBlock), diskFile);
	}

	// closes the file and errors out because we are just going to open this file again in a bit
	if(fclose(diskFile)) {
		ret = 0;
	}

out:
	return ret;
}

int main(int argc, char *argv[]) {
	int c;
	int size;
	char * filename;
	FILE * diskFile;

	int sfd;
	int cfd;
	struct sockaddr_un my_addr;
	struct sockaddr_un peer_addr;
	socklen_t peer_addr_size;

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
	sfd = socket(AF_UNIX, SOCK_STREAM, 0);
	if(sfd == -1) {
		perror("socket");
		fclose(diskFile);
		exit(EXIT_FAILURE); // we cannot recover from this
	}

	// bind socket to pathname
	memset(&my_addr, 0, sizeof(struct sockaddr_un));
	my_addr.sun_family = AF_UNIX;
	strncpy(my_addr.sun_path, MY_SOCK_PATH, sizeof(my_addr.sun_path) - 1);
	if(bind(sfd, (struct sockaddr *) &my_addr, sizeof(struct sockaddr_un)) == -1) {
		perror("bind");
		exit(EXIT_FAILURE); // we cannot recover from this
	}

	// start listening for socket connections
	if(listen(sfd, LISTEN_BACKLOG) == -1) {
		perror("listen");
		exit(EXIT_FAILURE); // we cannot recover from this
	}

	// ACCEPT LOOP
	// ---------------------------------------------------------------------------------------------
	while(1) { // TODO: allow some kind of exit condition
		// accept a new connection
		memset(&peer_addr, 0, sizeof(struct sockaddr_un));
		peer_addr_size = sizeof(struct sockaddr_un);
		cfd = accept(sfd, (struct sockaddr *) &peer_addr, &peer_addr_size); // TODO: make this non-blocking for user input from console?
		if(cfd == -1) {
			perror("accept");
			continue; // do we need to do any other cleanup?
		}

		// RECV LOOP
		// ---------------------------------------------------------------------------------------------
		while(1) { // TODO: allow some kind of exit condition
			// read from socket
			
			// seek address
			if(fseek(diskFile, address * BLOCKSIZE, SEEK_SET) != 0) {
				continue;
			}
			
			switch(opcode) {
				case READ:
					break;
				case WRITE:
					break;
				default:
					break;
			}
		} // RECV LOOP END

		if(close(cfd) == -1) {
			perror("c close");
			continue;
		}
	} // ACCEPT LOOP END

	// cleanup (unreachable?)
	// ---------------------------------------------------------------------------------------------
	if(close(sfd) == -1) {
		perror("s close");
	}

	if(unlink(MY_SOCK_PATH) == -1) {
		perror("unlink");
	}

	fclose(diskFile);

	return EXIT_SUCCESS;
}

