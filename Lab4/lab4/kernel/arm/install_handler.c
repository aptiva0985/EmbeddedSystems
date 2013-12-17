#include <exports.h>

/** @file install_handler.c
 *
 * @brief Install our handler
 *        Reach the original handler from the vector table
 *        Replace the first two instruction in the original handler
 *        with the entrance to our handler
 *
 * Authors: Yuanshuo Cui <ycui@andrew.cmu.edu>
 *          Yujing Zhang <yujingz@andrew.cmu.edu>
 *          Liyu Ma      <liyum@andrew.cmu.edu>
 * Date:    16:01, 11/21/2013
 */
 
#define OPMASK 0xFFFFF000
#define VALIDOP1 0xE59FF000
#define VALIDOP2 0xE51FF000
#define JUMPOP 0xE51FF004

int install_handler(unsigned new_addr, unsigned *vec_addr, unsigned *saved_instr1, unsigned *saved_instr2)
{
    unsigned vec_instr, op, sign, offset, hand_addr;

    // Get the instruction for Handler in the vector table
    vec_instr = *vec_addr;

    // Get up/down bit in the LDR instruction
    sign = vec_instr >> 23 & 1;

    // Get the operator part in the LDR instruction
    op = vec_instr & OPMASK;

    // Check whether instruction at 0x08 is in format of LDR pc, [pc, #offset]
    if(op != (unsigned)VALIDOP1 && op != (unsigned)VALIDOP2)
    {
        printf("Unrecognized instruction\n");
        return -1;
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
    *(unsigned *)hand_addr = JUMPOP;
    *(unsigned *)(hand_addr + 4) = new_addr;

    return 0;
}

