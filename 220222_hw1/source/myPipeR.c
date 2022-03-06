#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

int main(int argc, char *argv[])
{
    int fdFromParent = atoi(argv[0]);
    int fdToParent = atoi(argv[1]);
    char *testString = "This line was passed through the pipe (prog2 -> prog1)\0";
    char receivedString[64];
    size_t send, receive;

    /* Прочитать данные из первого пайпа */
    receive = read(fdFromParent, receivedString, 64);
    if (receive == -1) {
        perror("ERROR: read - receive (main - myPipeR)\n");
        exit(EXIT_FAILURE);
    }
    printf ("%s\n", receivedString);

    /* Записать данные во второй пайп */
    send = write(fdToParent, testString, strlen(testString));
    if (send !=strlen(testString)) {
        perror("ERROR: write - send (main - myPipeR)\n");
        exit(EXIT_FAILURE);
    }

    close(fdFromParent);
    close(fdToParent);

    exit(EXIT_SUCCESS);
}