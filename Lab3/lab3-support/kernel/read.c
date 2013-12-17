#include <bits/errno.h>
#include <exports.h>

/**
 * Read characters into buffer from stdin
 *
 * Authors: Yuanshuo Cui <ycui@andrew.cmu.edu>
 *          Yujing Zhang <yujingz@andrew.cmu.edu>
 *          Liyu Ma      <liyum@andrew.cmu.edu>
 * Date:    12:18, 10/14/2013
 */

#define EOT 4
#define BS 127
#define STDIN_FILENO 0

ssize_t read(int fd, char *buf, size_t count) {
    unsigned byteCount = 0;
    char curChar;

    // File descriptor that doesn't match stdin
    if(fd != STDIN_FILENO) {
        // Return error
        return -EBADF;
    }

    // Check whether the buffer memory range is in SDRAM.
    if ((unsigned)&buf < (unsigned)0xA0000000 ||
        (unsigned)&buf + count > (unsigned)0xA3FFFFFF) {
        // Return error
        return -EFAULT;
    }

    // Keep reading until met count
    while(byteCount < count) {
        curChar = getc();

        // Return immediately when met EOT
        if (curChar == EOT)
            return byteCount;
        // Remove previous character and print "\b \b"
        // when met backspace or delete
        else if(curChar == BS) {
            byteCount--;
            puts("\b \b");
        }
        // Place a new line in buffer and return
        // when met newline or carriage return
        else if(curChar == '\n' || curChar == '\r') {
            buf[byteCount] = '\n';
            byteCount++;
            putc('\n');
            return byteCount;
        }
        else {
            buf[byteCount] = curChar;
            byteCount++;
            putc(curChar);
        }
    }
    return byteCount;
}
