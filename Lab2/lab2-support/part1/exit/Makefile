CC      = gcc
OBJCOPY = objcopy
ASFLAGS = -nostdinc -I ../libc/include
LDFLAGS = -nostdlib -Wl,-Ttext,$(LOAD_ADDR) 

LOAD_ADDR = 0xa2000000

objects = exit.o

default: exit.bin

.PHONY: default clean clobber

exit.bin: exit
	$(OBJCOPY) -O binary $< $@

exit: $(objects)
	$(CC) $(LDFLAGS) -o $@ $^

%.o: %.S
	$(CC) -c $(ASFLAGS) -o $@ $<

clean:
	rm -f exit $(objects)

clobber: clean
	rm -f exit.bin
