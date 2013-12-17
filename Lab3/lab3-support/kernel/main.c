#include <exports.h>
#include <arm/psr.h>
#include <arm/exception.h>
#include <arm/interrupt.h>
#include <arm/timer.h>
#include <arm/reg.h>
#include <bits/fileno.h>
#include <bits/errno.h>
#include <types.h>
#include <bits/swi.h>

/**
 * Main routine of kernel
 *
 * Authors: Yuanshuo Cui <ycui@andrew.cmu.edu>
 *          Yujing Zhang <yujingz@andrew.cmu.edu>
 *          Liyu Ma      <liyum@andrew.cmu.edu>
 * Date:    14:18, 11/10/2013
 */

uint32_t global_data;
volatile unsigned long up_time;
#define NULL 0
#define invailid_instrction 0x0BADC0DE

extern unsigned s_handler();
extern unsigned irq_handler();
extern int setup();
extern ssize_t read(int fd, void *buf, size_t count);
extern ssize_t write(int fd, const void *buf, size_t count);
extern void exit(int status);
extern unsigned long time(void);
extern void sleep(unsigned long millis);

/**
 * C_SWI_handler
 * A SWI dispatcher that can support five kinds of system interrupt:
 * Read, Write, Exit, Time and Sleep.
 */
void C_SWI_handler(unsigned swi_num, unsigned *regs) {
    char* buf = NULL;
    int fd  = -1;
    int count = -1;
    int exit_status = -1;
    unsigned long millis;

    switch(swi_num) {
        case EXIT_SWI:
            // Extract parameter
            exit_status = regs[0];
            // Call EXIT
            exit(exit_status);

        case READ_SWI:
            // Extract parameters
            fd = regs[0];
            buf = (char *)regs[1];
            count = regs[2];

            // Read characters into buffer from stdin
            regs[0] = read(fd, buf, count);
            // Return with the number of characters read into buffer
            return;

        case WRITE_SWI:
            // Extract parameters
            fd = regs[0];
            buf = (char *)regs[1];
            count = regs[2];

            // Write characters from buffer to stdout
            regs[0] = write(fd, buf, count);
            // Return with the number of characters written to stdout
            return;

        case TIME_SWI:
            regs[0] = time();
            return;

        case SLEEP_SWI:
            millis = regs[0];
            sleep(millis);
            return;

        default:
            printf("Invalid SWI number: %d\n", swi_num);
            exit(invailid_instrction);
    }
    return;
}

/**
 * C_IRQ_handler
 * A IRQ handler that will be triggered every 10ms
 */
void C_IRQ_handler() {
    // Only triggered when OSSR has been set and there is no pending interrupt currently
    if((reg_read(OSTMR_OSSR_ADDR) & OSTMR_OSSR_M0) && (reg_read(INT_ICPR_ADDR) >> INT_OSTMR_0)) {
      // Increment time by 10ms
      up_time += 10;

      // Set OSMR to the next 10ms value
      reg_write(OSTMR_OSMR_ADDR(0), reg_read(OSTMR_OSMR_ADDR(0)) + 32500);

      // Writing a 1 to the bit acknowledges the match and hence, clears that bit
      reg_set(OSTMR_OSSR_ADDR, OSTMR_OSSR_M0);
    }

    return;
}

/**
 * Install our handler
 * Reach the original handler from the vector table
 * Replace the first two instruction in the original handler
 * with the entrance to our handler
 */
void install_handler(unsigned new_addr, unsigned *vec_addr, unsigned *saved_instr1, unsigned *saved_instr2) {
    unsigned vec_instr, op, sign, offset, hand_addr;

    // Get the instruction for Handler in the vector table
    vec_instr = *vec_addr;

    // Get up/down bit in the LDR instruction
    sign = vec_instr >> 23 & 1;

    // Get the operator part in the LDR instruction
    op = vec_instr & 0xFFFFF000;

    // Check whether instruction at 0x08 is in format of LDR pc, [pc, #offset]
    if(op != (unsigned)0xE59FF000 && op != (unsigned)0xE51FF000) {
        printf("Unrecognized instruction\n");
        exit(invailid_instrction);
    }

    // Get the offset part of the LDR instruction
    offset = vec_instr & 0xFFF;
    offset = (sign == 0) ? (-1) * offset : offset;

    // Get the address of the Handler
    hand_addr = *(unsigned *)((unsigned)vec_addr + offset + 0x8);

    // Preserve the original two instructions
    *saved_instr1 = *(unsigned *)hand_addr;
    *saved_instr2 = *(unsigned *)(hand_addr + 4);

    // Install our handler
    *(unsigned *)hand_addr = 0xE51FF004;
    *(unsigned *)(hand_addr + 4) = new_addr;
}

