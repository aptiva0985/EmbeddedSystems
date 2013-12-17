@ s_handler.S: SWI handler
@
@ Authors: Yuanshuo Cui <ycui@andrew.cmu.edu>
@          Yujing Zhang <yujingz@andrew.cmu.edu>
@          Liyu Ma      <liyum@andrew.cmu.edu>
@ Date:    13:21, 11/05/2013

.extern C_SWI_handler
.global s_handler

s_handler:
        @ Store user registers and lr
        STMFD sp!, {r0-r12, lr}

        @ Mask IRQ
        MRS   r2, cpsr
        AND   r2, r2, #0xFFFFFF7F
        MSR   cpsr, r2

        @ Recover r8 register
        LDR   r2, =SAVED_R8
        LDR   r8, [r2]

        @ r1 now contains pointer to parameters on stack
        MOV   r1, sp

        @ Get the whole SWI instruction
        LDR   r0, [lr, #-4]

        @ Extract the SWI number
        BIC   r0, r0, #0xFF000000

        @ Go to the corresponding handler
        BL    C_SWI_handler

        @ Restore user registers and lr
        LDMFD sp!, {r0-r12, lr}

        @ Return from handler
        MOVS  pc, lr
