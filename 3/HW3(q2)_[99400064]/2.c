#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

#define N 200

sem_t wheel_sem;
// pthread_mutex_t body_mutex[MAX_BICYCLES];

void *create_wheel(void *arg) {
    int *wheel_num = (int *)arg;

    // sleep(2);
    printf("Making Wheel %d\n", *wheel_num);
    // sleep(2);
    sem_post(&wheel_sem);

    return NULL;
}

void *create_bicycle(void *arg) {
    int *bike_num = (int *)arg;

    printf("Waiting for first wheel %d\n", *bike_num);
    sleep(2);

    sem_wait(&wheel_sem);
    printf("Attaching First Wheel For Bike %d\n", *bike_num);
    
    printf("Waiting for second wheel %d\n", *bike_num);
    sleep(2);

    sem_wait(&wheel_sem);
    printf("Attaching Second Wheel For Bike %d\n", *bike_num);

    return NULL;
}

int main() {
    int n;
    pthread_t wheel_array[2 * N], bike_array[N];
    int num_arr[2 * N];
    printf("please Enter the number of bicycles: ");
    scanf("%d", &n);

    sem_init(&wheel_sem, 0, 0);
    

    for (int i = 0; i < 2 * n; i++){
        num_arr[i] = i;
    }
    
    for (int i = 0; i < n; i++){
        pthread_create(&bike_array[i], NULL, create_bicycle, &num_arr[i]);
    }
    for (int i = 0; i < 2 * n; i++){
        pthread_create(&wheel_array[i], NULL, create_wheel, &num_arr[i]);
    }

    sleep(5);
    for (int i = 0; i < n; i++){
        pthread_join(&bike_array[i], NULL);
    }
    for (int i = 0; i < 2 * n; i++){
        pthread_join(&wheel_array[i], NULL);
    }
    sem_destroy(&wheel_sem);
    return 0;
}