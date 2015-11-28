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
	printk("sjfs_fops_read_iter -> generic_file_read_iter\n");
	return generic_file_read_iter(k, i);
}
ssize_t sjfs_fops_write_iter(struct kiocb *k, struct iov_iter *i) {
	printk("sjfs_fops_write_iter -> generic_file_write_iter\n");
	return generic_file_write_iter(k, i);
}
int sjfs_fops_mmap(struct file *f, struct vm_area_struct *v) {
	printk("sjfs_fops_mmap -> generic_file_mmap\n");
	return generic_file_mmap(f, v);
}
int sjfs_fops_fsync(struct file *f, loff_t l, loff_t l2, int datasync) {
	printk("sjfs_fops_fsync -> noop_fsync\n");
	return noop_fsync(f, l, l2, datasync);
}
ssize_t sjfs_fops_splice_read(struct file *f, loff_t *l, struct pipe_inode_info *p, size_t s, unsigned int ui) {
	printk("sjfs_fops_splice_read -> generic_file_splice_read\n");
	return generic_file_splice_read(f, l, p, s, ui);
}
ssize_t sjfs_fops_splice_write(struct pipe_inode_info *p, struct file *f, loff_t *l, size_t s, unsigned int ui) {
	printk("sjfs_fops_splice_write -> iter_file_splice_write\n");
	return iter_file_splice_write(p, f, l, s, ui);
}
loff_t sjfs_fops_llseek(struct file *f, loff_t l, int i) {
	printk("sjfs_fops_llseek -> generic_file_llseek\n");
	return generic_file_llseek(f, l, i);
}

const struct file_operations sjfs_file_operations = {
        .read_iter      = sjfs_fops_read_iter,
        .write_iter     = sjfs_fops_write_iter,
        .mmap           = sjfs_fops_mmap,
        .fsync          = sjfs_fops_fsync,
        .splice_read    = sjfs_fops_splice_read,
        .splice_write   = sjfs_fops_splice_write,
        .llseek         = sjfs_fops_llseek,
};

int sjfs_dir_fops_open(struct inode *f, struct file *f2) {
	printk("sjfs_dir_fops_open -> dcache_dir_open\n");
	return dcache_dir_open(f, f2);
}
int sjfs_dir_fops_release(struct inode *i, struct file *f) {
	printk("sjfs_dir_fops_release -> dcache_dir_close\n");
	return dcache_dir_close(i, f);
}
loff_t sjfs_dir_fops_llseek(struct file *f, loff_t l, int i) {
	printk("sjfs_dir_fops_llseek -> dcache_dir_lseek\n");
	return dcache_dir_lseek(f, l, i);
}
ssize_t sjfs_dir_fops_read(struct file *f, char __user *u, size_t s, loff_t *l) {
	printk("sjfs_dir_fops_read -> generic_read_dir\n");
	return generic_read_dir(f, u, s, l);
}
int sjfs_dir_fops_iterate(struct file *f, struct dir_context *d) {
	printk("sjfs_dir_fops_iterate -> dcache_readdir\n");
	return dcache_readdir(f, d);
}
int sjfs_dir_fops_fsync(struct file *f, loff_t l, loff_t l2, int datasync) {
	printk("sjfs_dir_fops_fsync -> noop_fsync\n");
	return noop_fsync(f, l, l2, datasync);
}

