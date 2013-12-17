@ exit.S: exit sycall wrapper
@
@ Authors: Yuanshuo Cui <ycui@andrew.cmu.edu>
@          Yujing Zhang <yujingz@andrew.cmu.edu>
@          Liyu Ma      <liyum@andrew.cmu.edu>
@ Date:    12:16, 10/14/2013

#include <bits/swi.h>

    .file    "exit.S"
    .text

    .global  exit
exit:
    @ No function argument need to convert.
    @ Use SWI instruction to invokes exit syscall.
    @ The exit syscall never returns, and never return an error.
    swi EXIT_SWI
