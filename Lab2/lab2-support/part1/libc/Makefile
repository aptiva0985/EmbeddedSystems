CC      = gcc
AR      = ar
CFLAGS  = -O2 -ffreestanding -Wall -Werror -nostdinc -I include
ASFLAGS = -nostdinc -I include

objects = errno.o exit.o read.o write.o

default: all

.PHONY: default all clean clobber

all: crt0.o libc.a

libc.a: $(objects)
	$(AR) rcs $@ $^

%.o: %.c
	$(CC) -c $(CFLAGS) -o $@ $<

%.o: %.S
	$(CC) -c $(ASFLAGS) -o $@ $<

clean:
	rm -f $(objects)

clobber: clean
	rm -f crt0.o libc.a
