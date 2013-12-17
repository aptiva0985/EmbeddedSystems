#include <config.h>
#include <syscall.h>
#include <arm/interrupt.h>
#include <arm/timer.h>
#include <arm/reg.h>
#include <device.h>
#include <sched.h>

/** @file C_IRQ_handler.c
 *
 * @brief A IRQ handler that will be triggered every 10ms
 *
 * Authors: Yuanshuo Cui <ycui@andrew.cmu.edu>
 *          Yujing Zhang <yujingz@andrew.cmu.edu>
 *          Liyu Ma      <liyum@andrew.cmu.edu>
 * Date:    16:11, 11/21/2013
 */

#define FREQ 32500

extern volatile unsigned long up_time;

void C_IRQ_handler()
{
    // Only triggered when OSSR has been set and there is no pending interrupt currently
    if((reg_read(OSTMR_OSSR_ADDR) & OSTMR_OSSR_M0) && (reg_read(INT_ICPR_ADDR) >> INT_OSTMR_0))
    {
      // Increment time by 10ms
      up_time += OS_TIMER_RESOLUTION;

      // Set OSMR to the next 10ms value
      reg_write(OSTMR_OSMR_ADDR(0), reg_read(OSTMR_OSCR_ADDR) + FREQ);

      // Writing a 1 to the bit acknowledges the match and hence, clears that bit
      reg_set(OSTMR_OSSR_ADDR, OSTMR_OSSR_M0);

      dev_update(up_time);
    }
}

