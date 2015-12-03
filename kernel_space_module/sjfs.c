#include "sjfs.h"

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Farbod Jahan <>");
MODULE_AUTHOR("Alex Kennedy <alexzanderkennedy@gmail.com>");
MODULE_AUTHOR("Patrick-Daniel Llanes <>");
MODULE_AUTHOR("Jonathan Lo <>");
MODULE_AUTHOR("Onyema Ude <>");
MODULE_DESCRIPTION("San Jose Filesystem");
MODULE_VERSION("0:1.0.1");

struct file_operations sjfs_file_fops;
struct inode_operations sjfs_file_iops;

// - socket level --------------------------------------------------------------------------

void cn_callback(struct cn_msg *msg, struct netlink_skb_parms *nsp) {
	// if we dont need a read, just do nothing
	if(!down_trylock(&cb_sem)) {
		up(&cb_sem); // FIXME: potential race
		return;
	}

	// copy message to read_block_buffer
	memcpy(read_block_buffer, (unsigned char *) msg->data, msg->len > SJFS_BLOCK_SIZE ? SJFS_BLOCK_SIZE : msg->len);

	up(&cb_sem);

	printk("cn_callback %s: idx=%x, val=%x, seq=%u, ack=%u, len=%d: %s.\n",
			__func__, msg->id.idx, msg->id.val, msg->seq,
			msg->ack, msg->len, msg->len ? (char *)msg->data : "");
}

// reads a block from disk (handles all the socket calling)
static int sjfs_read_block(unsigned int address, unsigned char * block) {
	struct cn_msg *m;
	unsigned char data[5];

	// TODO:fix this to actually do what is is supposed to
	m = kzalloc(sizeof(*m) + 5, GFP_ATOMIC);
	if (m) {
		memcpy(&m->id, &cn_id, sizeof(m->id));
		m->seq = cn_counter;
		m->len = scnprintf(data, sizeof(data), "%c%u", SJFS_OPCODE_READ, address) + 1;

		memcpy(m + 1, data, m->len);

		down(&cn_sem);
		down(&cb_sem);

		cn_netlink_send(m, 0, 0, GFP_ATOMIC);
	
		kfree(m);
	}

	cn_counter++;

	// TODO: make this call do a timeout
	down(&cb_sem); // returns when we get a callback
	up(&cb_sem); // imediate release of sem

	// copy read_block_buffer to block
	memcpy(block, read_block_buffer, SJFS_BLOCK_SIZE);
	up(&cn_sem);

	return 0;
}

// writes a block to disk (handles all the socket calling)
static int sjfs_write_block(unsigned int address, unsigned char * block) {
	down(&cn_sem);

	// send write info to user space

	up(&cn_sem);

	return 0;
}

// - data structures level -----------------------------------------------------------------------
static superblock_t * sjfs_get_disk_superblock(void) {
	unsigned char * block_zero_buffer;
	superblock_t * disk_sb;

	block_zero_buffer = kmalloc(sizeof(SJFS_BLOCK_SIZE), GFP_KERNEL);
	if(!block_zero_buffer || sjfs_read_block(0, block_zero_buffer) < 0) {
		return NULL;
	}

	disk_sb = kmalloc(sizeof(superblock_t), GFP_KERNEL);
	memcpy(disk_sb, block_zero_buffer, sizeof(superblock_t));

	kfree(block_zero_buffer);
	
	return disk_sb;
}

static inode_t * sjfs_get_disk_inode(unsigned int inode_number) {
	unsigned char * block_buffer;
	inode_t * disk_inode;

	block_buffer = kmalloc(SJFS_BLOCK_SIZE, GFP_KERNEL);
	// block number is going to be:
	// B0: super
	// B1: inode bitmap
	// B2: inodes: 0 to (BS/sizeof(inode_t)) - 1
	// B3: inodes: BS/sizeof(inode_t) to (2*BS/sizeof(inode_t)) - 1
	// Bn: inodes: ...
	// B = 2 + floor((ino * size) / BS)
	if(!block_buffer || sjfs_read_block(2 + ((inode_number * sizeof(inode_t)) / SJFS_BLOCK_SIZE), block_buffer) < 0) {
		return NULL;
	}
        disk_inode = kmalloc(sizeof(inode_t), GFP_KERNEL);
        memcpy(disk_inode, (block_buffer + (inode_number * sizeof(inode_t))), sizeof(inode_t));

        return disk_inode;
}

static void sjfs_set_disk_inode(struct inode * inode) {
	// copy things from inode to disk inode in i_private

	inode->i_ctime = CURRENT_TIME;
	((inode_t *) inode->i_private)->ctime = inode->i_ctime.tv_sec;

	// read the block from disk
	// save the inode
	// write the block to disk
}

// - fops functions -----------------------------------------------------------------------------------------

