#include <stdio.h>

int checkEndianness() {
    unsigned int num = 1;
    unsigned char *ptr = (unsigned char *)&num;

    return (*ptr == 1);
}

int main() {
    int userValue;
    printf("Enter an integer value: ");
    scanf("%d", &userValue);

    if (checkEndianness()) {
        printf("You are using a little-endian system.\n");
    } else {
        printf("You are using a big-endian system.\n");
    }

    return 0;
}

