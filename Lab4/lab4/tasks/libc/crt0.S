/** @file crt0.S
 *
 * @brief Execution startup routine
 *
 * @ Authors: Yuanshuo Cui <ycui@andrew.cmu.edu>
 * @          Yujing Zhang <yujingz@andrew.cmu.edu>
 * @          Liyu Ma      <liyum@andrew.cmu.edu>
 * @ Date:    18:38, 11/24/2013
 *
 */

#include <asm.h>
#include <bits/swi.h>

.file "crt0.S"

FUNC(_start)
    @ Generate main¡¯s argc parameter from values placed on stack by kernel.
    @ [sp] -> r0
    LDR   r0, [sp]

    @ Generate main¡¯s argv parameter from values placed on stack by kernel.
    @ [sp+4] -> r1
    ADD   r1, sp, #4

    @ Save the return address to link register.
    @ And then call the main function.
    BL    main

    @ If user program return, then enter infinite loop.
.loop:
    B .loop
