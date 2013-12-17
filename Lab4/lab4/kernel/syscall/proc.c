/** @file proc.c
 *
 * @brief Implementation of `process' syscalls
 *
 * Authors: Yuanshuo Cui <ycui@andrew.cmu.edu>
 *          Yujing Zhang <yujingz@andrew.cmu.edu>
 *          Liyu Ma      <liyum@andrew.cmu.edu>
 * Date:    19:44, 11/25/2013
 */

#include <exports.h>
#include <bits/errno.h>
#include <config.h>
#include <kernel.h>
#include <syscall.h>
#include <sched.h>
#include <../sched/sched_i.h>
#include <lock.h>

#include <arm/reg.h>
#include <arm/psr.h>
#include <arm/exception.h>
#include <arm/physmem.h>
#include <device.h>

#define NULL 0

/**
 * @brief Swap two task by deep copy
 */
void swap(task_t *tasks, size_t from, size_t to)
{
    task_t tmp;
    tmp.lambda = tasks[from].lambda;
    tmp.data = tasks[from].data;
    tmp.stack_pos = tasks[from].stack_pos;
    tmp.C = tasks[from].C;
    tmp.T = tasks[from].T;

    tasks[from].lambda = tasks[to].lambda;
    tasks[from].data = tasks[to].data;
    tasks[from].stack_pos = tasks[to].stack_pos;
    tasks[from].C = tasks[to].C;
    tasks[from].T = tasks[to].T;

    tasks[to].lambda = tmp.lambda;
    tasks[to].data = tmp.data;
    tasks[to].stack_pos = tmp.stack_pos;
    tasks[to].C = tmp.C;
    tasks[to].T = tmp.T;
}

/**
 * @brief Sort all task using bubble sort
 */
void sort_tasks(task_t* tasks, size_t num_tasks)
{
    size_t i;
    size_t j;
    for(i = 0; i < num_tasks; i++)
    {
        for(j = i + 1; j < num_tasks; j++)
        {
            if(tasks[i].T > tasks[j].T)
            {
                swap(tasks, i, j);
            }
        }
    }
}

/**
 * @brief Create tasks
 */
int task_create(task_t* tasks  __attribute__((unused)), size_t num_tasks  __attribute__((unused)))
{
    disable_interrupts();
    // Return EINVAL when num tasks is greater than
    // the maximum number of tasks the OS supports
    if(num_tasks <= 0 || num_tasks > OS_AVAIL_TASKS)
    {
        return -EINVAL;
    }

    // Return EFAULT when tasks points to region whose
    // bounds lie outside valid address space
    if(!valid_addr(tasks, num_tasks, USR_START_ADDR, USR_END_ADDR))
    {
        return -EFAULT;
    }

    // Initialize run queue
    runqueue_init();
    // Initialize device
    dev_init();

    // Do ubtest on tasks
    if (assign_schedule(&tasks, num_tasks) == 0)
    {
        printf("schedule failed\n");
        return -ESCHED;
    }

    // Initialize tcb for each task
    allocate_tasks(&tasks, num_tasks);
    // Initialize IDLE task
    sched_init(NULL);
    // Dispatch IDLE task
    dispatch_nosave();

    return 1;
}

/**
 * @brief Waiting for a certain device number
 */
int event_wait(unsigned int dev  __attribute__((unused)))
{
    tcb_t* cur_tcb = get_cur_tcb();

    // Return EINVAL when the provided device identifier is invalid
    if(dev >= NUM_DEVICES)
    {
        return -EINVAL;
    }

    if(cur_tcb->holds_lock >= 1)
    {
        //printf("current task:%d\n", cur_tcb->holds_lock);
        return -EHOLDSLOCK;
    }

    // Puts the calling task to sleep on given device number
    dev_wait(dev);

    return 0;
}

/* An invalid syscall causes the kernel to exit. */
void invalid_syscall(unsigned int call_num  __attribute__((unused)))
{
    printf("Kernel panic: invalid syscall -- 0x%08x\n", call_num);

    disable_interrupts();

    while(1);
}
