/* Modified from u-boot-1.1.4/include/exports.h */

#ifndef __EXPORTS_H__
#define __EXPORTS_H__

#ifndef __ASSEMBLY__

#include <bits/types.h>
#include <stdarg.h>

/* From common.h: */
typedef void (interrupt_handler_t)(void *);

/* These are declarations of exported functions available in C code */
unsigned long get_version(void);
int  getc(void);
int  tstc(void);
void putc(const char);
void puts(const char*);
void printf(const char* fmt, ...);
void install_hdlr(int, interrupt_handler_t*, void*);
void free_hdlr(int);
void *malloc(size_t);
void free(void*);
void udelay(unsigned long);
unsigned long get_timer(unsigned long);
void vprintf(const char *, va_list);
void do_reset (void);

void app_startup(char **);

#endif    /* ifndef __ASSEMBLY__ */

enum {
#define EXPORT_FUNC(x) XF_ ## x ,
#include <_exports.h>
#undef EXPORT_FUNC

	XF_MAX
};

#define XF_VERSION	2

#endif	/* __EXPORTS_H__ */
