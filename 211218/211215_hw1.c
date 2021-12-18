// main калькулятора
#include "calc_func.h"

int main()
{
    int op1 = 0, op2 = 0, result = 0, menu_num = 0;

    printMenu();
    while (menu_num != EXIT) {
        while (!selectOperation(MIN, MAX, &menu_num));

        if (menu_num <= PRINT - 2) { // PRINT-2 - временное простое решение. Последние 2 операции не нуждаются в операндах
            while (!enterValue(&op1, 1));
            while (!enterValue(&op2, 2));
        }

        performingOperation(&op1, &op2, &result, &menu_num);
    }

    return 0;
}
