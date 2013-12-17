/**
 * @file exception.h
 *
 * @brief Definitions for ARM exception vectoring.
 *
 * @author Kartik Subramanian <ksubrama@andrew.cmu.edu>
 *
 * @date 2008-07-23
 */

#ifndef _EXCEPTION_H_
#define _EXCEPTION_H_

#define EX_RESET        0
#define EX_UD           1
#define EX_SWI          2
#define EX_FABRT        3
#define EX_DABRT        4
#define EX_IRQ          6
#define EX_FIQ          7
#define NUM_EXCEPTIONS  8

#ifndef ASSEMBLER

/* C function prototypes go here! Add your group name at the top! */

#endif /* ASSEMBLER */

#endif /* _EXCEPTION_H_ */
