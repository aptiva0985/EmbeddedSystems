/** @file ub_test.c
 *
 * @brief The UB Test for basic schedulability
 *
 * Authors: Yuanshuo Cui <ycui@andrew.cmu.edu>
 *          Yujing Zhang <yujingz@andrew.cmu.edu>
 *          Liyu Ma      <liyum@andrew.cmu.edu>
 * Date:    15:15, 12/01/2013
 */

//#define DEBUG 0

#include <sched.h>
#ifdef DEBUG
#include <exports.h>
#endif

static double bounds[63] __attribute__((unused)) =
    {1.000000, 0.828427, 0.779763, 0.756828, 0.743492, 0.734772,
     0.728627, 0.724062, 0.720538, 0.717735, 0.715452, 0.713557,
     0.711959, 0.710593, 0.709412, 0.708381, 0.707472, 0.706666,
     0.705946, 0.705298, 0.704713, 0.704182, 0.703698, 0.703254,
     0.702846, 0.702469, 0.702121, 0.701798, 0.701497, 0.701217,
     0.700955, 0.700709, 0.700478, 0.700261, 0.700056, 0.699863,
     0.699681, 0.699508, 0.699343, 0.699188, 0.699040, 0.698898,
     0.698764, 0.698636, 0.698513, 0.698396, 0.698284, 0.698176,
     0.698073, 0.697974, 0.697879, 0.697788, 0.697700, 0.697615,
     0.697533, 0.697455, 0.697379, 0.697306, 0.697235, 0.697166,
     0.697100, 0.697036, 0.696974};
/**
 * @brief Perform UB Test and reorder the task list.
 *
 * The task list at the end of this method will be sorted in order is priority
 * -- from highest priority (lowest priority number) to lowest priority
 * (highest priority number).
 *
 * @param tasks  An array of task pointers containing the task set to schedule.
 * @param num_tasks  The number of tasks in the array.
 *
 * @return 0  The test failed.
 * @return 1  Test succeeded.  The tasks are now in order.
 */
int assign_schedule(task_t** tasks  __attribute__((unused)), size_t num_tasks  __attribute__((unused)))
{
    size_t i;
    size_t j;
    task_t tmp;
    double acc_start_time = 0.0;


    // Sort the tasks using bubble sort.
    for(i = 0; i < num_tasks; i++)
    {
        for(j = i+1; j < num_tasks; j++)
        {
            if((*tasks)[i].T > (*tasks)[j].T)
            {
                tmp = (*tasks)[i];
                (*tasks)[i] = (*tasks)[j];
                (*tasks)[j] = tmp;
            }
        }
    }

    // Use UB test to see if these tasks are schedulable.
    for(i = 0; i < num_tasks; i++)
    {
        if(acc_start_time + ((double)((*tasks)[i].C + (*tasks)[i].B) / (*tasks)[i].T) > bounds[i])
        {
            return 0;
        }
        acc_start_time += ((double)(*tasks)[i].C / (*tasks)[i].T);
    }

    return 1;
}



