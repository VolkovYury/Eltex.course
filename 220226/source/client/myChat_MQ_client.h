#ifndef MYCHAT_MQ_CLIENT_H
#define MYCHAT_MQ_CLIENT_H

#include <mqueue.h>
#include <panel.h>

#define MAX_ROOMNAME 20
#define MAX_NICKNAME 20

#define MAX_NUM_MSG 10  /* Может не создать очередь, если будет больше 10. Можно только уменьшать */
#define MAX_SIZE_MSG 512
#define MAX_SIZE_DATA 450   /* Примерно:  450 = MAX_SIZE_MSG - 2 разделителя - MAX_ROOMNAME - MAX_NICKNAME */

#define NUM_ROWS 45
#define NUM_COLS 140

typedef struct userConfiguration
{
    char nickname[MAX_NICKNAME];
    char roomNameReceive[MAX_NICKNAME+1];
    char roomNameSend[MAX_ROOMNAME+1];
    mqd_t queueSendMsg;
    mqd_t queueReceiveMsg;

    pthread_mutex_t mtx;
} userConfig;

typedef struct _windowConfiguration
{
    int size_y;
    int size_x;
    int start_y;
    int start_x;

    WINDOW *decoration;
    WINDOW *workspace;
    PANEL *panel;

    int panelHide;
} windowConfig;

typedef struct _configuration
{
    userConfig *user;
    windowConfig *chat_win;
    windowConfig *textLabel_win;
    windowConfig *users_win;
} configuration;

#endif
