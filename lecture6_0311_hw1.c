// Вывести квадратную матрицу по заданному значению N. Матрица заполняется числами по возрастанию слева направо, сверху вниз
#include <stdio.h>

#define MATRIX_SIZE 30

int main()
{
        int matrix[MATRIX_SIZE][MATRIX_SIZE];

        // Заполнение матрицы
        for (int i = 0, val = 1; i < MATRIX_SIZE; i++) {
                for (int k = 0; k < MATRIX_SIZE; k++, val++) {
                        matrix[i][k] = val;
                }
        }

        printf("Matrix:\n");
        for (int i = 0, val = 1; i < MATRIX_SIZE; i++) {
                for (int k = 0; k < MATRIX_SIZE; k++, val++) {
                        printf("%d\t", matrix[i][k]);
                }
                printf("\n");
        }

        return 0;
}
