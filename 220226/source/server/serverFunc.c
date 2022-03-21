#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <mqueue.h>
#include <errno.h>
#include <time.h>
#include <pthread.h>
#include <unistd.h>

#include "myChat_MQ_server.h"
#include "serverFunc.h"
#include "structFunc.h"

/**
 * Вывод информационного сообщения о том, как правильно запускать программу
 * @param program
 */
void usageError(const char *program)
{
    printf("Usage: %s /name_room\n", program);
    printf("The maximum length of the room name - %d characters\n", MAX_ROOMNAME);
    exit(EXIT_FAILURE);
}

/**
 * Функция-обёртка для создания новой очереди
 * @param room Название комнаты. Без пробелов, начинается с '/'
 * @param flags Параметры создаваемой очереди
 * @param maxmsg Максимальное количество сообщений в очереди
 * @param maxsizemsg Размер одного сообщения
 * @return Идентификатор созданной очереди
 */
mqd_t createMQ(char room[], int flags, unsigned int maxmsg, unsigned int maxsizemsg)
{
    int status;
    mqd_t mqd;
    struct mq_attr attr;

    mqd = mq_open(room, flags, 0666, NULL);
    if (mqd == (mqd_t) -1) {
        printf("ERROR: mq_open (openMQ)\n");

        switch (errno) {
        case EACCES:
            printf("The queue exists, but the caller does not have permission to open it in the specified mode\n"
                   "Or name contained more than one slash\n");
            break;
        case EEXIST:
            printf("The queue with this name already exists\n");
            break;
        case ENFILE:
            printf("The system-wide limit on the total number of open files and message queues has been reached\n");
            break;
        case ENOMEM:
            printf("Insufficient memory\n");
            break;
        default:
            printf("Other error (errno - %d; string - %s)\n", errno, room);
        }
        
        exit(EXIT_FAILURE);
    }

    attr.mq_flags = 0;
    attr.mq_maxmsg = MAX_NUM_MSG;
    attr.mq_msgsize = MAX_SIZE_MSG;

    status = mq_setattr(mqd, &attr, NULL);
    if (status == -1) {
        printf("ERROR: mq_setattr (openMQ) - %s: errno - %d\n", room, errno);

        status = mq_unlink(room);
        if (status == -1)
            printf("ERROR: mq_unlink (openMQ)\n");

        exit(EXIT_FAILURE);
    }

    return mqd;
}

/**
 * Поточная функция. Обрабатывает подключаемых пользователей и создаёт односвязный список.
 * @param arg Указатель на односвязный список
 */
void *checkUsers(void *arg)
{
    mqd_t mq_users = createMQ("/users", O_RDONLY | O_CREAT | O_EXCL, 20, MAX_NICKNAME);

    struct mq_attr attr;
    if (mq_getattr(mq_users, &attr) == -1) {
        printf("ERROR: mq_getattr (f: checkUsers)\n");
        exit(EXIT_FAILURE);
    }

    size_t len;
    char *nickname;
    nickname = malloc(MAX_NICKNAME * sizeof(char));
    if (nickname == NULL) {
        printf("ERROR: malloc (f: checkUsers)\n");
        exit(EXIT_FAILURE);
    }
    int count = 0;

    while (1) {
        memset(nickname, 0x00, MAX_NICKNAME * sizeof(char));
        len = mq_receive(mq_users, nickname,MAX_NICKNAME,NULL);
        if (len < 0) {
            switch (errno) {
            case EMSGSIZE:
                printf("msg_len was less than the mq_msgsize attribute of the message queue\n");
                pthread_exit(NULL);
            default:
                printf("Other error (errno = %d)\n", errno);
                pthread_exit(NULL);
            }
        } else {
            pthread_mutex_lock(&((singlyLinkedList *) arg)->mtx);

            addNewUserBack((singlyLinkedList *) arg, count++, time(NULL), nickname);

            pthread_mutex_unlock(&((singlyLinkedList *) arg)->mtx);
            pthread_cond_signal(&((singlyLinkedList *) arg)->cnd);
        }
    }
}

/**
 * Поточная функция. Обрабатывает получаемые сообщения и создаёт двусвязный список.
 * @param arg Указатель на двусвязный список
 */
