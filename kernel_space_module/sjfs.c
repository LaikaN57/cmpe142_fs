#include "sjfs.h"

MODULE_LICENSE("Proprietary");
MODULE_AUTHOR("Farbod Jahan <>");
MODULE_AUTHOR("Alex Kennedy <alexzanderkennedy@gmail.com>");
MODULE_AUTHOR("Patrick-Daniel Llanes <>");
MODULE_AUTHOR("Jonathan Lo <>");
MODULE_AUTHOR("Onyema Ude <>");
MODULE_DESCRIPTION("San Jose Filesystem");
MODULE_VERSION("0:1.0.1");

// - socket level --------------------------------------------------------------------------

// reads a block from disk (handles all the socket calling)
int sjfs_read_block(unsigned int address, (unsigned char *) block) {
	return -1
}

// writes a block to disk (handles all the socket calling)
int sjfs_write_block(unsigned int address, (unsigned char *) block) {
	return -1;
}

// reads a buffer from disk (this should handle all of the block looping)
int sjfs_read_data(unsigned int offset, (unsigned char *) buffer) {
	return -1;
}

// writes a buffer to disk (this should handle all of the block looping)
int sjfs_write_data(unsigned int offset, (unsigned char *) buffer) {
	return -1;
}

// - syscall level -----------------------------------------------------------------------------------

struct dentry * sjfs_iops_lookup(struct inode *i,struct dentry *d, unsigned int ui) { printk("sjfs_iops_lookup\n"); return NULL; }
const char * sjfs_iops_follow_link(struct dentry *d, void **v) { printk("sjfs_iops_follow_link\n"); return NULL; }
int sjfs_iops_permission(struct inode *inode, int mask) {
	printk("sjfs_iops_permission -> generic_permission(i=%lu, m=%#010x)\n", inode->i_ino, mask);
	
	return generic_permission(inode, mask);
}
struct posix_acl * sjfs_iops_posix_acl(struct inode *i, int a) { printk("sjfs_iops_posix_acl\n"); return NULL; }
int sjfs_iops_readlink(struct dentry *d, char __user *u,int i) { printk("sjfs_iops_readlink\n"); return 0; }
void sjfs_iops_put_link(struct inode *i, void *v) { printk("sjfs_iops_put_link\n"); }
int sjfs_iops_create(struct inode *i,struct dentry *d, umode_t u, bool b) { printk("sjfs_iops_create\n"); return 0; }
int sjfs_iops_link(struct dentry *d,struct inode *i,struct dentry *d2) { printk("sjfs_iops_link\n"); return 0; }
int sjfs_iops_unlink(struct inode *i,struct dentry *d) { printk("sjfs_iops_unlink\n"); return 0; }
int sjfs_iops_symlink(struct inode *i,struct dentry *d,const char *c) { printk("sjfs_iops_symlink\n"); return 0; }
int sjfs_iops_mkdir(struct inode *i,struct dentry *d,umode_t u) { printk("sjfs_iops_mkdir\n"); return 0; }
int sjfs_iops_rmdir(struct inode *i,struct dentry *d) { printk("sjfs_iops_rmdir\n"); return 0; }
int sjfs_iops_mknod(struct inode *i,struct dentry *d,umode_t u,dev_t d2) { printk("sjfs_iops_mknod\n"); return 0; }
int sjfs_iops_rename(struct inode *i, struct dentry *d, struct inode *i2, struct dentry *d2) { printk("sjfs_iops_rename\n"); return 0; }
int sjfs_iops_rename2(struct inode *i, struct dentry *d, struct inode *i2, struct dentry *d2, unsigned int ui) { printk("sjfs_iops_rename2\n"); return 0; }
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
int sjfs_iops_setxattr(struct dentry *dentry, const char *name,const void *value, size_t size, int flags) { printk("sjfs_iops_setxattr\n"); return 0; }
ssize_t sjfs_iops_getxattr(struct dentry *dentry, const char *name, void *value, size_t size) { printk("sjfs_iops_getxattr\n"); return 0; }
ssize_t sjfs_iops_listxattr(struct dentry *dentry, char *list, size_t size) { printk("sjfs_iops_listxattr\n"); return 0; }
int sjfs_iops_removexattr(struct dentry *dentry, const char *name) { printk("sjfs_iops_removexattr\n"); return 0; }
int sjfs_iops_fiemap(struct inode *i, struct fiemap_extent_info *f, u64 start, u64 len) { printk("sjfs_iops_fiemap\n"); return 0; }
int sjfs_iops_update_time(struct inode *i, struct timespec *t, int a) { printk("sjfs_iops_update_time\n"); return 0; }
int sjfs_iops_atomic_open(struct inode *dir, struct dentry *dentry, struct file *file, unsigned open_flag, umode_t create_mode, int *opened) {
	printk("sjfs_iops_atomic_open(i=%lu, d=\"%s\", of=%#010x, cm=%#06x, o=%#010x)\n", dir->i_ino, (&(dentry->d_name))->name, open_flag, create_mode, *opened);

	return 0;
}
int sjfs_iops_tmpfile(struct inode *i, struct dentry *d, umode_t t) { printk("sjfs_iops_tmpfile\n"); return 0; }
int sjfs_iops_set_acl(struct inode *i, struct posix_acl *p, int a) { printk("sjfs_iops_set_acl\n"); return 0; }

