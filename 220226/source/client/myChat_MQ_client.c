#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <signal.h>
#include <sys/ioctl.h>
#include <string.h>

#include "myChat_MQ_client.h"
#include "interfaceFunctions.h"
#include "clientFunc.h"

int main()
{
    configuration *config = (configuration *) malloc(sizeof(configuration));
    if (config == NULL) {
        printf("ERROR: malloc - config (main)\n");
        exit(EXIT_FAILURE);
    }

    userConfig *user = (userConfig *) malloc(sizeof(userConfig));
    if (user == NULL) {
        printf("ERROR: malloc - user (main)\n");
        exit(EXIT_FAILURE);
    }
    config->user = user;

    /* Не удалось настроить перерисовку интерфейса при изменении размера терминала в Konsole от Manjaro
     * При несоответствии размера терминала указанным параметрам NUM_ROWS и NUM_COLS чат закроется.
     * При самостоятельной настройке данных параметров работоспособность не гарантируется.
     * Все расчёты завязаны на этих константах - 45 строк, 140 столбцов. */
    struct winsize size;
    ioctl(fileno(stdout), TIOCGWINSZ, (char *) &size);
    if (size.ws_row != NUM_ROWS && size.ws_row != NUM_COLS){
        printf("Invalid terminal size!\n");
        exit(EXIT_FAILURE);
    }

    /* При изменении размера терминала вызывается обработчик, закрывающий чат.
     * При таком подходе хватило бы и signal().
     * Если в дальнейшем получится настроить корректную обработку размеров, то использовать надо signalaction(). */
    sigset_t set;
    sigemptyset(&set);
    sigaddset(&set, SIGWINCH);
    sigaddset(&set, SIGINT);
    struct sigaction act;
    memset(&act, 0, sizeof(act));
    act.sa_handler = sig_winch;
    act.sa_mask = set;

    if (sigaction(SIGWINCH, &act, 0) < 0) {
        printf("ERROR: sigaction (main)\n");
        exit(EXIT_FAILURE);
    }


    /* Начало ncurses */
    initscr();
    cbreak();
    keypad(stdscr, TRUE);
    start_color();
    use_default_colors();

    init_pair(1, COLOR_WHITE, COLOR_BLUE);

    interface_clientRegistration(user);

    refresh();
    windowConfig *main_win = createNewWindow(NUM_ROWS, NUM_COLS, 0, 0, "Chat. Message queue\0");

    /* height: NUM_ROWS - (3 + 1 + 8) ==== высота окна равна высоте чата после вычета 3 рамок, 1 строки Label и 8 строк окна "ввод текста"
     * width: NUM_COLS - (2 + MAXNICKNAME + 2) ==== ширина равна количеству колонок после вычета 4 рамок и максимальной длины никнейма
     * y: 3 ==== 2 рамки + 1 строка label
     * x: 1 ==== 1 рамка слева
     * */
    char chatLabel[40];
    sprintf(chatLabel, "Room name - %s", config->user->roomNameSend);
    windowConfig *chat = createNewWindow(NUM_ROWS - (3 + 1 + 8), NUM_COLS - (2 + MAX_NICKNAME + 2), 3, 1, chatLabel);

    /* height: 8 строк
     * width: NUM_COLS - (2 + MAXNICKNAME + 2) ==== ширина равна количеству колонок после вычета 4 рамок и максимальной длины никнейма
     * y: NUM_ROWS - (1 + 8) ==== количество строк после вычета 1 рамки и 8 строк окна "ввод текста"
     * x: 1 ==== 1 рамка слева
     */
    windowConfig *textLabel = createNewWindow(8, NUM_COLS - (2 + MAX_NICKNAME + 2), NUM_ROWS - (1 + 8), 1, "Enter message:");

    /* height: NUM_ROWS - (3 + 1) ==== высота окна равна высоте чата после вычета 3 рамок и 1 строки Label
     * width: MAXNICKNAME + 2 ==== ширина окна равна максимальной длины никнейма + 2 рамки
     * y: 3 ==== 2 рамки + 1 строка label
     * x: NUM_COLS-(MAXNICKNAME + 2 + 1) ==== количество колонок после вычета 3 рамок и максимальной длины никнейма
     */
    windowConfig *users = createNewWindow(NUM_ROWS - (3 + 1), MAX_NICKNAME + 2, 3, NUM_COLS-(MAX_NICKNAME + 2 + 1), "Users:");

    config->chat_win = chat;
    config->textLabel_win = textLabel;
    config->users_win = users;


    /* Поток 1. Прослушивание сообщений в очереди */
    int status;
    pthread_t t1_check_msgs;
    status = pthread_create(&t1_check_msgs, NULL, receiveMsgs, config);
    if (status != 0)
        myExit("main\0", "pthread_create - receiveMsg\0", user);

    /* Поток 2. Отправка сообщений на сервер */
    pthread_t t2_send_msgs;
    status = pthread_create(&t2_send_msgs, NULL, sendMsgs, config);
    if (status != 0)
        myExit("main\0", "pthread_create - sendMsg\0", user);


    status= pthread_join(t1_check_msgs, NULL);
    status= pthread_join(t1_check_msgs, NULL);

    getch();
    endwin();

    exit(EXIT_SUCCESS);
}