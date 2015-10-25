#include "sjfs.h"

MODULE_LICENSE("Proprietary");
MODULE_AUTHOR("Farbod Jahan <>");
MODULE_AUTHOR("Alex Kennedy <alexzanderkennedy@gmail.com>");
MODULE_AUTHOR("Patrick-Daniel Llanes <>");
MODULE_AUTHOR("Jonathan Lo <>");
MODULE_AUTHOR("Onyema Ude <>");
MODULE_DESCRIPTION("San Jose Filesystem");
MODULE_VERSION("0:1.0.1");

struct dentry * sjfs_iops_lookup(struct inode *i,struct dentry *d, unsigned int ui) { printk("sjfs_iops_lookup\n"); return NULL; }
const char * sjfs_iops_follow_link(struct dentry *d, void **v) { printk("sjfs_iops_follow_link\n"); return NULL; }
int sjfs_iops_permission(struct inode *i, int a) { printk("sjfs_iops_permission\n"); return 0; }
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
int sjfs_iops_setattr(struct dentry *d, struct iattr *i) { printk("sjfs_iops_setattr\n"); return 0; }
int sjfs_iops_getattr(struct vfsmount *mnt, struct dentry *d, struct kstat *k) { printk("sjfs_iops_getattr\n"); return 0; }
int sjfs_iops_setxattr(struct dentry *d, const char *c,const void *v,size_t s,int i) { printk("sjfs_iops_setxattr\n"); return 0; }
ssize_t sjfs_iops_getxattr(struct dentry *d, const char *c, void *v, size_t s) { printk("sjfs_iops_getxattr\n"); return NULL; }
ssize_t sjfs_iops_listxattr(struct dentry *d, char *c, size_t s) { printk("sjfs_iops_listxattr\n"); return NULL; }
int sjfs_iops_removexattr(struct dentry *d, const char *c) { printk("sjfs_iops_removexattr\n"); return 0; }
int sjfs_iops_fiemap(struct inode *i, struct fiemap_extent_info *f, u64 start, u64 len) { printk("sjfs_iops_fiemap\n"); return 0; }
int sjfs_iops_update_time(struct inode *i, struct timespec *t, int a) { printk("sjfs_iops_update_time\n"); return 0; }
int sjfs_iops_atomic_open(struct inode *i, struct dentry *d, struct file *f, unsigned open_flag, umode_t create_mode, int *opened) { printk("sjfs_iops_atomic_open\n"); return 0; }
int sjfs_iops_tmpfile(struct inode *i, struct dentry *d, umode_t t) { printk("sjfs_iops_tmpfile\n"); return 0; }
int sjfs_iops_set_acl(struct inode *i, struct posix_acl *p, int a) { printk("sjfs_iops_set_acl\n"); return 0; }

struct inode_operations sjfs_iops = {
	.lookup = sjfs_iops_lookup,
	//.follow_link = sjfs_iops_follow_link,
	.permission = sjfs_iops_permission,
	//.posix_acl = sjfs_iops_posix_acl,
	.readlink = sjfs_iops_readlink,
	//.put_link = sjfs_iops_put_link,
	.create = sjfs_iops_create,
	.link = sjfs_iops_link,
	.unlink = sjfs_iops_unlink,
	.symlink = sjfs_iops_symlink,
	.mkdir = sjfs_iops_mkdir,
	.rmdir = sjfs_iops_rmdir,
	.mknod = sjfs_iops_mknod,
	.rename = sjfs_iops_rename,
	.rename2 = sjfs_iops_rename2,
	.setattr = sjfs_iops_setattr,
	.getattr = sjfs_iops_getattr,
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

loff_t sjfs_fops_llseek(struct file *f, loff_t l, int i) { printk("sjfs_fops_llseek\n"); return NULL; }
ssize_t sjfs_fops_read(struct file *f, char __user *u, size_t s, loff_t *l) { printk("sjfs_fops_read\n"); return NULL; }
ssize_t sjfs_fops_write(struct file *f, const char __user *u, size_t s, loff_t *l) { printk("sjfs_fops_write\n"); return NULL; }
ssize_t sjfs_fops_read_iter(struct kiocb *k, struct iov_iter *i) { printk("sjfs_fops_read_iter\n"); return NULL; }
ssize_t sjfs_fops_write_iter(struct kiocb *k, struct iov_iter *i) { printk("sjfs_fops_write_iter\n"); return NULL; }
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
ssize_t sjfs_fops_sendpage(struct file *f, struct page *p, int i, size_t s, loff_t *l, int i2) { printk("sjfs_fops_sendpage\n"); return NULL; }
unsigned long sjfs_fops_get_unmapped_area(struct file *f, unsigned long ul, unsigned long ul2, unsigned long ul3, unsigned long ul4) { printk("sjfs_fops_get_unmapped_area\n"); return 0; }
int sjfs_fops_check_flags(int i) { printk("sjfs_fops_check_flags\n"); return 0; }
int sjfs_fops_flock(struct file *f, int i, struct file_lock *fl) { printk("sjfs_fops_flock\n"); return 0; }
ssize_t sjfs_fops_splice_write(struct pipe_inode_info *p, struct file *f, loff_t *l, size_t s, unsigned int ui) { printk("sjfs_fops_splice_write\n"); return NULL; }
ssize_t sjfs_fops_splice_read(struct file *f, loff_t *l, struct pipe_inode_info *p, size_t s, unsigned int ui) { printk("sjfs_fops_splice_read\n"); return NULL; }
int sjfs_fops_setlease(struct file *f, long l, struct file_lock **fl, void **v) { printk("sjfs_fops_setlease\n"); return 0; }
long sjfs_fops_fallocate(struct file *file, int mode, loff_t offset, loff_t len) { printk("sjfs_fops_fallocate\n"); return 0; }
void sjfs_fops_show_fdinfo(struct seq_file *m, struct file *f) { printk("sjfs_fops_show_fdinfo\n"); }
unsigned sjfs_fops_mmap_capabilities(struct file *f) { printk("sjfs_fops_mmap_capabilities\n"); return 0; }

struct file_operations sjfs_fops = {
	.owner = THIS_MODULE,
	.llseek = sjfs_fops_llseek,
	.read = sjfs_fops_read,
	.write = sjfs_fops_write,
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

//sjfs_iops_permission
//sjfs_iops_getattr
//sjfs_iops_atomic_open

// - super block level -----------------------------------------------------------------------------

struct super_operations sjfs_sops = { // TODO: look up which functions we are required to fill
	.drop_inode		= generic_delete_inode,
	.statfs			= simple_statfs,
	.show_options	= generic_show_options,
};

int sjfs_fill_super(struct super_block *sb, void *data, int silent) {
	struct inode *inode;
	struct dentry *root;

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
	inode->i_mode = S_IFDIR | 0755;
	inode->i_atime = inode->i_mtime = inode->i_ctime = CURRENT_TIME;
	inode->i_op = &sjfs_iops; // simple_dir_inode_operations
	inode->i_fop = &sjfs_fops; // simple_dir_operations

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
	// register the filesystem
	return register_filesystem(&sjfs_fs_type);
}

void __exit sjfs_exit(void) {
	// unregister the filesystem
	unregister_filesystem(&sjfs_fs_type);
}

module_init(sjfs_init);
module_exit(sjfs_exit);
