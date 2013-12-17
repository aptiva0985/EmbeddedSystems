AOBJS := reg.o psr.o
AOBJS := $(AOBJS:%=$(KDIR)/arm/%)
ALL_OBJS += $(AOBJS)

