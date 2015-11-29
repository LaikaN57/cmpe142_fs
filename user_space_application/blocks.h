/* Userspace "disk" library
 *
 * This will hold structures for superblocks and inodes on "disk".
 *
 *
 */

#ifndef BLOCKS_H
#define BLOCKS_H

#define BLOCK_SIZE		(1024)
#define INODE_SIZE		(sizeof(inode_t))
#define POINTER_SIZE	(4)

#define MIN_BLOCKS	(6)

#define MAX_INODES		(BLOCK_SIZE * 8)
#define MAX_DATABLOCKS_PER_INODE (66062)

typedef struct {
	unsigned short inodes_count;
	unsigned short blocks_count;
	unsigned long mtime;
	unsigned long wtime;
	unsigned long mnt_count;
	unsigned long magic;
	unsigned char dirty;
	unsigned char reserved[];
} superblock_t;

typedef struct {
	unsigned short mode; // type and permissions
	unsigned short uid;
	unsigned short gid;
	unsigned long size; // actual file size
	unsigned long atime;
	unsigned long ctime;
	unsigned long mtime;
	unsigned short links; // not used yet
	unsigned short blocks; // number of data blocks pointed to
	unsigned long dbp[12]; // direct block pointers
	unsigned long sidbp; // single depth block pointer
	unsigned long didbp; // double depth block pointer
	unsigned char reserved[26]; // fill to 2**x = 64 bytes
} inode_t;

typedef struct {
	unsigned long pointers[BLOCK_SIZE / POINTER_SIZE]; // 256 pointers
} indirect_pointer_block_t;

// please don't load the whole freakin disk into memory
/*
typedef struct {
	superblock_t superblock;						// 1 block
	unsigned char inodes_bitmap[BLOCK_SIZE];		// 1 block
	inode_t inodes[MAX_INODES];						// 512 blocks (16 inodes/block or 8192 inodes)
	unsigned char datablocks_bitmap[MAX_DATABLOCKS_PER_INODE][BLOCK_SIZE];	// y block
	unsigned char datablocks[MAX_INODES*MAX_DATABLOCKS_PER_INODE][BLOCK_SIZE];	// z blocks
	unsigned char reserved[BLOCK_SIZE];				// 1 block
} disk_t;
*/


// reading directory contents

typedef struct {
	unsigned long inode_number;
	unsigned char name_len;
	char * name;
} directory_entry_t;

typedef struct {
	unsigned short entries_len;
	directory_entry_t * entries;
} directory_t;

#endif

