#!/usr/bin/make -f

SHELL = /bin/sh

KDIR ?= /lib/modules/`uname -r`/build

.PHONY: all
all: module test_sjfs_ram


.PHONY: module
module:
	$(MAKE) -C $(KDIR) M=$$PWD modules

test_sjfs_ram: test_sjfs_ram.o
	$(CC) -g -o test_sjfs_ram test_sjfs_ram.o

test_sjfs_ram.o: test_sjfs_ram.c
	$(CC) -c test_sjfs_ram.c

.PHONY: clean
clean:
	$(MAKE) -C $(KDIR) M=$$PWD clean
	rm -f *.o test_sjfs_ram

