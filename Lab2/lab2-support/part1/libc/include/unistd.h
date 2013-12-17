/*
 * unistd.h: Declares C standard library functions
 *
 * Author: Mike Kasick <mkasick@andrew.cmu.edu>
 * Date:   Sun, 07 Oct 2007 01:38:30 -0400
 */

#ifndef UNISTD_H
#define UNISTD_H

#include <bits/fileno.h>
#include <sys/types.h>

ssize_t read(int fd, void *buf, size_t count);
ssize_t write(int fd, const void *buf, size_t count);

#endif /* UNISTD_H */
