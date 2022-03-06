#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>

int main(int argc, char *argv[])
{
    int fdToChild[2], fdFromChild[2], status;
    char fd1[11], fd2[11], receivedString[64];
    char *testString = "This line was passed through the pipe (prog1 -> prog2)\0";
    size_t send, receive;

    pid_t cpid;

    status = pipe(fdToChild);
    if (status == -1) {
        printf("ERROR: pipe - status (main)\n");
        exit(EXIT_FAILURE);
    }

    status = pipe(fdFromChild);
    if (status == -1) {
        printf("ERROR: pipe - status (main)\n");
        exit(EXIT_FAILURE);
    }

    sprintf(fd1, "%d", fdToChild[0]);
    sprintf(fd2, "%d", fdFromChild[1]);

    cpid = fork();
    switch (cpid) {
    case -1:
        printf("ERROR: fork (main)\n");
        exit(EXIT_FAILURE);
    case 0:
        /* child */
        status = execlp("./myPipeR", fd1, fd2, NULL);
        if (status == -1) {
            printf("ERROR: execpl - status (main)\n");
            exit(EXIT_FAILURE);
        }
        break;
    default:
        /* parent */
        close(fdToChild[0]);
        close(fdFromChild[1]);

        /* Отправить данные в один пайп */
        send = write(fdToChild[1], testString, strlen(testString));
        if (send !=strlen(testString)) {
            perror("ERROR: write - send (main - myPipeS)\n");
            exit(EXIT_FAILURE);
        }

        /* Прочитать данные из второго пайпа */
        receive = read(fdFromChild[0], receivedString, 64);
        if (receive == -1) {
            perror("ERROR: read - receive (main - myPipeS)\n");
            exit(EXIT_FAILURE);
        }
        printf ("%s\n", receivedString);

        close(fdToChild[1]);
        close(fdFromChild[0]);
    }

    exit(EXIT_SUCCESS);
}