PROGS_DAGGER_OBJS := sys_err.o
PROGS_DAGGER_OBJS := $(PROGS_DAGGER_OBJS:%=$(TDIR)/sys_err/%)
ALL_OBJS += $(PROGS_DAGGER_OBJS)

$(TDIR)/bin/sys_err : $(TSTART) $(PROGS_DAGGER_OBJS) $(TLIBC)

