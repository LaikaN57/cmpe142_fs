#ifndef IPC_H
#define IPC_H

#define SJFS_OPCODE_READ	0
#define SJFS_OPCODE_WRITE	1

typedef struct {
	unsigned char opcode;
	unsigned long inode;
	unsigned int pos;
	long long count;
} sjfs_request_packet_t;

typedef struct {
	long long count;
	unsigned char data[0];
} sjfs_response_packet_t;

#endif

