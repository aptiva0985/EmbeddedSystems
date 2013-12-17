CC     = gcc
CFLAGS = -O2 -Wall -Werror

objects = foo.o bar.o

default: baz

.PHONY: default clean clobber

baz: $(objects)
	$(CC) -o $@ $^

foo.o: foo.c common.h
bar.o: bar.c common.h

%.o: %.c
	$(CC) -c $(CFLAGS) -o $@ $<

clean:
	rm -f $(objects)

clobber: clean
	rm -f baz