struct inode_operations sjfs_iops = {
	.lookup = sjfs_iops_lookup, // Found in github/ms version (simple)
	//.follow_link = sjfs_iops_follow_link,
	.permission = sjfs_iops_permission,
	//.posix_acl = sjfs_iops_posix_acl,
	.readlink = sjfs_iops_readlink,
	//.put_link = sjfs_iops_put_link,
	.create = sjfs_iops_create, // Found in github/ms version
	.link = sjfs_iops_link, // Found in github/ms version (simple)
	.unlink = sjfs_iops_unlink, // Found in github/ms version (simple)
	.symlink = sjfs_iops_symlink,
	.mkdir = sjfs_iops_mkdir,
	.rmdir = sjfs_iops_rmdir,
	.mknod = sjfs_iops_mknod, // Found in github/ms version
	.rename = sjfs_iops_rename, // Found in github/ms version (simple)
	.rename2 = sjfs_iops_rename2,
	.setattr = sjfs_iops_setattr, // simple (need to rewrite, see todo comment in def)
	.getattr = sjfs_iops_getattr, // simple
	.setxattr = sjfs_iops_setxattr,
	.getxattr = sjfs_iops_getxattr,
	.listxattr = sjfs_iops_listxattr,
	.removexattr = sjfs_iops_removexattr,
	.fiemap = sjfs_iops_fiemap,
	.update_time = sjfs_iops_update_time,
	.atomic_open = sjfs_iops_atomic_open,
	.tmpfile = sjfs_iops_tmpfile,
	.set_acl = sjfs_iops_set_acl,
};

loff_t sjfs_fops_llseek(struct file *f, loff_t l, int i) { printk("sjfs_fops_llseek\n"); return 0; }
ssize_t sjfs_fops_read(struct file *f, char __user *u, size_t s, loff_t *l) { printk("sjfs_fops_read\n"); return 0; }
ssize_t sjfs_fops_write(struct file *f, const char __user *u, size_t s, loff_t *l) { printk("sjfs_fops_write\n"); return 0; }
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

struct file_operations sjfs_fops = {
	.owner = THIS_MODULE,
	.llseek = sjfs_fops_llseek,
	.read = sjfs_fops_read, // Found in both github versions
	.write = sjfs_fops_write, // Found in both github versions
	.read_iter = sjfs_fops_read_iter,
	.write_iter = sjfs_fops_write_iter,
	.iterate = sjfs_fops_iterate,
	.poll = sjfs_fops_poll,
	.unlocked_ioctl = sjfs_fops_unlocked_ioctl,
	.compat_ioctl = sjfs_fops_compat_ioctl,
	.mmap = sjfs_fops_mmap,
	//.mremap = sjfs_fops_mremap,
	.open = sjfs_fops_open,
	.flush = sjfs_fops_flush,
	.release = sjfs_fops_release,
	.fsync = sjfs_fops_fsync,
	.aio_fsync = sjfs_fops_aio_fsync,
	.fasync = sjfs_fops_fasync,
	.lock = sjfs_fops_lock,
	.sendpage = sjfs_fops_sendpage,
	.get_unmapped_area = sjfs_fops_get_unmapped_area,
	.check_flags = sjfs_fops_check_flags,
	.flock = sjfs_fops_flock,
	.splice_write = sjfs_fops_splice_write,
	.splice_read = sjfs_fops_splice_read,
	.setlease = sjfs_fops_setlease,
	.fallocate = sjfs_fops_fallocate,
	.show_fdinfo = sjfs_fops_show_fdinfo,
	//.mmap_capabilities = sjfs_fops_mmap_capabilities,
};

// - root directory level --------------------------------------------------------------------------

struct inode_operations sjfs_root_dir_iops {
	.lookup = sjfs_iops_lookup,
};

struct file_operations sjfs_root_dir_fops {
	.owner = THIS_MODULE,
	.read = sjfs_fops_read,
	.write = sjfs_fops_write,
	.open = sjfs_fops_open,
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

	sb->s_maxbytes = MAX_LFS_FILESIZE;
	sb->s_blocksize = PAGE_CACHE_SIZE;
	sb->s_blocksize_bits = PAGE_CACHE_SHIFT;
	sb->s_magic = SJFS_MAGIC;
	sb->s_op = &sjfs_sops;
	sb->s_time_gran = 1;
	
	save_mount_options(sb, data); // for generic_show_options

	inode = new_inode(sb);
	if (!inode)
		return -ENOMEM;

	inode->i_ino = 1;
	inode->i_mode = S_IFDIR | S_IRWXU | S_IRGRP | S_IXGRP | S_IROTH | S_IXOTH; // octal bitmask of file type and permissions 040755
	inode->i_atime = inode->i_mtime = inode->i_ctime = CURRENT_TIME;
	inode->i_op = &sjfs_root_dir_iops;
	inode->i_fop = &sjfs_root_dir_fops;

	set_nlink(inode, 2);

	root = d_make_root(inode);
	if (!root)
		return -ENOMEM;

	sb->s_root = root;
	return 0;
}

// - filesystem type level -------------------------------------------------------------------------

struct dentry * sjfs_mount(struct file_system_type *fs_type, int flags, const char *dev_name, void *data) {
	// start up our link to the userspace app?
	
	// mount to a single place with no device
	return mount_single(fs_type, flags, data, sjfs_fill_super);
}

void sjfs_kill_sb(struct super_block *sb) {
	// used for in memory fs
	kill_litter_super(sb);
	
	// shutdown our link to the userspace app?
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

	if (test_and_set_bit(0, &once))
		return -EBUSY;

	// start user mode application
	//call_usermodehelper("/usr/local/bin/sjfs_helper", NULL, NULL, 0);

	// register the filesystem
	return register_filesystem(&sjfs_fs_type);
}

void __exit sjfs_exit(void) {
	// unregister the filesystem
	unregister_filesystem(&sjfs_fs_type);
}

module_init(sjfs_init);
module_exit(sjfs_exit);
