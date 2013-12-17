/** @file time.S
 *
 * @brief time sycall wrapper
 *
 * @ Authors: Yuanshuo Cui <ycui@andrew.cmu.edu>
 * @          Yujing Zhang <yujingz@andrew.cmu.edu>
 * @          Liyu Ma      <liyum@andrew.cmu.edu>
 * @ Date:    14:29, 10/30/2013
 *
 */

#include <asm.h>
#include <bits/swi.h>

.file "time.S"

FUNC(time)
    @ Save registers and stack pointer
    @ STMFD sp!, {r1-r12, lr}

    @ Call time
    SWI   TIME_SWI

    @ Restore registers and program counter
    @ LDMFD sp!, {r1-r12, lr}
    MOV   pc, lr
