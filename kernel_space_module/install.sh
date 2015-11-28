#!/bin/sh

# build our module just in case we have not done so already
echo "Building kernel module..."
make

# install the module
echo "Installing kernel module..."
sudo insmod ./sjfs.ko

# make a root directory mount point
echo "Creating root directory mount point..."
sudo mkdir /mnt/sjfs_root

# mount the filesystem
echo "Mounting SJFS..."
sudo mount -t sjfs none /mnt/sjfs_root

