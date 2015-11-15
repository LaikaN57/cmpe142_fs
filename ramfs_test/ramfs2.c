#include <linux/module.h>
#include <linux/init.h>
#include <linux/fs.h>
#include <linux/pagemap.h>
#include <linux/highmem.h>
#include <linux/time.h>
#include <linux/init.h>
#include <linux/string.h>
#include <linux/backing-dev.h>
#include <linux/mm.h>
#include <linux/sched.h>
#include <linux/parser.h>
#include <linux/magic.h>
#include <linux/slab.h>
#include <asm/uaccess.h>

#define SJFS_MAGIC	0x534A5346

MODULE_LICENSE("Proprietary");
MODULE_AUTHOR("Farbod Jahan <>");
MODULE_AUTHOR("Alex Kennedy <alexzanderkennedy@gmail.com>");
MODULE_AUTHOR("Patrick-Daniel Llanes <>");
MODULE_AUTHOR("Jonathan Lo <>");
MODULE_AUTHOR("Onyema Ude <>");
MODULE_DESCRIPTION("San Jose Filesystem");
MODULE_VERSION("0:1.1.1");

ssize_t sjfs_fops_read_iter(struct kiocb *k, struct iov_iter *i) {
	printk("sjfs_fops_read_iter\n");
	return generic_file_read_iter(k, i);
}
ssize_t sjfs_fops_write_iter(struct kiocb *k, struct iov_iter *i) {
	printk("sjfs_fops_write_iter\n");
	return generic_file_write_iter(k, i);
}
int sjfs_fops_mmap(struct file *f, struct vm_area_struct *v) {
	printk("sjfs_fops_mmap\n");
	return generic_file_mmap(f, v);
}
int sjfs_fops_fsync(struct file *f, loff_t l, loff_t l2, int datasync) {
	printk("sjfs_fops_fsync\n");
	return noop_fsync(f, l, l2, datasync);
}
ssize_t sjfs_fops_splice_read(struct file *f, loff_t *l, struct pipe_inode_info *p, size_t s, unsigned int ui) {
	printk("sjfs_fops_splice_read\n");
	return generic_file_splice_read(f, l, p, s, ui);
}
ssize_t sjfs_fops_splice_write(struct pipe_inode_info *p, struct file *f, loff_t *l, size_t s, unsigned int ui) {
	printk("sjfs_fops_splice_write\n");
	return iter_file_splice_write(p, f, l, s, ui);
}
loff_t sjfs_fops_llseek(struct file *f, loff_t l, int i) {
	printk("sjfs_fops_llseek\n");
	return generic_file_llseek(f, l, i);
}

const struct file_operations sjfs_file_operations = {
        .read_iter      = generic_file_read_iter,
        .write_iter     = generic_file_write_iter,
        .mmap           = generic_file_mmap,
        .fsync          = noop_fsync,
        .splice_read    = generic_file_splice_read,
        .splice_write   = iter_file_splice_write,
        .llseek         = generic_file_llseek,
};

int sjfs_file_iops_setattr(struct dentry *dentry, struct iattr *iattr) {
	printk("sjfs_file_iops_setattr -> simple_setattr (for testing only)\n");

	// TODO: rewrite this to save the attributes back to user space
	return simple_setattr(dentry, iattr);
}
int sjfs_file_iops_getattr(struct vfsmount *mnt, struct dentry *dentry, struct kstat *stat) {
	printk("sjfs_file_iops_getattr -> simple_getattr\n");

	// enentually calls generic_fillattr() in fs/stat.c
	// simply copies inode data to stat data
	return simple_getattr(mnt, dentry, stat);
}

const struct inode_operations sjfs_file_inode_operations = {
        .setattr        = sjfs_file_iops_setattr,
        .getattr        = sjfs_file_iops_getattr,
};

static const struct inode_operations sjfs_dir_inode_operations; // circ dep

static const struct address_space_operations ramfs2_aops = {
	.readpage	= simple_readpage,
	.write_begin	= simple_write_begin,
	.write_end	= simple_write_end,
	//.set_page_dirty	= __set_page_dirty_no_writeback,
};

struct inode *ramfs2_get_inode(struct super_block *sb, const struct inode *dir, umode_t mode, dev_t dev) {
	struct inode * inode;

	printk("ramfs2_get_inode\n");

	inode = new_inode(sb);

	if (inode) {
		inode->i_ino = get_next_ino();
		inode_init_owner(inode, dir, mode);

		inode->i_mapping->a_ops = &ramfs2_aops;

		mapping_set_gfp_mask(inode->i_mapping, GFP_HIGHUSER);
		mapping_set_unevictable(inode->i_mapping);

		inode->i_atime = inode->i_mtime = inode->i_ctime = CURRENT_TIME;

		switch (mode & S_IFMT) {
			default:
				init_special_inode(inode, mode, dev);
				
				break;
			
			case S_IFREG:
				inode->i_op = &sjfs_file_inode_operations;
				inode->i_fop = &sjfs_file_operations;
				
				break;
			
			case S_IFDIR:
				inode->i_op = &sjfs_dir_inode_operations;
				inode->i_fop = &simple_dir_operations;
				
				inc_nlink(inode); // directory inodes start off with i_nlink == 2 (for "." entry)
				
				break;
			
			case S_IFLNK:
				inode->i_op = &page_symlink_inode_operations;
				
				break;
		}
	}

