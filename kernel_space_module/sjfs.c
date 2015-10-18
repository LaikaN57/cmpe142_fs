#include "sjfs.h"

MODULE_LICENSE("Proprietary");
MODULE_AUTHOR("Farbod Jahan <>");
MODULE_AUTHOR("Alex Kennedy <alexzanderkennedy@gmail.com>");
MODULE_AUTHOR("Patrick-Daniel Llanes <>");
MODULE_AUTHOR("Jonathan Lo <>");
MODULE_AUTHOR("Onyema Ude <>");
MODULE_DESCRIPTION("San Jose Filesystem");
MODULE_VERSION("0:1.0.1");

/*
struct inode_operations sjfs_iops {
	struct dentry * (*lookup) (struct inode *,struct dentry *, unsigned int);
	const char * (*follow_link) (struct dentry *, void **);
	int (*permission) (struct inode *, int);
	struct posix_acl * (*get_acl)(struct inode *, int);

	int (*readlink) (struct dentry *, char __user *,int);
	void (*put_link) (struct inode *, void *);

	int (*create) (struct inode *,struct dentry *, umode_t, bool);
	int (*link) (struct dentry *,struct inode *,struct dentry *);
	int (*unlink) (struct inode *,struct dentry *);
	int (*symlink) (struct inode *,struct dentry *,const char *);
	int (*mkdir) (struct inode *,struct dentry *,umode_t);
	int (*rmdir) (struct inode *,struct dentry *);
	int (*mknod) (struct inode *,struct dentry *,umode_t,dev_t);
	int (*rename) (struct inode *, struct dentry *, struct inode *, struct dentry *);
	int (*rename2) (struct inode *, struct dentry *, struct inode *, struct dentry *, unsigned int);
	int (*setattr) (struct dentry *, struct iattr *);
	int (*getattr) (struct vfsmount *mnt, struct dentry *, struct kstat *);
	int (*setxattr) (struct dentry *, const char *,const void *,size_t,int);
	ssize_t (*getxattr) (struct dentry *, const char *, void *, size_t);
	ssize_t (*listxattr) (struct dentry *, char *, size_t);
	int (*removexattr) (struct dentry *, const char *);
	int (*fiemap)(struct inode *, struct fiemap_extent_info *, u64 start, u64 len);
	int (*update_time)(struct inode *, struct timespec *, int);
	int (*atomic_open)(struct inode *, struct dentry *, struct file *, unsigned open_flag, umode_t create_mode, int *opened);
	int (*tmpfile) (struct inode *, struct dentry *, umode_t);
	int (*set_acl)(struct inode *, struct posix_acl *, int);

	/* WARNING: probably going away soon, do not use! */
};

const struct file_operations	*i_fop;
sjfs_fops // read / write
*/

// - super block level -----------------------------------------------------------------------------

struct super_operations sjfs_sops {
	.drop_inode		= generic_delete_inode,
	.statfs			= simple_statfs,
	.show_options	= generic_show_options,
};

int sjfs_fill_super(struct super_block *sb, void *data, int silent) {
	struct inode * inode;

	sb->s_magic	= 0x534A5346; // for funzies

	save_mount_options(sb, data); // just for generic_show_options

	sb->s_op	= &sjfs_sops;
	
	inode = new_inode(sb);
	inode->i_op		= &sjfs_iops;
	inode->i_fop	= &sjfs_fops;
	
	sb->s_root = d_make_root(inode);
	if(!sb->s_root) {
		return -1;
	}
	
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
	.mount		= sjfs_mount,
	.kill_sb	= sjfs_kill_sb, // just to keep everything abstracted to in-house functions
	.owner		= THIS_MODULE,
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