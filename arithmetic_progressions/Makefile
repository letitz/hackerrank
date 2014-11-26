# Rudimetary makefile
CC=gcc
CFLAGS= -ggdb --std=c11

all: ap

segtree.o: segtree.c
	$(CC) -o $@ -c $^ $(CFLAGS)

ap: arithmetic_progressions.c segtree.o
	$(CC) -o $@ $^ $(CFLAGS)

clean:
	rm -f segtree.o ap
