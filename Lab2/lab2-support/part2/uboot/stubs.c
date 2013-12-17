/* Modified from u-boot-1.1.4/examples/stubs.c */

#include <exports.h>

#ifndef GCC_VERSION
#define GCC_VERSION (__GNUC__ * 1000 + __GNUC_MINOR__)
#endif /* GCC_VERSION */

/* offsetof(gd_t, jt) */
#define OFFSETOF_JT_IN_GD_T 32

/*
 * r8 holds the pointer to the global_data, ip is a call-clobbered
 * register
 */
#define EXPORT_FUNC(x) \
	asm volatile (			\
"	.globl " #x "\n"		\
#x ":\n"				\
"	ldr	ip, [r8, %0]\n"		\
"	ldr	pc, [ip, %1]\n"		\
	: : "i"(OFFSETOF_JT_IN_GD_T), "i"(XF_ ## x * sizeof(void *)) : "ip");

/* This function is necessary to prevent the compiler from
 * generating prologue/epilogue, preparing stack frame etc.
 * The stub functions are special, they do not use the stack
 * frame passed to them, but pass it intact to the actual
 * implementation. On the other hand, asm() statements with
 * arguments can be used only inside the functions (gcc limitation)
 */
#if GCC_VERSION < 3004
static
#endif /* GCC_VERSION */
void __attribute__((unused)) dummy(void)
{
#include <_exports.h>
}

extern unsigned long __bss_start, _end;

void app_startup(char **argv)
{
	unsigned long * cp = &__bss_start;

	/* Zero out BSS */
	while (cp < &_end) {
		*cp++ = 0;
	}
}

#undef EXPORT_FUNC
