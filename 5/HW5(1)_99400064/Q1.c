#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>
#include <sys/mman.h>
#include <fcntl.h>
#define BUFFER_SIZE 10
#define MAX_ITEMS 100

typedef struct {
    int buffer[BUFFER_SIZE];
    int in;
    int out;
    sem_t *mutex;
    sem_t *empty;
    sem_t *full;
} shared_buffer;

typedef struct {
    int id;
    shared_buffer *sb;
} thread_args;

void *producer(void *arg) {
    thread_args *targs = (thread_args *)arg;
    shared_buffer *sb = targs->sb;
    int item;

    for (int i = 0; i < MAX_ITEMS; i++) {
        item = rand() % 1000;
        sem_wait(sb->empty);
        sem_wait(sb->mutex);

        sb->buffer[sb->in] = item;
        sb->in = (sb->in + 1) % BUFFER_SIZE;
        printf("Producer %d produced item %d\n", targs->id, item);

        sem_post(sb->mutex);
        sem_post(sb->full);
        usleep(rand() % 1000);
    }
    pthread_exit(NULL);
}

void *consumer(void *arg) {
    thread_args *targs = (thread_args *)arg;
    shared_buffer *sb = targs->sb;
    int item;

    while (1) {
        sem_wait(sb->full);
        sem_wait(sb->mutex);

        item = sb->buffer[sb->out];
        sb->out = (sb->out + 1) % BUFFER_SIZE;

        printf("Consumer %d consumed item %d\n", targs->id, item);
        sem_post(sb->mutex);
        sem_post(sb->empty);

        usleep(rand() % 1000);
    }
    pthread_exit(NULL);
}

int main() {
    int shm_fd;
    shared_buffer *sb;

    shm_fd = shm_open("/buffer", O_CREAT | O_RDWR, 0666);
    ftruncate(shm_fd, sizeof(shared_buffer));
    sb = mmap(NULL, sizeof(shared_buffer), PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd, 0);
    sb->in = 0;
    sb->out = 0;
    sb->mutex = sem_open("/mutex", O_CREAT | O_EXCL, 0666, 1);
    sb->empty = sem_open("/empty", O_CREAT | O_EXCL, 0666, BUFFER_SIZE);
    sb->full = sem_open("/full", O_CREAT | O_EXCL, 0666, 0);

    pthread_t producers[5], consumers[5];
    thread_args producer_args[5], consumer_args[5];

    for (int i = 0; i < 5; i++) {
        producer_args[i].id = i + 1;
        producer_args[i].sb = sb;
        consumer_args[i].id = i + 1;
        consumer_args[i].sb = sb;

        pthread_create(&producers[i], NULL, producer, &producer_args[i]);
        pthread_create(&consumers[i], NULL, consumer, &consumer_args[i]);
    }

    for (int i = 0; i < 5; i++) {
        pthread_join(producers[i], NULL);
        pthread_join(consumers[i], NULL);
    }

    munmap(sb, sizeof(shared_buffer));
    close(shm_fd);

    return EXIT_SUCCESS;
}