#include <math.h>
#include <limits.h>
#include <stdio.h>

unsigned long max_value(size_t numberChars){
    int charBit = CHAR_BIT;
    int longBit = sizeof(unsigned long) * charBit;

    // If max value is not representable in unsigned long size return 0.
    if (longBit < charBit * numberChars) return 0;
    // Return Two's complement max positive value.
    return (pow(2, charBit * numberChars)) / 2 - 1;
}

int main (){
    printf("%lu\n", max_value(1));
    return 0;
}
