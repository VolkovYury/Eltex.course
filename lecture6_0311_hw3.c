// Заполнить верхний треугольник матрицы нулями, а нижний - единицами
#include <stdio.h>

#define MATRIX_SIZE 6

int main()
{
        int matrix[MATRIX_SIZE][MATRIX_SIZE];

        // Заполнение матрицы
        for (int i = 0; i < MATRIX_SIZE; i++) {
                for (int k = 0; k < MATRIX_SIZE; k++) {
                        matrix[i][k] = ((k < MATRIX_SIZE-1-i)? 0 : 1);
                }
        }

        printf("Matrix:\n");
        for (int i = 0; i < MATRIX_SIZE; i++) {
                for (int k = 0; k < MATRIX_SIZE; k++) {
                        printf("%d\t", matrix[i][k]);
                }
                printf("\n");
        }

        return 0;
}