/** @file ctx_switch.c
 *
 * @brief C wrappers around assembly context switch routines.
 *
 * Authors: Yuanshuo Cui <ycui@andrew.cmu.edu>
 *          Yujing Zhang <yujingz@andrew.cmu.edu>
 *          Liyu Ma      <liyum@andrew.cmu.edu>
 * Date:    19:32, 11/25/2013
 */

#include <types.h>
#include <assert.h>

#include <config.h>
#include <kernel.h>
#include "sched_i.h"

//#ifdef DEBUG_MUTEX
//#include <exports.h>
//#endif

static tcb_t* cur_tcb; /* use this if needed __attribute__((unused))*/

/**
 * @brief Initialize the current TCB and priority.
 *
 * Set the initialization thread's priority to IDLE so that anything
 * will preempt it when dispatching the first task.
 */
void dispatch_init(tcb_t* idle __attribute__((unused)))
{
    cur_tcb = idle;
    // Remove IDLE from run queue
    runqueue_remove(idle->cur_prio);
    // Switch to IDLE task
    ctx_switch_half(&idle->context);
}


/**
 * @brief Context switch to the highest priority task while saving off the
 * current task state.
 *
 * This function needs to be externally synchronized.
 * We could be switching from the idle task.  The priority searcher has been tuned
 * to return IDLE_PRIO for a completely empty run_queue case.
 */
void dispatch_save(void)
{
    // Only switch to the highest priority task
    if(cur_tcb->cur_prio > highest_prio())
    {
        // Add current task to run queue
        runqueue_add(cur_tcb, cur_tcb->cur_prio);

        // Swap current task and the highest priority task in run queue
        tcb_t* new_tcb = runqueue_remove(highest_prio());
        tcb_t* old_tcb = cur_tcb;
        cur_tcb = new_tcb;

        // Switch to the highest priority task
        ctx_switch_full(&cur_tcb->context, &old_tcb->context);
    }
}

/**
 * @brief Context switch to the highest priority task that is not this task --
 * don't save the current task state.
 *
 * There is always an idle task to switch to.
 */
void dispatch_nosave(void)
{
    // Get the highest priority task in run queue
    tcb_t* new_tcb = runqueue_remove(highest_prio());
    cur_tcb = new_tcb;

    // Switch to the highest priority task
    ctx_switch_half(&new_tcb->context);
}


/**
 * @brief Context switch to the highest priority task that is not this task --
 * and save the current task but don't mark is runnable.
 *
 * There is always an idle task to switch to.
 */
void dispatch_sleep(void)
{
    // Get the highest priority task in run queue
    tcb_t* new_tcb = runqueue_remove(highest_prio());
    tcb_t* old_tcb = cur_tcb;
    cur_tcb = new_tcb;

    // Switch to the highest priority task
    ctx_switch_full(&cur_tcb->context, &old_tcb->context);
}

/**
 * @brief Returns the priority value of the current task.
 */
uint8_t get_cur_prio(void)
{
    return cur_tcb->cur_prio;
}

/**
 * @brief Returns the TCB of the current task.
 */
tcb_t* get_cur_tcb(void)
{
    return cur_tcb;
}
