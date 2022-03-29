#include <stdlib.h>
#include <unistd.h>
#include <ncurses.h>
#include <string.h>

#include "myChat_MQ_client.h"
#include "interfaceFunctions.h"
#include "clientFunc.h"

/**
 * Функция прорисовки интерфейса регистрации пользователя
 * @param user Указатель на структуру с конфигурацией пользователя
 */
void interface_clientRegistration(userConfig *user)
{
    windowConfig *registration;
    registration = createNewWindow(21, 80, (NUM_ROWS-21)/2, (NUM_COLS-80)/2, "Registration in MQ-chat\0");

    setUsername(user, registration);
    setRoomName(user, registration);
    clientRegistration(user);

    mvwprintw(registration->workspace, 16, 1, "                                                            ");
    mvwprintw(registration->workspace, 16, 1, "LOG: Registration successful!");
    wrefresh(registration->workspace);
    sleep(2);

    delwin(registration->decoration);
}

/**
 * Функция создающая рабочее окно с несколькими слоями
 * @param height Высота окна в количестве строк
 * @param width Ширина окна в количестве строк
 * @param start_y Начало построения окна. Координата y
 * @param start_x Начало построения окна. Координата x
 * @param label Название создаваемого окна
 * @return Указатель на структуру с конфигурацией созданного окна
 */
windowConfig *createNewWindow(int height, int width, int start_y, int start_x, char *label)
{
    windowConfig *local_win = (windowConfig *) malloc (sizeof(windowConfig));

    local_win->size_y = height;
    local_win->size_x = width;
    local_win->start_y = start_y;
    local_win->start_x = start_x;

    local_win->decoration = newwin(local_win->size_y, local_win->size_x, local_win->start_y, local_win->start_x);

    wbkgd(local_win->decoration, COLOR_PAIR(1));

    wborder(local_win->decoration, ACS_VLINE, ACS_VLINE, ACS_HLINE, ACS_HLINE, ACS_ULCORNER, ACS_URCORNER, ACS_LLCORNER, ACS_LRCORNER);
    mvwprintw(local_win->decoration, 1, (local_win->size_x - (int) strlen(label) - 2)/2, "%s", label);
    mvwhline(local_win->decoration, 2, 1, ACS_HLINE, local_win->size_x - 2);
    wrefresh(local_win->decoration);

    local_win->workspace = derwin(local_win->decoration, local_win->size_y - 4, local_win->size_x - 2, 3, 1);
    wbkgd(local_win->workspace, COLOR_PAIR(1));
    wrefresh(local_win->workspace);

    return local_win;
}
