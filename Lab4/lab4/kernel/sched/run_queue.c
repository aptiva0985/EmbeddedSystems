/** @file run_queue.c
 *
 * @brief Run queue maintainence routines.
 *
 * Authors: Yuanshuo Cui <ycui@andrew.cmu.edu>
 *          Yujing Zhang <yujingz@andrew.cmu.edu>
 *          Liyu Ma      <liyum@andrew.cmu.edu>
 * Date:    19:28, 11/24/2013
 */

#include <types.h>
#include <assert.h>

#include <kernel.h>
#include <sched.h>
#include "sched_i.h"

#define NULL 0

static tcb_t* run_list[OS_MAX_TASKS]  __attribute__((unused));

/* A high bit in this bitmap means that the task whose priority is
 * equal to the bit number of the high bit is runnable.
 */
static uint8_t run_bits[OS_MAX_TASKS/8] __attribute__((unused));

/* This is a trie structure.  Tasks are grouped in groups of 8.  If any task
 * in a particular group is runnable, the corresponding group flag is set.
 * Since we can only have 64 possible tasks, a single byte can represent the
 * run bits of all 8 groups.
 */
static uint8_t group_run_bits __attribute__((unused));

/* This unmap table finds the bit position of the lowest bit in a given byte
 * Useful for doing reverse lookup.
 */
static uint8_t prio_unmap_table[]  __attribute__((unused)) =
{
0, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0,
4, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0,
5, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0,
4, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0,
6, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0,
4, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0,
5, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0,
4, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0,
7, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0,
4, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0,
5, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0,
4, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0,
6, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0,
4, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0,
5, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0,
4, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0
};

/**
 * @brief Clears the run-queues and sets them all to empty.
 */
void runqueue_init(void)
{
    group_run_bits = 0;

    int i = 0;

    // Initialize run bit
    for(i = 0; i < OS_MAX_TASKS / 8; i++)
    {
        run_bits[i] = 0;
    }

    // Initialize run list
    for(i = 0; i < OS_MAX_TASKS; i++)
    {
        run_list[i] = NULL;
    }
}

/**
 * @brief Adds the thread identified by the given TCB to the runqueue at
 * a given priority.
 *
 * The native priority of the thread need not be the specified priority.  The
 * only requirement is that the run queue for that priority is empty.  This
 * function needs to be externally synchronized.
 */
void runqueue_add(tcb_t* tcb  __attribute__((unused)), uint8_t prio  __attribute__((unused)))
{
    tcb->cur_prio = prio;

    // Get group number and position
    uint8_t pos = prio & 0x07;
    uint8_t group = prio >> 3;

    // Update run queue
    run_list[prio] = tcb;
    run_bits[group] = run_bits[group] | (1 << pos);
    group_run_bits = group_run_bits | (1 << group);
}


/**
 * @brief Empty the run queue of the given priority.
 *
 * @return  The tcb at enqueued at the given priority.
 *
 * This function needs to be externally synchronized.
 */
tcb_t* runqueue_remove(uint8_t prio  __attribute__((unused)))
{
    // Get group number and position
    uint8_t pos = prio & 0x07;
    uint8_t group = prio >> 3;

    // Update run queue
    tcb_t* highest_task = run_list[prio];
    run_list[prio] = NULL;
    run_bits[group] = run_bits[group] & (~(1 << pos));

    // If there is no task in a group, modify group run bit
    if(run_bits[group] == 0)
    {
        group_run_bits = group_run_bits & (~(1 << group));
    }

    return highest_task;
}

/**
 * @brief This function examines the run bits and the run queue and returns the
 * priority of the runnable task with the highest priority (lower number).
 */
uint8_t highest_prio(void)
{
    // Get group number and position of the highest priority task
    uint8_t group = prio_unmap_table[group_run_bits];
    uint8_t pos = prio_unmap_table[run_bits[group]];

    return (group << 3) + pos;
}