	return inode;
}

// -------------------------------------------------------------------------------------------------

static int ramfs2_mknod(struct inode *dir, struct dentry *dentry, umode_t mode, dev_t dev) {
	struct inode * inode;
	int ret = -ENOSPC;

	printk("ramfs2_mknod\n");
	
	inode = ramfs2_get_inode(dir->i_sb, dir, mode, dev);
	
	if (inode) {
		d_instantiate(dentry, inode);
		dget(dentry); // Extra count - pin the dentry in core
		
		ret = 0;
		
		dir->i_mtime = dir->i_ctime = CURRENT_TIME;
	}
	
	return ret;
}

static int ramfs2_create(struct inode *file, struct dentry *dentry, umode_t mode, bool excl) {
	printk("ramfs2_create\n");
	
	return ramfs2_mknod(file, dentry, mode | S_IFREG, 0);
}

static int ramfs2_mkdir(struct inode * dir, struct dentry * dentry, umode_t mode) {
	int ret;

	printk("ramfs2_mkdir\n");
	
	ret = ramfs2_mknod(dir, dentry, mode | S_IFDIR, 0);
	
	if (!ret)
		inc_nlink(dir);
	
	return ret;
}

/*
static int ramfs2_symlink(struct inode * dir, struct dentry *dentry, const char * symname) {
	struct inode *inode;
	int ret = -ENOSPC;

	inode = ramfs2_get_inode(dir->i_sb, dir, S_IFLNK|S_IRWXUGO, 0);

	if (inode) {
		int l = strlen(symname)+1;
		
		ret = page_symlink(inode, symname, l);
		
		if (!ret) {
			d_instantiate(dentry, inode);
			dget(dentry);
			
			dir->i_mtime = dir->i_ctime = CURRENT_TIME;
		} else {
			iput(inode);
		}
	}

	return ret;
}
*/

int sjfs_dir_iops_create(struct inode *i,struct dentry *d, umode_t u, bool b) {
	printk("sjfs_dir_iops_create -> ramfs2_create\n");
	return ramfs2_create(i, d, u, b);
}
struct dentry * sjfs_dir_iops_lookup(struct inode *i,struct dentry *d, unsigned int ui) {
	printk("sjfs_dir_iops_lookup -> simple_lookup\n");
	return simple_lookup(i, d, ui);
}
int sjfs_dir_iops_link(struct dentry *d,struct inode *i,struct dentry *d2) {
	printk("sjfs_dir_iops_link -> simple_link\n");
	return simple_link(d, i, d2);
}
int sjfs_dir_iops_unlink(struct inode *i,struct dentry *d) {
	printk("sjfs_dir_iops_unlink -> simple_unlink\n");
	return simple_unlink(i, d);
}
/*
int sjfs_dir_iops_symlink(struct inode *i,struct dentry *d,const char *c) {
	printk("sjfs_dir_iops_symlink -> ramfs2_symlink\n");
	return ramfs2_symlink(i, d, c);
}
*/
int sjfs_dir_iops_mkdir(struct inode *i,struct dentry *d,umode_t u) {
	printk("sjfs_dir_iops_mkdir -> ramfs2_mkdir\n");
	return ramfs2_mkdir(i, d, u);
}
int sjfs_dir_iops_rmdir(struct inode *i,struct dentry *d) {
	printk("sjfs_dir_iops_rmdir -> simple_rmdir\n");
	return simple_rmdir(i, d);
}
int sjfs_dir_iops_mknod(struct inode *i,struct dentry *d,umode_t u,dev_t d2) {
	printk("sjfs_dir_iops_mknod -> ramfs2_mknod\n");
	return ramfs2_mknod(i, d, u, d2);
}
int sjfs_dir_iops_rename(struct inode *i, struct dentry *d, struct inode *i2, struct dentry *d2) {
	printk("sjfs_dir_iops_rename -> simple_rename\n");
	return simple_rename(i, d, i2, d2);
}

static const struct inode_operations sjfs_dir_inode_operations = {
	.create		= sjfs_dir_iops_create,
	.lookup		= sjfs_dir_iops_lookup,
	.link		= sjfs_dir_iops_link,
	.unlink		= sjfs_dir_iops_unlink,
	//.symlink	= sjfs_dir_iops_symlink,
	.mkdir		= sjfs_dir_iops_mkdir,
	.rmdir		= sjfs_dir_iops_rmdir,
	.mknod		= sjfs_dir_iops_mknod,
	.rename		= sjfs_dir_iops_rename,
};

// - super block level -----------------------------------------------------------------------------

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

	inode = ramfs2_get_inode(sb, NULL, S_IFDIR | S_IRWXU | S_IRGRP | S_IXGRP | S_IROTH | S_IXOTH, 0);
	if (!inode)
		return -ENOMEM;

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
	kfree(sb->s_fs_info);

	// used for in memory fs
	kill_litter_super(sb);
	
	// shutdown our link to the userspace app?
} 

struct file_system_type sjfs_fs_type = {
	.name		= "sjfs",
	.fs_flags	= 0, // FS_USERNS_MOUNT
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

	// register the filesystem
	return register_filesystem(&sjfs_fs_type);
}

void __exit sjfs_exit(void) {
	// unregister the filesystem
	unregister_filesystem(&sjfs_fs_type);
}

module_init(sjfs_init);
module_exit(sjfs_exit);
