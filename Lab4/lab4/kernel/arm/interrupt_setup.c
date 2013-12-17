#include <arm/reg.h>
#include <arm/timer.h>
#include <arm/interrupt.h>
#include <exports.h>

/** @file interrupt_setup.c
 *
 * @brief Set up interrupt controller registers
 *
 * Authors: Yuanshuo Cui <ycui@andrew.cmu.edu>
 *          Yujing Zhang <yujingz@andrew.cmu.edu>
 *          Liyu Ma      <liyum@andrew.cmu.edu>
 * Date:    16:01, 11/21/2013
 */

void interrupt_setup()
{
    // Set ICMR and ICLR register
    // For ICMR, we only enable OS Timer Match Register 0
    // For ICLR, we will set all interrupt come from OS Timer
    // Match Register 0 to be IRQ.
    reg_write(INT_ICMR_ADDR, 1 << INT_OSTMR_0);
    reg_write(INT_ICLR_ADDR, 0x0);
}
