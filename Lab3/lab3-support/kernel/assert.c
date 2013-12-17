/**
 * @file assert.c
 *
 * @brief Assertion and debugging infrastructure.
 *
 * @author Kartik Subramanian
 */

#include <assert.h>
#include <stdarg.h>
#include <exports.h>

#include <arm/psr.h>
#include <arm/exception.h>

void panic(const char* fmt, ...)
{
	int i;
	va_list list;

	va_start(list, fmt);
	// XXX This is not working... fix it later.
	//vprintf(fmt, list);
	printf("PANIC");
	va_end(list);

	// We have not provided disable_interrupts.  If you choose to make
	// a similarly named function, uncomment this to take advantage of it.
	// disable_interrupts();

	while(1)
	{
		putc('!');
		for(i = 0; i < 100000; i++)
			*(volatile uint32_t*)0;
	}
}
