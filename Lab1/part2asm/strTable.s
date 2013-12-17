@ Group 32: strTable.s
@ Yuanshuo Cui (ycui)
@ Yujing Zhang (yujingz)
@ Liyu Ma (liyum)

	.file	"part1-strTable.c"
	.text
	.align	2
	.global	strTable
	.type	strTable, %function
strTable:
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	stmfd	sp!, {r4, r5, r6}
	mov	r4, #0				@ reduce redundant moves by using original registers
	cmp	r4, r3
	bge	.L11
.L9:
	ldrb	ip, [r1, r4]	@ zero_extendqisi2
	cmp	ip, #69				@ reduce number of loops by using pseudo binary search
	subgt	ip, ip, #69
.L7:						@ combine common code of .L9 and .L7
	cmp	ip, #22
	ble	.L13
	subgt	ip, ip, #23
	b	.L7
.L13:
	cmp	ip, r2
	bgt	.L4
	ldrb	r6, [r1, r4]	@ zero_extendqisi2
	strb	r6, [r0, ip]	@ write to source string directly, instead of reading from it, compare and then write
.L4:
	add	r4, r4, #1
	cmp	r4, r3
	blt	.L9
.L11:
	ldmfd	sp!, {r4, r5, r6}
	mov pc, lr
	.size	strTable, .-strTable
	.ident	"GCC: (GNU) 3.4.5"
