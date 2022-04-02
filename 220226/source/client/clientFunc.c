#include <mqueue.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <stdio.h>

#include "myChat_MQ_client.h"
#include "interfaceFunctions.h"
#include "clientFunc.h"

/**
 * Получение ника клиента. Формирование структуры конфигурации - часть 1 из 5.
 * Создание очереди для получения сообщений от сервера. Формирование структуры конфигурации - часть 2,3 из 5.
 * @param user Указатель на структуру с конфигурацией пользователя
 */
void setUsername(userConfig *user, windowConfig *win)
{
    /* Получение ника */
    char *name = (char *) malloc(MAX_NICKNAME *sizeof(char));
    if (name == NULL)
        myExit("setUsername\0", "malloc - name\0", user);

    mvwprintw(win->workspace, 1, 1, "Enter username");
    mvwprintw(win->workspace, 3, 1, "Latin letters and numbers, %d characters maximum", MAX_NICKNAME);
    mvwprintw(win->workspace, 5, 1, "UN: ");

    /* Флаг "имя корректно" */
    int nameCorrect = 0;
    while (!nameCorrect) {
        mvwprintw(win->workspace, 5, 5, "                    ");    /* Очистка поля для ввода */
        wrefresh(win->workspace);
        mvwgetnstr(win->workspace, 5, 5, name, MAX_NICKNAME);

        int i;
        for (i = 0; name[i] != '\0'; i++) {
            /* Проверка валидности имени */
            if ((name[i] >= 48 && name[i] <= 57) || (name[i] >= 65 && name[i] <= 90) || (name[i] >= 97 && name[i] <= 122)) {
                nameCorrect = 1;
                continue;
            } else {
                /* Очистка поля для логов */
                mvwprintw(win->workspace, 16, 1, "                                                            ");
                mvwprintw(win->workspace, 16, 1, "LOG: The name contains an invalid character: %c. Try again!", name[i]);
                nameCorrect = 0;
                break;
            }
        }
    }


    strcpy(user->nickname, name);

    /* Создание очереди */
    user->roomNameReceive[0] = '/';
    strncat(user->roomNameReceive, user->nickname, 20);

    struct mq_attr attr;
    attr.mq_flags = 0;
    attr.mq_msgsize = MAX_SIZE_MSG;
    attr.mq_maxmsg = MAX_NUM_MSG;
    attr.mq_curmsgs = 0;

    user->queueReceiveMsg = mq_open(user->roomNameReceive, O_RDONLY | O_CREAT | O_EXCL, 0666, &attr);
    if (user->queueReceiveMsg == (mqd_t) -1)
        myExit("setUsername\0", "mq_open\0", user);

    free(name);
}

/**
 * Получение названия комнаты чата. Формирование структуры конфигурации - часть 4 из 5.
 * Подключение к очереди для отправки сообщений на сервер. Формирование структуры конфигурации - часть 5 из 5.
 * @param user Указатель на структуру с конфигурацией пользователя
 */
void setRoomName(userConfig *user, windowConfig *win)
{
    /* Получение названия комнаты */
    char *room = (char *) malloc(MAX_ROOMNAME *sizeof(char));
    memset(room, 0x00, MAX_ROOMNAME);
    if (room == NULL)
        myExit("setRoomName\0", "malloc - room\0", user);

    mvwprintw(win->workspace, 8, 1, "Enter room name");
    mvwprintw(win->workspace, 10, 1, "Latin letters and numbers, %d characters maximum", MAX_ROOMNAME);
    mvwprintw(win->workspace, 12, 1, "RN: ");

    int roomCorrect = 0;
    while (!roomCorrect) {
        mvwprintw(win->workspace, 12, 5, "                    ");
        wrefresh(win->workspace);
        mvwgetnstr(win->workspace, 12, 5, room, MAX_ROOMNAME);

        int i;
        for (i = 0; room[i] != '\0'; i++) {
            /* Проверка валидности имени */
            if ((room[i] >= 48 && room[i] <= 57) || (room[i] >= 65 && room[i] <= 90) || (room[i] >= 97 && room[i] <= 122)) {
                roomCorrect = 1;
                continue;
            } else {
                mvwprintw(win->workspace, 16, 1, "                                                            ");
                mvwprintw(win->workspace, 16, 1, "LOG: The room name contains an invalid character: %c. Try again!", room[i]);
                roomCorrect = 0;
                break;
            }
        }
    }

    /* Подключение к очереди */
    user->roomNameSend[0] = '/';
    strncat(user->roomNameSend, room, 20);

    user->queueSendMsg = mq_open(user->roomNameSend, O_WRONLY);
    if (user->queueSendMsg == (mqd_t) -1)
        myExit("setRoomName\0", "mq_open\0", user);

    free(room);
}

/**
 * Регистрация клиента: Отправка ника данного пользователя на сервер для регистрации.
 * @param user Указатель на структуру с конфигурацией пользователя
 */
