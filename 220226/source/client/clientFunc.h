#ifndef CLIENTFUNC_H
#define CLIENTFUNC_H

void registerClient(userConf *user);
void setNickname(userConf *user);
void createQueueReceiveMsg(userConf *user);
void setRoomName(userConf *user);
void openQueueSendMsg(userConf *user);
void sendNickname(userConf *user);

void *receiveMsgs(void *arg);
void parseMessage(const char *buffer, char *time, char *nick, char *text);
void *sendMsgs(void *arg);


#endif
