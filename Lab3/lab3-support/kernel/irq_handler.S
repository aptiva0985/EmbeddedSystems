@ irq_handler.S: IRQ handler
@
@ Authors: Yuanshuo Cui <ycui@andrew.cmu.edu>
@          Yujing Zhang <yujingz@andrew.cmu.edu>
@          Liyu Ma      <liyum@andrew.cmu.edu>
@ Date:    11:03, 11/06/2013

.extern C_IRQ_handler
.global irq_handler

irq_handler:
        @ Store user registers and lr
        STMFD sp!, {r0-r12, lr}

        @ Recover r8 register
        LDR   r2, =SAVED_R8
        LDR   r8, [r2]

        BL    C_IRQ_handler

        @ Restore user registers and lr
        LDMFD sp!, {r0-r12, lr}

        @ Return from handler
        @ Make sure the value of pc will be the add
        @ of next instruction
        SUBS  pc, lr, #4
