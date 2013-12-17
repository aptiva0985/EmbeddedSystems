/** @file unistd.h
 *
 * @brief Declares C standard library functions
 *
 * @author Mike Kasick <mkasick@andrew.cmu.edu>
 * @date   Sun, 07 Oct 2007 01:38:30 -0400
 */

#ifndef UNISTD_H
#define UNISTD_H

#include <bits/fileno.h>
#include <sys/types.h>

ssize_t read(int fd, void *buf, size_t count);
ssize_t write(int fd, const void *buf, size_t count);
unsigned long time(void);
void sleep(unsigned long millis);

#endif /* UNISTD_H */
