#include "sjfs.h"

MODULE_LICENSE("Proprietary");
MODULE_AUTHOR("Farbod Jahan <>");
MODULE_AUTHOR("Alex Kennedy <alexzanderkennedy@gmail.com>");
MODULE_AUTHOR("Patrick-Daniel Llanes <>");
MODULE_AUTHOR("Jonathan Lo <>");
MODULE_AUTHOR("Onyema Ude <>");
MODULE_DESCRIPTION("San Jose Filesystem");
MODULE_VERSION("0:1.0.1");

struct dentry * sjfs_iops_lookup(struct inode *,struct dentry *, unsigned int) { printk("sjfs_iops_lookup\n"); return NULL; }
const char * sjfs_iops_follow_link(struct dentry *, void **) { printk("sjfs_iops_follow_link\n"); return NULL; }
int sjfs_iops_permission(struct inode *, int) { printk("sjfs_iops_permission\n"); return NULL; }
struct posix_acl * sjfs_iops_posix_acl(struct inode *, int) { printk("sjfs_iops_posix_acl\n"); return NULL; }
int sjfs_iops_readlink(struct dentry *, char __user *,int) { printk("sjfs_iops_readlink\n"); return NULL; }
void sjfs_iops_put_link(struct inode *, void *) { printk("sjfs_iops_put_link\n"); }
int sjfs_iops_create(struct inode *,struct dentry *, umode_t, bool) { printk("sjfs_iops_create\n"); return NULL; }
int sjfs_iops_link(struct dentry *,struct inode *,struct dentry *) { printk("sjfs_iops_link\n"); return NULL; }
int sjfs_iops_unlink(struct inode *,struct dentry *) { printk("sjfs_iops_unlink\n"); return NULL; }
int sjfs_iops_symlink(struct inode *,struct dentry *,const char *) { printk("sjfs_iops_symlink\n"); return NULL; }
int sjfs_iops_mkdir(struct inode *,struct dentry *,umode_t) { printk("sjfs_iops_mkdir\n"); return NULL; }
int sjfs_iops_rmdir(struct inode *,struct dentry *) { printk("sjfs_iops_rmdir\n"); return NULL; }
int sjfs_iops_mknod(struct inode *,struct dentry *,umode_t,dev_t) { printk("sjfs_iops_mknod\n"); return NULL; }
int sjfs_iops_rename(struct inode *, struct dentry *, struct inode *, struct dentry *) { printk("sjfs_iops_rename\n"); return NULL; }
int sjfs_iops_rename2(struct inode *, struct dentry *, struct inode *, struct dentry *, unsigned int) { printk("sjfs_iops_rename2\n"); return NULL; }
int sjfs_iops_setattr(struct dentry *, struct iattr *) { printk("sjfs_iops_setattr\n"); return NULL; }
int sjfs_iops_getattr(struct vfsmount *mnt, struct dentry *, struct kstat *) { printk("sjfs_iops_getattr\n"); return NULL; }
int sjfs_iops_setxattr(struct dentry *, const char *,const void *,size_t,int) { printk("sjfs_iops_setxattr\n"); return NULL; }
ssize_t sjfs_iops_getxattr(struct dentry *, const char *, void *, size_t) { printk("sjfs_iops_getxattr\n"); return NULL; }
ssize_t sjfs_iops_listxattr(struct dentry *, char *, size_t) { printk("sjfs_iops_listxattr\n"); return NULL; }
int sjfs_iops_removexattr(struct dentry *, const char *) { printk("sjfs_iops_removexattr\n"); return NULL; }
int sjfs_iops_fiemap(struct inode *, struct fiemap_extent_info *, u64 start, u64 len) { printk("sjfs_iops_fiemap\n"); return NULL; }
int sjfs_iops_update_time(struct inode *, struct timespec *, int) { printk("sjfs_iops_update_time\n"); return NULL; }
int sjfs_iops_atomic_open(struct inode *, struct dentry *, struct file *, unsigned open_flag, umode_t create_mode, int *opened) { printk("sjfs_iops_atomic_open\n"); return NULL; }
int sjfs_iops_tmpfile(struct inode *, struct dentry *, umode_t) { printk("sjfs_iops_tmpfile\n"); return NULL; }
int sjfs_iops_set_acl(struct inode *, struct posix_acl *, int) { printk("sjfs_iops_set_acl\n"); return NULL; }

