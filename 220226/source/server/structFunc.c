#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "myChat_MQ_server.h"
#include "structFunc.h"

/* Функции для работы с односвязным списком */
/**
 * Инициализация односвязного списка
 * @return Указатель на односвязный список
 */
singlyLinkedList *createSinglyLinkedList()
{
    singlyLinkedList *tmp = (singlyLinkedList *) malloc (sizeof(singlyLinkedList));
    if (tmp == NULL) {
        printf("ERROR: malloc (f: createSinglyLinkedList)\n");
        exit(EXIT_FAILURE);
    }

    tmp->size = 0;
    tmp->raw = 0;

    int status;
    status = pthread_mutex_init(&(tmp->mtx), NULL);
    if (status != 0) {
        printf("ERROR: pthread_mutex_init - status (createSinglyLinkedList)\n");
        exit(EXIT_FAILURE);
    }
    status = pthread_cond_init(&(tmp->cnd), NULL);
    if (status != 0) {
        printf("ERROR: pthread_cond_init - status (createSinglyLinkedList)\n");
        exit(EXIT_FAILURE);
    }

    tmp->head = NULL;
    tmp->tail = NULL;

    return tmp;
}

/**
 * Добавление пользователя в начало списка
 * @param list Указатель на односвязный список
 * @param number Порядковый номер пользователя
 * @param time Время подключения пользователем
 * @param name Имя пользователя
 */
void addNewUser(singlyLinkedList *list, unsigned int number, unsigned int time, const char *name)
{
    users_t *tmp = (users_t *) malloc (sizeof(users_t));
    if (tmp == NULL) {
        printf("ERROR: malloc (f: addNewUser)\n");
        exit(EXIT_FAILURE);
    }

    tmp->num = number;
    tmp->connectTime = time;
    strcpy(tmp->nickname, name);

    tmp->roomnameReceive[0] = '/';
    strncat(tmp->roomnameReceive, tmp->nickname, 20);

    tmp->next = list->head;

    if (list->tail == NULL)
        list->tail = tmp;

    list->head = tmp;
    list->size++;
}

/**
 * Добавление пользователя в конец списка
 * @param list Указатель на односвязный список
 * @param number Порядковый номер пользователя
 * @param time Время подключения пользователем
 * @param name Имя пользователя
 */
void addNewUserBack(singlyLinkedList *list, unsigned int number, unsigned int time, const char *name)
{
    users_t *tmp = (users_t *) malloc (sizeof(users_t));
    if (tmp == NULL) {
        printf("ERROR: malloc (f: addNewUserBack)\n");
        exit(EXIT_FAILURE);
    }

    tmp->num = number;
    tmp->connectTime = time;
    strcpy(tmp->nickname, name);

    tmp->roomnameReceive[0] = '/';
    strncat(tmp->roomnameReceive, tmp->nickname, MAX_NICKNAME);

    tmp->next = NULL;

    if (list->head == NULL) {
        list->head = tmp;
        list->head_const = *tmp;
    } else {

        /* Если список не пуст и head_const не имеет ссылки на следующее сообщение, т.е. на второе */
        /* При отсутствии реализации удаления сообщений из структуры - этот if срабатывает только на втором сообщении */
        if (list->head_const.next == NULL)
            list->head_const.next = tmp;

    }

    if (list->tail)
        list->tail->next = tmp;

    list->tail = tmp;
    list->size++;
    list->raw++;
}

/**
 * Печать элементов односвязного списка
 * @param list Указатель на односвязный список
 */
void printSinglyLinkedList(singlyLinkedList *list)
{
    pthread_mutex_lock(&list->mtx);

    if (list->head == NULL) {
        printf("Singly linked list is empty!\n");
    } else {
        singlyLinkedList tmp = *list;

        if (tmp.head->num != tmp.head_const.num) {
            /* Указатель head не установлен на первый элемент */
            /* Поэтому сначала выводится реальный первый элемент, а затем устанавливается корректный указатель */
            printf("#%d: time - %d, nick - %s\n", tmp.head->num, tmp.head->connectTime, tmp.head->nickname);
            tmp.head = tmp.head_const.next;
        }

        while (tmp.head) {
            printf("#%d: time - %d, nick - %s\n", tmp.head->num, tmp.head->connectTime, tmp.head->nickname);
            tmp.head = tmp.head->next;
        }
    }

    pthread_mutex_unlock(&list->mtx);
}





