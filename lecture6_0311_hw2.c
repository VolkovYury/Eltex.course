// Вывести заданный массив размером N в обратном порядке
#include <stdio.h>

#define ARRAY_SIZE 10

void print_array(int array[], int size);

int main()
{
        int arr[ARRAY_SIZE] = {1, 2, 3, 4, 5, 6, 8, 50, 54, 128};
        print_array(arr, ARRAY_SIZE);

        return 0;
}

void print_array(int array[], int size)
{
        if (size > 1) {
                int index = size-1;
                print_array(array, index);
        }

        printf("%d\t", array[ARRAY_SIZE-size]);
}
