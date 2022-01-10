#include <stdio.h>
#include <stdlib.h>
#include <dlfcn.h>

#include "calcFunctions.h"
#include "initComponents.h"

/**
 * Функция выводит в консоль текстовое меню калькулятора
 * @param head Указатель на первый элемент односвязного списка
 */
void printMenu(menu_t *head)
{
    printf("===================================[  MENU  ]===================================\n");
    int countPlugins = getSizeStruct(head);
    while (head != NULL) {
        printf("%d. %s\n", head->numMenu, head->operation);
        head = head->ptrNextOp;
    }

    printf("%d. Print menu\n", countPlugins + 1);
    printf("%d. Exit\n", countPlugins + 2);
    printf("================================================================================\n");
}

/**
 * Функция обрабатывает пользовательский ввод выбора операции
 * @param min минимальное допустимое значение пункта меню
 * @param max максимальное допустимое значение пункта меню
 * @param menu_num указатель на номер выбранной операции
 * @return В случае успешного завершения возвращает 1, если пользователь ввёл недопустимое значение - 0
 */
int selectOperation(const unsigned int min, const unsigned int max, unsigned int *menu_num)
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

/**
 * Функция обрабатывает пользовательский ввод значения операнда (ТОЛЬКО int)
 * @param operand Указатель на значение операнда
 * @param num Порядковый номер операнда, по сути нужен только для printf
 * @return В случае успешного завершения возвращает 1, если пользователь ввёл недопустимое значение - 0
 */
int enterValue(int *operand, const unsigned char num)
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

/**
 * Функция выполняет выбранную операцию
 * @param op1 Указатель на операнд 1
 * @param op2 Указатель на операнд 2
 * @param result Указатель на результат операции
 * @param operation_num Указатель на номер выбранной операции
 */
void performingOperation(menu_t *head, const int *op1, const int *op2, int *result, unsigned int *operation_num)
{
    unsigned int countPlugins = getSizeStruct(head);
    if (*operation_num <= countPlugins) {

        head = getOperation(head, operation_num);

        // Подключение динамической библиотеки
        const char libPath[11] = "./plugins/\0";
        char path[80];
        sprintf(path, "%s%s.so", libPath, head->operation);
        void *lib;
        lib = dlopen(path, RTLD_LAZY);
        if (!lib)
            printf("Something wrong (dlopen)\n");

        // Операция
        int (*libFunc) (int const *, int const *, int *);
        libFunc = dlsym (lib, head->operation);
        if (!libFunc)
            printf("Something wrong (dlsym)\n");

        (libFunc) (op1, op2, result);
        printf("The result of the operation: %d\n\n", *result);

    } else if (*operation_num == countPlugins + 1) {
        printMenu(head);
    } else if (*operation_num == countPlugins + 2) {
        printf("Closing the application...\n");
        exit(0);
    } else {
        printf("Something wrong (menu)!\n");
    }
}
