/*
 * kernel.c: Kernel main (entry) function
 *
 * Authors: Yuanshuo Cui <ycui@andrew.cmu.edu>
 *          Yujing Zhang <yujingz@andrew.cmu.edu>
 *          Liyu Ma      <liyum@andrew.cmu.edu>
 * Date:    12:18, 10/14/2013
 */

#include <exports.h>
#include <bits/fileno.h>
#include <bits/errno.h>
#include <bits/swi.h>

#define NULL 0

extern unsigned s_handler();
extern int setup();
extern ssize_t read(int fd, void *buf, size_t count);
extern ssize_t write(int fd, const void *buf, size_t count);
extern void exit(int status);

// C_SWI_handler: SWI dispatcher
void C_SWI_handler(unsigned swi_num, unsigned *regs) {
    char* buf = NULL;
    int fd  = -1;
    int count = -1;
    int exit_status = -1;

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

        default:
            printf("Invalid SWI number: %d\n", swi_num);
            exit(0x0BADC0DE);
    }
    return;
}

/**
 * Install our SWI handler
 * Reach the original SWI handler from the vector table
 * Replace the first two instruction in the original handler
 * with the entrance to our SWI handler
 *
 */
void install_handler(unsigned new_S_addr, unsigned *SWI_vec_addr, unsigned *saved_instr1, unsigned *saved_instr2) {
    unsigned vec_instr, op, sign, offset, SWI_hand_addr;

    // Get the instruction for SWI Handler in the vector table
    vec_instr = *SWI_vec_addr;

    // Get up/down bit in the LDR instruction
    sign = vec_instr >> 23 & 1;

    // Get the operator part in the LDR instruction
    op = vec_instr & 0xFFFFF000;

    // Check whether instruction at 0x08 is in format of LDR pc, [pc, #offset]
    if(op != (unsigned)0xe59FF000 && op != (unsigned)0xE51FF000) {
        printf("Unrecognized instruction\n");
        exit(0x0BADC0E);
    }

    // Get the offset part of the LDR instruction
    offset = vec_instr & 0xFFF;
    offset = (sign == 0) ? (-1) * offset : offset;

    // Get the address of the SWI Handler
    SWI_hand_addr = *(unsigned *)((unsigned)SWI_vec_addr + offset + 0x8);

    // Preserve the original two instructions
    *saved_instr1 = *(unsigned *)SWI_hand_addr;
    *saved_instr2 = *(unsigned *)(SWI_hand_addr + 4);

    // Install our SWI handler
    *(unsigned *)SWI_hand_addr = 0xE51FF004; // LDR pc, [pc, #-4]
    *(unsigned *)(SWI_hand_addr + 4) = new_S_addr;
}

/**
 * Recover original SWI handler
 */
void recover_handler(unsigned *SWI_vec_addr, unsigned saved_instr1, unsigned saved_instr2) {
    unsigned vec_instr, sign, offset, SWI_hand_addr;

    // Get the instruction for SWI Handler in the vector table
    vec_instr = *SWI_vec_addr;
    // Get up/down bit in the LDR instruction
    sign = vec_instr >> 23 & 1;

    // Get the offset part of the LDR instruction
    offset = vec_instr & 0xFFF;
    offset = (sign == 0) ? -1 * offset : offset;

    // Get the address of the SWI Handler
    SWI_hand_addr = *(unsigned *)((unsigned)SWI_vec_addr + offset + 0x8);

    // Recover the changed two instructions
    *(unsigned *)SWI_hand_addr = saved_instr1;
    *(unsigned *)(SWI_hand_addr + 4) = saved_instr2;
}

/**
 * Install handler, stack setup and restore handler
 */
int main(int argc, char *argv[]) {
    // Original SWI Handler's address in the vector table
    unsigned *SWI_vec_addr = (unsigned *)0x08;
    // Address of new SWI handler
    unsigned new_S_addr = (unsigned)&s_handler;
    // Save first two instructions in original handler for restore
    unsigned saved_instr1, saved_instr2;

    // Install our SWI Handler
    install_handler(new_S_addr, SWI_vec_addr, &saved_instr1, &saved_instr2);

    // Execute some assembly code: user mode, jump to user program
    // get exit status
    int exit_status;
    exit_status = setup(argc, argv);

    // Recover original SWI handler
    recover_handler(SWI_vec_addr, saved_instr1, saved_instr2);

    return exit_status;
}
