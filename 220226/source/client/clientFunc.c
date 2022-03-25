#include <mqueue.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <pthread.h>

#include "myChat_MQ_client.h"
#include "clientFunc.h"

/**
 * Регистрация клиента: формирование пользовательской конфигурации, создание/подключение к очередям сообщений
 * @param user Указатель на структуру с конфигурацией пользователя
 */
void registerClient(userConf *user)
{
    /* Получение ника. Пользовательский ввод (scanf) */
    setNickname(user);

    /* Создание очереди для получения сообщений */
    createQueueReceiveMsg(user);

    /* Получение названия комнаты чата. Пользовательский ввод (scanf) */
    setRoomName(user);

    /* Подключение к очереди для отправки сообщений */
    openQueueSendMsg(user);

    /* Отправка ника в очередь "users" для регистрации на сервере */
    sendNickname(user);

    /* Вывод окна об успешной регистрации в системе */
    printf("Connect successful!\n");
}

/**
 * Получение ника клиента. Формирование структуры конфигурации - часть 1 из 5.
 * @param user Указатель на структуру с конфигурацией пользователя
 */
void setNickname(userConf *user)
{
    char *name = (char *) malloc(MAX_NICKNAME *sizeof(char));
    if (name == NULL) {
        perror("ERROR: malloc - name (setNickname)");
        exit(EXIT_FAILURE);
    }
    printf("Enter name (Latin letters and numbers, no special characters, %d characters maximum): ", MAX_NICKNAME);

    int nameCorrect = 0;
    while (!nameCorrect) {
        scanf("%s", name);
        char clr;
        while((clr = (char) getchar()) != '\n' && clr != EOF ); /* Очистка stdin */

        int i;
        for (i = 0; name[i] != '\0'; i++) {
            /* Проверка валидности имени */
            if ((name[i] >= 48 && name[i] <= 57) || (name[i] >= 65 && name[i] <= 90) || (name[i] >= 97 && name[i] <= 122)) {
                nameCorrect = 1;
                continue;
            } else {
                printf("The name contains an invalid character: %c\n", name[i]);
                printf("Try again: ");
                nameCorrect = 0;
                break;
            }
        }

        if (nameCorrect) {
            break;
        }
    }

    strcpy(user->nickname, name);
    printf("system: Current nickname: %s\n", user->nickname);
    free(name);
}

/**
 * Создание очереди для получения сообщений от сервера. Формирование структуры конфигурации - часть 2,3 из 5.
 * @param user Указатель на структуру с конфигурацией пользователя
 */
void createQueueReceiveMsg(userConf *user)
{
    user->roomNameReceive[0] = '/';
    strncat(user->roomNameReceive, user->nickname, 20);

    struct mq_attr attr;
    attr.mq_flags = 0;
    attr.mq_msgsize = MAX_SIZE_MSG;
    attr.mq_maxmsg = MAX_NUM_MSG;
    attr.mq_curmsgs = 0;

    user->queueReceiveMsg = mq_open(user->roomNameReceive, O_RDONLY | O_CREAT | O_EXCL, 0666, &attr);
    if (user->queueReceiveMsg == (mqd_t) -1) {
        perror("ERROR: mq_open (createQueueReceiveMsg)");
        exit(EXIT_FAILURE);
    }

    printf("system: Message queue (receive) created: mqd_t - %d\n", user->queueReceiveMsg);
}

/**
 * Получение названия комнаты чата. Формирование структуры конфигурации - часть 4 из 5.
 * @param user Указатель на структуру с конфигурацией пользователя
 */
void setRoomName(userConf *user)
{
    char *room = (char *) malloc(MAX_ROOMNAME *sizeof(char));
    if (room == NULL) {
        perror("ERROR: malloc - room (setRoomName)");
        exit(EXIT_FAILURE);
    }
    printf("Enter room name (Latin letters and numbers, no special characters, %d characters maximum): ", MAX_ROOMNAME);

    int roomCorrect = 0;
    while (!roomCorrect) {
        scanf("%s", room);
        char clr;
        while((clr = (char) getchar()) != '\n' && clr != EOF ); /* Очистка stdin */

        int i;
        for (i = 0; room[i] != '\0'; i++) {
            /* Проверка валидности имени */
            if ((room[i] >= 48 && room[i] <= 57) || (room[i] >= 65 && room[i] <= 90) || (room[i] >= 97 && room[i] <= 122)) {
                roomCorrect = 1;
                continue;
            } else {
                printf("The room name contains an invalid character: %c\n", room[i]);
                printf("Try again: ");
                roomCorrect = 0;
                break;
            }
        }

        if (roomCorrect) {
            break;
        }
    }

    user->roomNameSend[0] = '/';
    strncat(user->roomNameSend, room, 20);
    free(room);
}

/**
 * Подключение к очереди для отправки сообщений на сервер. Формирование структуры конфигурации - часть 5 из 5.
 * @param user Указатель на структуру с конфигурацией пользователя
 */
