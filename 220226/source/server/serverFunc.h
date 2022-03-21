#ifndef SERVERFUNC_H
#define SERVERFUNC_H

void usageError(const char *program);
mqd_t createMQ(char room[], int flags, unsigned int maxmsg, unsigned int maxsizemsg);

void *checkUsers(void *arg);
void *checkMessages(void *arg);
void *receiver(void *arg);

void parseMessage(const char *buffer,char *time, char *nick, char *text);

#endif
