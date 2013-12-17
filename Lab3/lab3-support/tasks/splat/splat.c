/** file splat.c
 *
 * Brief Print out a spinning cursor, transition between '|/-\'.
 * Each transition takes 200 ms, total transition takes 1.6s.
 *
 * @ Authors: Yuanshuo Cui <ycui@andrew.cmu.edu>
 * @          Yujing Zhang <yujingz@andrew.cmu.edu>
 * @          Liyu Ma      <liyum@andrew.cmu.edu>
 * @ Date:    01:05, 11/03/2013
 *
 */

#include <unistd.h>

int main(int argc, char** argv) {
    const char* displays[5] = {"|", "/", "-", "\\"};
    int i;


    while(1) {
        for(i = 0; i < 4; i++) {
            write(STDOUT_FILENO, displays[i], 1);

            sleep(200);

            write(STDOUT_FILENO, "\b \b", 3);
        }
    }
    return 0;
}
