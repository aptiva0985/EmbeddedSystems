/*
 * rot13.c: ROT13 cipher cur application
 *
 * Authors: Yuanshuo Cui <ycui@andrew.cmu.edu>
 *          Yujing Zhang <yujingz@andrew.cmu.edu>
 *          Liyu Ma      <liyum@andrew.cmu.edu>
 * Date:    18:26, 10/20/2013
 */

#include <errno.h>
#include <stdlib.h>
#include <unistd.h>

char *encrypt(char *input, int len) {
    int i;
    int cur;

    for(i = 0; i < len; i++) {
        // Get ASCII value of current character.
        cur = (int)input[i];

        // If the string ends before reach length, break.
        if (input[i] == '\n') {
            break;
        }

        // Encryption A-M and a-m.
        if ((cur >= 'A' && cur <= 'M') || (cur >= 'a' && cur <= 'm')) {
            input[i] += 13;
        }
        // Encryption N-Z and n-z.
        else if ((cur >= 'N' && cur <= 'Z') || (cur >= 'a' && cur <= 'z')) {
            input[i] -= 13;
        }
        // Ignore all other characters.
        else {
            continue;
        }
    }
    // Return the string after encryption.
    return input;
}

int strlen(const char *str) {
    int i;
    for(i = 0; str[i] != '\0'; i++);
    return i;
}


int main(int argc, char *argv[]) {
    // print parameters out to the screen
    int i;

    for(i = 1; i < argc; i++) {
        write(STDOUT_FILENO, argv[i], strlen(argv[i]));
        write(STDOUT_FILENO, "\n", 1);
    }

    // Buffer to save current input.
    char buf[256];
    int readCount;
    char* output;

    // Indefinitely loops to read another block of input.
    while(1) {
        // Reads a block of input from stdin.
        // Save return value of system call.
        readCount = read(STDIN_FILENO, buf, 256);

        // If zero bytes are read, terminates with exit status 0.
        if(!readCount) {
            exit(0);
        }
        // If any error encountered, immediately terminate with exit status 1.
        else if(readCount < 0) {
            exit(1);
        }

        // Performs ROT13 on the input data.
        output = encrypt(buf, readCount);
        // Writes the entire ciphertext to stdout.
        // If any error encountered, terminate with exit status 1.
        if(write(STDOUT_FILENO, output, readCount) < 0) {
            exit(1);
        }
    }

  exit(0);
}
