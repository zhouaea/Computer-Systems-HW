/*
 * Author: Neo Zhou - zhouaea@bc.edu
 */

#include <stdio.h>

long multiply(long a, long b);

int main(void) {
    long a;
    long b;
    printf(">");
    while (scanf("%ld %ld", &a, &b) != 0) {
        printf("%ld * %ld = %ld\n", a, b, multiply(a, b));   
        printf(">");
    }
    
    return 0;
}