void *checkMessages(void *arg)
{
    /* Имя очереди будет записано в структуре message_t под индексом 0 в поле nickname */
    mqd_t mq_messages = createMQ(((doublyLinkedList *) arg)->head->nickname, O_RDWR | O_CREAT | O_EXCL, 50, 1024);

    mq_send(mq_messages, "11111:tom:Hi qwerrt!", 1024, 0);
    mq_send(mq_messages, "22222:lera:Hi asddd!", 1024, 0);
    mq_send(mq_messages, "33333:yura:Hi sdas!", 1024, 0);
    mq_send(mq_messages, "44444:don:Hi dddc!", 1024, 0);
    mq_send(mq_messages, "55555:karleone:Hi lkjhjh!", 1024, 0);

    struct mq_attr attr;
    if (mq_getattr(mq_messages, &attr) == -1) {
        printf("ERROR: mq_getattr (f: checkMessages)\n");
        exit(EXIT_FAILURE);
    }


    char *timeSend = (char *) malloc(10 * sizeof(char));
    if (timeSend == NULL) {
        printf("ERROR: malloc (f: checkUsers)\n");
        exit(EXIT_FAILURE);
    }
    char *nickname = (char *) malloc(MAX_NICKNAME * sizeof(char));
    if (nickname == NULL) {
        printf("ERROR: malloc (f: checkUsers)\n");
        exit(EXIT_FAILURE);
    }
    char *text;
    text = malloc(attr.mq_msgsize * sizeof(char));
    if (text == NULL) {
        printf("ERROR: malloc (f: checkUsers)\n");
        exit(EXIT_FAILURE);
    }
    char *buffer;
    buffer = malloc(attr.mq_msgsize * sizeof(char));
    if (buffer == NULL) {
        printf("ERROR: malloc (f: checkUsers)\n");
        exit(EXIT_FAILURE);
    }

    size_t len;
    int count = 0;

    while (count < 5) {
        memset(timeSend, 0x00, 10 * sizeof(char));
        memset(nickname, 0x00, MAX_NICKNAME * sizeof(char));
        memset(text, 0x00, attr.mq_msgsize * sizeof(char));
        memset(buffer, 0x00, attr.mq_msgsize * sizeof(char));

        len = mq_receive(mq_messages, buffer,attr.mq_msgsize,NULL);
        if (len < 0) {
            switch (errno) {
            case EMSGSIZE:
                printf("msg_len was less than the mq_msgsize attribute of the message queue\n");
                pthread_exit(NULL);
            default:
                printf("Other error (errno = %d)\n", errno);
                pthread_exit(NULL);
            }
        } else {
            /* Парсинг буфера на элементы по маске "time:nickname:text" */
            parseMessage(buffer, timeSend, nickname, text);

            addNewMessageBack((doublyLinkedList *) arg, count++, (int) strtol (timeSend, 0, 10), nickname, text);
        }
    }

    pthread_exit(NULL);
}

/**
 * Поточная функция. Отправляет сообщения в очереди сообщений каждого участника чата
 * @param arg Указатель на структуру из односвязного списка пользователей и двусвязного списка сообщений
 */
void *receiver(void *arg)
{
    int total = 0, len;

    mqd_t mq_user = mq_open(((database *) arg)->user->roomnameReceive, O_WRONLY);
    if (mq_user == (mqd_t) -1) {
        printf("ERROR: mq_open (openMQ)\n");
        switch (errno) {
        case EACCES:
            printf("The queue exists, but the caller does not have permission to open it in the specified mode\n"
                   "Or name contained more than one slash\n");
            break;
        case ENFILE:
            printf("The system-wide limit on the total number of open files and message queues has been reached\n");
            break;
        case ENOMEM:
            printf("Insufficient memory\n");
            break;
        default:
            printf("Other error (errno - %d; string - %s)\n", errno, ((database *) arg)->user->roomnameReceive);
        }
        exit(EXIT_FAILURE);
    }

    char *buffer = (char *) malloc(MAX_SIZE_MSG *sizeof(char));
    char time[10];
    char delim[1] = ":";

    while (1) {
        sprintf(time, "%d", ((database *) arg)->messages->head[total].time);
        strcpy(buffer, time);
        strncat(buffer, delim, 1);
        strncat(buffer, ((database *) arg)->messages->head[total].nickname, MAX_NICKNAME);
        strncat(buffer, delim, 1);
        strncat(buffer, ((database *) arg)->messages->head[total].text, 900);

        len = mq_send(mq_user, buffer, MAX_SIZE_MSG, 0);

        total++;
        while (total == ((database *) arg)->messages->size)
            sleep(2);
    }
}

/**
 * Функция парсинга получаемого сообщения по маске "время отправки":"ник пользователя":"текст сообщения"
 * @param buffer Полученное сообщение
 * @param time Время в секундах, начиная от 01.01.1970
 * @param nick Имя пользователя отправившего сообщение
 * @param text Текс сообщения
 */
void parseMessage(const char *buffer, char *time, char *nick, char *text)
{
    int i = 0, k = 0, l = 0, m = 0;
    for(; buffer[i] != ':';) {
        time[k++] = buffer[i++];
    }
    i++;

    for(; buffer[i] != ':';) {
        nick[l++] = buffer[i++];
    }
    i++;

    for(; buffer[i] != '\0';) {
        text[m++] = buffer[i++];
    }
}