loff_t sjfs_fops_llseek(struct file *f, loff_t l, int i) { printk("sjfs_fops_llseek\n"); return 0; }
ssize_t sjfs_fops_read(struct file *file, char __user *to, size_t count, loff_t *ppos) {
	printk("sjfs_fops_read\n");
	
	// see simple_read_from_buffer where "from" is our user space app.

	return 0;
}
ssize_t sjfs_fops_write(struct file *f, const char __user *from, size_t count, loff_t *ppos) {
	printk("sjfs_fops_write\n");
	
	// see simple_write_to_buffer where "to" is our user space app.
	
	return 0;
}
ssize_t sjfs_fops_read_iter(struct kiocb *k, struct iov_iter *i) { printk("sjfs_fops_read_iter\n"); return 0; }
ssize_t sjfs_fops_write_iter(struct kiocb *k, struct iov_iter *i) { printk("sjfs_fops_write_iter\n"); return 0; }
int sjfs_fops_iterate(struct file *dir, struct dir_context *ctx) { printk("sjfs_fops_iterate\n"); return 0; }
unsigned int sjfs_fops_poll(struct file *f, struct poll_table_struct *p) { printk("sjfs_fops_poll\n"); return 0; }
long sjfs_fops_unlocked_ioctl(struct file *f, unsigned int ui, unsigned long ul) { printk("sjfs_fops_unlocked_ioctl\n"); return 0; }
long sjfs_fops_compat_ioctl(struct file *f, unsigned int ui, unsigned long ul) { printk("sjfs_fops_compat_ioctl\n"); return 0; }
int sjfs_fops_mmap(struct file *f, struct vm_area_struct *v) { printk("sjfs_fops_mmap\n"); return 0; }
int sjfs_fops_mremap(struct file *f, struct vm_area_struct *v) { printk("sjfs_fops_mremap\n"); return 0; }
int sjfs_fops_open(struct inode *f, struct file *f2) { printk("sjfs_fops_open\n"); return 0; }
int sjfs_fops_flush(struct file *f, fl_owner_t id) { printk("sjfs_fops_flush\n"); return 0; }
int sjfs_fops_release(struct inode *i, struct file *f) { printk("sjfs_fops_release\n"); return 0; }
int sjfs_fops_fsync(struct file *f, loff_t l, loff_t l2, int datasync) { printk("sjfs_fops_fsync\n"); return 0; }
int sjfs_fops_aio_fsync(struct kiocb *k, int datasync) { printk("sjfs_fops_aio_fsync\n"); return 0; }
int sjfs_fops_fasync(int i, struct file *f, int i2) { printk("sjfs_fops_fasync\n"); return 0; }
int sjfs_fops_lock(struct file *f, int i, struct file_lock *fl) { printk("sjfs_fops_lock\n"); return 0; }
ssize_t sjfs_fops_sendpage(struct file *f, struct page *p, int i, size_t s, loff_t *l, int i2) { printk("sjfs_fops_sendpage\n"); return 0; }
unsigned long sjfs_fops_get_unmapped_area(struct file *f, unsigned long ul, unsigned long ul2, unsigned long ul3, unsigned long ul4) { printk("sjfs_fops_get_unmapped_area\n"); return 0; }
int sjfs_fops_check_flags(int i) { printk("sjfs_fops_check_flags\n"); return 0; }
int sjfs_fops_flock(struct file *f, int i, struct file_lock *fl) { printk("sjfs_fops_flock\n"); return 0; }
ssize_t sjfs_fops_splice_write(struct pipe_inode_info *p, struct file *f, loff_t *l, size_t s, unsigned int ui) { printk("sjfs_fops_splice_write\n"); return 0; }
ssize_t sjfs_fops_splice_read(struct file *f, loff_t *l, struct pipe_inode_info *p, size_t s, unsigned int ui) { printk("sjfs_fops_splice_read\n"); return 0; }
int sjfs_fops_setlease(struct file *f, long l, struct file_lock **fl, void **v) { printk("sjfs_fops_setlease\n"); return 0; }
long sjfs_fops_fallocate(struct file *file, int mode, loff_t offset, loff_t len) { printk("sjfs_fops_fallocate\n"); return 0; }
void sjfs_fops_show_fdinfo(struct seq_file *m, struct file *f) { printk("sjfs_fops_show_fdinfo\n"); }
unsigned sjfs_fops_mmap_capabilities(struct file *f) { printk("sjfs_fops_mmap_capabilities\n"); return 0; }

// - iopc level --------------------------------------------------------------------------------------

// root directory ONLY iops
struct dentry * sjfs_iops_lookup(struct inode *dir,struct dentry *dentry, unsigned int flags) {
        printk("sjfs_iops_lookup\n");

