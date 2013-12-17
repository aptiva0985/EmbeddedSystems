/** @file mutex_create.S
 *
 * @brief mutex_create sycall wrapper
 *
 * @ Authors: Yuanshuo Cui <ycui@andrew.cmu.edu>
 * @          Yujing Zhang <yujingz@andrew.cmu.edu>
 * @          Liyu Ma      <liyum@andrew.cmu.edu>
 * @ Date:    13:32, 11/24/2013
 *
 */

#include <asm.h>
#include <bits/swi.h>

.file "mutex_create.S"

FUNC(mutex_create)
    @ Call mutex_create
    SWI MUTEX_CREATE

    @ Find out the return is negative or not.
    CMP   r0, #0

    @ For positive return value, just return original value.
    BGE   .L1

    @ For negative return value, compute absolute value.
    RSB   r0, r0, #0

    @ Set errno value to the global errno varible.
    LDR   r1, =errno
    STR   r0, [r1]

    @ Set return value to -1.
    MOV   r0, #-1
.L1:
    MOV   pc, lr
