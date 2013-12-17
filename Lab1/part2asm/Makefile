CC      = gcc
CFLAGS  = -g -O2 -Wall -Werror
ASFLAGS = -g

objects = main.o strTable.o

default: part2asm

.PHONY: default clean clobber

part2asm: $(objects)
	$(CC) -g -o $@ $^

main.o: main.c
strTable.o: strTable.s

%.o: %.c
	$(CC) -c $(CFLAGS) -o $@ $<

%.o: %.s
	$(CC) -c $(ASFLAGS) -o $@ $<

clean:
	rm -f $(objects)

clobber: clean
	rm -f part2asm
