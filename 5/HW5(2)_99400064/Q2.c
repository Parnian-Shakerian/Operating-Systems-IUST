#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main() {
    int fd[2];
    pid_t pid1, pid2;

    if (pipe(fd) == -1) {
        perror("pipe");
        exit(EXIT_FAILURE);
    }

    pid1 = fork();

    if (pid1 == -1) {
        perror("fork");
        exit(EXIT_FAILURE);
    } else if (pid1 == 0) {
        // First child process
        pid2 = fork();

        if (pid2 == -1) {
            perror("fork");
            exit(EXIT_FAILURE);
        } else if (pid2 == 0) {
            // Second child process
            close(fd[1]);

            char buf[100];
            read(fd[0], buf, sizeof(buf));
            printf("%s\n", buf);

            close(fd[0]);
        } else {
            // First child process
            close(fd[0]);

            char buf[100];
            sprintf(buf, "My process id is %d, my parent's id is %d, and my sibling's id is %d", getpid(), getppid(), pid2);
            write(fd[1], buf, sizeof(buf));
            close(fd[1]);
        }
    } else {
        // Parent process
        wait(NULL);
    }
    return EXIT_SUCCESS;
}
