/** @file swi.h
 *
 * @brief Defines syscall numbers used in SWI instructions
 *
 * @author Mike Kasick <mkasick@andrew.cmu.edu>
 * @date   Sun, 07 Oct 2007 01:36:02 -0400
 *
 * @author Kartik Subramanian <ksubrama@andrew.cmu.edu>
 * @date 2008-10-31
 */

#ifndef BITS_SWI_H
#define BITS_SWI_H

#define SWI_BASE 0x900000

#define EXIT_SWI  (SWI_BASE + 1)
#define READ_SWI  (SWI_BASE + 3)
#define WRITE_SWI (SWI_BASE + 4)

/* The following are not linux compatible */
#define TIME_SWI  (SWI_BASE + 6)
#define SLEEP_SWI (SWI_BASE + 7)

#endif /* BITS_SWI_H */