void openQueueSendMsg(userConf *user)
{
    user->queueSendMsg = mq_open(user->roomNameSend, O_WRONLY);
    if (user->queueReceiveMsg == (mqd_t) -1) {
        perror("ERROR: mq_open (openQueueSendMsg)");
        exit(EXIT_FAILURE);
    }
    printf("system: Message queue (send) created: mqd_t - %d\n", user->queueSendMsg);
}

/**
 * Отправка ника данного пользователя на сервер для регистрации.
 * @param user Указатель на структуру с конфигурацией пользователя
 */
void sendNickname(userConf *user)
{
    mqd_t mq_users;
    mq_users = mq_open("/users", O_WRONLY);
    if (mq_users == (mqd_t) -1) {
        perror("ERROR: mq_open (sendNickname)");
        exit(EXIT_FAILURE);
    }

    int status;
    status = mq_send(mq_users, user->nickname, MAX_NICKNAME, 0);
    if (status < 0) {
        perror("ERROR: mq_send (sendNickname)");
        exit(EXIT_FAILURE);
    }
}

/**
 * Поточная функция. Получает сообщения с сервера в специальную очередь (ник текущего пользователя)
 * @param arg Указатель на структуру с конфигурацией пользователя
 */
void *receiveMsgs(void *arg)
{
    userConf *tmp = arg;

    mqd_t mq_receiveMsg;
    mq_receiveMsg = mq_open(tmp->roomNameReceive, O_RDONLY);
    if (mq_receiveMsg == (mqd_t) -1)  {
        perror("ERROR: mq_open (receiveMsgs)");
        exit(EXIT_FAILURE);
    }

    int status;
    struct mq_attr attr;
    status = mq_getattr(mq_receiveMsg, &attr);
    if (status < 0) {
        perror("ERROR: mq_getattr (receiveMsgs)");
        exit(EXIT_FAILURE);
    }

    char *buffer;
    buffer = malloc(attr.mq_msgsize * sizeof(char));
    if (buffer == NULL) {
        perror("ERROR: malloc - buffer (f: receiveMsgs)");
        exit(EXIT_FAILURE);
    }

    char *timeSend = (char *) malloc(10 * sizeof(char));
    if (timeSend == NULL) {
        perror("ERROR: malloc - timeSend (f: receiveMsgs)");
        exit(EXIT_FAILURE);
    }

    char *nickname = (char *) malloc(MAX_NICKNAME * sizeof(char));
    if (nickname == NULL) {
        perror("ERROR: malloc - nickname (f: receiveMsgs)");
        exit(EXIT_FAILURE);
    }

    char *text;
    text = malloc(MAX_SIZE_DATA * sizeof(char));
    if (text == NULL) {
        perror("ERROR: malloc - text (f: receiveMsgs)");
        exit(EXIT_FAILURE);
    }

    ssize_t len;
    int count = 0;
    while (1) {
        memset(buffer, 0x00, attr.mq_msgsize * sizeof(char));
        memset(timeSend, 0x00, 10 * sizeof(char));
        memset(nickname, 0x00, MAX_NICKNAME * sizeof(char));
        memset(text, 0x00, MAX_SIZE_DATA * sizeof(char));

        len = mq_receive(mq_receiveMsg, buffer, attr.mq_msgsize, NULL);
        if (len < 0) {
            perror("ERROR: mq_receive (receiveMsgs)");
            exit(EXIT_FAILURE);
        } else {
            parseMessage(buffer, timeSend, nickname, text);
            printf("%s: %s (%s)\n", nickname, text, timeSend);
        }
        count++;
    }
}

/**
 * Функция парсинга получаемого сообщения по маске "время отправки":"ник пользователя":"текст сообщения"
 * @param buffer Полученное сообщение
 * @param time Время в секундах, начиная от 01.01.1970
 * @param nick Имя пользователя отправившего сообщение
 * @param text Текст сообщения
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

/**
 * Поточная функция. Отправляет сообщения на сервер в специальную очередь (название комнаты чата)
 * @param arg Указатель на структуру с конфигурацией пользователя
 */
void *sendMsgs(void *arg)
{
    userConf *tmp = arg;

    char *buffer = (char *) malloc (MAX_SIZE_MSG * sizeof(char));
    if (buffer == NULL) {
        perror("ERROR: malloc - buffer (f: sendMsgs)");
        exit(EXIT_FAILURE);
    }

    char *text = (char *) malloc (MAX_SIZE_DATA * sizeof(char));
    if (text == NULL) {
        perror("ERROR: malloc - text (f: sendMsgs)");
        exit(EXIT_FAILURE);
    }

    char delim[1] = ":";

    while (1) {
        fgets(text, MAX_SIZE_DATA, stdin);

        char timeSend[10];
        sprintf(timeSend, "%d", (int) time(NULL));
        strcpy(buffer, timeSend);
        strncat(buffer, delim, 1);
        strncat(buffer, tmp->nickname, MAX_NICKNAME);
        strncat(buffer, delim, 1);
        strncat(buffer, text, strlen(text) - 1);

        int status;
        status = mq_send(tmp->queueSendMsg, buffer, MAX_SIZE_MSG, 0);
        if (status < 0) {
            perror("ERROR: mq_send (sendMsgs)");
            exit(EXIT_FAILURE);
        }
    }
}
