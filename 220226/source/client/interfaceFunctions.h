#ifndef INTERFACEFUNCTIONS_H
#define INTERFACEFUNCTIONS_H

#include <ncurses.h>
#include <panel.h>

#include "myChat_MQ_client.h"

void interface_clientRegistration(userConfig *user);
windowConfig *createNewWindow(int height, int width, int start_y, int start_x, char *label);

#endif
