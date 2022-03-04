#ifndef LIFEGAME_H
#define LIFEGAME_H

#include <pthread.h>
#define NUM_SHOP 5
#define NUM_BUYER 3
#define SUPPLIERSIZE 1000

#define MAXDEMAND 11000
#define MINDEMAND 9000
#define MAXSUPPLY 1100
#define MINSUPPLY 900

#define SLEEP_IF_SHOP_IS_EMPTY 1
#define SLEEP_AFTER_BUYING 3
#define SLEEP_AFTER_REFILLS 1

#define MYRAND(lvalue, rvalue) lvalue+rand()%(rvalue-lvalue+1)

typedef struct supply_s
{
    int numShop;
    pthread_mutex_t mtx;
    int supply;
} supply_t;

#endif
