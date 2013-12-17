/** file typo.c
 *
 * Brief echo whatever user input, and print out the time
 * (in tenth of a second) of how long takes a user to print out a command.
 *
 * @ Authors: Yuanshuo Cui <ycui@andrew.cmu.edu>
 * @          Yujing Zhang <yujingz@andrew.cmu.edu>
 * @          Liyu Ma      <liyum@andrew.cmu.edu>
 * @ Date:    02:40, 11/03/2013
 *
 */

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

#define BUFSIZE 256

int main(int argc, char** argv) {
    // User input start and end time
    unsigned long startTime, endTime;
    // Buffer for input
    char input[BUFSIZE];
    // The duration of input
    unsigned long duration;
    int byteCount;

    while(1) {
        // Print prompt on screen
        write(STDOUT_FILENO, "$ ", 2);

        // Note start time
        startTime = time();

        // Read from STDIN
        byteCount = read(STDIN_FILENO, input, BUFSIZE);
        if(byteCount < 0) {
            exit(1);
        }

        endTime = time();

        // Write back to STDOUT
        if(write(STDOUT_FILENO, input, byteCount) < 0) {
            exit(1);
        }

        // Print out the time duration
        duration = (endTime - startTime) * 100 / 100;
        printf("Time for last input line: %lu.%lus.\n", duration / 1000, (duration % 1000) / 100);
    }

    return 0;
}
