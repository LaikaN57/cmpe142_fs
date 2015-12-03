/* Userspace "disk" library
 *
 * This will hold structures for superblocks and inodes on "disk".
 *
 *
 */

#ifndef BLOCKS_H
#define BLOCKS_H

#define SJFS_BLOCK_SIZE		(1024)
#define INODE_SIZE		(sizeof(inode_t))
#define POINTER_SIZE	(4)

#define MIN_BLOCKS	(6)

#define MAX_INODES		(BLOCK_SIZE * 8)
#define MAX_DATABLOCKS_PER_INODE (66062)

typedef struct {
	/* Holds the total number of inodes. The inode bitmap block will show which inodes are in use.
	Maybe this show actually be the number of inode bitmap blocks. */
	unsigned short inodes_count;
	/* Same as inodes_count but for data blocks */
	unsigned long data_blocks_count;
	// last mount time
	unsigned long mtime;
	// last write time for any inode, inode bitmap, or datablock bitmap
	unsigned long wtime;
	// total number of times mounted
	unsigned long mnt_count;
	// should always be equal to SJFS_MAGIC
	unsigned long magic;
	/* Notes if we did not successfully unmount. set on mount and clear as last op in unmount. */
	unsigned char dirty;
	// just round out the block to make things easy for memcpy() and malloc().
	unsigned char reserved[SJFS_BLOCK_SIZE - 23];
} superblock_t;

typedef struct {
	unsigned short mode; // type (S_IFREG or S_IFDIR) and permissions (u:rwx g:rwx w:rwx)
	unsigned short uid; // owning user id
	unsigned short gid; // owning group id
	unsigned long size; // actual file size in bytes
	unsigned long atime; // last data access time (not including metadata)
	unsigned long ctime; // create time
	unsigned long mtime; // last data modification time (not including metadata)
	unsigned short links; // not used yet? (1 for files, 2 for dir)
	unsigned short blocks; // number of data blocks pointed to (not counting dbs used as indirect pointers)
	unsigned long dbp[12]; // direct block pointers
	unsigned long sidbp; // single depth block pointer
	unsigned long didbp; // double depth block pointer
	unsigned char reserved[26]; // fill to 2**x = 64 bytes
} inode_t;

typedef struct {
	// list of pointers, abstrated for fun
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
	char * name; // name of file in dir
} directory_entry_t;

typedef struct {
	unsigned short entries_len;
	directory_entry_t * entries; // list of files in a dir
} directory_t;

#endif

