#ifndef _CALC_FUNC_H_
#define _CALC_FUNC_H_

#define MIN 1
#define MAX 6

enum operations {
    ADDITION = 1,
    SUBTRACTION,
    MULTIPLICATION,
    DIVISION,
    EXIT,
    PRINT,
};

void printMenu();

int selectOperation(int min, int max, int *menu_num);
int enterValue(int *operand, int num);
void performingOperation(int const *op1, int const *op2, int *result, int const *operation_num);

#endif