#include "sjfs.h"

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Farbod Jahan <>");
MODULE_AUTHOR("Alex Kennedy <alexzanderkennedy@gmail.com>");
MODULE_AUTHOR("Patrick-Daniel Llanes <>");
MODULE_AUTHOR("Jonathan Lo <>");
MODULE_AUTHOR("Onyema Ude <>");
MODULE_DESCRIPTION("San Jose Filesystem");
MODULE_VERSION("0:1.0.1");

static unsigned char read_block_buffer[1024];

// dummy function for netlink socket
void cn_callback(struct cn_msg *msg, struct netlink_skb_parms *nsp) {
	printk("cn_callback %s: idx=%x, val=%x, seq=%u, ack=%u, len=%d: %s.\n", __func__, msg->id.idx, msg->id.val,
			msg->seq, msg->ack, msg->len, msg->len ? (char *)msg->data : "");

	memcpy(read_block_buffer, (unsigned char *) msg->data + 5, SJFS_BLOCK_SIZE);
}

// - socket level --------------------------------------------------------------------------

// TODO: this is only a test function
static int send_notify(void) {
	// insert code to send data to user space app
	return 0;
}
// reads a block from disk (handles all the socket calling)
static int sjfs_read_block(unsigned int address, unsigned char * block) {
	down(&cn_sem);
	down(&cb_sem);

	// send read info to user space

	// TODO: make this call do a timeout
	down(&cb_sem); // returns when we get a callback
	up(&cb_sem); // imediate release of sem

	// copy read_block_buffer to block
	memcpy(block, read_block_buffer, 1024);
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

// reads a buffer from disk (this should handle all of the block looping)
static int sjfs_read_buffer(unsigned int offset, unsigned int len, unsigned char * buffer) {
	return -1;
}

// writes a buffer to disk (this should handle all of the block looping)
static int sjfs_write_buffer(unsigned int offset, unsigned int len, unsigned char * buffer) {
	return -1;
}

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

// - file level -----------------------------------------------------------------------------------

/*
const char * sjfs_iops_follow_link(struct dentry *d, void **v) { printk("sjfs_iops_follow_link\n"); return NULL; }
struct posix_acl * sjfs_iops_get_acl(struct inode *i, int a) { printk("sjfs_iops_posix_acl\n"); return NULL; }
int sjfs_iops_readlink(struct dentry *d, char __user *u,int i) { printk("sjfs_iops_readlink\n"); return 0; }
void sjfs_iops_put_link(struct inode *i, void *v) { printk("sjfs_iops_put_link\n"); }
int sjfs_iops_link(struct dentry *d,struct inode *i,struct dentry *d2) { printk("sjfs_iops_link\n"); return 0; }
int sjfs_iops_symlink(struct inode *i,struct dentry *d,const char *c) { printk("sjfs_iops_symlink\n"); return 0; }
int sjfs_iops_mkdir(struct inode *i,struct dentry *d,umode_t u) { printk("sjfs_iops_mkdir\n"); return 0; }
int sjfs_iops_rmdir(struct inode *i,struct dentry *d) { printk("sjfs_iops_rmdir\n"); return 0; }
int sjfs_iops_mknod(struct inode *i,struct dentry *d,umode_t u,dev_t d2) { printk("sjfs_iops_mknod\n"); return 0; }
int sjfs_iops_rename2(struct inode *i, struct dentry *d, struct inode *i2, struct dentry *d2, unsigned int flags) { printk("sjfs_iops_rename2\n"); return 0; }
int sjfs_iops_setxattr(struct dentry *dentry, const char *name,const void *value, size_t size, int flags) { printk("sjfs_iops_setxattr\n"); return 0; }
ssize_t sjfs_iops_getxattr(struct dentry *dentry, const char *name, void *value, size_t size) { printk("sjfs_iops_getxattr\n"); return 0; }
ssize_t sjfs_iops_listxattr(struct dentry *dentry, char *list, size_t size) { printk("sjfs_iops_listxattr\n"); return 0; }
int sjfs_iops_removexattr(struct dentry *dentry, const char *name) { printk("sjfs_iops_removexattr\n"); return 0; }
int sjfs_iops_fiemap(struct inode *i, struct fiemap_extent_info *f, u64 start, u64 len) { printk("sjfs_iops_fiemap\n"); return 0; }
int sjfs_iops_atomic_open(struct inode *dir, struct dentry *d, struct file *fil, unsigned f, umode_t c, int *o) { printk("sjfs_iops_atomic_open\n"); return 0; }
int sjfs_iops_tmpfile(struct inode *i, struct dentry *d, umode_t t) { printk("sjfs_iops_tmpfile\n"); return 0; }
int sjfs_iops_set_acl(struct inode *i, struct posix_acl *p, int a) { printk("sjfs_iops_set_acl\n"); return 0; }
*/

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

        return generic_permission(inode, mask);
}
int sjfs_iops_setattr(struct dentry *dentry, struct iattr *iattr) {
        printk("sjfs_iops_setattr -> simple_setattr (for testing only)\n");

        // TODO: rewrite this to save the attributes back to user space
        return simple_setattr(dentry, iattr);
}
int sjfs_iops_getattr(struct vfsmount *mnt, struct dentry *dentry, struct kstat *stat) {
        printk("sjfs_iops_getattr -> simple_getattr\n");

        // enentually calls generic_fillattr() in fs/stat.c
        // simply copies inode data to stat data
        return simple_getattr(mnt, dentry, stat);
}
int sjfs_iops_update_time(struct inode *inode, struct timespec *time, int flags) {
	printk("sjfs_iops_update_time\n");
	
	return 0;
}

