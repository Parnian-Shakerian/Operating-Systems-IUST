#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>

#define QUEUE_SIZE 100
#define NUM_CALCULATORS 5

typedef struct {
    double input1;
    double input2;
    char operation;
    int num_operations;
} Task;

Task task_queue[QUEUE_SIZE];
int queue_head = 0; // index of the head of the queue
int queue_tail = 0; // index of the tail of the queue
pthread_mutex_t queue_mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t queue_not_empty = PTHREAD_COND_INITIALIZER; // signal when queue is not empty

void enqueue_task(Task task) {
    pthread_mutex_lock(&queue_mutex);
    while ((queue_tail + 1) % QUEUE_SIZE == queue_head) { // queue is full
        pthread_cond_wait(&queue_not_empty, &queue_mutex);
    }
    task_queue[queue_tail] = task;
    queue_tail = (queue_tail + 1) % QUEUE_SIZE;
    pthread_cond_signal(&queue_not_empty);
    pthread_mutex_unlock(&queue_mutex);
}

Task dequeue_task() {
    pthread_mutex_lock(&queue_mutex);
    while (queue_head == queue_tail) { // queue is empty
        pthread_cond_wait(&queue_not_empty, &queue_mutex);
    }
    Task task = task_queue[queue_head];
    queue_head = (queue_head + 1) % QUEUE_SIZE;
    pthread_cond_signal(&queue_not_empty);
    pthread_mutex_unlock(&queue_mutex);
    return task;
}

Task calculate(Task task) {
    double result;
    switch (task.operation) {
        case '+':
            result = task.input1 + task.input2;
            break;
        case '-':
            result = task.input1 - task.input2;
            break;
        case '*':
            result = task.input1 * task.input2;
            break;
        case '/':
            result = task.input1 / task.input2;
            break;
        default:
            fprintf(stderr, "Invalid operation: %c\n", task.operation);
            exit(1);
    }
    task.num_operations++;
    if (task.operation == '+' || task.operation == '-') {
        if (task.num_operations % 2 == 0) {
            Task new_task = {result, task.input2, task.operation, task.num_operations};
            enqueue_task(new_task);
        }
    } else if (task.operation == '*' || task.operation == '/') {
        if (task.num_operations % 2 == 1) {
            Task new_task = {task.input1, result, task.operation, task.num_operations};
            enqueue_task(new_task);
        }
    }
    task.input1 = result;
    task.input2 = 0;
    return task;
}

void* calculator_thread(void* arg) {
    Task task;
    long id = (long) arg;
    while (1) {
        task = dequeue_task();
        task = calculate(task);
        printf("Calculator %ld: %g %c %g = %g, num_operations=%d\n",
               id, task.input1, task.operation, task.input2, task.input1, task.num_operations);
    }
    pthread_exit(NULL);
}

int main() {
    pthread_t calculator_threads[NUM_CALCULATORS];
    int num_arr[NUM_CALCULATORS];
    for (int i = 0; i < NUM_CALCULATORS; i++){
        num_arr[i] = i;
    }
    
    for (int i = 0; i < NUM_CALCULATORS; i++) {
        pthread_create(&calculator_threads[i], NULL, calculator_thread, &num_arr[i]);
    }
    while (1) {
        Task task;
        printf("please Enter the task (input1 operation input2): ");
        int result = scanf("%lf %c %lf", &task.input1, &task.operation, &task.input2);
       
        if (result != 3) {
            fprintf(stderr, "Invalid input. Please try again.\n");
            continue;
        }
        enqueue_task(task);  
    }

 return 0;
}