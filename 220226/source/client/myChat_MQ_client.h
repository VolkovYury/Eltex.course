#ifndef MYCHAT_MQ_CLIENT_H
#define MYCHAT_MQ_CLIENT_H

#include <mqueue.h>

#define MAX_ROOMNAME 20
#define MAX_NICKNAME 20

#define MAX_NUM_MSG 10  /* Может не создать очередь, если будет больше 10. Можно только уменьшать */
#define MAX_SIZE_MSG 512
#define MAX_SIZE_DATA 450   /* Примерно:  450 = MAX_SIZE_MSG - 2 разделителя - MAX_ROOMNAME - MAX_NICKNAME */

typedef struct userConfiguration
{
    char nickname[MAX_NICKNAME];
    char roomNameReceive[MAX_NICKNAME+1];
    char roomNameSend[MAX_ROOMNAME+1];
    mqd_t queueSendMsg;
    mqd_t queueReceiveMsg;

    pthread_mutex_t mtx;
} userConf;

#endif
