#include <linux/fs.h>
#include <linux/pagemap.h>
#include <linux/highmem.h>
#include <linux/time.h>
#include <linux/init.h>
#include <linux/string.h>
#include <linux/backing-dev.h>
#include <linux/sched.h>
#include <linux/parser.h>
#include <linux/magic.h>
#include <linux/slab.h>
#include <asm/uaccess.h>
#include <linux/mm.h>

#define SJFS_MAGIC	0x534A5346

static const struct inode_operations sjfs_dir_inode_operations;

static const struct file_operations sjfs_file_operations = {
	.read_iter	= generic_file_read_iter,
	.write_iter	= generic_file_write_iter,
	.mmap		= generic_file_mmap,
	.fsync		= noop_fsync,
	.splice_read	= generic_file_splice_read,
	.splice_write	= iter_file_splice_write,
	.llseek		= generic_file_llseek,
};

static const struct inode_operations sjfs_file_inode_operations = {
	.setattr	= simple_setattr,
	.getattr	= simple_getattr,
};

static const struct address_space_operations sjfs_aops = {
	.readpage	= simple_readpage,
	.write_begin	= simple_write_begin,
	.write_end	= simple_write_end,
	.set_page_dirty	= __set_page_dirty_no_writeback,
};

struct inode *sjfs_get_inode(struct super_block *sb, const struct inode *dir, umode_t mode, dev_t dev) {
	struct inode * inode = new_inode(sb);

	if (inode) {
		inode->i_ino = get_next_ino();
		inode_init_owner(inode, dir, mode);
		inode->i_mapping->a_ops = &sjfs_aops;
		mapping_set_gfp_mask(inode->i_mapping, GFP_HIGHUSER);
		mapping_set_unevictable(inode->i_mapping);
		inode->i_atime = inode->i_mtime = inode->i_ctime = CURRENT_TIME;
	
		switch (mode & S_IFMT) {
			case S_IFREG:
				inode->i_op = &sjfs_file_inode_operations;
				inode->i_fop = &sjfs_file_operations;
				break;
				
			case S_IFDIR:
				inode->i_op = &sjfs_dir_inode_operations;
				inode->i_fop = &simple_dir_operations;

				inc_nlink(inode);
				break;
				
			case S_IFLNK:
				inode->i_op = &page_symlink_inode_operations;
				break;
			
			default:
				init_special_inode(inode, mode, dev);
				break;
		}
	}

	return inode;
}

static int sjfs_mknod(struct inode *dir, struct dentry *dentry, umode_t mode, dev_t dev) {
	struct inode * inode = sjfs_get_inode(dir->i_sb, dir, mode, dev);
	int error = -ENOSPC;

	if (inode) {
		d_instantiate(dentry, inode);
		dget(dentry);
		error = 0;
		dir->i_mtime = dir->i_ctime = CURRENT_TIME;
	}

	return error;
}

static int sjfs_mkdir(struct inode * dir, struct dentry * dentry, umode_t mode) {
	int retval = sjfs_mknod(dir, dentry, mode | S_IFDIR, 0);
	if (!retval) {
		inc_nlink(dir);

	}
	return retval;
}

static int sjfs_create(struct inode *dir, struct dentry *dentry, umode_t mode, bool excl) {
	return sjfs_mknod(dir, dentry, mode | S_IFREG, 0);
}

static int sjfs_symlink(struct inode * dir, struct dentry *dentry, const char * symname) {
	struct inode *inode;
	int error = -ENOSPC;

	inode = sjfs_get_inode(dir->i_sb, dir, S_IFLNK|S_IRWXUGO, 0);
	if (inode) {
		int l = strlen(symname)+1;
		error = page_symlink(inode, symname, l);
		if (!error) {
			d_instantiate(dentry, inode);
			dget(dentry);
			dir->i_mtime = dir->i_ctime = CURRENT_TIME;
		} else {
			iput(inode);
		}
	}

	return error;
}

static const struct inode_operations sjfs_dir_inode_operations = {
	.create		= sjfs_create,
	.lookup		= simple_lookup,
	.link		= simple_link,
	.unlink		= simple_unlink,
	.symlink	= sjfs_symlink,
	.mkdir		= sjfs_mkdir,
	.rmdir		= simple_rmdir,
	.mknod		= sjfs_mknod,
	.rename		= simple_rename,
};

static const struct super_operations sjfs_ops = {
	.statfs		= simple_statfs,
	.drop_inode	= generic_delete_inode,
	.show_options	= generic_show_options,
};

int sjfs_fill_super(struct super_block *sb, void *data, int silent) {
	struct inode *inode;
	int err;

	save_mount_options(sb, data);

	sb->s_maxbytes		= MAX_LFS_FILESIZE;
	sb->s_blocksize		= PAGE_CACHE_SIZE;
	sb->s_blocksize_bits	= PAGE_CACHE_SHIFT;
	sb->s_magic		= SJFS_MAGIC;
	sb->s_op		= &sjfs_ops;
	sb->s_time_gran		= 1;

	inode = sjfs_get_inode(sb, NULL, S_IFDIR | 0755, 0);
	sb->s_root = d_make_root(inode);
	if (!sb->s_root) {
		return -ENOMEM;
	}

	return 0;
}

struct dentry *sjfs_mount(struct file_system_type *fs_type, int flags, const char *dev_name, void *data) {
	return mount_nodev(fs_type, flags, data, sjfs_fill_super);
}

static void sjfs_kill_sb(struct super_block *sb) {
	kill_litter_super(sb);
}

static struct file_system_type sjfs_fs_type = {
	.name		= "sjfs",
	.fs_flags	= FS_USERNS_MOUNT,
	.mount		= sjfs_mount,
	.kill_sb	= sjfs_kill_sb,
	.owner		= THIS_MODULE,
};

static int __init init_sjfs_fs(void) {
	static unsigned long once;

	if(test_and_set_bit(0, &once)) {
		return 0;
	}

	return register_filesystem(&sjfs_fs_type);
}

static void __exit exit_sjfs_fs(void) {
	unregister_filesystem(&sjfs_fs_type);
}

module_init(init_sjfs_fs);
module_exit(exit_sjfs_fs);

