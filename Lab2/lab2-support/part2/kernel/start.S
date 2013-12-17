@ start.S: Kernel startup routine
@
@ Author: Mike Kasick <mkasick@andrew.cmu.edu>
@ Date:   Tue, 23 Oct 2007 11:20:33 -0400

	.file	"start.S"
	.text

	.global	_start
_start:
	@ Branch immediately to main without altering the execution environment.
	@ This indirection is needed to guarantee that the program entry point is
	@ LOAD_ADDR (probably 0xa3000000) without placing strict
	@ requirements on the layout of kernel.c.
	b	main
