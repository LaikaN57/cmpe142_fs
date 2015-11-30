#ifndef IPC_H
#define IPC_H

typedef struct {
	unsigned char opcode;
	unsigned long address;
	unsigned char * data;
} sjfs_message;

#define OPCODE_READ	0
#define OPCODE_WRITE	1
#define OPCODE_NOTIFY	2

#endif

