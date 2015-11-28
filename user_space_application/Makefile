#!/usr/bin/make -f

SHELL = /bin/sh

CC = gcc
INSTALL = /usr/bin/install -c
CDEBUG = -g
CFLAGS = $(CDEBUG)

prefix=/usr/local
bindir=$(prefix)/bin

.PHONY: all
all: sjfs_helper


sjfs_helper: main.o
	$(CC) $(CFLAGS) -o sjfs_helper main.o

main.o: main.c
	$(CC) $(CFLAGS) -c main.c

.PHONY: install
install: all
	$(INSTALL) sjfs_helper $(bindir)/sjfs_helper

.PHONY: uninstall
uninstall:
	rm -f $(bindir)/sjfs_helper

.PHONY: clean
clean:
	rm -f *.o sjfs_helper
