@ setup.S: Change to user mode, setup user stack, push argc and argv
@          call user program.
@
@ Authors: Yuanshuo Cui <ycui@andrew.cmu.edu>
@          Yujing Zhang <yujingz@andrew.cmu.edu>
@          Liyu Ma      <liyum@andrew.cmu.edu>
@ Date:    18:23, 11/09/2013

.global setup

.comm SAVED_SP, 4, 4
.comm SAVED_R8, 4, 4

.equ USER_PROG_ADD, 0xA0000000
.equ IRQ_STACK, 0xA2000000

#include <bits/swi.h>

setup:
        @ Save registers, r8 and stack pointer
        STMFD sp!, {r1-r7, r9-r12, lr}
        LDR   r2, =SAVED_SP
        STR   sp, [r2]
        LDR   r2, =SAVED_R8
        STR   r8, [r2]

        @ Switch to IRQ mode, set its stack
        MRS   r2, cpsr
        AND   r2, r2, #0xFFFFFFF0
        ORR   r2, r2, #0x2
        MSR   cpsr, r2
        MOV   sp, #IRQ_STACK

        @ Switch to user mode with FIQ masked
        MRS   r2, cpsr
        AND   r2, r2, #0xFFFFFF00
        ORR   r2, r2, #0x50
        MSR   cpsr, r2

        @ Setup a full descending user mode stack
        @ Stack top at #0xA3000000
        MOV   sp, #0xA3000000

        @ Push NULL at the and of argv
        MOV   r2, #0
        STMFD sp!, {r2}

        @ Save U-Boots argc
        MOV   r2, r0

        @ Push U-Boots argvs to the user stack
.pushargv:
        @ Get offset for next argument (multiple of 4)
        SUBS  r2, r2, #1
        @ If already pushed all arguments
        BLT   .loadprogram
        @ Load the argument into register
        LDR   r3, [r1, r2, LSL #2]
        @ Push the arugment into user stack
        STMFD sp!, {r3}
        B     .pushargv

.loadprogram:
        @ Push U-Boots argc to the user stack
        STMFD sp!, {r0}

        @ Jump to a loaded program at address #0xA0000000
        MOV   r2, #USER_PROG_ADD
        BLX   r2
        SWI   EXIT_SWI
