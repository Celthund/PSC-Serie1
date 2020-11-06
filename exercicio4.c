#include <stdio.h>

#define MANTISSA_SIZE  23 // bits
#define EXPONENT_SIZE  8 // bits

#define MANTISSA_MASK 0x7FFFFF // bits
#define EXPONENT_MASK 0xFF // bits
#define SIGN_MASK  0x1 // bit

#define EXPONENT_SUB 127

int power(int base, int exponent){
    if (exponent == 0) return 1;
    if (base == 2) return base << exponent -1;
    int mul = base;
    for (int i = 1; i < exponent; i++){
        mul *= base;
    }
    return mul;
}

void printb(int n){
    int i = 1;
    while (n) {
        if (n & 1)
            printf("1");
        else
            printf("0");
        if ( i % 4 == 0 )
            printf("%c", ' ');
        i++;
        n >>= 1;
    }
    printf("\n");
}

int countDigits(int n){
    int numDigits = 0;
    while (n != 0){
        n = n / 10;
        numDigits++;
    }
    return numDigits;
}

size_t print_float(char *buffer, size_t buffer_size, float value){
    char *arrayBegin = buffer;
    // Save the float 32 bits in a unsigned int.
    unsigned int float_value = *(unsigned int *) &value;

    // Create one variable for sign, exponent and mantissa.
    unsigned int mantissa = float_value & MANTISSA_MASK;
    float_value = float_value >> MANTISSA_SIZE;
    unsigned int exponent = float_value & EXPONENT_MASK;
    float_value = float_value >> EXPONENT_SIZE;
    unsigned int sign = float_value & SIGN_MASK;

    // If the exponent is 255 and mantissa is zero return +/- infinite.
    if (exponent == 255 && mantissa == 0) {
        if (buffer_size > 4) {
            if (sign)
                *buffer++ = '-';
            else
                *buffer++ = '+';
            *buffer++ = 'i';
            *buffer++ = 'n';
            *buffer++ = 'f';
            *buffer = 0;
            return 4;
        }
        return 0;
    }

    // If the exponent is zero and mantissa is not zero return NaN (Not a number).
    if (exponent == 255 && mantissa != 0){
        if (buffer_size > 3) {
            *buffer++ = 'N';
            *buffer++ = 'a';
            *buffer++ = 'N';
            *buffer = 0;
            return 3;
        }
        return 0;
    }

    // If the exponent and mantissa is zero return +/- 0.
    if (!exponent && !mantissa){
        if (buffer_size > 2) {
            if (sign)
                *buffer++ = '-';
            else
                *buffer++ = '+';
            *buffer++ = '0';
            *buffer = 0;
            return 2;
        }
        return 0;
    }

    // Number of decimal places that should stay
    int dec_places = 6;
    // Number of shifts that can be made before losing the required decimal place.
    int shift = (MANTISSA_SIZE - dec_places);
    mantissa = ((1 << MANTISSA_SIZE) | mantissa) << exponent - EXPONENT_SUB;

    // After add 1 to the mantissa and multiplying by 2 ^ exponent - 127
    // the integer part of the number is all the bits after the mantissa.
    int integer = mantissa >> MANTISSA_SIZE;
    // The decimal part is the whole mantissa_mask
    // We shift here to the right for not overflowing the int size when multiplying by 10^(number of decimal places).
    int decimal = (((mantissa & MANTISSA_MASK) >> shift) * power(10, dec_places)) >> dec_places;

    // Get integer and decimal size so its easier to add the chars corresponding to the number to the buffer
    int i, integer_size = countDigits(integer), decimal_size = countDigits(decimal);
    char number;
    // Check if there is space in the array put the number with the following format
    // integer.decimal followed by end of string.
    if (integer_size + decimal_size + 3 > buffer_size) {
        *buffer = 0;
        return 0;
    }
    if (sign)
        *buffer++ = '-';
    else
        *buffer++ = '+';

    for (i = integer_size - 1; i >= 0; i--){
        number = integer / (power(10, i)) % 10;
        *buffer++ = (char)('0' + number);
    }
    // Add dot between in integer and decimal.
    *buffer++ = '.';

    for (i = decimal_size - 1; i >= 0; i--){
        number = decimal / (power(10, i)) % 10;
        *buffer++ = (char)('0' + number);
    }

    // Add end of string.
    *buffer++ = 0;

    return (buffer - arrayBegin) - 1;
}

void print(float foo){
    printf("%s %f \n", "Original value = ", foo);
    // Get bytes of float.
    unsigned int float_value = *(unsigned int *) &foo;

    // Set sign, exponent and sign in there corresponding variables.
    unsigned int mantissa = float_value & MANTISSA_MASK;

    float_value = float_value >> MANTISSA_SIZE;
    unsigned int exponent = float_value & EXPONENT_MASK;

    float_value = float_value >> EXPONENT_SIZE;
    unsigned int sign = float_value & SIGN_MASK;

    // dividir por 2 ^ 23 e multiplicar por 2 ^ 4

    int dec_places = 6;
    int shift = (MANTISSA_SIZE - dec_places);
//    printb(mantissa);
//    printb((1 << MANTISSA_SIZE) | mantissa);
    mantissa = ((1 << MANTISSA_SIZE) | mantissa) << exponent - EXPONENT_SUB;


    int integer = mantissa >> MANTISSA_SIZE;
    int decimal_places = mantissa & MANTISSA_MASK;
    int fraction10 = (decimal_places >> shift) * power(10, dec_places);
    int fraction =  fraction10 >> dec_places;
    //    printf("%s %d - ", "RESULT: ", mantissa);
    //    printb(mantissa);
    //    printf("%s %d - ", "DECIMAL:", decimal);
    //    printb(decimal);
    //    printf("%s %d - ", "FRACTION10: ", fraction10);
    //    printb(fraction);
    //    printf("%s %d - ", "FRACTION: ", fraction);
    //    printb(fraction);

    printf("Integer+/- = %c%d.%d\n",(sign) ? '-': ' ', integer, fraction);
}

int main (){
    float foo;
    char buffer[100] = "";
    foo = -118.625f;
    int size = print_float(&buffer, sizeof(buffer), foo);
    printf("Initial value = %f \nEnding value = %s\nSize of string = %d\n", foo, buffer, size);
//    foo = -118.625f;
//    print_float(foo, 6);
//    print_float(foo, 6);
//    foo =-15.9999980927f;
//    print(foo);
    foo = -118.625f;
    print(foo);
//
//    foo = -50.31231234f;
//    print(foo);
//    foo = 2.31231234f;
//    print(foo);
//    foo =1.2f;
//    print(foo);
//    print_float(foo, 6);
}

