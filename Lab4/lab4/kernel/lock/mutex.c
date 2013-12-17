/**
 * @file mutex.c
 *
 * @brief Implements mutices.
 *
 * Authors: Yuanshuo Cui <ycui@andrew.cmu.edu>
 *          Yujing Zhang <yujingz@andrew.cmu.edu>
 *          Liyu Ma      <liyum@andrew.cmu.edu>
 * Date:    19:56, 12/1/2013
 */

//#define DEBUG_MUTEX

#include <lock.h>
#include <task.h>
#include <sched.h>
#include <bits/errno.h>
#include <arm/psr.h>
#include <arm/exception.h>
#ifdef DEBUG_MUTEX
#include <exports.h>
#endif

#define NULL 0

mutex_t gtMutex[OS_NUM_MUTEX];
volatile int cur_mutex_num = 0;

/**
 * @brief Initialize all mutex
 */
void mutex_init()
{
    int i;
    for(i = 0; i < OS_NUM_MUTEX; i++)
    {
        gtMutex[i].bAvailable = TRUE;
        gtMutex[i].bLock = FALSE;
        gtMutex[i].pHolding_Tcb = NULL;
        gtMutex[i].pSleep_queue = NULL;
    }
}

/**
 * @brief Try to create a new mutex
 */
int mutex_create(void)
{
    int i = 0;

    disable_interrupts();

    // If no available mutex
    if(cur_mutex_num == OS_NUM_MUTEX)
    {
        enable_interrupts();
        // Return error value
        return -ENOMEM;
    }
    else
    {
        // Try to find an available mutex
        while(i < OS_NUM_MUTEX)
        {
            if (gtMutex[i].bAvailable)
            {
                // Change state after find
                gtMutex[i].bAvailable = FALSE;
                break;
            }
            i++;
        }
        // Update current in-use mutex amount
        cur_mutex_num++;

        enable_interrupts();
        return i;
    }
}

/**
 * @brief Lock a mutex
 *
 * @param mutex  Mutex number that try to lock
 */
int mutex_lock(int mutex  __attribute__((unused)))
{
    mutex_t *tmp_mutex;
    tcb_t* cur_tcb;

    disable_interrupts();

    // If mutex number is invalid
    if(mutex < 0 || mutex >= OS_NUM_MUTEX)
    {
        enable_interrupts();
        // Return error value
        return -EINVAL;
    }

    // If mutex is still available
    tmp_mutex = &(gtMutex[mutex]);
    if(tmp_mutex->bAvailable == TRUE)
    {
        enable_interrupts();
        // Return error value
        return -EINVAL;
    }

    // If mutex is held by current task
    cur_tcb = get_cur_tcb();
    if(cur_tcb == tmp_mutex->pHolding_Tcb)
    {
        enable_interrupts();
        // Return error value
        return -EDEADLOCK;
    }

    // If mutex is held by other task
    if(tmp_mutex->bLock == TRUE)
    {
        // Find the end of sleep queue to put current task
        tcb_t * tcb_sleep;
        if(tmp_mutex->pSleep_queue == NULL)
        {
            tmp_mutex->pSleep_queue = cur_tcb;
            cur_tcb->sleep_queue = NULL;
        }
        else
        {
            tcb_sleep = tmp_mutex->pSleep_queue;
            while(tcb_sleep->sleep_queue != NULL)
            {
                tcb_sleep = tcb_sleep->sleep_queue;
            }
            tcb_sleep->sleep_queue = cur_tcb;
            cur_tcb->sleep_queue = NULL;
        }

        // Sleep current task
        dispatch_sleep();
    }

    // When mutex is not held by any other process,
    // hold this mutex immediately.
    tmp_mutex->bLock = TRUE;
    tmp_mutex->pHolding_Tcb = cur_tcb;

    // Set the priority of current task to be the highest.
    //    Set the "holds_lock" flag to.
    cur_tcb->holds_lock = cur_tcb->holds_lock + 1;
    cur_tcb->native_prio = cur_tcb->cur_prio;
    cur_tcb->cur_prio = 0;

    enable_interrupts();
    return 0;
}

/**
 * @brief Unlock a mutex
 *
 * @param mutex  Mutex number that try to unlock
 */
int mutex_unlock(int mutex  __attribute__((unused)))
{
    mutex_t *tmp_mutex;
    tcb_t * cur_tcb, *next_tcb;
    //int i = 0;

    disable_interrupts();

    // If mutex number is invalid
    if(mutex < 0 || mutex >= OS_NUM_MUTEX)
    {
        enable_interrupts();
        // Return error value
        return -EINVAL;
    }

    // If mutex is still available
    tmp_mutex = &(gtMutex[mutex]);
    if(tmp_mutex->bAvailable == TRUE)
    {
        enable_interrupts();
        // Return error value
        return -EINVAL;
    }

    // If mutex is not held by current task
    cur_tcb = get_cur_tcb();
    if(cur_tcb != tmp_mutex->pHolding_Tcb)
    {
        enable_interrupts();
        // Return error value
        return -EPERM;
    }

    // Release this mutex immediately.
    tmp_mutex->bLock = FALSE;
    tmp_mutex->pHolding_Tcb = NULL;

    // Let the first task in sleep queue to continue
    if(tmp_mutex->pSleep_queue != NULL)
    {
        next_tcb = tmp_mutex->pSleep_queue;
        tmp_mutex->pSleep_queue = next_tcb->sleep_queue;
        next_tcb->sleep_queue = NULL;

        // Add first task in sleep queue to run queue
        runqueue_add(next_tcb, next_tcb->cur_prio);
    }

    // See if the current task also holds other mutexes.
    cur_tcb->holds_lock = cur_tcb->holds_lock - 1;

    // If not, set priority back to its original priority
    //    and set "holds_lock" flag back to not holding any lock.
    if (cur_tcb->holds_lock == 0)
    {
        cur_tcb->cur_prio = cur_tcb->native_prio;
    }

    enable_interrupts();
    return 0;
}
