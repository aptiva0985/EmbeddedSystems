/** @file io.c
 *
 * @brief Kernel I/O syscall implementations
 *
 *
 * Authors: Yuanshuo Cui <ycui@andrew.cmu.edu>
 *          Yujing Zhang <yujingz@andrew.cmu.edu>
 *          Liyu Ma      <liyum@andrew.cmu.edu>
 * Date:    13:01, 11/20/2013
 */

#include <types.h>
#include <bits/errno.h>
#include <bits/fileno.h>
#include <arm/physmem.h>
#include <syscall.h>
#include <exports.h>
#include <kernel.h>

#define EOT_CHAR 0x04
#define DEL_CHAR 0x7f

/* Read count bytes (or less) from fd into the buffer buf. */
ssize_t read_syscall(int fd __attribute__((unused)), void *buf __attribute__((unused)), size_t count __attribute__((unused)))
{
    unsigned byteCount = 0;
    char curChar;

    // File descriptor that doesn't match stdin
    if(fd != STDIN_FILENO)
    {
        // Return error
        return -EBADF;
    }

    // Check whether the buffer memory range is in ROM/SDRAM.
    if((unsigned)&buf < (unsigned)RAM_START_ADDR ||
       (unsigned)&buf + count > (unsigned)RAM_END_ADDR - 1 ||
       (unsigned)&buf + count < (unsigned)FLASH_END_ADDR - 1)
    {
        // Return error
        return -EFAULT;
    }

    // Keep reading until met count
    while(byteCount < count)
    {
        curChar = getc();

        // Return immediately when met EOT
        if (curChar == EOT_CHAR)
            return byteCount;
        // Remove previous character and print "\b \b"
        // when met backspace or delete
        else if(curChar == DEL_CHAR)
        {
            byteCount--;
            puts("\b \b");
        }
        // Place a new line in buffer and return
        // when met newline or carriage return
        else if(curChar == '\n' || curChar == '\r')
        {
            ((char*)buf)[byteCount] = '\n';
            byteCount++;
            putc('\n');
            return byteCount;
        }
        else {
            ((char*)buf)[byteCount] = curChar;
            byteCount++;
            putc(curChar);
        }
    }
    return byteCount;
}

/* Write count bytes to fd from the buffer buf. */
ssize_t write_syscall(int fd  __attribute__((unused)), const void *buf  __attribute__((unused)), size_t count  __attribute__((unused)))
{
    unsigned byteCount = 0;

    // File descriptor that doesn't match stdout
    if(fd != STDOUT_FILENO)
    {
        // Return error
        return -EBADF;
    }

    // Check whether the buffer memory range is in ROM/SDRAM.
    if((unsigned)&buf < (unsigned)RAM_START_ADDR ||
       (unsigned)&buf + count > (unsigned)RAM_END_ADDR - 1 ||
       (unsigned)&buf + count < (unsigned)FLASH_END_ADDR - 1)
    {
        // Return error
        return -EFAULT;
    }

    // Keep writing until met count
    while(byteCount < count)
    {
        if(((char*)buf)[byteCount] == '\0')
        {
            return byteCount;
        }
        else
        {
          putc(((char*)buf)[byteCount]);
          byteCount++;
        }
    }

    return byteCount;
}
