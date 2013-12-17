/** file CountDownTimer.c
 *
 * Allow user to set a time duration, and starts to count down and alarm.
 * 
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
    // count-down time duration
    unsigned long time_duration = 0;
    // Buffer for input
    char input[BUFSIZE];
    int byte_count;
    int i = 0;
    unsigned long j = 0;
    int valid = 0;
    
    while (!valid) {
        valid = 1;
        time_duration = 0;
        printf("Please set a time duration to count down (in seconds):");
    
        // Read from STDIN
        byte_count = read(STDIN_FILENO, input, BUFSIZE);
        if(byte_count < 0) {
            exit(1);
        }
    
        // Calculate time duration from user input
        for (i = 0; i < byte_count - 1; i++) {
            if (input[i] < '0' || input[i] > '9') {
                valid = 0;
                printf("Please only input numbers.\n");
                break;
            }
                
            time_duration = 10 * time_duration + input[i] - '0';
        } 
    }
    // Count down
    for (j = time_duration; j > 0; j--) {
        printf("%lu                                     \r", j);
        sleep(1000);
    }
    
    // Alert
    printf("Times up!\n");

    return 0;
}
