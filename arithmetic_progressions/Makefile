# Rudimetary makefile
CC=gcc
CFLAGS= -ggdb --std=c11

all: ap

%.o: %.c
	$(CC) -o $@ -c $^ $(CFLAGS)

ap: arithmetic_progressions.c itree.o
	$(CC) -o $@ $^ $(CFLAGS)

clean:
	rm -f segtree.o itree.o ap
