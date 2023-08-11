#include <stdio.h>

int main() {
    int num = 42;
    int *ptr = &num;

    printf("Value of the variable: %d\n", *ptr);
    printf("Memory address of the variable: %p\n", (void *)ptr);

    return 0;
}

