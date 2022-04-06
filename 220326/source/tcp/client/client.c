#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define MAXLENSEND 64
#define MAXLENRECV 128

int main()
{
    int fd, status;
    struct sockaddr_in addressServer;
    ssize_t lenSend, lenRecv;

    fd = socket(AF_INET, SOCK_STREAM, 0);
    if (fd == -1) {
        perror("ERROR: socket (f: main)");
        exit(EXIT_FAILURE);
    }

    memset(&addressServer, 0x00, sizeof(struct sockaddr_in));
    addressServer.sin_family = AF_INET;
    addressServer.sin_port = htons(2304);

    status = inet_pton(AF_INET, "127.0.0.1", &addressServer.sin_addr);
    if (status == 0) {
        printf("ERROR: inet_pton - src doesn't contain a character string ...\n");
        exit(EXIT_FAILURE);
    } else if (status == -1) {
        perror("ERROR: inet_pton (f: main)");
        exit(EXIT_FAILURE);
    }

    status = connect(fd, (struct sockaddr *) &addressServer, sizeof(struct sockaddr_in));
    if (status == -1) {
        perror("ERROR: connect (f: main)");
        exit(EXIT_FAILURE);
    }

    char *msgToServer = (char *) malloc (MAXLENSEND *sizeof(char));
    char *msgFromServer = (char *) malloc (MAXLENRECV *sizeof(char));

    /* Отправка сообщений серверу */
    scanf("%64s", msgToServer);

    lenSend = write(fd, msgToServer, MAXLENSEND);
    if (lenSend == -1) {
        perror("ERROR: write (f: main)");
        exit(EXIT_FAILURE);
    } else if (lenSend == 0) {
        printf("empty\n");
    }

    /* Получение сообщений от сервера */
    lenRecv = read(fd, msgFromServer, MAXLENRECV);
    if (lenRecv == -1) {
        perror("ERROR: read (f: main)");
        exit(EXIT_FAILURE);
    } else if (lenRecv == 0) {
        printf("empty\n");
    }

    printf("%s\n", msgFromServer);

    free(msgToServer);
    free(msgFromServer);

    close(fd);
    exit(EXIT_SUCCESS);
}