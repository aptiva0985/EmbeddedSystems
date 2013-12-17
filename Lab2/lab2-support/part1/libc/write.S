@ write.S: write sycall wrapper
@
@ Authors: Yuanshuo Cui <ycui@andrew.cmu.edu>
@          Yujing Zhang <yujingz@andrew.cmu.edu>
@          Liyu Ma      <liyum@andrew.cmu.edu>
@ Date:    12:16, 10/14/2013

#include <bits/swi.h>

    .file    "write.S"
    .text

    .global  write
write:
    @ Call write.
    swi WRITE_SWI
    @ Find out the return is negative or not.
    cmp r0, #0
    @ For positive return value, just return original value.
    bge .L1
    @ For negative return value, compute absolute value.
    rsb r0, r0, #0
    @ Set errno value to the global errno varible.
    ldr r1, =errno
    str r0, [r1]
    @ Set return value to -1.
    mov r0, #-1
.L1:
    mov pc, lr
