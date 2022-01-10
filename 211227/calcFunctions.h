#include "initComponents.h"

#ifndef COURSE_CALCFUNCTIONS_H
#define COURSE_CALCFUNCTIONS_H

void printMenu(menu_t *head);
int selectOperation(unsigned int min, unsigned int max, unsigned int *menu_num);
int enterValue(int *operand, unsigned char num);
void performingOperation(menu_t *head, const int *op1, const int *op2, int *result, unsigned int *operation_num);

#endif //COURSE_CALCFUNCTIONS_H