/* Функции для работы с двусвязным списком */
/**
 * Инициализация двусвязного списка
 * @return Указатель на двусвязный список
 */
doublyLinkedList *createDoublyLinkedList()
{
    doublyLinkedList *tmp = (doublyLinkedList *) malloc (sizeof(doublyLinkedList));
    if (tmp == NULL) {
        printf("ERROR: malloc (f: createDoublyLinkedList)\n");
        exit(EXIT_FAILURE);
    }

    tmp->size = 0;
    tmp->head = NULL;
    tmp->tail = NULL;

    return tmp;
}

/**
 * Добавление сообщения в начало списка
 * @param list Указатель на двусвязный список
 * @param number Порядковый номер сообщения
 * @param time Время отправки(!) сообщения пользователем
 * @param name Имя пользователя
 * @param text Текст сообщения
 */
void addNewMessage(doublyLinkedList *list, unsigned int number, unsigned int time, const char *name, const char *text)
{
    messages_t *tmp = (messages_t *) malloc(sizeof(messages_t));
    if (tmp == NULL) {
        printf("ERROR: malloc (f: addNewMessage)\n");
        exit(EXIT_FAILURE);
    }

    tmp->num = number;
    tmp->time = time;
    strcpy(tmp->nickname, name);
    strcpy(tmp->text, text);
    tmp->next = list->head;
    tmp->prev = NULL;

    if (list->head)
        list->head->prev = tmp;

    if (list->tail == NULL)
        list->tail = tmp;

    list->head = tmp;
    list->size++;
}

/**
 * Добавление сообщения в конец списка
 * @param list Указатель на двусвязный список
 * @param number Порядковый номер сообщения
 * @param time Время отправки(!) сообщения пользователем
 * @param name Имя пользователя
 * @param text Текст сообщения
 */
void addNewMessageBack(doublyLinkedList *list, unsigned int number, unsigned int time, const char *name, const char *text)
{
    messages_t *tmp = (messages_t *) malloc(sizeof(messages_t));
    if (tmp == NULL) {
        printf("ERROR: malloc (f: addNewMessageBack)\n");
        exit(EXIT_FAILURE);
    }

    tmp->num = number;
    tmp->time = time;
    strcpy(tmp->nickname, name);
    strcpy(tmp->text, text);
    tmp->next = NULL;
    tmp->prev = list->tail;

    /* Если список пуст (отсутствует голова) */
    if (list->head == NULL) {
        list->head = tmp;
        list->head_const = *tmp;
    } else {

        /* Если список не пуст и head_const не имеет ссылки на следующее сообщение, т.е. на второе */
        /* При отсутствии реализации удаления сообщений из структуры - этот if срабатывает только на втором сообщении */
        if (list->head_const.next == NULL)
            list->head_const.next = tmp;

    }

    /* Изменить в старом хвосте указатель на новый хвост */
    if (list->tail)
        list->tail->next = tmp;

    list->tail = tmp;
    list->size++;
}

/**
 * Печать элементов двусвязного списка
 * @param list Указатель на двусвязный список
 */
void printDoublyLinkedList(doublyLinkedList *list)
{
    pthread_mutex_lock(&list->mtx);

    if (list->head == NULL) {
        printf("Doubly linked list is empty!\n");
    } else {

        doublyLinkedList tmp = *list;

        if (tmp.head->num != tmp.head_const.num) {
            /* Указатель head не установлен на первый элемент */
            /* Поэтому сначала выводится реальный первый элемент, а затем устанавливается корректный указатель */
            printf("#%d: time - %d, nick - %s, text - %s\n",
                    tmp.head_const.num, tmp.head_const.time, tmp.head_const.nickname, tmp.head_const.text);
            tmp.head = tmp.head_const.next;
        }

        while (tmp.head) {
            printf("#%d: time - %d, nick - %s, text - %s\n",
                    tmp.head->num, tmp.head->time, tmp.head->nickname, tmp.head->text);
            tmp.head = tmp.head->next;
        }
    }

    pthread_mutex_unlock(&list->mtx);
}