        return NULL;
}
int sjfs_iops_create(struct inode *dir,struct dentry *dentry, umode_t mode, bool excl) {
        printk("sjfs_iops_create\n");

        return 0;
}
int sjfs_iops_unlink(struct inode *dir,struct dentry *dentry) {
        printk("sjfs_iops_unlink\n");

        return 0;
}
int sjfs_iops_rename(struct inode *old_dir, struct dentry *old_dentry, struct inode *new_dir, struct dentry *new_dentry) {
        printk("sjfs_iops_rename\n");

        return 0;
}

// shared iops
int sjfs_iops_permission(struct inode *inode, int mask) {
        printk("sjfs_iops_permission -> generic_permission(i=%lu, m=%#010x)\n", inode->i_ino, mask);

        //TODO: set permissions in memroy and on disk
        return generic_permission(inode, mask);
}
int sjfs_iops_setattr(struct dentry *dentry, struct iattr *iattr) {
        printk("sjfs_iops_setattr -> simple_setattr (for testing only)\n");

        // TODO: rewrite this to save the attributes back to user space
        return simple_setattr(dentry, iattr);
}
int sjfs_iops_getattr(struct vfsmount *mnt, struct dentry *dentry, struct kstat *stat) {
        printk("sjfs_iops_getattr -> simple_getattr\n");

        // TODO: i think we can just ignore this because we can get the attributes from the in-mem inode
        return simple_getattr(mnt, dentry, stat);
}
int sjfs_iops_update_time(struct inode *inode, struct timespec *time, int flags) {
        printk("sjfs_iops_update_time\n");

        // TODO: set whatever time in memory and on disk
        return 0;
}

// - file level -----------------------------------------------------------------------------------

struct file_operations sjfs_file_fops = {
	.owner = THIS_MODULE,			// yes - informational
	.llseek = sjfs_fops_llseek,
	.read = sjfs_fops_read,			// yes - for read call on self
	.write = sjfs_fops_write,		// yes - for write call on self
	.poll = sjfs_fops_poll,
	.open = sjfs_fops_open,			// yes - for open call on self
	.release = sjfs_fops_release,		// yes - for last close call on self
	.lock = sjfs_fops_lock,
	.check_flags = sjfs_fops_check_flags,
	.flock = sjfs_fops_flock,
	.setlease = sjfs_fops_setlease,
	.fallocate = sjfs_fops_fallocate,
	.show_fdinfo = sjfs_fops_show_fdinfo,
};

struct inode_operations sjfs_file_iops = {
        .permission = sjfs_iops_permission,     // for () changes own permissions
        .setattr = sjfs_iops_setattr,           // for (chmod) change own attributes
        .getattr = sjfs_iops_getattr,           // for (stat) get own attributes
        .update_time = sjfs_iops_update_time,   // for () changes own times
};

// - root directory level --------------------------------------------------------------------------

struct file_operations sjfs_root_dir_fops = {
        .owner = THIS_MODULE,			// yes - informational
        .llseek = sjfs_fops_llseek,
        .read = sjfs_fops_read,                 // yes - for read call
        .write = sjfs_fops_write,               // yes - for write call
        .iterate = sjfs_fops_iterate,           // yes - for readdir call (ls) read self
        .poll = sjfs_fops_poll,
        .open = sjfs_fops_open,                 // yes - for open call
        .flush = sjfs_fops_flush,               // yes - for close call
        .release = sjfs_fops_release,           // yes - for last close call?
        .lock = sjfs_fops_lock,
        .check_flags = sjfs_fops_check_flags,
        .flock = sjfs_fops_flock,
        .setlease = sjfs_fops_setlease,
        .fallocate = sjfs_fops_fallocate,
        .show_fdinfo = sjfs_fops_show_fdinfo,
};

struct inode_operations sjfs_root_dir_iops = {
        .lookup = sjfs_iops_lookup,             // yes - for () finds a child file
        .permission = sjfs_iops_permission,     // yes - for () changes own permissions
        .create = sjfs_iops_create,             // yes - for open call (touch) creates a child file
        .unlink = sjfs_iops_unlink,             // yes - for unlink call (rm) unlinks a child file
        .rename = sjfs_iops_rename,             // yes - for rename call (mv?) renames a child file
        .setattr = sjfs_iops_setattr,           // yes - for (chmod) changes own attributes
        .getattr = sjfs_iops_getattr,           // yes - for (stat) gets own attributes
        .update_time = sjfs_iops_update_time,   // yes - for () changes own times
};

// - super block level -----------------------------------------------------------------------------

// TODO: look up which functions we are required to fill
struct super_operations sjfs_sops = {
	.drop_inode		= generic_delete_inode,
	.statfs			= simple_statfs,
	.show_options	= generic_show_options,
};

