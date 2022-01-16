#include <stdio.h>
#include <stdlib.h>

#include <fcntl.h>
#include <unistd.h>

int main()
{
    const char *pathToFile = "./testFile.txt";
    int fd;
    if ((fd = open(pathToFile, O_RDWR | O_CREAT, 0666)) < 0) {
        printf("Something wrong! Failed to open file\n");
        exit(EXIT_FAILURE);
    }

    int size, totalSize = 0;
    char buffer[80];

    while ((size = read(fd, buffer, 80)) > 0) {
        write(1, buffer, size);
        totalSize += size;
    }

    if (size == -1) {
        printf("Something wrong! Failed to read data from file\n");
        close(fd);
        exit(EXIT_FAILURE);
    }

    printf("%d bytes were read\n", totalSize);

    close(fd);
    return 0;
}