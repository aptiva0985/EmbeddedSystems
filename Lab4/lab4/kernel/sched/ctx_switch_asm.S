/** @file ctx_switch_asm.S
 *
 * @brief Contains the core context switch routine.
 *
 * These routines all assume that the caller is following AAPCS, needs
 * no co-processor registers and that the caller is in SVC mode. Furthermore,
 * the SPSR is considered a spare/scratch register and hence, is not saved.
 *
 * Authors: Yuanshuo Cui <ycui@andrew.cmu.edu>
 *          Yujing Zhang <yujingz@andrew.cmu.edu>
 *          Liyu Ma      <liyum@andrew.cmu.edu>
 * Date:    17:23, 11/23/2013
 */

    .file "ctx_switch_asm.S"

#include <asm.h>
#include <arm/psr.h>

/**
 * @brief Special exit routine from the scheduler that launches a task for the
 * first time.
 *
 * r4 contains the user entry point.
 * r5 contains the single argument to the user function called.
 * r6 contains the user-mode stack pointer.
 * Upon completion, we should be in user mode.
 */
FUNC(launch_task)
    mov     r0, r5
    mov     r1, #0
    mov     r2, #0
    mov     r3, #0
    mov     r5, #0
    mov     r8, #0
    mov     ip, #0
    msr     cpsr_c, #(PSR_MODE_USR)
    mov     sp, r6
    mov     r6, #0
    ldr     lr, =0xdeadbeef   /* Causes a crash instead of calling the reset vector */
    mov     pc, r4

/* r0 points to the target context, r1 to the current context. */
/* Perform a full context switch */
FUNC(ctx_switch_full)
    @ Save current context
    STMIA r1, {r4-r11, sp, lr}
    @ Load target context
    LDMIA r0, {r4-r11, sp, lr}
    MOV   pc, lr


/* Perform a half context switch */
FUNC(ctx_switch_half)
    @ Load target context
    LDMIA r0, {r4-r11, sp, lr}
    MOV   pc, lr
