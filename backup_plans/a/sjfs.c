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
#include <linux/module.h>

// kernel connector related
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/moduleparam.h>
#include <linux/skbuff.h>
#include <linux/slab.h>
#include <linux/timer.h>

#include <linux/connector.h>

// semaphore related
#include <linux/semaphore.h>

#include "ipc.h"

#define SJFS_MAGIC      0x534A5346

MODULE_LICENSE("GPL");

static struct semaphore cn_sem;
static struct semaphore cb_sem;

static struct cb_id cn_sjfs_id = {
	CN_NETLINK_USERS + 3,
	0x456
};
static char cn_sjfs_name[] = "cn_sjfs";
static struct sock *nls;
static unsigned int cn_sjfs_counter;

static unsigned char * sjfs_read_buffer;

static void cn_sjfs_callback(struct cn_msg *msg, struct netlink_skb_parms *nsp) {
	sjfs_response_packet_t * packet;

	if(!down_trylock(&cb_sem)) {
		up(&cb_sem); // FIXME: potential race
		return;
	}

	packet = (sjfs_response_packet_t *) msg->data;

	sjfs_read_buffer = kzalloc(packet->count, GFP_ATOMIC);
	if(sjfs_read_buffer) {
		memcpy(sjfs_read_buffer, packet + 1, packet->count);
	}

	up(&cb_sem);
}

static const struct inode_operations sjfs_dir_inode_operations;

static ssize_t sjfs_fops_read(struct file *f, char __user *to, size_t count, loff_t *ppos) {
	struct cn_msg *m;
	sjfs_request_packet_t packet = {
		SJFS_OPCODE_READ,
		f->f_inode->i_ino,
		*ppos,
		count,
	};

	down(&cn_sem);
	down(&cb_sem);

	m = kzalloc(sizeof(*m) + sizeof(packet), GFP_ATOMIC);
	if(m) {
		memcpy(&m->id, &cn_sjfs_id, sizeof(m->id));
		m->seq = cn_sjfs_counter;
		m->len = sizeof(packet);
		memcpy(m + 1, &packet, m->len);

		cn_netlink_send(m, 0, 0, GFP_ATOMIC);
		kfree(m);
	}

	cn_sjfs_counter++;

	down(&cb_sem);
	up(&cb_sem);

	copy_to_user(to, sjfs_read_buffer, count);

	up(&cn_sem);

	return count;
}
static ssize_t sjfs_fops_write(struct file *f, const char __user *from, size_t count, loff_t *ppos) {
	struct cn_msg *m;
	sjfs_request_packet_t packet = {
		SJFS_OPCODE_WRITE,
		f->f_inode->i_ino,
		*ppos,
		count,
	};
	unsigned char * data;
	size_t res;

	down(&cn_sem);

	m = kzalloc(sizeof(*m) + sizeof(packet) + count, GFP_ATOMIC);
	data = kzalloc(sizeof(packet) + count, GFP_ATOMIC);
	if(m && data) {
		memcpy(&m->id, &cn_sjfs_id, sizeof(m->id));
		m->seq = cn_sjfs_counter;
		m->len = sizeof(packet) + count;
		memcpy(data, &packet, sizeof(packet));
		res = copy_from_user(data + sizeof(packet), from, count);
		if(res == count) {
			memcpy(m + 1, data, m->len);

			cn_netlink_send(m, 0, 0, GFP_ATOMIC);
		}
		kfree(m);
	}

	cn_sjfs_counter++;

	up(&cn_sem);

	return count;
}

static const struct file_operations sjfs_file_operations = {
	.read_iter	= generic_file_read_iter,
	.write_iter	= generic_file_write_iter,
	.mmap		= generic_file_mmap,
	.fsync		= noop_fsync,
	.splice_read	= generic_file_splice_read,
	.splice_write	= iter_file_splice_write,
	.llseek		= generic_file_llseek,
	.read		= sjfs_fops_read,
	.write		= sjfs_fops_write,
};

static const struct inode_operations sjfs_file_inode_operations = {
	.setattr	= simple_setattr,
	.getattr	= simple_getattr,
};

static int sjfs_set_page_dirty_no_writeback(struct page *page) {
	if (!PageDirty(page))
		return !TestSetPageDirty(page);
	return 0;
}

static const struct address_space_operations sjfs_aops = {
	.readpage	= simple_readpage,
	.write_begin	= simple_write_begin,
	.write_end	= simple_write_end,
	.set_page_dirty	= sjfs_set_page_dirty_no_writeback,
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
	int err;

	if(test_and_set_bit(0, &once)) {
		return 0;
	}

	sema_init(&cn_sem, 1);
	sema_init(&cb_sem, 1);

	err = cn_add_callback(&cn_sjfs_id, cn_sjfs_name, cn_sjfs_callback);
	if (err)
		goto err_out;

	return register_filesystem(&sjfs_fs_type);

err_out:
	if (nls && nls->sk_socket)
		sock_release(nls->sk_socket);

	return err;
}

static void __exit exit_sjfs_fs(void) {
	unregister_filesystem(&sjfs_fs_type);

	cn_del_callback(&cn_sjfs_id);
	if (nls && nls->sk_socket)
		sock_release(nls->sk_socket);

	kfree(&cb_sem);
	kfree(&cn_sem);
}

module_init(init_sjfs_fs);
module_exit(exit_sjfs_fs);

