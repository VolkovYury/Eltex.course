#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "parseFunctions.h"

#define STRLEN 64       /* Первоначальный ориентировочный размер строки */
#define NUMARGS 10      /* Первоначальное ориентировочное количество аргументов */

/**
 * Функция считывает посимвольно строку введённую пользователем
 * @return Возвращает строку в виде массива символов
 */
char *scanString()
{
    char *string = (char *) malloc (STRLEN * sizeof(char));
    if (string == NULL) {
        printf("ERROR: malloc error - *string (scanString)\n");
        exit(EXIT_FAILURE);
    }

    unsigned int realLen = STRLEN;
    unsigned int strPtr = 0;

    while (1) {
        char c = fgetc(stdin);

        /* Если размер строки превышает (!) (strPtr - индекс, поэтому ">=") размер ранее выделенной памяти */
        if (strPtr >= realLen) {
            realLen += STRLEN;
            string = realloc(string, realLen);

            if (string == NULL) {
                printf("ERROR: reallocate error - *string (scanString)\n");
                exit(EXIT_FAILURE);
            }
        }

        if (c == '\n' || c == EOF) {
            string[strPtr] = '\0';
            return string;
        } else {
            string[strPtr++] = c;
        }
    }
}

/**
 * Функция разбирает строку на массив аргументов
 * @param string Строка, подлежащая разбору на составляющие
 * @return Массив строк-аргументов
 */
char **parseString(char *string)
{
    unsigned int strPtr = 0;
    unsigned int strPtrPairQuote = 0;

    /* Ориентировочное выделение памяти под указатели */
    /* Временная заглушка. Есть потенциал для улучшения строк 61-67 */
    /* Сейчас выделяется место либо для максимального количества аргументов, либо для 10 */
    unsigned int maxNumArgs = (strlen(string) + 1)/2;
    char **args = NULL;
    if (maxNumArgs < NUMARGS) {
        args = (char **) malloc(maxNumArgs * sizeof(char *));
    } else {
        args = (char **) malloc(NUMARGS * sizeof(char *));
    }
    if (args == NULL) {
        printf("ERROR: malloc error - **args (parseString)\n");
        exit(EXIT_FAILURE);
    }
    unsigned int argsIndex = 0;

    /* Буфер для аргумента */
    char *arg = (char *) malloc (strlen(string) * sizeof (char));
    unsigned int argPtr = 0;

    /* Флаг "предыдущий символ был разделителем" */
    unsigned char prevDelim = 1;

    for (;string[strPtr] != '\0'; strPtr++) {

        switch (string[strPtr]) {
        case ' ':
        case '\t':
            /* Если встречается разделитель, то считанная информация записывается в массив */
            addArg(&prevDelim, arg, &argPtr, args, &argsIndex);
            prevDelim = 1;
            break;
        case '\"':
        case '\'':
            /* Обработка служебных символов - кавычек */
            if (!(strPtrPairQuote = findPairQuotes(string, &strPtr))) {
                printf("Try again:\n");

                args = NULL;
                free(arg);
                return args;
            }

            strPtr++;
            for (;strPtr < strPtrPairQuote; strPtr++) {
                arg[argPtr++] = string[strPtr];
            }
            arg[argPtr] = '\0';

            prevDelim = 0;

            if (string[strPtr+1] == '\0') {
                addArg(&prevDelim, arg, &argPtr, args, &argsIndex);
            }

            break;
        default:
            arg[argPtr++] = string[strPtr];
            prevDelim = 0;

            if (string[strPtr+1] == '\0') {
                addArg(&prevDelim, arg, &argPtr, args, &argsIndex);
            }
        }
    }
    args[argsIndex] = NULL;

    free(arg);
    return args;
}

/**
 * Функция добавления аргумента в массив аргументов
 * @param flag Флаг "предыдущий символ был разделителем"
 * @param arg Аргумент, найденный в исходной строке
 * @param argPtr Указатель на символ строки arg
 * @param args Массив аргументов, в который будет добавлен найденный аргумент
 * @param argsIndex Индекс массива аргументов
 */
void addArg(unsigned const char *flag, char *arg, unsigned int *argPtr, char **args, unsigned int *argsIndex)
{
    if (!*flag) {
        arg[*argPtr] = '\0';

        args[*argsIndex] = (char *) malloc((*argPtr + 1) * sizeof(char));
        if (args[*argsIndex] == NULL) {
            printf("ERROR: malloc error - args[*argsIndex] (addArg)\n");
            exit(EXIT_FAILURE);
        }

        strcpy(args[*argsIndex], arg);
        (*argsIndex)++;
        *argPtr = 0;
    }
}

/**
 * Функция поиска пары служебному символу
 * @param string Исходная строка
 * @param strPtr Указатель на первый символ
 * @return Указатель на пару служебному символу. Если пара не найдена - возвращается 0. Если служебные символы
 * не отделены от других аргументов разделителями - возвращается 0.
 */
unsigned int findPairQuotes(char *string, unsigned const int *strPtr)
{
    int i = *strPtr;

    do {
        i++;
        if (string[i] == '\0') {
            printf("Quote pair not found\n");
            return 0;
        }

        if (string[i] == string[*strPtr]) {
            break;
        }
    } while (string[i] != '\0');

    if (*strPtr > 0 && !(string[*strPtr - 1] == ' ' || string[*strPtr - 1] == '\t')) {
        printf("WARNING: Left quote is not separated by a space on the left side\n");
        return 0;
    }

    if (!(string[i + 1] == ' ' || string[i + 1] == '\t' || string[i + 1] == '\0' || string[i + 1] == '\n')) {
        printf("WARNING: Right quote is not separated by a space on the right side\n");
        return 0;
    }

    return i;
}