/**
 * Recover original handler
 */
void recover_handler(unsigned *vec_addr, unsigned saved_instr1, unsigned saved_instr2) {
    unsigned vec_instr, sign, offset, hand_addr;

    // Get the instruction for Handler in the vector table
    vec_instr = *vec_addr;
    // Get up/down bit in the LDR instruction
    sign = vec_instr >> 23 & 1;

    // Get the offset part of the LDR instruction
    offset = vec_instr & 0xFFF;
    offset = (sign == 0) ? -1 * offset : offset;

    // Get the address of the Handler
    hand_addr = *(unsigned *)((unsigned)vec_addr + offset + 0x8);

    // Recover the changed two instructions
    *(unsigned *)hand_addr = saved_instr1;
    *(unsigned *)(hand_addr + 4) = saved_instr2;
}


int kmain(int argc, char** argv, uint32_t table) {
	app_startup(); /* bss is valid after this point */
	global_data = table;

    // Original SWI Handler's address in the vector table
    unsigned *vec_addr1 = (unsigned *)0x08;
    // Address of new SWI handler
    unsigned new_addr1 = (unsigned)&s_handler;

    // Original IRQ Handler's address in the vector table
    unsigned *vec_addr2 = (unsigned *)0x18;
    // Address of new IRQ handler
    unsigned new_addr2 = (unsigned)&irq_handler;

    // Save first two instructions in original handler for restore
    unsigned saved_instr1, saved_instr2, saved_instr3, saved_instr4;

    // Install our SWI Handler
    install_handler(new_addr1, vec_addr1, &saved_instr1, &saved_instr2);

    // Install our IRQ Handler
    install_handler(new_addr2, vec_addr2, &saved_instr3, &saved_instr4);

    // Initialize up time
    up_time = 0;

    // Save register values
    unsigned int saved_ICMR, saved_ICLR, saved_OIER;
    saved_ICMR = reg_read(INT_ICMR_ADDR);
    saved_ICLR = reg_read(INT_ICLR_ADDR);
    saved_OIER = reg_read(OSTMR_OIER_ADDR);

    // Set ICMR and ICLR register
    // For ICMR, we only enable OS Timer Match Register 0
    // For ICLR, we will set all interrupt come from OS Timer
    // Match Register 0 to be IRQ.
    reg_write(INT_ICMR_ADDR, 1 << INT_OSTMR_0);
    reg_write(INT_ICLR_ADDR, 0x0);

    // Set OIER and OSMR register
    // For OIER, we only enable OS Timer Match Register 0
    // For OSMR, we set it value to current OSCR value plus 10ms
    reg_clear(OSTMR_OIER_ADDR, OSTMR_OIER_E1 | OSTMR_OIER_E2 | OSTMR_OIER_E3);
    reg_set(OSTMR_OIER_ADDR, OSTMR_OIER_E0);
    reg_write(OSTMR_OSMR_ADDR(0), reg_read(OSTMR_OSCR_ADDR) + 32500);

    // Execute some assembly code: user mode, jump to user program
    // get exit status
    int exit_status;
    exit_status = setup(argc, argv);

    // Restore register values
    reg_write(INT_ICMR_ADDR, saved_ICMR);
    reg_write(INT_ICLR_ADDR, saved_ICLR);
    reg_write(OSTMR_OIER_ADDR, saved_OIER);

    // Recover original SWI handler
    recover_handler(vec_addr1, saved_instr1, saved_instr2);
    // Recover original IRQ handler
    recover_handler(vec_addr2, saved_instr3, saved_instr4);

    return exit_status;
}
