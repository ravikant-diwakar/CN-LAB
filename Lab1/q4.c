#include <stdio.h>

struct pkt {
    char ch1;
    char ch2[2];
    char ch3;
};

int main() {
    struct pkt packet;
    int number;

    printf("Enter a number: ");
    scanf("%d", &number);
    packet.ch1 = number & 0xFF;
    packet.ch2[0] = (number >> 8) & 0xFF;
    packet.ch2[1] = (number >> 16) & 0xFF;
    packet.ch3 = (number >> 24) & 0xFF;

    printf("Content of the structure:\n");
    printf("ch1: %d\n", packet.ch1);
    printf("ch2[0]: %d\n", packet.ch2[0]);
    printf("ch2[1]: %d\n", packet.ch2[1]);
    printf("ch3: %d\n", packet.ch3);

    int originalNumber = 0;
    originalNumber |= packet.ch1;
    originalNumber |= (packet.ch2[0] << 8);
    originalNumber |= (packet.ch2[1] << 16);
    originalNumber |= (packet.ch3 << 24);

    printf("\nOriginal Number: %d\n", originalNumber);

    return 0;
}

