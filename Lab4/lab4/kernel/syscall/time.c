/** @file time.c
 *
 * @brief Kernel timer based syscall implementations
 *
 * Authors: Yuanshuo Cui <ycui@andrew.cmu.edu>
 *          Yujing Zhang <yujingz@andrew.cmu.edu>
 *          Liyu Ma      <liyum@andrew.cmu.edu>
 * Date:    18:01, 11/20/2013
 */

#include <types.h>
#include <config.h>
#include <bits/errno.h>
#include <arm/timer.h>
#include <syscall.h>

extern volatile unsigned long up_time;

/** @brief Return current system time.
 *
 */
unsigned long time_syscall(void)
{
    return up_time;
}



/** @brief Waits in a tight loop for atleast the given number of milliseconds.
 *
 * @param millis  The number of milliseconds to sleep.
 *
 *
 */
void sleep_syscall(unsigned long millis  __attribute__((unused)))
{
    unsigned long target_time = up_time + millis;

    while(up_time < target_time)
    {

    }

    return;
}
