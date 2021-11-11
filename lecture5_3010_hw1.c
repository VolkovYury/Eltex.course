// Вывести в консоль BB, AA, DD, 00. Для этого использовать операции сдвигов и логические операции
#include <stdio.h>

int main() {
        int a = 0x00DDAABB;
        unsigned char c;

        printf("Initial value:\nIn hexadecimal notation: %X;\nIn decimal notation: %d\n", a, a);

        c = a & 0xFF;
        printf("First byte: %X\n", c);

        c = (a >> 8) & 0xFF;
        printf("Second byte: %X\n", c);

        c = (a >> 16) & 0xFF;
        printf("Third byte: %X\n", c);

        c = (a >> 24) & 0xFF;
        printf("Fourth byte: %X\n", c);
}

