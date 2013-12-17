CC      = gcc
CFLAGS  = -O2 -ffixed-r8 -ffreestanding -Wall -Werror -nostdinc -I include

default: stubs.o

.PHONY: default clobber

%.o: %.c
	$(CC) -c $(CFLAGS) -o $@ $<

clobber:
	rm -f stubs.o
