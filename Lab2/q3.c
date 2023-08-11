#include <stdio.h>

unsigned int swapEndianness(unsigned int num) {
    return ((num >> 24) & 0xFF) |
           ((num >> 8) & 0xFF00) |
           ((num << 8) & 0xFF0000) |
           ((num << 24) & 0xFF000000);
}

int main() {
    unsigned int value;
    
    printf("Enter an integer value: ");
    scanf("%u", &value);

    printf("Original value: %u\n", value);
    
    unsigned int swappedValue = swapEndianness(value);
    
    printf("Value after endianness conversion: %u\n", swappedValue);

    return 0;
}

