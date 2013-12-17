#include <exports.h>
#include <lock.h>
#include <syscall.h>
#include <bits/swi.h>

/** @file C_SWI_handler.c
 *
 * @brief A SWI dispatcher that can support ten kinds of system interrupt:
 *        Read, Write, Exit, Time, Sleep, Task_create, Event_wait, Mutex_create,
 *        Mutex_lock, Mutex_unlock.
 *
 * Authors: Yuanshuo Cui <ycui@andrew.cmu.edu>
 *          Yujing Zhang <yujingz@andrew.cmu.edu>
 *          Liyu Ma      <liyum@andrew.cmu.edu>
 * Date:    18:01, 11/21/2013
 */

#define NULL 0
#define invailid_instruction 0x0BADC0DE

void C_SWI_handler(unsigned swi_num, unsigned *regs)
{
    char* buf = NULL;
    int fd  = -1;
    int count = -1;
    int mutex = -1;
    unsigned int dev = -1;
    task_t* task = NULL;
    unsigned long millis = 0;

    switch(swi_num)
    {
        case READ_SWI:
            // Extract parameters
            fd = regs[0];
            buf = (char *)regs[1];
            count = regs[2];

            // Read characters into buffer from stdin
            regs[0] = read_syscall(fd, buf, count);
            // Return with the number of characters read into buffer
            return;

        case WRITE_SWI:
            // Extract parameters
            fd = regs[0];
            buf = (char *)regs[1];
            count = regs[2];

            // Write characters from buffer to stdout
            regs[0] = write_syscall(fd, buf, count);
            // Return with the number of characters written to stdout
            return;

        case TIME_SWI:
            regs[0] = time_syscall();
            return;

        case SLEEP_SWI:
            // Extract parameters
            millis = regs[0];

            sleep_syscall(millis);
            return;

        case CREATE_SWI:
            // Extract parameters
            task = (task_t*)regs[0];
            count = (size_t)regs[1];

            regs[0] = task_create(task, count);
            return;

        case MUTEX_CREATE:
            regs[0] = mutex_create();
            return;

        case MUTEX_LOCK:
            // Extract parameters
            mutex = regs[0];

            regs[0] = mutex_lock(mutex);
            return;

        case MUTEX_UNLOCK:
            // Extract parameters
            mutex = regs[0];

            regs[0] = mutex_unlock(mutex);
            return;

        case EVENT_WAIT:
            // Extract parameters
            dev = regs[0];

            regs[0] = event_wait(dev);
            return;

        default:
            printf("Invalid SWI number: %d\n", swi_num);
            regs[0] = invailid_instruction;
    }
    return;
}