struct inode_operations sjfs_file_iops = {
	.permission = sjfs_iops_permission,	// for () changes own permissions
	.setattr = sjfs_iops_setattr,		// for (chmod) change own attributes
	.getattr = sjfs_iops_getattr,		// for (stat) get own attributes
	.update_time = sjfs_iops_update_time,	// for () changes own times
};

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
int sjfs_fops_iterate(struct file *f, struct dir_context *d) { printk("sjfs_fops_iterate\n"); return 0; }
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

// - root directory level --------------------------------------------------------------------------

struct inode_operations sjfs_root_dir_iops = {
        .lookup = sjfs_iops_lookup,             // yes - for () finds a child file 
        .permission = sjfs_iops_permission,	// yes - for () changes own permissions
        .create = sjfs_iops_create,             // yes - for open call (touch) creates a child file
        .unlink = sjfs_iops_unlink,             // yes - for unlink call (rm) unlinks a child file
        .rename = sjfs_iops_rename,             // yes - for rename call (mv?) renames a child file
        .setattr = sjfs_iops_setattr,           // yes - for (chmod) changes own attributes
        .getattr = sjfs_iops_getattr,           // yes - for (stat) gets own attributes
        .update_time = sjfs_iops_update_time,	// yes - for () changes own times
};

struct file_operations sjfs_root_dir_fops = {
        .owner = THIS_MODULE,			// yes - informational
        .llseek = sjfs_fops_llseek,
        .read = sjfs_fops_read,                 // yes - for read call
        .write = sjfs_fops_write,               // yes - for write call
        .iterate = sjfs_fops_iterate,           // yes - read dir
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

// - super block level -----------------------------------------------------------------------------
/*
struct inode *sjfs_sops_alloc_inode(struct super_block *sb) { printk("sjfs_sops_alloc_inode\n"); return NULL; };
void sjfs_sops_destroy_inode(struct inode *i) { printk("sjfs_sops_destroy_inode\n"); return; };
void sjfs_sops_dirty_inode(struct inode *i, int flags) { printk("sjfs_sops_dirty_inode\n"); return; };
int sjfs_sops_write_inode(struct inode *i, struct writeback_control *wbc) { printk("sjfs_sops_write_inode\n"); return 0; };
int sjfs_sops_drop_inode(struct inode *i) { printk("sjfs_sops_drop_inode\n"); return 0; };
void sjfs_sops_evict_inode(struct inode *i) { printk("sjfs_sops_evict_inode\n"); return; };
void sjfs_sops_put_super(struct super_block *sb) { printk("sjfs_sops_put_super\n"); return; };
int sjfs_sops_sync_fs(struct super_block *sb, int wait) { printk("sjfs_sops_sync_fs\n"); return 0; };
int sjfs_sops_freeze_super(struct super_block *sb) { printk("sjfs_sops_freeze_super\n"); return 0; };
int sjfs_sops_freeze_fs(struct super_block *sb) { printk("sjfs_sops_freeze_fs\n"); return 0; };
int sjfs_sops_thaw_super(struct super_block *sb) { printk("sjfs_sops_thaw_super\n"); return 0; };
int sjfs_sops_unfreeze_fs(struct super_block *sb) { printk("sjfs_sops_unfreeze_fs\n"); return 0; };
int sjfs_sops_statfs(struct dentry *d, struct kstatfs *s) { printk("sjfs_sops_statfs\n"); return 0; };
int sjfs_sops_remount_fs(struct super_block *sb, int *i, char *c) { printk("sjfs_sops_remount_fs\n"); return 0; };
void sjfs_sops_umount_begin(struct super_block *sb) { printk("sjfs_sops_umount_begin\n"); return; };
int sjfs_sops_show_options(struct seq_file *s, struct dentry *d) { printk("sjfs_sops_show_options\n"); return 0; };
int sjfs_sops_show_devname(struct seq_file *s, struct dentry *d) { printk("sjfs_sops_show_devname\n"); return 0; };
int sjfs_sops_show_path(struct seq_file *s, struct dentry *d) { printk("sjfs_sops_show_path\n"); return 0; };
int sjfs_sops_show_stats(struct seq_file *s, struct dentry *d) { printk("sjfs_sops_show_stats\n"); return 0; };
ssize_t sjfs_sops_quota_read(struct super_block *sb, int i, char *c, size_t s, loff_t l) { printk("sjfs_sops_quota_read\n"); return 0; };
ssize_t sjfs_sops_quota_write(struct super_block *sb, int i, const char *c, size_t s, loff_t l) { printk("sjfs_sops_quota_write\n"); return 0; };
struct dquot **sjfs_sops_get_dquots(struct inode *i) { printk("sjfs_sops_get_dquots\n"); return NULL; };
int sjfs_sops_bdev_try_to_free_page(struct super_block*sb, struct page* p, gfp_t g) { printk("sjfs_sops_bdev_try_to_free_page\n"); return 0; };
long sjfs_sops_nr_cached_objects(struct super_block *sb, struct shrink_control *sc) { printk("sjfs_sops_nr_cached_objects\n"); return 0; };
long sjfs_sops_free_cached_objects(struct super_block *sb, struct shrink_control *sc) { printk("sjfs_sops_free_cached_objects\n"); return 0; };
*/
struct super_operations sjfs_sops = { // TODO: look up which functions we are required to fill
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
	sb->s_blocksize		= SJFS_BLOCK_SIZE;
	sb->s_maxbytes		= MAX_LFS_FILESIZE;
        sb->s_time_gran		= 1;

