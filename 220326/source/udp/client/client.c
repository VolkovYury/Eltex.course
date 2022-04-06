#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <time.h>

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define MAXLENMSG 128

int main()
{
    int fd, status;
    struct sockaddr_in addressServer;

    fd = socket(AF_INET, SOCK_DGRAM, 0);
    if (fd == -1) {
        perror("ERROR: socket (f: main)");
        exit(EXIT_FAILURE);
    }

    addressServer.sin_family = AF_INET;
    addressServer.sin_port = htons(2304);
    status = inet_pton(AF_INET, "127.0.0.1", &addressServer.sin_addr);
    if (status == 0) {
        printf("ERROR: inet_pton - src doesn't contain a character string ...\n");
        close(fd);
        exit(EXIT_FAILURE);
    } else if (status == -1) {
        perror("ERROR: inet_pton (f: main)");
        close(fd);
        exit(EXIT_FAILURE);
    }

    ssize_t sendLen, recvLen;

    char message[] = "Hello server!\0";
    char *recvMsg = (char *) malloc (MAXLENMSG *sizeof(char));

    /* Отправка сообщения на сервер */
    socklen_t sizeAddressServer = sizeof(struct sockaddr_in);
    sendLen = sendto(fd, message, MAXLENMSG, 0, (struct sockaddr *) &addressServer, sizeAddressServer);
    if (sendLen == -1) {
        perror("ERROR: sendto (f: main)");
        close(fd);
        exit(EXIT_FAILURE);
    }

    /* Получение сообщения от сервера */
    memset(recvMsg, 0x00, MAXLENMSG);
    recvLen = recvfrom(fd, recvMsg, MAXLENMSG, 0, (struct sockaddr *) &addressServer, &sizeAddressServer);
    if (recvLen == -1) {
        perror("ERROR: recvfrom (f: main)");
        close(fd);
        exit(EXIT_FAILURE);
    }
    printf("%s\n", recvMsg);

    free(recvMsg);
    close(fd);
    exit(EXIT_SUCCESS);
}