@ s_handler.S: SWI handler
@
@ Authors: Yuanshuo Cui <ycui@andrew.cmu.edu>
@          Yujing Zhang <yujingz@andrew.cmu.edu>
@          Liyu Ma      <liyum@andrew.cmu.edu>
@ Date:    19:17, 11/25/2013

.extern C_SWI_handler
.extern global_data
.global s_handler

s_handler:
        SUB   sp, sp, #4
        @ Store user registers and lr
        STMFD sp!, {r0-r12, lr}

        @ Recover r8 register
        LDR   r2, =global_data
        LDR   r8, [r2]

        @ Store spsr to a specific position
        MRS   r2, spsr
        STR   r2, [sp, #56]

        @ Enable IRQ
        MRS   r2, cpsr
        AND   r2, r2, #0xFFFFFF7F
        MSR   cpsr, r2

        @ r1 now contains pointer to parameters on stack
        MOV   r1, sp

        @ Get the whole SWI instruction
        LDR   r0, [lr, #-4]

        @ Extract the SWI number
        BIC   r0, r0, #0xFF000000

        @ Store stack and link register
        STMFD sp, {sp, lr}^
        ADD   sp, sp, #-8

        @ Go to the corresponding handler
        BL    C_SWI_handler

        @ Restore stack and link register
        LDMFD sp, {sp, lr}^
        add   sp, sp, #8

        @ Restore spsr
        LDR   r2, [sp, #56]
        MSR   spsr, r2

        @ Restore user registers and lr
        LDMFD sp!, {r0-r12, lr}
        ADD   sp, sp, #4

        @ Return from handler
        MOVS  pc, lr
