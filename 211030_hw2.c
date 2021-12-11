// Вывести в консоль CC. Не использовать при этом оператор присваивания
#include <stdio.h>

int main() {
        int a = 0x00DDAABB;

        printf("Initial value:\nIn hexadecimal notation: %X;\nIn decimal notation: %d\n", a, a);
        printf("Value: %X\n", 2 * (a & 0xFF) - (a >> 8) & 0xFF); // 2 * BB - AA => 2*187-170 => 204 => CC
}

