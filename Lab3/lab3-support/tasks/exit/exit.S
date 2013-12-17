@ exit.S: A minimal ARM Linux assembly program
@
@ Author: Mike Kasick <mkasick@andrew.cmu.edu>
@ Date:   Sun, 07 Oct 2007 01:23:24 -0400

#include <bits/swi.h>

	.file	"exit.S"
	.text

	.global	_start
_start:
	mov	r0, #42
	swi	EXIT_SWI