const struct file_operations sjfs_dir_operations = {
	.open		= sjfs_dir_fops_open,
	.release	= sjfs_dir_fops_release,
	.llseek		= sjfs_dir_fops_llseek,
	.read		= sjfs_dir_fops_read,
	.iterate	= sjfs_dir_fops_iterate,
	.fsync		= sjfs_dir_fops_fsync,
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

int sjfs_aops_readpage(struct file *f, struct page *p) {
	printk("sjfs_aops_readpage -> simple_readpage\n");
	
	return simple_readpage(f, p);
}
int sjfs_aops_write_begin(struct file *f, struct address_space *mapping, loff_t pos, unsigned len, unsigned flags, struct page **pagep, void **fsdata) {
	printk("sjfs_aops_write_begin -> simple_write_begin\n");
	
	return simple_write_begin(f, mapping, pos, len, flags, pagep, fsdata);
}
int sjfs_aops_write_end(struct file *f, struct address_space *mapping, loff_t pos, unsigned len, unsigned copied, struct page *page, void *fsdata) {
	printk("sjfs_aops_write_end -> simple_write_end\n");
	
	return simple_write_end(f, mapping, pos, len, copied, page, fsdata);
}

static const struct address_space_operations ramfs2_aops = {
	.readpage	= sjfs_aops_readpage,
	.write_begin	= sjfs_aops_write_begin,
	.write_end	= sjfs_aops_write_end,
	//.set_page_dirty	= __set_page_dirty_no_writeback,
};

struct inode *ramfs2_get_inode(struct super_block *sb, const struct inode *dir, umode_t mode, dev_t dev) {
	struct inode * inode;

	if(dir) {
		printk("ramfs2_get_inode(sb. dir.ino:%lu, m:%#0hx, dev)\n", dir->i_ino, (unsigned int) mode);
	} else {
		printk("ramfs2_get_inode(sb. dir.ino:NULL, m:%#0hx, dev)\n", (unsigned int) mode);
	}

	inode = new_inode(sb);

	if (inode) {
		inode->i_ino = get_next_ino();

		printk("--- ret: %lu\n", inode->i_ino);

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
				inode->i_fop = &sjfs_dir_operations;
				
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

	printk("ramfs2_mknod(dir.ino:%lu, *dentry, m:%#0hx, dev)\n", dir->i_ino, (unsigned int) mode);
	
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
	printk("ramfs2_create -> ramfs2_mknod\n");
	
	return ramfs2_mknod(file, dentry, mode | S_IFREG, 0);
}

static int ramfs2_mkdir(struct inode * dir, struct dentry * dentry, umode_t mode) {
	int ret;

	printk("ramfs2_mkdir (will force error)\n");
	
	/*
	ret = ramfs2_mknod(dir, dentry, mode | S_IFDIR, 0);
	
	if (!ret)
		inc_nlink(dir);
	*/

	ret = -ENOSPC;

	return ret;
}

static int ramfs2_symlink(struct inode * dir, struct dentry *dentry, const char * symname) {
	//struct inode *inode;
	int ret = -ENOSPC;

	printk("ramfs2_symlink (will force error)\n");

	/*
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
	*/

	return ret;
}

int sjfs_dir_iops_create(struct inode *i,struct dentry *d, umode_t u, bool b) {
	printk("sjfs_dir_iops_create -> ramfs2_create\n");
	return ramfs2_create(i, d, u, b);
}
struct dentry * sjfs_dir_iops_lookup(struct inode *dir, struct dentry *dentry, unsigned int flags) {
	if(dir) {
		printk("sjfs_dir_iops_lookup -> simple_lookupi(dir.ino:%lu, *dentry, f:%#0hx)\n", dir->i_ino, flags);
	} else {
		printk("sjfs_dir_iops_lookup -> simple_lookupi(dir.ino:NULL, *dentry, f:%#0hx)\n", flags);
	}
	return simple_lookup(dir, dentry, flags);
}
int sjfs_dir_iops_link(struct dentry *d,struct inode *i,struct dentry *d2) {
	printk("sjfs_dir_iops_link -> simple_link\n");
	return simple_link(d, i, d2);
}
int sjfs_dir_iops_unlink(struct inode *i,struct dentry *d) {
	printk("sjfs_dir_iops_unlink -> simple_unlink\n");
	return simple_unlink(i, d);
}

int sjfs_dir_iops_symlink(struct inode *i,struct dentry *d,const char *c) {
	printk("sjfs_dir_iops_symlink -> ramfs2_symlink\n");
	return ramfs2_symlink(i, d, c);
}

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
	.symlink	= sjfs_dir_iops_symlink,
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

	printk("sjfs_fill_super\n");

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
	printk("sjfs_mount -> mount_single\n");

	// start up our link to the userspace app?
	
	// mount to a single place with no device
	return mount_single(fs_type, flags, data, sjfs_fill_super);
}

void sjfs_kill_sb(struct super_block *sb) {
	printk("sjfs_kill_sb\n");

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

	printk("sjfs_init\n");

	if (test_and_set_bit(0, &once))
		return -EBUSY;

	// register the filesystem
	return register_filesystem(&sjfs_fs_type);
}

void __exit sjfs_exit(void) {
	printk("sjfs_exit\n");

	// unregister the filesystem
	unregister_filesystem(&sjfs_fs_type);
}

module_init(sjfs_init);
module_exit(sjfs_exit);
