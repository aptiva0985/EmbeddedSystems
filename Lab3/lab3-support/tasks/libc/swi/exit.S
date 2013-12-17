/** @file exit.S
 *
 * @brief exit sycall wrapper
 *
 * @ Authors: Yuanshuo Cui <ycui@andrew.cmu.edu>
 * @          Yujing Zhang <yujingz@andrew.cmu.edu>
 * @          Liyu Ma      <liyum@andrew.cmu.edu>
 * @ Date:    12:16, 10/14/2013
 *
 */

#include <asm.h>
#include <bits/swi.h>

	.file "exit.S"

FUNC(exit)
	@ No function argument need to convert.
    @ Use SWI instruction to invokes exit syscall.
    @ The exit syscall never returns, and never return an error.
    SWI   EXIT_SWI
