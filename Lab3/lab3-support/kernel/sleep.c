/**
 * Suspend execution for an interval measured in milliseconds
 *
 * Authors: Yuanshuo Cui <ycui@andrew.cmu.edu>
 *          Yujing Zhang <yujingz@andrew.cmu.edu>
 *          Liyu Ma      <liyum@andrew.cmu.edu>
 * Date:    12:18, 11/01/2013
 */
extern volatile unsigned long up_time;

void sleep(unsigned long millis) {
    unsigned long target_time = up_time + millis;

    while(up_time < target_time) {

    }

    return;
}

