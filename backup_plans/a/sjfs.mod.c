#include <linux/module.h>
#include <linux/vermagic.h>
#include <linux/compiler.h>

MODULE_INFO(vermagic, VERMAGIC_STRING);

__visible struct module __this_module
__attribute__((section(".gnu.linkonce.this_module"))) = {
	.name = KBUILD_MODNAME,
	.init = init_module,
#ifdef CONFIG_MODULE_UNLOAD
	.exit = cleanup_module,
#endif
	.arch = MODULE_ARCH_INIT,
};

static const struct modversion_info ____versions[]
__used
__attribute__((section("__versions"))) = {
	{ 0xbf8d6d33, __VMLINUX_SYMBOL_STR(module_layout) },
	{ 0x4593e24f, __VMLINUX_SYMBOL_STR(simple_rename) },
	{ 0xdcbf7fe2, __VMLINUX_SYMBOL_STR(simple_rmdir) },
	{ 0x2856afe7, __VMLINUX_SYMBOL_STR(simple_unlink) },
	{ 0x7cb09a6a, __VMLINUX_SYMBOL_STR(simple_link) },
	{ 0xaf302ddb, __VMLINUX_SYMBOL_STR(simple_lookup) },
	{ 0x422d9885, __VMLINUX_SYMBOL_STR(generic_file_splice_read) },
	{ 0x507135f8, __VMLINUX_SYMBOL_STR(iter_file_splice_write) },
	{ 0x82694cd6, __VMLINUX_SYMBOL_STR(noop_fsync) },
	{ 0x6221d4b9, __VMLINUX_SYMBOL_STR(generic_file_mmap) },
	{ 0xeb9dbf96, __VMLINUX_SYMBOL_STR(generic_file_write_iter) },
	{ 0x1c3fe049, __VMLINUX_SYMBOL_STR(generic_file_read_iter) },
	{ 0x41d544ff, __VMLINUX_SYMBOL_STR(generic_file_llseek) },
	{ 0x1780b3b6, __VMLINUX_SYMBOL_STR(simple_getattr) },
	{ 0x93002127, __VMLINUX_SYMBOL_STR(simple_setattr) },
	{ 0xd1b91b7e, __VMLINUX_SYMBOL_STR(simple_write_end) },
	{ 0x64a926af, __VMLINUX_SYMBOL_STR(simple_write_begin) },
	{ 0x1acffd67, __VMLINUX_SYMBOL_STR(simple_readpage) },
	{ 0xbd5237d1, __VMLINUX_SYMBOL_STR(generic_show_options) },
	{ 0x1faaa9bd, __VMLINUX_SYMBOL_STR(simple_statfs) },
	{ 0xefae173d, __VMLINUX_SYMBOL_STR(generic_delete_inode) },
	{ 0xff5a8cfe, __VMLINUX_SYMBOL_STR(cn_del_callback) },
	{ 0xeec75c99, __VMLINUX_SYMBOL_STR(unregister_filesystem) },
	{ 0x6de0cdbe, __VMLINUX_SYMBOL_STR(register_filesystem) },
	{ 0x50ac852a, __VMLINUX_SYMBOL_STR(cn_add_callback) },
	{ 0x5f45c0bb, __VMLINUX_SYMBOL_STR(d_make_root) },
	{ 0x9a25eb13, __VMLINUX_SYMBOL_STR(save_mount_options) },
	{ 0x838b54b1, __VMLINUX_SYMBOL_STR(iput) },
	{ 0x2146a6cb, __VMLINUX_SYMBOL_STR(page_symlink) },
	{ 0xd0d8621b, __VMLINUX_SYMBOL_STR(strlen) },
	{ 0xce9e41e9, __VMLINUX_SYMBOL_STR(lockref_get) },
	{ 0x868e42a0, __VMLINUX_SYMBOL_STR(d_instantiate) },
	{ 0x584497c8, __VMLINUX_SYMBOL_STR(inc_nlink) },
	{ 0x3829dc13, __VMLINUX_SYMBOL_STR(simple_dir_operations) },
	{ 0x19dc4855, __VMLINUX_SYMBOL_STR(page_symlink_inode_operations) },
	{ 0xc6b73471, __VMLINUX_SYMBOL_STR(init_special_inode) },
	{ 0x34184afe, __VMLINUX_SYMBOL_STR(current_kernel_time) },
	{ 0x7348b31c, __VMLINUX_SYMBOL_STR(inode_init_owner) },
	{ 0xe953b21f, __VMLINUX_SYMBOL_STR(get_next_ino) },
	{ 0x4731e2e0, __VMLINUX_SYMBOL_STR(new_inode) },
	{ 0x4f6b400b, __VMLINUX_SYMBOL_STR(_copy_from_user) },
	{ 0x37a0cba, __VMLINUX_SYMBOL_STR(kfree) },
	{ 0xc8de802f, __VMLINUX_SYMBOL_STR(cn_netlink_send) },
	{ 0x10a03d, __VMLINUX_SYMBOL_STR(kmem_cache_alloc_trace) },
	{ 0x877de68b, __VMLINUX_SYMBOL_STR(kmalloc_caches) },
	{ 0x6dc6dd56, __VMLINUX_SYMBOL_STR(down) },
	{ 0x78e739aa, __VMLINUX_SYMBOL_STR(up) },
	{ 0x4a619f83, __VMLINUX_SYMBOL_STR(memcpy) },
	{ 0x12da5bb2, __VMLINUX_SYMBOL_STR(__kmalloc) },
	{ 0x156a8a59, __VMLINUX_SYMBOL_STR(down_trylock) },
	{ 0xf33c5e7f, __VMLINUX_SYMBOL_STR(kill_litter_super) },
	{ 0x7a530679, __VMLINUX_SYMBOL_STR(mount_nodev) },
	{ 0xb4390f9a, __VMLINUX_SYMBOL_STR(mcount) },
};

static const char __module_depends[]
__used
__attribute__((section(".modinfo"))) =
"depends=";


MODULE_INFO(srcversion, "A9100FB6F91A768122DAE86");
