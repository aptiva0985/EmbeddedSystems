@ setup.S: Change to user mode, setup user stack, push argc and argv
@          call user program.
@
@ Authors: Yuanshuo Cui <ycui@andrew.cmu.edu>
@          Yujing Zhang <yujingz@andrew.cmu.edu>
@          Liyu Ma      <liyum@andrew.cmu.edu>
@ Date:    18:33, 11/24/2013

.global setup
.extern global_data

#include <config.h>

setup:
        @ Switch to user mode with FIQ masked
        MRS   r2, cpsr
        AND   r2, r2, #0xFFFFFF00
        ORR   r2, r2, #0x10
        MSR   cpsr, r2

        @ Setup a full descending user mode stack
        @ Stack top at #0xA3000000
        MOV   sp, #USR_STACK

        @ Restore register r8
        LDR  r2, =global_data
        LDR  r8, [r2]

        @ Jump to a loaded program at address #0xA0000000
        MOV   r2, #LOAD_ADDR
        BLX   r2
