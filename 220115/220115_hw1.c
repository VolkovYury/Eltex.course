#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <fcntl.h>
#include <unistd.h>

int main()
{
    const char *pathToFile = "./testFile.txt";
    const char *testString = "This line is written in the program 220115_hw1\n";
    int fd;
    if ((fd = open(pathToFile, O_RDWR | O_CREAT, 0666)) < 0) {
        printf("Something wrong! Failed to open file\n");
        exit(EXIT_FAILURE);
    }

    int size;
    if ((size = write(fd, testString, strlen(testString))) < 0) {
        printf("Something wrong! Failed to write data to file\n");
        close(fd);
        exit(EXIT_FAILURE);
    } else {
        printf("%d bytes were written\n", size);
    }

    close(fd);
    return 0;
}