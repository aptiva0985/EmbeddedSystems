/*
 * hello.c: Hello world! with U-Boot Exports API
 *
 * Author: Mike Kasick <mkasick@andrew.cmu.edu>
 * Date:   Sat, 13 Oct 2007 23:51:13 -0400
 */

#include <exports.h>

int main(void) {
	puts("Hello world!\n");

	return 0;
}
