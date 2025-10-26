#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>

#define MAX_BUF_SIZE 1024
#define PIPE_NAME "chat_pipe"

int main(int argc, char *argv[]) {
    int fd;
    char username[100];
    char input[MAX_BUF_SIZE];
    char response[MAX_BUF_SIZE];

    if (argc != 2) {
        printf("Usage: %s <username>\n", argv[0]);
        return 1;
    }

    strcpy(username, argv[1]);
    mkfifo(PIPE_NAME, 0666);

    pid_t pid = fork(); // Create a child process
    if (pid < 0) {
        fprintf(stderr, "Fork failed\n");
        return 1;
    }

    if (pid > 0) {
        // Parent process
        fd = open(PIPE_NAME, O_WRONLY);

        while (1) {
            printf("[%s] Enter a message: ", username);
            fgets(input, sizeof(input), stdin);
            write(fd, input, strlen(input) + 1);

            if (strcmp(input, "exit\n") == 0)
                break;

            read(fd, response, sizeof(response));
            printf("[Server] Response: %s", response);
        }

        close(fd);
        unlink(PIPE_NAME);
    } else {
        fd = open(PIPE_NAME, O_RDONLY);

        while (1) {
            read(fd, response, sizeof(response));
            printf("[%s] Received message: %s", username, response);

            if (strcmp(response, "exit\n") == 0)
                break;

            printf("[%s] Enter a response: ", username);
            fgets(input, sizeof(input), stdin);
            write(fd, input, strlen(input) + 1);
        }
        // Close the named pipe
        close(fd);
    }
    return 0;
}