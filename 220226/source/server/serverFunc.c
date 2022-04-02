#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <mqueue.h>
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
    struct mq_attr attr;
    attr.mq_flags = 0;
    attr.mq_maxmsg = maxmsg;
    attr.mq_msgsize = maxsizemsg;
    attr.mq_curmsgs = 0;

    mqd_t mqd;
    mqd = mq_open(room, flags, 0666, &attr);
    if (mqd == (mqd_t) -1) {
        perror("ERROR: mq_open (createMQ)");
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
    mqd_t mq_users = createMQ("/users", O_RDONLY | O_CREAT | O_EXCL, MAX_NUM_MSG, MAX_NICKNAME);

    char *nickname = (char *) malloc(MAX_NICKNAME *sizeof (char));
    int count = 0;
    ssize_t len;

    while (1) {
        memset(nickname, 0x00, MAX_NICKNAME * sizeof(char));
        len = mq_receive(mq_users, nickname,MAX_NICKNAME,NULL);
        if (len < 0) {
            perror("ERROR: mq_receive (checkUsers)");
            exit(EXIT_FAILURE);
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
    mqd_t mq_messages = createMQ(((doublyLinkedList *) arg)->head->nickname, O_RDONLY | O_CREAT | O_EXCL, MAX_NUM_MSG, MAX_SIZE_MSG);

    struct mq_attr attr;
    if (mq_getattr(mq_messages, &attr) == -1) {
        perror("ERROR: mq_getattr (f: checkMessages)");
        exit(EXIT_FAILURE);
    }

    char *buffer = malloc(MAX_SIZE_MSG * sizeof(char));
    if (buffer == NULL) {
        printf("ERROR: malloc - buffer (f: checkUsers)\n");
        exit(EXIT_FAILURE);
    }

    char *timeSend = (char *) malloc(10 * sizeof(char));
    if (timeSend == NULL) {
        printf("ERROR: malloc - timeSend (f: checkUsers)\n");
        exit(EXIT_FAILURE);
    }

    char *nickname = (char *) malloc(MAX_NICKNAME * sizeof(char));
    if (nickname == NULL) {
        printf("ERROR: malloc - nickname (f: checkUsers)\n");
        exit(EXIT_FAILURE);
    }

    char *text = malloc(MAX_SIZE_DATA * sizeof(char));
    if (text == NULL) {
        printf("ERROR: malloc - text (f: checkUsers)\n");
        exit(EXIT_FAILURE);
    }

    size_t len;
    int count = 1;

    while (1) {
        memset(buffer, 0x00, MAX_SIZE_MSG * sizeof(char));
        memset(timeSend, 0x00, 10 * sizeof(char));
        memset(nickname, 0x00, MAX_NICKNAME * sizeof(char));
        memset(text, 0x00, MAX_SIZE_DATA * sizeof(char));

        len = mq_receive(mq_messages, buffer,MAX_SIZE_MSG,NULL);
        if (len < 0) {
            perror("ERROR: mq_receive (checkMessages)");
            exit(EXIT_FAILURE);
        } else {
            /* Парсинг буфера на элементы по маске "time:nickname:text" */
            printf("MSG: %s\n", buffer);

            parseMessage(buffer, timeSend, nickname, text);

            pthread_mutex_lock(&((doublyLinkedList *) arg)->mtx);
            addNewMessageBack((doublyLinkedList *) arg, count++, (int) strtol (timeSend, 0, 10), nickname, text);
            pthread_mutex_unlock(&((doublyLinkedList *) arg)->mtx);
            pthread_cond_signal(&((doublyLinkedList *) arg)->cnd);
        }
    }
}

/**
 * Поточная функция. Отправляет сообщения в очереди сообщений каждого участника чата
 * @param arg Указатель на структуру из односвязного списка пользователей и двусвязного списка сообщений
 */
void *receiver(void *arg)
{
    database *tmp = arg;
    int status;

    mqd_t mq_user = mq_open(tmp->user.roomnameReceive, O_WRONLY);
    if (mq_user == (mqd_t) -1)  {
        perror("ERROR: mq_open (registerClient)");
        exit(EXIT_FAILURE);
    }

    char *buffer = (char *) malloc(MAX_SIZE_MSG * sizeof(char));
    if (buffer == NULL) {
        printf("ERROR: malloc - buffer (f: receiver)\n");
        exit(EXIT_FAILURE);
    }
    char time[10];
    char delim[1] = ":";

    messages_t *currentMsg = &(tmp->messages->head_const);

    /* Ожидание появления сообщений. Если начался цикл, то в сообщениях только 1 техническое нулевое сообщение */
    status = pthread_mutex_lock(&(tmp->messages->mtx));
    while (tmp->messages->size < 2) {
        status = pthread_cond_wait(&(tmp->messages->cnd), &(tmp->messages->mtx));
    }
    status = pthread_mutex_unlock(&(tmp->messages->mtx));

    int total = 1;
    while (1) {
        status = pthread_mutex_lock(&(tmp->messages->mtx));

        /* Условие при котором пересылка сообщений прекращается */
        /* "Индекс текущего сообщения" равен "Размеру списка сообщений" */
        while (total == tmp->messages->size) {
            status = pthread_cond_wait(&(tmp->messages->cnd), &(tmp->messages->mtx));
        }

        while (total < tmp->messages->size) {

            currentMsg = currentMsg->next;

            sprintf(time, "%d", currentMsg->time);
            strcpy(buffer, time);
            strncat(buffer, delim, 1);
            strncat(buffer, currentMsg->nickname, MAX_NICKNAME);
            strncat(buffer, delim, 1);
            strncat(buffer, currentMsg->text, MAX_SIZE_DATA);

            status = mq_send(mq_user, buffer, MAX_SIZE_MSG, 0);
            if (status < 0) {
                perror("ERROR: mq_send (receiver)");
                pthread_exit(NULL);
            }

            total++;
        }
        status = pthread_mutex_unlock(&(tmp->messages->mtx));
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
    int i = 0, k = 0;
    for(; buffer[i] != ':';) {
        time[k++] = buffer[i++];
    }
    i++;
    k = 0;

    for(; buffer[i] != ':';) {
        nick[k++] = buffer[i++];
    }
    i++;
    k = 0;

    for(; buffer[i] != '\0';) {
        text[k++] = buffer[i++];
    }
}
