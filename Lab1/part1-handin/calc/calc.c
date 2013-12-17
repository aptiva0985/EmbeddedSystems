/**
 * Group 32: calc.c
 * Yuanshuo Cui (ycui)
 * Yujing Zhang (yujingz)
 * Liyu Ma (liyum)
 */

#include <stdio.h>
#include "math.h"

int main(int argc, char *argv[]) {
    int arg1 = 0;
    int arg2 = 0;
    int result = 0;
    char op;
    int overflow = 0;
    int divide_zero = 0;

    /* Read user input */
    while(scanf("%d %c %d", &arg1, &op, &arg2) == 3) {
        if(op == '+') {
            result = add(arg1, arg2);
            /* Overflow */
            if((arg1 > 0 && arg2 > 0 && result < 0) ||
               (arg1 < 0 && arg2 < 0 && result > 0))
                overflow = 1;
        }
        else if(op == '-') {
            result = sub(arg1, arg2);
            /* Overflow */
            if((arg1 > 0 && arg2 < 0 && result < 0) ||
               (arg1 < 0 && arg2 < 0 && result > 0))
                overflow = 1;
        }
        else if(op == '*') {
            result = mul(arg1, arg2);
            /* Overflow */
            if((arg1 > 0 && arg2 > 0 && result < 0) ||
               (arg1 < 0 && arg2 < 0 && result < 0))
                overflow = 1;
        }
        else if (op == '/') {
            /* Divide zero */
            if (arg2 == 0)
                divide_zero = 1;
            else
                result = cdiv(arg1, arg2);
        }
        else if (op == '%') {
            /* Divide zero */
            if (arg2 == 0)
                divide_zero = 1;
            else
                result = mod(arg1, arg2);
        }
        else {
            printf("Invalid operator\n");
        }

        if(overflow)
            printf("Overflow\n");
        else if(divide_zero)
            printf("Cannot divide zero\n");
        else
            printf("%d\n", result);

        divide_zero = 0;
        overflow = 0;
    }
    return 0;
}
