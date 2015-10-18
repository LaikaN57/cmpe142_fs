#include "sjfs.h"

MODULE_LICENSE("Proprietary");
MODULE_AUTHOR("Farbod Jahan <>");
MODULE_AUTHOR("Alex Kennedy <alexzanderkennedy@gmail.com>");
MODULE_AUTHOR("Patrick-Daniel Llanes <>");
MODULE_AUTHOR("Jonathan Lo <>");
MODULE_AUTHOR("Onyema Ude <>");
MODULE_DESCRIPTION("San Jose Filesystem");
MODULE_VERSION("0:1.0.1");


// - super block level -----------------------------------------------------------------------------

int sjfs_fill_super(struct super_block *sb, void *data, int silent) {
	
}

// - filesystem type level -------------------------------------------------------------------------

struct dentry * sjfs_mount(struct file_system_type *fs_type, int flags, const char *dev_name, void *data) {
	// mount to a single place with no device
	return mount_single(fs_type, flags, data, sjfs_fill_super);
}

void sjfs_kill_sb(struct super_block *sb) {
	// used for in memory fs
	kill_litter_super(sb);
}

struct file_system_type sjfs_fs_type {
	.name		= "sjfs";
	.mount		= sjfs_mount;
	.kill_sb	= sjfs_kill_sb;
	.owner		= THIS_MODULE;
};

// - module level ----------------------------------------------------------------------------------

int __init_or_module sjfs_init(void) {
	// register the filesystem
	return register_filesystem(&sjfs_fs_type);
}

void __exit sjfs_exit(void) {
	// unmount any filesystems
	// unregister the filesystem
	unregister_filesystem(&sjfs_fs_type);
}

module_init(sjfs_init);
module_exit(sjfs_exit);