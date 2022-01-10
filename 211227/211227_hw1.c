#include <stddef.h>
#include <stdio.h>

#include "initComponents.h"
#include "calcFunctions.h"

int main()
{
    int op1 = 0, op2 = 0, result = 0;
    unsigned int menu_num = 0;

    menu_t *plugins = NULL;
    plugins = componentsInit();
    int countPlugins = getSizeStruct(plugins);

    printMenu(plugins);
    while (menu_num != countPlugins + 2) {    // [плагины + печать + выход]. Для выхода (+2)
        while (!selectOperation(1, countPlugins + 2, &menu_num));

        if (menu_num <= countPlugins) {
            while (!enterValue(&op1, 1));
            while (!enterValue(&op2, 2));
        }

        performingOperation(plugins, &op1, &op2, &result, &menu_num);
    }

    return 0;
}