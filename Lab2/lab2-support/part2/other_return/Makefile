CC      = gcc
OBJCOPY = objcopy
ASFLAGS = -nostdinc -I ../libc/include
LDFLAGS = -nostdlib -Wl,-Ttext,$(LOAD_ADDR)

LOAD_ADDR = 0xa2000000

objects = other_return.o

default: other_return.bin

.PHONY: default clean clobber

other_return.bin: other_return
	$(OBJCOPY) -O binary $< $@

other_return: $(objects)
	$(CC) $(LDFLAGS) -o $@ $^

%.o: %.S
	$(CC) -c $(ASFLAGS) -o $@ $<

clean:
	rm -f other_return $(objects)

clobber: clean
	rm -f other_return.bin
