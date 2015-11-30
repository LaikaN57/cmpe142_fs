#ifndef _SJFS_H_
#define _SJFS_H_

#include <linux/module.h>
#include <linux/init.h>
#include <linux/fs.h>
#include <linux/dcache.h>
#include <linux/pagemap.h>
#include <linux/connector.h>

#include "../include/ipc.h"
#include "../include/blocks.h"

#define SJFS_MAGIC	0x534A5346

static struct cb_id cn_id = { CN_NETLINK_USERS + 3, 0x456 };
static char cn_name[] = "sjfs";
static struct sock *nls;

#endif
