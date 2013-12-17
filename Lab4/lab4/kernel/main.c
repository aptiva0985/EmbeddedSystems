#include <assert.h>
#include <syscall.h>
#include <arm/psr.h>
#include <arm/interrupt.h>
#include <arm/reg.h>
#include <arm/timer.h>
#include <lock.h>
/**
 * Main routine of kernel
 *
 * Authors: Yuanshuo Cui <ycui@andrew.cmu.edu>
 *          Yujing Zhang <yujingz@andrew.cmu.edu>
 *          Liyu Ma      <liyum@andrew.cmu.edu>
 * Date:    18:46, 11/25/2013
 */

uint32_t global_data;
volatile unsigned long up_time;

extern unsigned s_handler();
extern void irq_wrapper();
extern void C_SWI_handler(unsigned swi_num, unsigned *regs);
extern void C_IRQ_handler();
extern int setup();
extern ssize_t read(int fd, void *buf, size_t count);
extern ssize_t write(int fd, const void *buf, size_t count);
extern unsigned long time(void);
extern void sleep(unsigned long millis);
extern int install_handler(unsigned new_addr, unsigned *vec_addr, unsigned *saved_instr1, unsigned *saved_instr2);
extern void interrupt_setup();
extern void timer_setup();

int kmain(int argc, char** argv, uint32_t table)
{
    app_startup();
    global_data = table;

    // Original SWI Handler's address in the vector table
    unsigned *vec_addr1 = (unsigned *)0x08;
    // Address of new SWI handler
    unsigned new_addr1 = (unsigned)&s_handler;

    // Original IRQ Handler's address in the vector table
    unsigned *vec_addr2 = (unsigned *)0x18;
    // Address of new IRQ handler
    unsigned new_addr2 = (unsigned)&irq_wrapper;

    // Save first two instructions in original handler for restore
    unsigned saved_instr1, saved_instr2, saved_instr3, saved_instr4;

    // Install our SWI Handler
    if(install_handler(new_addr1, vec_addr1, &saved_instr1, &saved_instr2) < 0)
    {
        printf("SWI handler install fail.\n");
        return 0;
    }


    // Install our IRQ Handler
    if(install_handler(new_addr2, vec_addr2, &saved_instr3, &saved_instr4))
    {
        printf("IRQ handler install fail.\n");
        return 0;
    }

    // Initialize up time
    up_time = 0;

    // Set up interrupt controller registers
    interrupt_setup();

    // Set up OS timer registers
    timer_setup();

    // Initialize mutex
    mutex_init();

    // Execute some assembly code: user mode, jump to user program
    setup(argc, argv);

    // Should never get here
    assert(0);
}
