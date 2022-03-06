#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "lifeGame.h"
#include "functions.h"

/**
 * Функция "покупатель". Одна из поточных функций. Описание в ../README.md
 * @param arg Принимает структуру, в которой отражена информация о имеющихся магазинах
 */
void *iamBuyer(void *arg)
{
    supply_t *shops = (supply_t *) arg;
    int status;

    srand(pthread_self());
    /* Диапазон спроса от 9000 до 11000 */
    int demand = MYRAND(MINDEMAND, MAXDEMAND);

    for (;demand != 0;) {

        /* Диапазон магазинов от 0 до 4 */
        int currentShop = MYRAND(0, NUM_SHOP-1);
        status = pthread_mutex_trylock(&shops[currentShop].mtx);
        if (status != 0)
            continue;

        /* Начало критической секции */
        if (shops[currentShop].supply == 0) {
            printf("The Buyer (pthread #%lu) in shop %d. Shop is empty. Go next shop\n",
                    (unsigned long) pthread_self(), shops[currentShop].numShop);
            /* "Экстренный" выход из критической секции и открытие мьютекса */
            status = pthread_mutex_unlock(&shops[currentShop].mtx);
            if (status != 0) {
                printf("ERROR: pthread_mutex_unlock (#%lu) - status (iamBuyer)\n", (unsigned long) pthread_self());
                exit(EXIT_FAILURE);
            }
            sleep(SLEEP_IF_SHOP_IS_EMPTY);
            continue;
        }

        if (demand >= shops[currentShop].supply) {
            int items = shops[currentShop].supply;
            demand -= items;
            shops[currentShop].supply -= items;
            printf("The Buyer (pthread #%lu) in shop %d. Bought all %d items. Demand: %d items. Go next shop\n",
                    (unsigned long) pthread_self(), shops[currentShop].numShop, items, demand);
        } else {
            int items = demand;
            demand -= items;
            shops[currentShop].supply -= items;
            printf("The Buyer (pthread #%lu) in shop %d. Bought %d items. Left in shop: %d items\n",
                    (unsigned long) pthread_self(), shops[currentShop].numShop, items, shops[currentShop].supply);
        }
        /* Конец критической секции */
        status = pthread_mutex_unlock(&shops[currentShop].mtx);
        if (status != 0) {
            printf("ERROR: pthread_mutex_unlock (#%lu) - status (iamBuyer)\n", (unsigned long) pthread_self());
            exit(EXIT_FAILURE);
        }
        sleep(SLEEP_AFTER_BUYING);
    }

    printf("The Buyer (pthread #%lu) has finished shopping\n", (unsigned long) pthread_self());
    pthread_exit(NULL);
}

/**
 * Функция "поставщик". Вторая поточная функция. Описание приведено в ../README.md
 * @param arg Принимает структуру, в которой отражена информация о имеющихся магазинах
 */
void *iamSupplier(void *arg)
{
    supply_t *shops = (supply_t *) arg;
    int status;
    srand(pthread_self());

    for (;;) {

        /* Диапазон магазинов от 0 до 4 */
        int currentShop = MYRAND(0, NUM_SHOP-1);
        status = pthread_mutex_trylock(&shops[currentShop].mtx);
        if (status != 0)
            continue;

        /* Начало критической секции */
        if (shops[currentShop].supply < SUPPLIERSIZE) {
            shops[currentShop].supply += SUPPLIERSIZE;
            printf(">>>The Supplier replenished the shop (%d) with %d products. Now in shop: %d. Go next shop\n",
                    shops[currentShop].numShop, SUPPLIERSIZE, shops[currentShop].supply);
        }
        /* Конец критической секции */

        status = pthread_mutex_unlock(&shops[currentShop].mtx);
        if (status != 0) {
            printf("ERROR: pthread_mutex_unlock (#%lu) - status (iamSupplier)\n", (unsigned long) pthread_self());
            exit(EXIT_FAILURE);
        }
        sleep(SLEEP_AFTER_REFILLS);
    }
}
