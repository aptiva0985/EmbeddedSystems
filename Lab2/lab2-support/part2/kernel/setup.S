@ setup.S: Change to user mode, setup user stack, push argc and argv
@          call user program.
@
@ Authors: Yuanshuo Cui <ycui@andrew.cmu.edu>
@          Yujing Zhang <yujingz@andrew.cmu.edu>
@          Liyu Ma      <liyum@andrew.cmu.edu>
@ Date:    18:23, 10/20/2013

.global setup

#include <bits/swi.h>

setup:
        @ Save registers and stack pointer
        STMFD sp!, {r1-r12, lr}
        MOV   r2, #0xA0000004
        STR   sp, [r2]

        @ Switch to user mode with IRQs and FIQs masked
        MRS   r2, cpsr
        AND   r2, r2, #0xFFFFFF00
        ORR   r2, r2, #0xD0
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

        @ Jump to a loaded program at address #0xA2000000
        MOV   r2, #0xA2000000
        BLX   r2
        SWI   EXIT_SWI
