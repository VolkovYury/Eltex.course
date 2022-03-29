#ifndef CLIENTFUNC_H
#define CLIENTFUNC_H

#include "myChat_MQ_client.h"

void setUsername(userConfig *user, windowConfig *win);
void setRoomName(userConfig *user, windowConfig *win);
void clientRegistration(userConfig *user);

void *receiveMsgs(void *arg);
void parseMessage(const char *buffer, char *time, char *nick, char *text);
void buildMsgSpace(const char *nick, const char *time, char *text, configuration *config, char **space_str);
void *sendMsgs(void *arg);

void myExit(char *funcName, char *textError, userConfig *user);
void sig_winch(int sig);

#endif
