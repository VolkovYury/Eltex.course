#ifndef MYCHAT_MQ_SERVER_H
#define MYCHAT_MQ_SERVER_H

#include <stddef.h>
#include <pthread.h>

#define MAX_ROOMNAME 20
#define MAX_NICKNAME 20

#define MAX_NUM_MSG 20
#define MAX_SIZE_MSG 1024

#define MAX_USERS 20

/* Структура для пользователя */
typedef struct users_s
{
    unsigned int num;
    unsigned int connectTime;
    char nickname[MAX_NICKNAME];
    char roomnameReceive[MAX_ROOMNAME+1];
    struct users_s *next;
} users_t;

/* Односвязный список с пользователями */
typedef struct _singlyLinkedList
{
    size_t size;
    unsigned int raw;

    pthread_mutex_t  mtx;
    pthread_cond_t cnd;

    users_t *head;
    users_t *tail;
} singlyLinkedList;

/* Структура для письма */
typedef struct messages_s
{
    unsigned int num;
    unsigned int time;
    char nickname[MAX_NICKNAME];
    char text[900];
    struct messages_s *next;
    struct messages_s *prev;
} messages_t;

/* Двусвязный список с сообщениями */
typedef struct _doublyLinkedList {
    size_t size;
    messages_t *head;
    messages_t *tail;
} doublyLinkedList;

typedef struct _database {
    users_t *user;
    doublyLinkedList *messages;
} database;

#endif
