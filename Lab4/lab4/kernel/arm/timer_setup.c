#include <arm/reg.h>
#include <arm/timer.h>
#include <arm/interrupt.h>
#include <exports.h>

/** @file timer_setup.c
 *
 * @brief Set up OS timer
 *
 * Authors: Yuanshuo Cui <ycui@andrew.cmu.edu>
 *          Yujing Zhang <yujingz@andrew.cmu.edu>
 *          Liyu Ma      <liyum@andrew.cmu.edu>
 * Date:    16:01, 11/23/2013
 */

#define RESOLUTION 32500

void timer_setup() {
    // Set OIER and OSMR register
    // For OIER, we only enable OS Timer Match Register 0
    // For OSMR, we set it value to current OSCR value plus 10ms
    reg_clear(OSTMR_OIER_ADDR, OSTMR_OIER_E1 | OSTMR_OIER_E2 | OSTMR_OIER_E3);
    reg_set(OSTMR_OIER_ADDR, OSTMR_OIER_E0);
    reg_write(OSTMR_OSMR_ADDR(0), reg_read(OSTMR_OSCR_ADDR) + RESOLUTION);
}
