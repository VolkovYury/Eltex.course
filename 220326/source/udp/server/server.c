#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <time.h>

#include <sys/socket.h>
#include <netinet/in.h>

#define MAXLENMSG 128

int main()
{
    int fd, status;
    struct sockaddr_in addressServer, addressClient;

    fd = socket(AF_INET, SOCK_DGRAM, 0);
    if (fd == -1) {
        perror("ERROR: socket (f: main)");
        exit(EXIT_FAILURE);
    }

    memset(&addressServer, 0x00, sizeof(struct sockaddr_in));
    addressServer.sin_family = AF_INET;
    addressServer.sin_port = htons(2304);

    status = bind(fd, (struct sockaddr *) &addressServer, sizeof(struct sockaddr_in));
    if (status == -1) {
        perror("ERROR: bind (f: main)");
        close(fd);
        exit(EXIT_FAILURE);
    }

    char *recvMsg = (char *) malloc (MAXLENMSG *sizeof(char));
    char *sendMsg = (char *) malloc (MAXLENMSG *sizeof(char));

    ssize_t recvLen, sendLen;

    /* Получение сообщения от клиента */
    socklen_t sizeAddressClient = sizeof(struct sockaddr_in);
    recvLen = recvfrom(fd, recvMsg, MAXLENMSG, 0, (struct sockaddr *) &addressClient, &sizeAddressClient);
    long curTime = time(NULL);
    if (recvLen == -1) {
        perror("ERROR: recvfrom (f: main)");
        close(fd);
        exit(EXIT_FAILURE);
    }
    printf("Received message: %s\n", recvMsg);

    /* Отправка сообщения клиенту */
    char time[24]; /* 24 символа чтобы "отрезать" последний символ '\n' от результата выполнения ctime() */
    strncpy(time, ctime(&curTime), 24);
    sprintf(sendMsg, "From server: message receive! (%s)", time);

    sendLen = sendto(fd, sendMsg, MAXLENMSG, 0, (struct sockaddr *) &addressClient, sizeAddressClient);
    if (sendLen == -1) {
        perror("ERROR: sendto (f: main)");
        close(fd);
        exit(EXIT_FAILURE);
    }

    sleep(1);
    free(sendMsg);
    free(recvMsg);

    close(fd);
    exit(EXIT_SUCCESS);
}