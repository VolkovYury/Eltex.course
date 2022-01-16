#include <stdio.h>
#include <stdlib.h>

#include <fcntl.h>
#include <unistd.h>

#define BUFSIZE 20

int main()
{
    const char *pathToFile = "./testFile2.txt";
    int fd;
    if ((fd = open(pathToFile, O_RDWR | O_CREAT, 0666)) < 0) {
        printf("Something wrong! Failed to open file\n");
        exit(EXIT_FAILURE);
    }

    int size;
    char buffer[BUFSIZE];
    off_t offset = lseek(fd, 0, SEEK_END);
    int i = 1;
    for (; offset != -1; i++) {
        /* Условие, если каретка находится около начала файла. Обрабатывает последний фрагмент */
        if (offset < BUFSIZE) {
            lseek(fd, 0, SEEK_SET);
            if ((size = read(fd, buffer, offset)) < 0) {
                printf("Something wrong! Failed to read data from file\n");
                close(fd);
                exit(EXIT_FAILURE);
            }

            write(1, buffer, offset);
            printf("\n");
            break;
        }

        /* В первый раз каретка находится в нужном месте. Затем необходимо перемещать на 2 размера буфера */
        if (i != 1)
            offset = lseek(fd, -BUFSIZE*2, SEEK_CUR);
        else
            offset = lseek(fd, -BUFSIZE, SEEK_CUR);

        if ((size = read(fd, buffer, BUFSIZE)) < 0) {
            printf("Something wrong! Failed to read data from file\n");
            close(fd);
            exit(EXIT_FAILURE);
        }

        write(1, buffer, size);
        printf("\n");
    }

    return 0;
}