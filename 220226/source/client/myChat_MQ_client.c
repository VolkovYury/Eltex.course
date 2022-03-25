#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>

#include "myChat_MQ_client.h"
#include "clientFunc.h"

int main()
{
    userConf *user = (userConf *) malloc(sizeof(userConf));
    if (user == NULL) {
        perror("ERROR: malloc - user (main)");
        exit(EXIT_FAILURE);
    }

    registerClient(user);

    /* Поток 1. Прослушивание сообщений в очереди */
    int status;
    pthread_t t1_check_msgs;
    status = pthread_create(&t1_check_msgs, NULL, receiveMsgs, user);
    if (status != 0) {
        printf("ERROR: pthread_create - receiveMsg (main)\n");
        exit(EXIT_FAILURE);
    }

    /* Поток 2. Отправка сообщений на сервер */
    pthread_t t2_send_msgs;
    status = pthread_create(&t2_send_msgs, NULL, sendMsgs, user);
    if (status != 0) {
        printf("ERROR: pthread_create - sendMsg (main)\n");
        exit(EXIT_FAILURE);
    }

    status= pthread_join(t1_check_msgs, NULL);
    status= pthread_join(t1_check_msgs, NULL);

    exit(EXIT_SUCCESS);
}