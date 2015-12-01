# cmpe142_fs
**Fall 2015**

This project implements a new filesystem with all the calls redirected to userspace.

## Kernel Source
This is the kernel we will be building our module against.
* [Linux kernel stable tree](https://git.kernel.org/cgit/linux/kernel/git/stable/linux-stable.git/tree/)

## Documentation
Here is some authoritative information on kernel module and vfs development.
* [Documentation](https://git.kernel.org/cgit/linux/kernel/git/stable/linux-stable.git/tree/Documentation)
  * [Building External Modules](https://git.kernel.org/cgit/linux/kernel/git/stable/linux-stable.git/tree/Documentation/kbuild/modules.txt)
  * [Overview of the Linux Virtual File System](https://git.kernel.org/cgit/linux/kernel/git/stable/linux-stable.git/tree/Documentation/filesystems/vfs.txt)
  * [Kernel Connector](https://git.kernel.org/cgit/linux/kernel/git/stable/linux-stable.git/tree/Documentation/connector/connector.txt)

## Headers / Sources
These are some of the core includes we will need to get our module and filesystem installed.
* [Include](https://git.kernel.org/cgit/linux/kernel/git/stable/linux-stable.git/tree/include)
  * [linux/module.h](https://git.kernel.org/cgit/linux/kernel/git/stable/linux-stable.git/tree/include/linux/module.h)
  * [linux/fs.h](https://git.kernel.org/cgit/linux/kernel/git/stable/linux-stable.git/tree/include/linux/fs.h)
* [Library for filesystems writers (fs/libfs.c)](https://git.kernel.org/cgit/linux/kernel/git/stable/linux-stable.git/tree/fs/libfs.c) Note: The following symbols are good to look at.
  * 31 simple_getattr
  * 55 simple_dentry_operations
  * 70 simple_lookup
  * 196 simple_dir_operations
  * 201 simple_dir_inode_operations
  * 264 simple_open
  * 277 simple_link
  * 298 simple_empty
  * 309 simple_unlink
  * 321 simple_rmdir
  * 348 simple_rename
  * 379 simple_setattr
  * 389 simple_readpage
  * 413 simple_write_begin
  * 465 simple_write_end
  * 534 simple_fill_super
  * 604 simple_read_from_buffer
  * 639 simple_write_to_buffer
  * 778 simple_attr_open
  * 785 simple_attr_release
  * 821 simple_attr_read
  * 854 simple_attr_write

## Example Filesystems
These are some links to different file systems that can be used for reference.
* [Filesystems](https://git.kernel.org/cgit/linux/kernel/git/stable/linux-stable.git/tree/fs)
  * [RamFS](https://git.kernel.org/cgit/linux/kernel/git/stable/linux-stable.git/tree/fs/ramfs) *Ignore the file-nommu.c
  * [SCO UnixWare Boot Filesystem (BFS)](https://git.kernel.org/cgit/linux/kernel/git/stable/linux-stable.git/tree/fs/bfs) *Not to be confused with BeFS

## Previous Semester's Work
This is work from students in previous semesters including their references. This code cannot be trusted to work.
* [alexanderkoumis/basedfs](https://github.com/alexanderkoumis/basedfs) - writes but does not read?
* [mwcarlis/Thunder_FS](https://github.com/mwcarlis/Thunder_FS)
* [mschneider90/cmpe142](https://github.com/mschneider90/cmpe142) - just saves data to RAM
  * https://casper.berkeley.edu/svn/trunk/roach/sw/linux/security/inode.c
  * http://www.win.tue.nl/~aeb/linux/lk/lk-8.html
  * http://www.tldp.org/LDP/khg/HyperNews/get/fs/vfstour.html
  * http://lwn.net/Articles/57369/

## Debugging
* strace (user space tracing)
  * [Ubuntu manuals - strace(1)](http://manpages.ubuntu.com/manpages/wily/en/man1/strace.1.html)
* ftrace (kernel space tracing)
  * [LWN.net - Debugging the kernel using Ftrace - part 1](https://lwn.net/Articles/365835/)
  * [LWN.net - Debugging the kernel using Ftrace - part 2](https://lwn.net/Articles/366796/)
  * `echo ":mod:<your_mod_name>" > set_ftrace_filter` is really useful.

## Other Information
* [Ext4 Disk Layout](https://ext4.wiki.kernel.org/index.php/Ext4_Disk_Layout)
* [Robert Love - Linux Kernel Development 2nd Ed.](http://www.makelinux.net/books/lkd2/ch12)
* [How to Write Your Own Linux Kernel Module with a Simple Example](http://www.thegeekstuff.com/2013/07/write-linux-kernel-module/)
* [theurbanpenguin - LINUX Understanding inodes](https://www.youtube.com/watch?v=_6VJ8WfWI4k)
* [Writing a Simple File System](http://www2.comp.ufscar.br/~helio/fs/rkfs.html)
