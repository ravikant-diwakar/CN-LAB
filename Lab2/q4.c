#include <stdio.h>

int checkEndianness() {
    unsigned int num = 1;
    unsigned char *ptr = (unsigned char *)&num;
    return (*ptr == 1);
}

void printBits(unsigned int num) {
    int numBits = sizeof(num) * 8;
    for (int i = numBits - 1; i >= 0; i--) {
        printf("%d", (num >> i) & 1);
    }
    printf("\n");
}

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

    printf("Machine Endianness: ");
    if (checkEndianness()) {
        printf("Little-Endian\n");
    } else {
        printf("Big-Endian\n");
    }

    printf("Bits in original value: ");
    printBits(value);

    unsigned int swappedValue = swapEndianness(value);
    printf("Value after endianness conversion: %u\n", swappedValue);

    printf("Bits in converted value: ");
    printBits(swappedValue);

    return 0;
}

