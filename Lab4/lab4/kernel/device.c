/**
 * @file device.c
 *
 * @brief Implements simulated devices.
 *
 * Authors: Yuanshuo Cui <ycui@andrew.cmu.edu>
 *          Yujing Zhang <yujingz@andrew.cmu.edu>
 *          Liyu Ma      <liyum@andrew.cmu.edu>
 * Date:    19:50, 11/25/2013
 */

#include <types.h>
#include <assert.h>
#include <task.h>
#include <sched.h>
#include <device.h>
#include <arm/reg.h>
#include <arm/psr.h>
#include <arm/exception.h>

#define NULL 0
extern volatile unsigned long up_time;

/**
 * @brief Fake device maintainence structure.
 * Since our tasks are periodic, we can represent
 * tasks with logical devices.
 * These logical devices should be signalled periodically
 * so that you can instantiate a new job every time period.
 * Devices are signaled by calling dev_update
 * on every timer interrupt. In dev_update check if it is
 * time to create a tasks new job. If so, make the task runnable.
 * There is a wait queue for every device which contains the tcbs of
 * all tasks waiting on the device event to occur.
 */

struct dev
{
    tcb_t* sleep_queue;
    unsigned long   next_match;
};
typedef struct dev dev_t;

/* devices will be periodically signaled at the following frequencies */
const unsigned long dev_freq[NUM_DEVICES] = {100, 200, 500, 50};
static dev_t devices[NUM_DEVICES];

/**
 * @brief Initialize the sleep queues and match values for all devices.
 */
void dev_init(void)
{
    int i = 0;
    for(i = 0; i < NUM_DEVICES; i++)
    {
        devices[i].next_match = up_time + dev_freq[i];
        devices[i].sleep_queue = NULL;
    }
}

/**
 * @brief Puts a task to sleep on the sleep queue until the next
 * event is signalled for the device.
 *
 * @param dev  Device number.
 */
void dev_wait(unsigned int dev __attribute__((unused)))
{
    disable_interrupts();

    // Fetch sleep queue of current device
    tcb_t *tmp = devices[dev].sleep_queue;
    tcb_t *cur_tcb = get_cur_tcb();
    // Try to put cur tcb to the end of the sleep queue
    if(tmp == NULL)
    {
        devices[dev].sleep_queue = cur_tcb;
        devices[dev].sleep_queue->sleep_queue = NULL;
    }
    else
    {
        while(tmp->sleep_queue != NULL)
        {
            tmp = tmp->sleep_queue;
        }
        tmp->sleep_queue = cur_tcb;
        tmp = tmp->sleep_queue;
        tmp->sleep_queue = NULL;
    }

    enable_interrupts();
    // Let current task sleep to wait for device
    dispatch_sleep();
}


/**
 * @brief Signals the occurrence of an event on all applicable devices.
 * This function should be called on timer interrupts to determine that
 * the interrupt corresponds to the event frequency of a device. If the
 * interrupt corresponded to the interrupt frequency of a device, this
 * function should ensure that the task is made ready to run
 */
void dev_update(unsigned long millis __attribute__((unused)))
{
    tcb_t *sleep_tcb = NULL;
    tcb_t *next_tcb = NULL;
    int i;
    int flag = 0;

    disable_interrupts();

    // Iterate through all devices
    for(i = 0; i < NUM_DEVICES; i++)
    {
        // If device match current system time
        if(devices[i].next_match <= millis)
        {
            // Update its next match time
            devices[i].next_match = millis + dev_freq[i];
            // Empty sleep queue
            sleep_tcb = devices[i].sleep_queue;
            devices[i].sleep_queue = NULL;

            // Put every tcb in sleep queue into run queue
            while (sleep_tcb != NULL)
            {
                flag = 1;
                runqueue_add(sleep_tcb, sleep_tcb->cur_prio);

                next_tcb = sleep_tcb->sleep_queue;
                sleep_tcb->sleep_queue = NULL;
                sleep_tcb = next_tcb;
            }
        }
    }

    // If there is at least one tcb has been put to run queue,
    // try to dispatch a task
    if(flag == 1)
    {
        dispatch_save();
    }

    enable_interrupts();
}
