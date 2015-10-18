# cmpe142_fs
This project implements a new filesystem with all the calls redirected to userspace.

## Kernel Source
This is the kernel we will be building our module against.
* [Linux kernel stable tree](https://git.kernel.org/cgit/linux/kernel/git/stable/linux-stable.git/tree/)

## Documentation
Here is some authoritative information on kernel module and vfs development.
* [Documentation](https://git.kernel.org/cgit/linux/kernel/git/stable/linux-stable.git/tree/Documentation)
  * [Building External Modules](https://git.kernel.org/cgit/linux/kernel/git/stable/linux-stable.git/tree/Documentation/kbuild/modules.txt)
  * [Overview of the Linux Virtual File System](https://git.kernel.org/cgit/linux/kernel/git/stable/linux-stable.git/tree/Documentation/filesystems/vfs.txt)

## Headers
These are some of the core includes we will need to get our module and filesystem installed.
* [Include](https://git.kernel.org/cgit/linux/kernel/git/stable/linux-stable.git/tree/include)
  * [linux/module.h](https://git.kernel.org/cgit/linux/kernel/git/stable/linux-stable.git/tree/include/linux/module.h)
  * [linux/fs.h](https://git.kernel.org/cgit/linux/kernel/git/stable/linux-stable.git/tree/include/linux/fs.h)

## Example Filesystems
These are some links to different file systems that can be used for reference.
* [Filesystems](https://git.kernel.org/cgit/linux/kernel/git/stable/linux-stable.git/tree/fs)
  * [RamFS](https://git.kernel.org/cgit/linux/kernel/git/stable/linux-stable.git/tree/fs/ramfs) *Ignore the file-nommu.c
  * [SCO UnixWare Boot Filesystem (BFS)](https://git.kernel.org/cgit/linux/kernel/git/stable/linux-stable.git/tree/fs/bfs) *Not to be confused with BeFS

## Other Information
* [How to Write Your Own Linux Kernel Module with a Simple Example](http://www.thegeekstuff.com/2013/07/write-linux-kernel-module/)
* [theurbanpenguin - LINUX Understanding inodes](https://www.youtube.com/watch?v=_6VJ8WfWI4k)
* [Writing a Simple File System](http://www2.comp.ufscar.br/~helio/fs/rkfs.html)