#include <bits/errno.h>
#include <exports.h>

#define STDOUT_FILENO 1

/**
 * Write characters from buffer to stdout
 *
 * Authors: Yuanshuo Cui <ycui@andrew.cmu.edu>
 *          Yujing Zhang <yujingz@andrew.cmu.edu>
 *          Liyu Ma      <liyum@andrew.cmu.edu>
 * Date:    12:18, 10/14/2013
 */
ssize_t write(int fd, const char *buf, size_t count) {
    int byteCount = 0;

    // File descriptor that doesn't match stdout
    if(fd != STDOUT_FILENO) {
        // Return error
        return -EBADF;
    }

    // Check whether the buffer memory range is in ROM/SDRAM.
    if((unsigned)&buf < (unsigned)0xA0000000 ||
       (unsigned)&buf + count > (unsigned)0xA3FFFFFF ||
       (unsigned)&buf < (unsigned)0x00000000 ||
       (unsigned)&buf + count < (unsigned)0x00FFFFFF) {
        // Return error
        return -EFAULT;
    }

    // Keep writing until met count
    while(byteCount < count) {
        if(buf[byteCount] == '\0') {
            return byteCount;
        }
        else {
          putc(buf[byteCount]);
          byteCount++;
        }
    }

    return byteCount;
}
