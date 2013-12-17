/*
 * swi.h: Defines syscall numbers used in SWI instructions
 *
 * Author: Mike Kasick <mkasick@andrew.cmu.edu>
 * Date:   Sun, 07 Oct 2007 01:36:02 -0400
 */

#ifndef BITS_SWI_H
#define BITS_SWI_H

#define SWI_BASE 0x900000

#define EXIT_SWI  (SWI_BASE + 1)
#define READ_SWI  (SWI_BASE + 3)
#define WRITE_SWI (SWI_BASE + 4)

#endif /* BITS_SWI_H */