        sb->s_op                = &sjfs_sops; // operations

	// dynamic inits based on the on-disk super block
	sb->s_magic	= disk_sb->magic;
	sb->s_fs_info	= disk_sb;

	save_mount_options(sb, data); // for generic_show_options

	disk_root_inode = sjfs_get_disk_inode(0);

	if(!disk_root_inode) {
		return -ENOMEM;
	}

	inode = new_inode(sb);
	if (!inode)
		return -ENOMEM;

	// statically set info for root directory inode
	inode->i_ino	= 0;
	set_nlink(inode, 2);

	inode->i_op	= &sjfs_root_dir_iops; // root dir operations
        inode->i_fop	= &sjfs_root_dir_fops;

	// dynamically set based on the on-disk inode
	inode->i_mode	= disk_root_inode->mode; // S_IFDIR | S_IRWXU | S_IRGRP | S_IXGRP | S_IROTH | S_IXOTH
	i_uid_write(inode, disk_root_inode->uid);
	i_gid_write(inode, disk_root_inode->gid);
	i_size_write(inode, disk_root_inode->size);
	//TODO: fix casting these
	inode->i_atime.tv_sec	= disk_root_inode->atime;
        inode->i_mtime.tv_sec	= disk_root_inode->mtime;
        inode->i_ctime.tv_sec	= disk_root_inode->ctime;
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
	// TODO: replace with actual callback code
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

