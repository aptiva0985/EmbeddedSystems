/** @file twister.c
 *
 * @brief Utilizes and exercises the system libraries.
 *
 * Links to libc.
 *
 * @author Kartik Subramanian <ksubrama@andrew.cmu.edu>
 * @date   2008-11-11
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

extern int forty;
extern int three;

int main(int argc, char** argv)
{
	const char hello[] = "Hello";
	const char goodbye[] = "Goodbye";
	const char mynum[] = "1234";
	const char *ptr;
	int num;

	printf("%s\n", hello);
	printf("%s %s %s %s\n", hello, goodbye, hello, goodbye);
	printf("0x%08x\t0x%08x\n", (uintptr_t)(&hello), (uintptr_t)(&goodbye));
	printf("sizes: %d %d %d %d %d\n", sizeof(uint8_t), sizeof(uint16_t), sizeof(uint32_t),
		sizeof(uintptr_t), sizeof(size_t));

	printf("\"%s\"->length = %lu\n", hello, strlen(hello));
	printf("H");
	printf("e");
	printf("l");
	printf("l");
	printf("o");
	printf("\n");

	putchar('\"');
	ptr = hello;
	while(*ptr)
	{
		if (isupper(*ptr++))
			putchar('U');
		else
			putchar('L');
	}
	putchar('\"');
	putchar('\n');

	printf("%d / %d = %d, %d", forty, three, forty / three, forty % three);

	num = atoi(mynum);
	if (num != 1234)
		printf("failure!\n");
	printf("%d\n", num);

	return 0;
}
