CC      = gcc
OBJCOPY = objcopy
CFLAGS  = -O2 -ffixed-r8 -ffreestanding -Wall -Werror -nostdinc \
          -I ../uboot/include -I include
LDFLAGS = -nostdlib -Wl,-e,$(ENTRY_SYMBOL),-N,-Ttext,$(LOAD_ADDR)

ENTRY_SYMBOL = main
LOAD_ADDR    = 0xa2000000

STUBS = ../uboot/stubs.o

objects = hello.o

default: hello.bin

.PHONY: default clean clobber

hello.bin: hello
	$(OBJCOPY) -O binary $< $@

hello: $(objects)
	$(CC) $(LDFLAGS) -o $@ $^ $(STUBS)

%.o: %.c
	$(CC) -c $(CFLAGS) -o $@ $<

clean:
	rm -f hello $(objects)

clobber: clean
	rm -f hello.bin
