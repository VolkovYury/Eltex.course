// Заполнить матрицу размера NxN числами от 1 до N^2 улиткой по часовой стрелке
#include <stdio.h>

#define MATRIX_SIZE 7

void one_cycle(int matrix[MATRIX_SIZE][MATRIX_SIZE], int cycle, int value, int currentIter);

int main()
{
        int matrix[MATRIX_SIZE][MATRIX_SIZE];
        int currentValue = 1;   // Начальное значение "улитки" в (0;0)

        // Подсчёт количества циклов. Под циклом подразумевается количество "полных окружностей", содержащихся в матрице
        // Так матрица 8*8 содержит 3 окружности (пояса, слоя, как у луковицы) и квадрат 2х2 в центре
        int cycle = (MATRIX_SIZE %2 == 0)? (MATRIX_SIZE-2)/2 : (MATRIX_SIZE-1)/2;

        // Функция заполнения матрицы улиткой. Рекурсивная.
        // Также заполняет центры (либо квадрат 2х2, либо одну клетку), хотя логичнее было бы вынести в отдельную функцию
        one_cycle(matrix, cycle, currentValue, 0);

        printf("Matrix:\n");
        for (int i = 0; i < MATRIX_SIZE; i++) {
                for (int k = 0; k < MATRIX_SIZE; k++) {
                        printf("%d\t", matrix[i][k]);
                }
                printf("\n");
        }

        return 0;
}

// matrix - массив, cycle - количество слоёв-окружностей, value - текущее значение для клетки,
// currentIter - количество успешно заполненных слоёв-окружностей
void one_cycle(int matrix[MATRIX_SIZE][MATRIX_SIZE], int cycle, int value, int currentIter)
{
        // Заполнение по часовой стрелке
        if (cycle > 0) {
                int x = currentIter;
                int y = currentIter;
                // Заполнение верхней строки "окружности"
                for (int i = 0; i < MATRIX_SIZE-2*currentIter; i++) {
                        matrix[y][x++] = value++;
                }
                --x; // Компенсирование лишнего перемещения

                // Заполнение правого столбца "окружности". i = 1, т.к. один элемент столбца уже был заполнен
                for (int i = 1; i < MATRIX_SIZE-2*currentIter; i++) {
                        matrix[++y][x] = value++;
                }

                // Заполнение нижней строки "окружности"
                for (int i = 1; i < MATRIX_SIZE-2*currentIter; i++) {
                        matrix[y][--x] = value++;
                }

                // Заполнение левого столбца "окружности". i= 2, т.к. 2 элемента данного столбца уже были заполнены
                for (int i = 2; i < MATRIX_SIZE-2*currentIter; i++) {
                        matrix[--y][x] = value++;
                }

                // Если количество циклов ненулевое, то рекурсивный вызов функции
                one_cycle(matrix, --cycle, value, ++currentIter);
        }
        /* Для заполнения против часовой стрелки:
        if (cycle > 0) {
                int x = currentIter;
                int y = currentIter;
                // Заполнение верхней строки "окружности"
                for (int i = 0; i < MATRIX_SIZE-2*currentIter; i++) {
                        matrix[x++][y] = value++;
                }
                --x; // Компенсирование лишнего перемещения

                // Заполнение правого столбца "окружности". i = 1, т.к. первый элемент столбца заполнен
                for (int i = 1; i < MATRIX_SIZE-2*currentIter; i++) {
                        matrix[x][++y] = value++;
                }

                // Заполнение нижней строки "окружности"
                for (int i = 1; i < MATRIX_SIZE-2*currentIter; i++) {
                        matrix[--x][y] = value++;
                }

                // Заполнение левого столбца "окружности". i= 2, т.к. 2 элемента данного столбца уже на своих местах
                for (int i = 2; i < MATRIX_SIZE-2*currentIter; i++) {
                        matrix[x][--y] = value++;
                }
        }
         */

        // Если циклов больше нет, то заполняется центр. По хорошему лучше вынести в отдельную функцию
        if (cycle == 0 && MATRIX_SIZE %2 == 0) {
                int startCoordinate = (MATRIX_SIZE-2)/2;
                matrix[startCoordinate][startCoordinate] = MATRIX_SIZE*MATRIX_SIZE - 3;
                matrix[startCoordinate][startCoordinate+1] = MATRIX_SIZE*MATRIX_SIZE - 2;
                matrix[startCoordinate+1][startCoordinate] = MATRIX_SIZE*MATRIX_SIZE;
                matrix[startCoordinate+1][startCoordinate+1] = MATRIX_SIZE*MATRIX_SIZE - 1;

                /*Для заполнения против часовой стрелки:
                matrix[startCoordinate][startCoordinate] = MATRIX_SIZE*MATRIX_SIZE - 3;
                matrix[startCoordinate][startCoordinate+1] = MATRIX_SIZE*MATRIX_SIZE;
                matrix[startCoordinate+1][startCoordinate] = MATRIX_SIZE*MATRIX_SIZE-2;
                matrix[startCoordinate+1][startCoordinate+1] = MATRIX_SIZE*MATRIX_SIZE - 1;
                 */
        } else if (cycle == 0 && MATRIX_SIZE %2 == 1){
                int centerCoordinate = (MATRIX_SIZE-1)/2;
                matrix[centerCoordinate][centerCoordinate] = MATRIX_SIZE*MATRIX_SIZE;
        }
}