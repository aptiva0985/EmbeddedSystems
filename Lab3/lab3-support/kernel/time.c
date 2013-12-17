/**
 * Retrieve the time in milliseconds since the kernel booted up
 *
 * Authors: Yuanshuo Cui <ycui@andrew.cmu.edu>
 *          Yujing Zhang <yujingz@andrew.cmu.edu>
 *          Liyu Ma      <liyum@andrew.cmu.edu>
 * Date:    12:18, 11/01/2013
 */
extern volatile unsigned long up_time;

unsigned long time(void) {
    return up_time;
}

