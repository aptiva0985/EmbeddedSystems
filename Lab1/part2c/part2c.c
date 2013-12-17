/**
 * Group 32: part2c.c
 * Yuanshuo Cui (ycui)
 * Yujing Zhang (yujingz)
 * Liyu Ma (liyum)
 */

#include <stdlib.h>
#include <stdio.h>

/**
 * Function: oddball
 * Description: Baseline implementation for finding the number that
 *   occurs only once in the array.
 * Arguments:
 *   arr - pointer to start of integer array
 *   len - number of elements in array
 * Return:
 *   The number that occurs only once
 */
#ifdef DEFAULT
int oddball(int *arr, int len) {
    int i, j;
    int foundInner;
    int result = 0;

    for (i = 0; i < len; i++) {
        foundInner = 0;
        for (j = 0; j < len; j++) {
            if (i == j) {
                continue;
            }
            if (arr[i] == arr[j]) {
                foundInner = 1;
            }
        }
        if (foundInner != 1) {
            result = arr[i];
        }
    }

    return result;
}
#endif

/**
 * This method use property of XOR, which a number xor with itself is zero,
 * and we know only one number in the array appear once, also 0 xor any number
 * is the number itself. If we xor all numbers in the array, the final result
 * will be the number we want.
 *
 * Time complexity: O(n), space complexity: O(1).
 */
#ifdef OPTIMIZE1
int oddball(int *arr, int len) {
    // Final result.
    int result = 0;
    // Index variable.
    int i;

    // Loop through entire array, keep doing XOR.
    for (i = 0; i < len; i++) {
        result = result ^ arr[i];
    }

    // x ^ x = 0, so the number in result is the only number that appear once.
    return result;
}
#endif

/**
 * If every number appear twice, we can compute the sum of all numbers with
 * formula: 2*n*(n+1)/2 (n*(n+1)). So if we minus all numbers in the array from
 * the sum value, we can get the odd one.
 *
 * Time complexity: O(n), space complexity: O(1).
 */
#ifdef OPTIMIZE2
int oddball(int *arr, int len) {
    // Index variable.
    int i;

    // Get n.
    int n = (len + 1) / 2;

    // if we have each number in 1 to n for twice, the sum will be n*(n+1).
    int sum = n * (n + 1);

    // Traverse the entire array and minus each element from sum.
    for (i = 0; i < len; i++) {
        sum = sum - arr[i];
    }

    // The number in sum is the only number that appear once.
    return sum;
}
#endif

/**
 * Function: randGenerator
 * Description: Generate a random array that is in compliance with
 *   lab specification
 * Arguments:
 *   arr - pointer to start of integer array
 *   len - number of elements in array
 * Return:
 *   Unused, but feel free to utilize it.
 */

int randGenerator(int *arr, int len) {
    int i, j, r, rcount;
    for (i = 0; i < len; i++) {
        do {
            rcount = 0;
            r = rand()%(len/2 + 1) + 1;
            for (j = 0; j < i && rcount < 2; j++) {
                if (arr[j] == r) {
                    rcount++;
                }
            }
        } while(rcount >= 2);
        arr[i] = r;
        printf("%d ", r);
    }
    printf("\nDone generating\n");
    return 0;
}
