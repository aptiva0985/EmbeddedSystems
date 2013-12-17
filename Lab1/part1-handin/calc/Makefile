CC     = gcc
CFLAGS = -O2 -Wall -Werror

objects = math.o calc.o

default: calc

.PHONY: default clean clobber

calc: $(objects)
	$(CC) -o $@ $^

calc.o: calc.c math.h
math.o: math.c math.h

%.o: %.c
	$(CC) -MM $<
	$(CC) -c $(CFLAGS) -o $@ $<

clean:
	rm -f $(objects) calc

clobber: clean
	rm -f calc
