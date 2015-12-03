#ifndef _SJFS_H_
#define _SJFS_H_

#include <linux/module.h>
#include <linux/init.h>
#include <linux/fs.h>
#include <linux/dcache.h>
#include <linux/pagemap.h>

// used for netlink connector
#include <linux/connector.h>

// used for semeaphores
#include <linux/semaphore.h>

// our includes
#include "../include/ipc.h"
#include "../include/blocks.h"

#define SJFS_MAGIC	0x534A5346

// netlink connector socket stuff
static struct cb_id cn_id = { CN_NETLINK_USERS + 3, 0x456 };
static char cn_name[] = "sjfs";
static struct sock *nls;
static u32 cn_counter;

// netlink socket semaphore for read/write block
static struct semaphore cn_sem;
static struct semaphore cb_sem;

// use for block reads
static unsigned char read_block_buffer[1024];

#endif
