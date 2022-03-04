#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

#include "lifeGame.h"
#include "functions.h"

int main()
{
    int status;

    /* Инициализация массива магазинов данными */
    int i = 0;
    supply_t *shops = (supply_t *) malloc(NUM_SHOP * sizeof(supply_t));
    srand(time(NULL));
    for (; i < NUM_SHOP; i++) {
        shops[i].numShop = i+1;
        status = pthread_mutex_init(&shops[i].mtx, NULL);
        if (status != 0) {
            printf("ERROR: pthread_mutex_init error - status (main)\n");
            exit(EXIT_FAILURE);
        }
        /* Диапазон предложения от 900 до 1100 */
        shops[i].supply = MYRAND(MINSUPPLY, MAXSUPPLY);
    }

    /* Создание потоков */
    pthread_t threadsBuyer[NUM_BUYER];
    int t;
    for (t = 0; t < NUM_BUYER; t++) {
        status = pthread_create(&threadsBuyer[t], NULL, iamBuyer, &shops[0]);
        if (status != 0) {
            printf("ERROR: pthread_create - iamBuyer (%d) (main)\n", t);
            exit(EXIT_FAILURE);
        }
    }

    pthread_t t1;
    status = pthread_create(&t1, NULL, iamSupplier,  &shops[0]);
    if (status != 0) {
        printf("ERROR: pthread_create - iamSupplier (main)\n");
        exit(EXIT_FAILURE);
    }

    /* Присоединение потоков */
    for (t = 0; t < NUM_BUYER; t++) {
        status = pthread_join(threadsBuyer[t], NULL);
        if (status != 0) {
            printf("ERROR: pthread_create - iamBuyer (%d) (main)\n", t);
            exit(EXIT_FAILURE);
        }
    }

    status = pthread_cancel(t1);
    if (status != 0) {
        printf("ERROR: pthread_cancel - iamSupplier (main)\n");
        exit(EXIT_FAILURE);
    }

    printf("Game over!\n");
    exit(EXIT_SUCCESS);
}





