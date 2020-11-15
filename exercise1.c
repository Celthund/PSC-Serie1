#include <limits.h>
#include <stdio.h>

/**
 * Return the max value represent in complement of 2 for the given number of char * bits of char.
 * @param numberChars
 * @return
 */
unsigned long max_value(size_t numberChars){
    // If max value is not representable in unsigned long size return 0.
    if (sizeof(unsigned long) < numberChars) return 0;
    // Return Two's complement max positive value.
    return (1 << CHAR_BIT * numberChars) / 2 - 1;
}

int main (){
    printf("%lu\n", max_value(1));
    return 0;
}
