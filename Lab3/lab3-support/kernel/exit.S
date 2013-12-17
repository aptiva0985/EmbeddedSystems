@ exit.S: EXIT SWI routine
@
@ Authors: Yuanshuo Cui <ycui@andrew.cmu.edu>
@          Yujing Zhang <yujingz@andrew.cmu.edu>
@          Liyu Ma      <liyum@andrew.cmu.edu>
@ Date:    09:30, 11/05/2013

.global exit

exit:
        @ Restore registers and stack pointer
        LDR   r2, =SAVED_SP
        LDR   sp, [r2]
        LDMFD sp!, {r1-r7, r9-r12, lr}
        MOV   pc, lr
