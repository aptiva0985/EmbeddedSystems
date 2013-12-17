/**
 * @file swi.h
 *
 * @brief Definitions for the software interrupt handler.
 *
 * @author Kartik Subramanian <ksubrama@andrew.cmu.edu>
 *
 * @date 2008-11-18
 */

#ifndef _SWI_H_
#define _SWI_H_

#ifndef ASSEMBLER

void init_swi(void);
void destroy_swi(void);
void swi_dispatch(unsigned int swi_num, struct ex_context* c);

#endif /* ASSEMBLER */

#endif /* _SWI_H_ */
