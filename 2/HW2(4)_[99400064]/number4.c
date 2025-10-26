#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

int main(int argc, char* argv[]) {
    if(argc<2){
        printf("No folder name provided.\n");
        exit(0);
    }

    char* folder_name = argv[1];
    char* text_name = "99400064.txt";
    char* text_content = "Doing OS homework.";
    char* mkdir_args[] = {"mkdir", folder_name, NULL};
    char filepath[100];
    snprintf(filepath, sizeof(filepath), "%s/%s", folder_name, text_name);
    char* touch_args[] = {"touch", filepath, NULL};

    int pid = fork();

    if (pid == 0) {
        execvp(mkdir_args[0], mkdir_args);
    } else if (pid > 0) {
        // int status;
        // wait(&status);
        waitpid(pid, NULL, 0);
        
        int touch_pid = fork();
        if (pid < 0)
            printf("Error in fork\n");
        else if (pid == 0){
            execvp(touch_args[0], touch_args);
            exit(0);
        }
        else{
            waitpid(touch_pid, NULL, 0);

            FILE* fp = fopen(filepath, "w");
            fputs(text_content, fp);
            fclose(fp);
        }

    } else {
        printf("Error: fork() failed.\n");
    }
    return 0;
}
