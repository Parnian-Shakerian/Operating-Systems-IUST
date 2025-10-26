#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#define NUM_LINES 100
#define NUM_THREADS 4
#define LIMIT 100000

struct custom_data{
    int thread_num;
    double thread_avg;
};

void* generate_random_files(void* arg) {
    int thread_num = *((int*) arg);
    char filename[20];
    sprintf(filename, "file_%d.txt", thread_num);
    FILE* fp = fopen(filename, "w");

    for (int i = 0; i < NUM_LINES; i++) {
        int random_number = (rand() % LIMIT) + 1;
        
        char num_str[10];
        sprintf(num_str, "%d\n", random_number);

        fputs(num_str, fp);
    }
    
    fclose(fp);
    pthread_exit(NULL);
}

void* calculate_average(void* arg) {
    struct custom_data* inp = (struct custom_data*) arg;
    int thread_num = inp->thread_num;
    int sum = 0;
    char filename[20];
    sprintf(filename, "file_%d.txt", thread_num);
    FILE* fp = fopen(filename, "r");

    if (fp == NULL) {
        printf("error in thread num %d\n", thread_num);
        pthread_exit(NULL);
    }
    for (int i = 0; i < NUM_LINES; i++) {
        int num;
        fscanf(fp, "%d", &num);
        sum += num;
    }

    fclose(fp);
    inp->thread_avg = (double) sum / NUM_LINES;
    pthread_exit(NULL);
}

int main() {
    pthread_t gen_threads[NUM_THREADS];
    pthread_t calc_threads[NUM_THREADS];
    int thread_args[NUM_THREADS];
    struct custom_data calc_thread_args[NUM_THREADS];
    srand(time(NULL));

    // starting to generate.
    for (int i = 0; i < NUM_THREADS; i++) {
        thread_args[i] = i;
        if (pthread_create(&gen_threads[i], NULL, generate_random_files, &thread_args[i])) {
            printf("Error: failed to create thread %d.\n", i);
            return 1;
        }
    }
    // wait for generating to finish.
    for (int i = 0; i < NUM_THREADS; i++) {
        pthread_join(gen_threads[i], NULL);
    }
    for (int i = 0; i < NUM_THREADS; i++) {
        struct custom_data s = {i, 0};
        calc_thread_args[i] = s;

        if (pthread_create(&calc_threads[i], NULL, calculate_average, &calc_thread_args[i])) {
            fprintf(stderr, "Error: failed to create thread %d.\n", i);
            return 1;
        }
    }
    // wait for calculating to finish.
    double ans = 0;

    for (int i = 0; i < NUM_THREADS; i++) {
        pthread_join(calc_threads[i], NULL);
        ans += calc_thread_args[i].thread_avg;
    }

    ans = ((double)ans / (double)NUM_THREADS);
    printf("avg = %.4f \n", ans);

    return 0;
}