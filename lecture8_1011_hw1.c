// Заменить 3-й байт числа, используя указатели
#include <stdio.h>

int main()
{
        int num = 123456789;
        unsigned char *ptr;
        ptr = (unsigned char*) &num;

        printf("Number (decimal): %d\n", num);
        printf("Number (hexadecimal): %X\n", num);
        printf("First byte - %X\n", *ptr);
        ptr++;
        printf("Second byte - %X\n", *ptr);
        ptr++;
        printf("Third byte - %X\n", *ptr);
        ptr++;
        printf("Fourth byte - %X\n", *ptr);

        --ptr;
        *ptr = 0xAA;
        printf("New third byte - %X\n", *ptr);
        printf("New number (decimal): %d\n", num);
        printf("New number (hexadecimal): %X\n", num);

        return 0;
}