struct inode_operations sjfs_iops {
	.lookup = sjfs_iops_lookup,
	.follow_link = sjfs_iops_follow_link,
	.permission = sjfs_iops_permission,
	.posix_acl = sjfs_iops_posix_acl,
	.readlink = sjfs_iops_readlink,
	.put_link = sjfs_iops_put_link,
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

loff_t sjfs_fops_llseek(struct file *, loff_t, int) { printk("sjfs_fops_llseek\n"); return NULL; }
ssize_t sjfs_fops_read(struct file *, char __user *, size_t, loff_t *) { printk("sjfs_fops_read\n"); return NULL; }
ssize_t sjfs_fops_write(struct file *, const char __user *, size_t, loff_t *) { printk("sjfs_fops_write\n"); return NULL; }
ssize_t sjfs_fops_read_iter(struct kiocb *, struct iov_iter *) { printk("sjfs_fops_read_iter\n"); return NULL; }
ssize_t sjfs_fops_write_iter(struct kiocb *, struct iov_iter *) { printk("sjfs_fops_write_iter\n"); return NULL; }
int sjfs_fops_iterate(struct file *, struct dir_context *) { printk("sjfs_fops_iterate\n"); return NULL; }
unsigned int sjfs_fops_poll(struct file *, struct poll_table_struct *) { printk("sjfs_fops_poll\n"); return NULL; }
long sjfs_fops_unlocked_ioctl(struct file *, unsigned int, unsigned long) { printk("sjfs_fops_unlocked_ioctl\n"); return NULL; }
long sjfs_fops_compat_ioctl(struct file *, unsigned int, unsigned long) { printk("sjfs_fops_compat_ioctl\n"); return NULL; }
int sjfs_fops_mmap(struct file *, struct vm_area_struct *) { printk("sjfs_fops_mmap\n"); return NULL; }
int sjfs_fops_mremap(struct file *, struct vm_area_struct *) { printk("sjfs_fops_mremap\n"); return NULL; }
int sjfs_fops_open(struct inode *, struct file *) { printk("sjfs_fops_open\n"); return NULL; }
int sjfs_fops_flush(struct file *, fl_owner_t id) { printk("sjfs_fops_flush\n"); return NULL; }
int sjfs_fops_release(struct inode *, struct file *) { printk("sjfs_fops_release\n"); return NULL; }
int sjfs_fops_fsync(struct file *, loff_t, loff_t, int datasync) { printk("sjfs_fops_fsync\n"); return NULL; }
int sjfs_fops_aio_fsync(struct kiocb *, int datasync) { printk("sjfs_fops_aio_fsync\n"); return NULL; }
int sjfs_fops_fasync(int, struct file *, int) { printk("sjfs_fops_fasync\n"); return NULL; }
int sjfs_fops_lock(struct file *, int, struct file_lock *) { printk("sjfs_fops_lock\n"); return NULL; }
ssize_t sjfs_fops_sendpage(struct file *, struct page *, int, size_t, loff_t *, int) { printk("sjfs_fops_sendpage\n"); return NULL; }
unsigned long sjfs_fops_get_unmapped_area(struct file *, unsigned long, unsigned long, unsigned long, unsigned long) { printk("sjfs_fops_get_unmapped_area\n"); return NULL; }
int sjfs_fops_check_flags(int) { printk("sjfs_fops_check_flags\n"); return NULL; }
int sjfs_fops_flock(struct file *, int, struct file_lock *) { printk("sjfs_fops_flock\n"); return NULL; }
ssize_t sjfs_fops_splice_write(struct pipe_inode_info *, struct file *, loff_t *, size_t, unsigned int) { printk("sjfs_fops_splice_write\n"); return NULL; }
ssize_t sjfs_fops_splice_read(struct file *, loff_t *, struct pipe_inode_info *, size_t, unsigned int) { printk("sjfs_fops_splice_read\n"); return NULL; }
int sjfs_fops_setlease(struct file *, long, struct file_lock **, void **) { printk("sjfs_fops_setlease\n"); return NULL; }
long sjfs_fops_fallocate(struct file *file, int mode, loff_t offset, loff_t len) { printk("sjfs_fops_fallocate\n"); return NULL; }
void sjfs_fops_show_fdinfo(struct seq_file *m, struct file *f) { printk("sjfs_fops_show_fdinfo\n"); }
unsigned sjfs_fops_mmap_capabilities(struct file *) { printk("sjfs_fops_mmap_capabilities\n"); return NULL; }

struct file_operations sjfs_fops {
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
	.mremap = sjfs_fops_mremap,
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
	.mmap_capabilities = sjfs_fops_mmap_capabilities,
};

// - super block level -----------------------------------------------------------------------------

struct super_operations sjfs_sops { // TODO: look up which functions we are required to fill
	.drop_inode		= generic_delete_inode,
	.statfs			= simple_statfs,
	.show_options	= generic_show_options,
};

int sjfs_fill_super(struct super_block *sb, void *data, int silent) {
	struct inode *inode;
	struct dentry *root;

	s->s_blocksize = PAGE_CACHE_SIZE;
	s->s_blocksize_bits = PAGE_CACHE_SHIFT;
	s->s_magic = SJFS_MAGIC;
	s->s_op = &sjfs_sops;
	s->s_time_gran = 1;
	
	save_mount_options(sb, data); // for generic_show_options

	inode = new_inode(s);
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

	s->s_root = root;
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

struct file_system_type sjfs_fs_type {
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
