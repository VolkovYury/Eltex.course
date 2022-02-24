#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "parseFunctions.h"
#include "utilityFunctions.h"
#include "executionFunctions.h"

int main()
{

    /* Для автотеста (блок 1 из 3)
    FILE *mf;
    char str[50];
    char *estr;
    mf = fopen ("../tests.txt", "r");
    if (mf == NULL) {
        printf ("ERROR: fopen error - tests.txt (main)\n");
        exit(EXIT_FAILURE);
    }
     */

    while (1) {
        printPrefixString();

        /* Для автотеста (блок 2 из 3)
        estr = fgets (str,sizeof(str),mf);

        if (estr == NULL) {
            if (feof(mf) != 0) {
                printf ("\nЧтение файла закончено\n");
                break;
            } else {
                printf ("\nОшибка чтения из файла\n");
                break;
            }
        }
        */

        char *str = scanString();
        char **testArr = parseString(str);

        /* Упрощённая реализация выхода */
        if (strcmp(testArr[0], "exit") == 0) {
            printf("Exit...\n");
            free(str);
            free(testArr);
            break;
        }

        execFunction(testArr);
    }


    /* Для автотеста (блок 3 из 3)
    if (fclose (mf) == EOF)
        printf ("WARNING: fclose error - mf (main)\n");
    */

    exit(EXIT_SUCCESS);
}