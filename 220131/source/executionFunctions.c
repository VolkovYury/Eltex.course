#include <unistd.h>
#include <sys/wait.h>
#include <stdio.h>
#include <stdlib.h>
#include <bits/waitflags.h>

#include "executionFunctions.h"

/**
 * Функция для создания нового процесса и запуска на выполнение новой программы
 * @param args Двумерный массив аргументов, где args[0] - имя программы, которая будет выполняться
 */
void execFunction(char **args)
{
    pid_t cpid, w;
    int wstatus;

    cpid = fork();
    if (cpid < 0) {
        printf("ERROR: fork error - cpid (execFunction)\n");
        exit(EXIT_FAILURE);
    } else if (cpid == 0) {

        execvp(args[0], args);

        printf("ERROR: execvp error - cpid (execFunction)\n");
        exit(EXIT_FAILURE);

    } else {

        do {
            w = waitpid(cpid, &wstatus, WUNTRACED);
            if (w == -1) {
                perror("ERROR: waitpid - w (execFunction)\n");
                exit(EXIT_FAILURE);
            }

            /* Справочная информация. Пока без надобности
            if (WIFSIGNALED(wstatus)) {
                printf("killed by signal %d\n", WTERMSIG(wstatus));
            } else if (WIFSTOPPED(wstatus)) {
                printf("stopped by signal %d\n", WSTOPSIG(wstatus));
            }
            */

        } while (!WIFEXITED(wstatus) && !WIFSIGNALED(wstatus));
    }
}