int sjfs_fill_super(struct super_block *sb, void *data, int silent) {
	struct inode *inode;
	struct dentry *root;
	superblock_t * disk_sb;
	inode_t * disk_root_inode;

	disk_sb = sjfs_get_disk_superblock();
	if(!disk_sb) {
		return -ENOMEM;
	}

	// statically set info
	sb->s_blocksize_bits	= 8;
	sb->s_blocksize		= SJFS_BLOCKSIZE;
	sb->s_maxbytes		= MAX_LFS_FILESIZE;
        sb->s_time_gran		= 1;

        sb->s_op                = &sjfs_sops; // operations

	// dynamic inits based on the on-disk super block
	if(disk_sb->magic != SJFS_MAGIC) {
		return -ENOMEM;
	}
	sb->s_magic	= disk_sb->magic;
	sb->s_fs_info	= disk_sb;

	save_mount_options(sb, data); // for generic_show_options

	// save the inodes bitmap to memory
	inodes_bitmap_cache = kalloc(SJFS_BLOCKSIZE, GFP_KERNEL);
	sjsfs_read_block(1, inodes_bitmap_cache); // for now we only have 1 inode bitmap to support 8k files

	// read each datablock bitmap into the datablocks bitmap cache
	datablocks_bitmap_cache = kalloc(SJFS_BLOCKSIZE*data_blocks_count, GFP_KERNEL);
	for(i = 2 + SJFS_BLOCKSIZE*8; i < (2 + SJFS_BLOCKSIZE*8) + data_blocks_count; i++) {
		sjfs_read_block(i, datablocks_bitmaps_cache + (i - (2 + SJFS_BLOCKSIZE*8)) * SJFS_BLOCKSIZE);
	}

	// get and make our root inode
	disk_root_inode = sjfs_get_disk_inode(0);
	if(!disk_root_inode || !(*inodes_bitmap_cache >> 7)) {
		return -ENOMEM;
	}

	inode = new_inode(sb);
	if (!inode)
		return -ENOMEM;

	// statically set info for root directory inode
	inode->i_ino	= 0;

	inode->i_op	= &sjfs_root_dir_iops; // root dir operations
        inode->i_fop	= &sjfs_root_dir_fops;

	// dynamically set based on the on-disk inode
	inode->i_mode	= disk_root_inode->mode; // S_IFDIR | S_IRWXU | S_IRGRP | S_IXGRP | S_IROTH | S_IXOTH
	i_uid_write(inode, disk_root_inode->uid);
	i_gid_write(inode, disk_root_inode->gid);
	i_size_write(inode, disk_root_inode->size);
	inode->i_atime.tv_sec	= disk_root_inode->atime;
        inode->i_mtime.tv_sec	= disk_root_inode->mtime;
        inode->i_ctime.tv_sec	= disk_root_inode->ctime;
	set_nlink(inode, disk_root_inode->links);
	inode->i_blocks	= disk_root_inode->blocks;
	inode->i_private = disk_root_inode;

	root = d_make_root(inode);
	if (!root)
		return -ENOMEM;

	sb->s_root = root;
	return 0;
}

// - filesystem type level -------------------------------------------------------------------------

struct dentry * sjfs_mount(struct file_system_type *fs_type, int flags, const char *dev_name, void *data) {
	int err;

	// start up our link to the userspace app
	err = cn_add_callback(&cn_id, cn_name, cn_callback);
	if(err) {
		if(nls && nls->sk_socket) {
			sock_release(nls->sk_socket);
		}
		return NULL;
	}

	// mount to a single place with no device
	return mount_single(fs_type, flags, data, sjfs_fill_super);
}

void sjfs_kill_sb(struct super_block *sb) {
	// used for in memory fs
	kill_litter_super(sb);
	
	// shutdown our link to the userspace app
	cn_del_callback(&cn_id);
	if(nls && nls->sk_socket) {
		sock_release(nls->sk_socket);
	}
} 

struct file_system_type sjfs_fs_type = {
	.name		= "sjfs",
	.fs_flags	= 0,
	.mount		= sjfs_mount,
	.kill_sb	= sjfs_kill_sb,
	.owner		= THIS_MODULE,
	.next		= NULL,
};

// - module level ----------------------------------------------------------------------------------

int __init_or_module sjfs_init(void) {
	static unsigned long once;
	char *helper_argv[] = {
			"/usr/local/bin/sjfs_helper",
			"/tmp/sjfs.img",
			NULL
	};

	if (test_and_set_bit(0, &once))
		return -EBUSY;

	sema_init(&cn_sem, 1);
	sema_init(&cb_sem, 1);

	// start user mode application
	call_usermodehelper(helper_argv[0], helper_argv, NULL, UMH_WAIT_EXEC);

	// register the filesystem
	return register_filesystem(&sjfs_fs_type);
}

void __exit sjfs_exit(void) {
	// unregister the filesystem
	unregister_filesystem(&sjfs_fs_type);
}

module_init(sjfs_init);
module_exit(sjfs_exit);

