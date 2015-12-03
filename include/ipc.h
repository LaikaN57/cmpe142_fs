#ifndef IPC_H
#define IPC_H

typedef struct {
	unsigned char opcode;
	unsigned long address;
	unsigned char * data;
} sjfs_message;

#define SJFS_OPCODE_READ	0
#define SJFS_OPCODE_WRITE	1
#define SJFS_OPCODE_NOTIFY	2

#endif

