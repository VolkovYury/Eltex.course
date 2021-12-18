// Вспомогательные функции
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "calc_func.h"
#include "libmycalc/libmycalc.h"

void printMenu()
{
    printf("===============================\n"
            "Select an operation:\n"
           "\t1. Addition\n"
           "\t2. Subtraction\n"
           "\t3. Multiplication\n"
           "\t4. Division\n"
           "\t5. Exit\n"
           "-------------------------------\n"
           "\t6. Print menu\n"
           "===============================\n");
}

// Описание: функция отвечает за выбор пользователем арифметической операции
// Параметры: min и max - минимальное и максимальное допустимые значения пунктов меню. *menu_num - указатель на
// введённый пользователь номер меню
// Return: В случае успешного завершения возвращает 1, если пользователь ввёл недопустимое значение - 0
int selectOperation(int min, int max, int *menu_num)
{
    char operation_num[3], tmp;

    printf("Enter the operation number: ");

    scanf("%3s", operation_num);
    while((tmp = (char) getchar()) != '\n' && tmp != EOF );                 // "Очистка" stdin
    int menu = (int) strtol(operation_num, NULL, 10);

    if (menu >= min && menu <= max) {
        *menu_num = menu;
        return 1;
    } else {
        printf("WARNING: Invalid value. Try again:\n");
        return 0;
    }
}

// Описание: функция запрашивает у пользователя значение операнда. Только int
// Параметры: *operand - указатель на значение операнда, num - порядковый номер операнда, по сути нужен только для printf
// Return: В случае успешного завершения возвращает 1, если пользователь ввёл недопустимое значение - 0
int enterValue(int *operand, int num)
{
    char enteredString[10], tmp;

    printf("Enter the operand %d: ", num);

    scanf("%10s", enteredString);
    while((tmp = (char) getchar()) != '\n' && tmp != EOF );                 // "Очистка" stdin
    int value = (int) strtol(enteredString, NULL, 10);

    if (value >= -2147483648 && value <= 2147483647) {
        *operand = value;
        return 1;
    } else {
        printf("WARNING: Invalid value. Try again:\n");
        return 0;
    }
}

// Описание: функция выполняет выбранную операцию
// Параметры: *op1, *op2 - указатели на операнды, *result - указатель на результат операции, *operation_num - выбранная операция
void performingOperation(int const *op1, int const *op2, int *result, int const *operation_num)
{
    switch (*operation_num) {
    case ADDITION:
        addition(op1, op2, result);
        printf("The result of the operation: %d\n\n", *result);
        break;
    case SUBTRACTION:
        subtraction(op1, op2, result);
        printf("The result of the operation: %d\n\n", *result);
        break;
    case MULTIPLICATION:
        multiplication(op1, op2, result);
        printf("The result of the operation: %d\n\n", *result);
        break;
    case DIVISION:
        division(op1, op2, result);
        printf("The result of the operation: %d\n\n", *result);
        break;
    case EXIT:
        printf("Closing the application...\n");
        break;
    case PRINT:
        printMenu();
        break;
    default:
        printf("Something wrong (menu-switch)!\n");
    }
}







