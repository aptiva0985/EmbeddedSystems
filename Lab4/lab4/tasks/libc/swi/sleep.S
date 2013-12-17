/** @file sleep.S
 *
 * @brief sleep syscall wrapper
 *
 * @ Authors: Yuanshuo Cui <ycui@andrew.cmu.edu>
 * @          Yujing Zhang <yujingz@andrew.cmu.edu>
 * @          Liyu Ma      <liyum@andrew.cmu.edu>
 * @ Date:    14:29, 10/30/2013
 *
 */

#include <asm.h>
#include <bits/swi.h>

.file "sleep.S"

FUNC(sleep)
	@ Save registers and stack pointer
    @STMFD sp!, {r1-r12, lr}

    @ Call sleep
    SWI   SLEEP_SWI

    @ Restore registers and program counter
    @LDMFD sp!, {r1-r12, lr}
    MOV   pc, lr
