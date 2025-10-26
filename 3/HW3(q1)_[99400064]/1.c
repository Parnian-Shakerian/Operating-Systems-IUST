#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

#define N 1000

typedef struct {
    int m;
    int transactions[N];
    int index;
} transaction_list_type;

long long balance = 0;
sem_t mutex;

void *do_transactions(void *arg) {
    transaction_list_type *person_i = (transaction_list_type *)arg;
    for (int i = 0; i < person_i->m; i++){
        //wait
        sem_wait(&mutex);
        printf("Applying transaction %d of person %d : %d\n", i, person_i->index, person_i->transactions[i]);

        //critical section
        balance += (person_i->transactions[i]);
        printf("Current balance is : %ld\n----\n", balance);
        sleep(2);

        //signal
        sem_post(&mutex);
    }
    pthread_exit(NULL);
}


int main()
{
    int n, m;
    printf("Please enter n and m in the next line: ");
    scanf("%d %d", &n, &m);

    transaction_list_type transaction_list[N];
    pthread_t threads[N];

    for (int i = 0; i < n; i++){
        printf("Enter the transactions for person %d.\n", i);
        for (int j = 0; j < m; j++){
            scanf("%d", &transaction_list[i].transactions[j]);
        }
        transaction_list[i].m = m;
        transaction_list[i].index = i;
    }

    for (int i = 0; i < n; i++){
        printf("this is person %d\n", i);
        for (int j = 0; j < m; j++){
            printf("%d ", transaction_list[i].transactions[j]);
        }
        printf("\n");
    }

    sem_init(&mutex, 0, 1);
    for (int i = 0; i < n; i++){
        pthread_create(&threads[i], NULL, do_transactions, &transaction_list[i]);
    }

    for (int i = 0; i < n; i++){
        pthread_join(threads[i], NULL);
    }

    sem_destroy(&mutex);
    printf("Final Balance : %lld", balance);

    return 0;
}