#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main()
{
    pid_t cpid = fork();
    int wstatus;

    if (cpid < 0) {
        printf("WARNING: Something wrong! Fork error\n");
        exit(EXIT_FAILURE);
    } else if (cpid == 0) {
        /* child process */
        printf("In child process:\nPID: %d\nPPID: %d\nExit child process\n\n", getpid(), getppid());
    } else {
        /* parent process */
        if (waitpid(cpid, &wstatus, 0) < 0) {
            printf("WARNING: Something wrong! Waitpid error\n");
            exit(EXIT_FAILURE);
        }

        if (WIFEXITED(wstatus)) {
            printf("Child process terminated normally\n");
        } else {
            printf("WARNING: Something wrong with child process!\n");
        }

        printf("In parent process:\nPID: %d\nPPID: %d\nExit parent process\n", getpid(), getppid());
    }

    exit(EXIT_SUCCESS);
}