ARM_OBJS := reg.o psr.o setup.o int_asm.o s_handler.o C_IRQ_handler.o \
            C_SWI_handler.o install_handler.o \
            timer_setup.o interrupt_setup.o
ARM_OBJS := $(ARM_OBJS:%=$(KDIR)/arm/%)

KOBJS += $(ARM_OBJS)
