#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int oddball(int *arr, int len);
int randGenerator(int *arr, int len);

#ifndef N
	#define N (40)
#endif

#define ARRLEN (2*(N) - 1)

int main()
{
	/* Keep this line here */
	srand(time(NULL));

	int *arr = malloc(ARRLEN*sizeof(int));
	int num;

	if (!arr) {
		return -1;
	}

	randGenerator(arr, ARRLEN);
	num = oddball(arr, ARRLEN);

	printf("Oddball is %d\n", num);

	return 0;
}

