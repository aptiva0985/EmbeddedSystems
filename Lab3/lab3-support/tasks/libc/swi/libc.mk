TLIBC_SWI_OBJS := exit.o read.o write.o time.o sleep.o
TLIBC_SWI_OBJS := $(TLIBC_SWI_OBJS:%=$(TLIBCDIR)/swi/%)
TLIBC_OBJS += $(TLIBC_SWI_OBJS)
