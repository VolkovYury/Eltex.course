#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <time.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

#define MAXLENRECV 64   /* for read() */
#define MAXLENSEND 128  /* for write() */


void modificationFunc(char *result, const char *msgFromClient);

int main()
{
    int listenSocket, fd, status;
    struct sockaddr_in addressServer, addressClient;

    listenSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (listenSocket == -1) {
        perror("ERROR: socket (f: main)");
        exit(EXIT_FAILURE);
    }

    memset(&addressServer, 0x00, sizeof(struct sockaddr_in));
    addressServer.sin_family = AF_INET;
    addressServer.sin_port = htons(2304);

    memset(&addressClient, 0, sizeof(struct sockaddr_in));

    status = bind(listenSocket, (struct sockaddr *) &addressServer, sizeof(struct sockaddr_in));
    if (status == -1) {
        perror("ERROR: bind (f: main)");
        exit(EXIT_FAILURE);
    }

    status = listen(listenSocket, 10);
    if (status == -1) {
        perror("ERROR: listen (f: main)");
        exit(EXIT_FAILURE);
    }

    socklen_t sizeAddressClient = sizeof(struct sockaddr_in);
    fd = accept(listenSocket, (struct sockaddr *) &addressClient, &sizeAddressClient);
    if (fd == -1) {
        perror("ERROR: accept (f: main)");
        exit(EXIT_FAILURE);
    }

    ssize_t lenRecv, lenSend;
    char *msgFromClient = (char *) malloc (MAXLENRECV *sizeof(char));
    char *msgToClient = (char *) malloc (MAXLENSEND *sizeof(char));

    /* Получение сообщений от клиента */
    memset(msgFromClient, 0x00, MAXLENRECV);
    lenRecv = read(fd, msgFromClient, MAXLENRECV);
    if (lenRecv == -1) {
        perror("ERROR: read (f: main)");
        exit(EXIT_FAILURE);
    } else if (lenRecv == 0) {
        printf("empty\n");
    }
    printf("From client: %s\n", msgFromClient);


    /* Отправка сообщений клиенту */
    memset(msgToClient, 0x00, MAXLENSEND);

    modificationFunc(msgToClient, msgFromClient);
    lenSend = write(fd, msgToClient, MAXLENSEND);
    if (lenSend == -1) {
        perror("ERROR: write (f: main)");
        exit(EXIT_FAILURE);
    } else if (lenSend == 0) {
        printf("empty\n");
    }

    /* Клиент должен первым завершить работу (закрыть дескрипторы).
     * Иначе системе потребуется время, чтобы появилась возможность снова запустить сервер при данной конфигурации.
     * Это связано с тем, что сокет сервера какое-то время будет в состоянии TIME_WAIT*/
    sleep(1);

    free(msgFromClient);
    free(msgToClient);

    close(listenSocket);
    close(fd);

    exit(EXIT_SUCCESS);
}

void modificationFunc(char *result, const char *msgFromClient)
{
    long curTime = time(NULL);
    char time[24]; /* 24 символа чтобы "отрезать" символ '\n' от результата выполнения ctime() */
    strncpy(time, ctime(&curTime), 24);
    sprintf(result, "From server: %s (%s)", msgFromClient, time);
}