#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "utilityFunctions.h"

/**
 * Функция для печати префикса строки ввода в формате "логин"а"имя_машины":"путь"$
 */
void printPrefixString()
{
    char *userName = getenv("USER");
    if (userName == NULL) {
        printf("ERROR: username not found - *user (printPrefixString)\n");
        exit(EXIT_FAILURE);
    }

    char *sessionManager = getenv("SESSION_MANAGER");
    if (sessionManager == NULL) {
        printf("ERROR: sessionManager not found - *sessionManager (printPrefixString)\n");
        exit(EXIT_FAILURE);
    }
    char *ptrLocal = strstr(sessionManager, "local/");
    if (ptrLocal == NULL) {
        printf("ERROR: local not found - *ptrLocal (printPrefixString)\n");
        exit(EXIT_FAILURE);
    }
    char *localName = (char *) malloc(strlen(ptrLocal) *sizeof(char));
    if (localName == NULL) {
        printf("ERROR: malloc error - *localName (printPrefixString)\n");
        exit(EXIT_FAILURE);
    }
    int i = 6; /* strlen("local/") */
    int k = 0;
    for ( ; !(ptrLocal[i] == ':' || ptrLocal[i] == '\0'); i++, k++) {
        localName[k] = ptrLocal[i];
    }
    localName[k] = '\0';

    char *homeDir = getenv("HOME");
    if (homeDir == NULL) {
        printf("ERROR: home not found - *homeDir (printPrefixString)\n");
        exit(EXIT_FAILURE);
    }
    char *currentDir = getenv("PWD");
    if (currentDir == NULL) {
        printf("ERROR: pwd not found - *currentDir (printPrefixString)\n");
        exit(EXIT_FAILURE);
    }
    char *checkDir = strstr(currentDir, homeDir);
    if (checkDir == NULL) {
        printf("%s@%s:%s$ ", userName, localName, currentDir);
    } else {
        int dirPtr = strlen(homeDir);
        char *printDir = malloc(strlen(currentDir) *sizeof(char));
        if (printDir == NULL) {
            printf("ERROR: malloc error - *printDir (printPrefixString)\n");
            exit(EXIT_FAILURE);
        }
        int j = 1;
        printDir[0] = '~';
        for (;!(currentDir[j] == '\n' || currentDir[j] == '\0'); j++, dirPtr++) {
            printDir[j] = currentDir[dirPtr];
        }
        printDir[dirPtr] = '\0';

        printf("%s@%s:%s$ ", userName, localName, printDir);
        free(printDir);
    }

    free(localName);
}
