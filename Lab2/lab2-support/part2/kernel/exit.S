@ exit.S: EXIT SWI routine
@
@ Authors: Yuanshuo Cui <ycui@andrew.cmu.edu>
@          Yujing Zhang <yujingz@andrew.cmu.edu>
@          Liyu Ma      <liyum@andrew.cmu.edu>
@ Date:    09:30, 10/19/2013

.global exit

exit:
        @ Restore registers and stack pointer
        MOV   r1, #0xA0000004
        LDR   sp, [r1]
        LDMFD sp!, {r1-r12, lr}
        MOV   pc, lr
