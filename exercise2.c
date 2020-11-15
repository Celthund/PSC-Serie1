#include <stdio.h>
#include <limits.h>

/**
 * Returns bits in data array that are between idx and len - 1.
 * Note that the difference between idx and len should not be bigger than unsigned long bits.
 * @param data array to retrieve.
 * @param idx index for begin.
 * @param len length to the used.
 * @return
 */
unsigned long getbits(unsigned long data[], size_t idx, size_t len){
    unsigned long mask_idx = ~0 << idx;
    int unsigned_long_bits = sizeof(unsigned long) * CHAR_BIT;
    if ( idx + len < unsigned_long_bits){
        unsigned long mask_len = ~(mask_idx << (len - 1));
        unsigned long mask = mask_idx & mask_len;
        int result = (data[idx / unsigned_long_bits] & mask) >> idx;
        return result;
    }
    unsigned long mask_len = (1 << (len - (unsigned_long_bits - idx) )) - 1;
    unsigned long lower_part = (data[idx / unsigned_long_bits] & mask_idx) >> idx;
    unsigned long higher_part = (data[(idx + len) / unsigned_long_bits] & mask_len);
    int result = (higher_part << (unsigned_long_bits - idx))| lower_part ;
    return result;
}


/**
 * Sets bits in data array that are between idx and len - 1 as the value in those same bits.
 * Note that the difference between idx and len should not be bigger than unsigned long bits.
 * @param data array to retrieve.
 * @param idx index for begin.
 * @param len length to the used.
 * @param val value to set in the bits.
 */
void setbits(unsigned long data[], size_t idx, size_t len, unsigned long val){
    unsigned long mask_idx = ~0 << idx;
    int unsigned_long_bits = sizeof(unsigned long) * CHAR_BIT;
    if ( idx + len < unsigned_long_bits ){
        unsigned long mask_len = ~(mask_idx << (len - 1));
        unsigned long mask = mask_idx & mask_len;
        data[idx / unsigned_long_bits] = ( (data[idx / unsigned_long_bits] & ~mask) | (mask & val) );
    } else{
        unsigned long mask_len = (1 << (len - (unsigned_long_bits - idx) )) - 1;
        data[(idx) / unsigned_long_bits] = ((data[(idx) / unsigned_long_bits] & ~mask_idx) | (mask_idx & val));
        data[(idx + len) / unsigned_long_bits] = ((data[(idx + len) / unsigned_long_bits] & ~mask_len) | (mask_len & val));
    }
}

int main (){
    printf("%d\n", sizeof(unsigned long));
    unsigned long int data[] = {0xBFFFFFECABCD1234, 0xC, 2, 3};
    unsigned long number = getbits(data, 29, 8);
    printf("0x%lx\n", number);
    unsigned long int data1[] = {0xBFFFFFECABCD1234, 0x3, 2, 3};
    unsigned long number1 = getbits(data1, 60, 5);
    printf("0x%lx\n", number1);
    return 0;
}
