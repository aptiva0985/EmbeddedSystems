/** @file read.S
 *
 * @brief read sycall wrapper
 *
 * @ Authors: Yuanshuo Cui <ycui@andrew.cmu.edu>
 * @          Yujing Zhang <yujingz@andrew.cmu.edu>
 * @          Liyu Ma      <liyum@andrew.cmu.edu>
 * @ Date:    12:16, 10/14/2013
 *
 */

#include <asm.h>
#include <bits/swi.h>

.file "read.S"

FUNC(read)
    @ Call read.
    SWI   READ_SWI
    
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