void clientRegistration(userConfig *user)
{
    mqd_t mq_users;
    mq_users = mq_open("/users", O_WRONLY);
    if (mq_users == (mqd_t) -1)
        myExit("clientRegistration\0", "mq_open\0", user);

    int status;
    status = mq_send(mq_users, user->nickname, MAX_NICKNAME, 0);
    if (status < 0)
        myExit("clientRegistration\0", "mq_send\0", user);

    status = mq_close(mq_users);
    if (status < 0)
        myExit("clientRegistration\0", "mq_close\0", user);
}

/**
 * Поточная функция. Получает сообщения с сервера в специальную очередь (ник текущего пользователя)
 * @param arg Указатель на структуру с конфигурацией пользователя
 */
void *receiveMsgs(void *arg)
{
    configuration *tmp = arg;

    mqd_t mq_receiveMsg;
    mq_receiveMsg = mq_open(tmp->user->roomNameReceive, O_RDONLY);
    if (mq_receiveMsg == (mqd_t) -1)
        myExit("receiveMsgs\0", "mq_open\0", tmp->user);


    int status;
    struct mq_attr attr;
    status = mq_getattr(mq_receiveMsg, &attr);
    if (status < 0)
        myExit("receiveMsgs\0", "mq_getattr\0", tmp->user);


    char *buffer = malloc(attr.mq_msgsize * sizeof(char));
    if (buffer == NULL)
        myExit("receiveMsgs\0", "malloc - buffer\0", tmp->user);

    char *timeSend = (char *) malloc(10 * sizeof(char));
    if (timeSend == NULL)
        myExit("receiveMsgs\0", "malloc - timeSend\0", tmp->user);

    char *nickname = (char *) malloc(MAX_NICKNAME * sizeof(char));
    if (nickname == NULL)
        myExit("receiveMsgs\0", "malloc - nickname\0", tmp->user);

    char *text = malloc(MAX_SIZE_DATA * sizeof(char));
    if (text == NULL)
        myExit("receiveMsgs\0", "malloc - text\0", tmp->user);


    /* Память под строки в рабочей области */
    char **spaceStr = (char **) malloc ((tmp->chat_win->size_y - 4) * sizeof(char *));
    int i = 0;
    for (;i < tmp->chat_win->size_y - 4; i++) {
        char *str = (char *) malloc ((tmp->chat_win->size_x - 2) * sizeof(char));
        memset(str, ' ', tmp->chat_win->size_x - 2);
        spaceStr[i] = str;
    }

    /* Память под подключенных пользователей */
    int sizeList = 0;
    char **usersList = (char **) malloc(MAX_NUM_USERS *sizeof(char *));
    i = 0;
    for (; i < MAX_NUM_USERS; i++) {
        char *connectedUser = (char *) malloc(MAX_NICKNAME *sizeof(char));
        memset(connectedUser, '\0', MAX_NICKNAME);
        usersList[i] = connectedUser;
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
            myExit("receiveMsgs\0", "mq_receive\0", tmp->user);
            break; /* Нужен для того, чтобы IDE не подсвечивала Endless loop*/
        }

        parseMessage(buffer, timeSend, nickname, text);

        status = checkUser(usersList, &sizeList, nickname);
        if (status == 1) {
            int str = 0;
            wclear(tmp->users_win->workspace);

            for (; str < sizeList; str++) {
                mvwprintw(tmp->users_win->workspace, str, 0, "%s", usersList[str]);
                wrefresh(tmp->users_win->workspace);
            }
        }

        buildMsgSpace(nickname, timeSend, text, tmp, spaceStr);

        int k = 0;
        wclear(tmp->chat_win->workspace);
        for (; k < tmp->chat_win->size_y - 4; k++) {
            mvwprintw(tmp->chat_win->workspace, k, 0, "%s", spaceStr[k]);
            wrefresh(tmp->chat_win->workspace);
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
 * Функция анализирует массив с информацией о всех подключенных пользователях
 * @param usersList Указатель на массив с подключенными пользователями
 * @param sizeList Размер списка
 * @param username Текущий анализируемый пользователь
 * @return Если пользователь есть в списке - 0, иначе - 1
 */
int checkUser(char **usersList, int *sizeList, const char *username)
{
    int status;
    int i = 0;
    for (; i < *sizeList; i++) {
        status = strcmp(usersList[i], username);
        if (status == 0)
            return 0;
    }
    strcpy(usersList[*sizeList], username);
    ++(*sizeList);

    return 1;
}

/**
 * Функция построения структуры поля с сообщениями
 * @param nick Имя пользователя - отправителя сообщения
 * @param time Время отправки сообщения
 * @param text Текст сообщения
 * @param config Указатель на структуру с полной конфигурацией
 * @param space_str Указатель на массив строк, представляющий собой рабочее полотно
 */
void buildMsgSpace(const char *nick, const char *time, char *text, configuration *config, char **space_str)
{
    int maxStrLength = 70;  /* Если изменить эту константу, то надо подправить функции snprintf */
    int sizeText = strlen(text);
    int numStrText = sizeText/maxStrLength+1;
    int numStr = 1 + numStrText + 2;

    /* флаг "клиент - автор сообщения" */
    int self_send;
    if (strcmp(config->user->nickname, nick) == 0) {
        self_send = 1;
    } else {
        self_send = 0;
    }

    char *tmp = (char *) malloc ((config->chat_win->size_x - 2) *sizeof(char));


    char **msgStruct = (char **) malloc ((numStr) *sizeof(char *));
    int i, k;
    for (i = 0, k = 1; i < numStr; i++) {
        char *msgStr = (char *) malloc ((config->chat_win->size_x - 2) *sizeof(char));
        memset(msgStr, ' ', config->chat_win->size_x - 2);

        if (i == 0) {
            /* Если "отправляю" сообщение себе, то выравниваю по правому краю */
            memset(tmp, ' ', config->chat_win->size_x - 2);
            snprintf(tmp, config->chat_win->size_x - 2,"%s: (send time: %10s)", nick, time);

            if (self_send == 1) {
                snprintf(msgStr, config->chat_win->size_x - 2,"%112s", tmp);
            } else {
                snprintf(msgStr, config->chat_win->size_x - 2,"%s", tmp);
            }
        }

        if (i > 0 && i < numStrText) {

            if (self_send == 1) {
                snprintf(msgStr, config->chat_win->size_x - 2,"%112s", text + maxStrLength * (k-1));
            } else {
                snprintf(msgStr, config->chat_win->size_x - 2,"%s", text + maxStrLength * (k-1));
            }

            k++;
        }

        if (i == numStrText) {
            if (self_send == 1) {
                /* BAG#1. Настроить выравнивание последней строки. Сейчас при отправке себе - выравнивается по правому краю */
                snprintf(msgStr, config->chat_win->size_x - 2,"%112s", text + maxStrLength * (k-1));
            } else {
                snprintf(msgStr, config->chat_win->size_x - 2,"%s", text + maxStrLength * (k-1));
            }
        }

        if (i > numStrText) {
            memset(msgStr, ' ', config->chat_win->size_x - 2);
        }

        msgStruct[i] = msgStr;
    }

    i = 0;
    for (; i < config->chat_win->size_y - 4  - numStr; i++) {
        memset(space_str[i], ' ', (maxStrLength + 1));
        strcpy(space_str[i], space_str[i + numStr]);
    }
    k = 0;
    for (; i < config->chat_win->size_y - 4; i++, k++) {
        memset(space_str[i], ' ', (maxStrLength + 1));
        strcpy(space_str[i], msgStruct[k]);
    }

    i = 0;
    free(tmp);
    for (; i < numStr; i++) {
        free(msgStruct[i]);
    }
    free(msgStruct);
}

/**
 * Поточная функция. Отправляет сообщения на сервер в специальную очередь (название комнаты чата)
 * @param arg Указатель на структуру с конфигурацией пользователя
 */
void *sendMsgs(void *arg)
{
    configuration *tmp = arg;

    char *buffer = (char *) malloc (MAX_SIZE_MSG * sizeof(char));
    if (buffer == NULL)
        myExit("sendMsgs\0", "malloc - buffer\0", tmp->user);

    char *text = (char *) malloc (MAX_SIZE_DATA * sizeof(char));
    if (text == NULL)
        myExit("sendMsgs\0", "malloc - text\0", tmp->user);

    char delim[1] = ":";


    while (1) {
        wrefresh(tmp->textLabel_win->workspace);
        mvwprintw(tmp->textLabel_win->workspace, 0, 0, ">> ");
        mvwgetnstr(tmp->textLabel_win->workspace, 0, 3, text, MAX_SIZE_DATA);

        char timeSend[10];
        sprintf(timeSend, "%d", (int) time(NULL));
        strcpy(buffer, timeSend);
        strncat(buffer, delim, 1);
        strncat(buffer, tmp->user->nickname, MAX_NICKNAME);
        strncat(buffer, delim, 1);
        strncat(buffer, text, strlen(text));

        int status;
        status = mq_send(tmp->user->queueSendMsg, buffer, MAX_SIZE_MSG, 0);
        if (status < 0) {
            myExit("sendMsgs\0", "mq_send\0", tmp->user);
            break;  /* Нужен для того, чтобы IDE не подсвечивала Endless loop*/
        }
        werase(tmp->textLabel_win->workspace);
    }
}

/**
 * Функция выхода
 * @param funcName Название функции. Для справки
 * @param textError Текст ошибки. Для справки
 * @param user Указатель на структуру с конфигурацией пользователя
 */
void myExit(char *funcName, char *textError, userConfig *user)
{
    endwin();
    printf("%s: ERROR - %s\n", funcName, textError);

    /* При закрытии сигналом очереди не будут удалены */
    if (user != NULL) {
        mq_close(user->queueSendMsg);
        mq_close(user->queueReceiveMsg);
        mq_unlink(user->roomNameReceive);
    }

    exit(EXIT_FAILURE);
}

/**
 * Обработчик сигналов. Обрабатывает SIGWINCH и SIGILL
 * @param sig Идентификационный номер сигнала
 */
void sig_winch(int sig)
{
    myExit("none\0", "signal\0", NULL);
}
