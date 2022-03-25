#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <pthread.h>
#include <sys/stat.h>
#include <errno.h>
#include <mqueue.h>

#include "myChat_MQ_server.h"
#include "serverFunc.h"
#include "structFunc.h"

int main(int argc, char *argv[])
{
    if (argc != 2) {
        usageError(argv[0]);
    }

    char room[MAX_ROOMNAME+2];
    if (strlen(argv[1]) > MAX_ROOMNAME) {
        printf("ERROR: The maximum length of the room name - %d characters\n", MAX_ROOMNAME);
        usageError(argv[0]);
        exit(EXIT_FAILURE);
    }
    strcpy(room, argv[1]);

    /*
     * Потоки:
     * 1. Следит за очередью сообщений "users". Туда будут "подключаться" новые пользователи
     * 2. Следит за очередью сообщений "name_room". Туда будут приходить все сообщения от пользователей
     * 3... Поток-пользователь (каждый подключаемый пользователь)
     *
     * */

    int status;

    /* Поток 1 */
    /* Инициализация односвязного списка с подключёнными пользователями */
    singlyLinkedList *users = NULL;
    users = createSinglyLinkedList();
    /*addNewUserBack(users, 0, 0, "admin");*/

    pthread_t t1_check_users;
    status = pthread_create(&t1_check_users, NULL, checkUsers, &users[0]);
    if (status != 0) {
        printf("ERROR: pthread_create - checkUsers (main)\n");
        exit(EXIT_FAILURE);
    }

    /* Поток 2 */
    /* Инициализация двусвязного списка с историей сообщений */
    doublyLinkedList *messages = NULL;
    messages = createDoublyLinkedList();
    /* Формируется "техническое сообщение", в котором главное поле - name */
    addNewMessageBack(messages, 0, 0, argv[1], "start chat");

    pthread_t t2_check_messages;
    status = pthread_create(&t2_check_messages, NULL, checkMessages, &messages[0]);
    if (status != 0) {
        printf("ERROR: pthread_create - checkMessages (main)\n");
        exit(EXIT_FAILURE);
    }

    /* Потоки 3-(MAX_USERS+2) */
    /* Инициализация структуры пользователи+сообщения */
    database *db = NULL;
    db = (database *) malloc(MAX_USERS * sizeof(database));
    if (db == NULL) {
        printf("ERROR: malloc - db (f: main)\n");
        exit(EXIT_FAILURE);
    }

    int k = 0;
    pthread_t t_users[MAX_USERS];

    for (;k < MAX_USERS; k++) {
        status = pthread_mutex_lock(&(users->mtx));

        while (users->raw == 0)
            status = pthread_cond_wait(&(users->cnd), &(users->mtx));

        db[k].messages = messages;

        if (users->raw == 1) {
            db[k].user = *(users->tail);
        } else {
            printf("Будут потеряны пользователи. При появлении данного сообщения переработать фрагмент кода\n");
            printf("Скорее всего, это не будет работать, если 2+ потока будут анализировать 'users'\n");
        }

        while (users->raw > 0) {
            status = pthread_create(&t_users[k], NULL, receiver, &db[k]);
            (users->raw)--;
        }

        status = pthread_mutex_unlock(&(users->mtx));
    }

    status = pthread_join(t2_check_messages, NULL);
    status = pthread_join(t1_check_users, NULL);

    status = mq_unlink(room);
    if (status == -1) {
        printf("Error - %d\n", errno);
        exit(EXIT_FAILURE);
    }

    status = mq_unlink("/users");
    if (status == -1) {
        printf("Error - %d\n", errno);
        exit(EXIT_FAILURE);
    }

    exit(EXIT_SUCCESS);
}

