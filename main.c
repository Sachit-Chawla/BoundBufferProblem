#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define BUFFER_SIZE 5
typedef int bufferItem;

bufferItem buffer[BUFFER_SIZE];
int in = 0, out = 0;
sem_t empty, full;
pthread_mutex_t mutex;

void *producer(void *param);
void *consumer(void *param);
void insertItem(bufferItem item, int threadId);
void removeItem(int thread_id);

int main(int argc, char *argv[]) {
    if (argc != 4) {
        fprintf(stderr,"Invalid arguments\n", argv[0]);
        return 1;
    }

    int sleepTime = atoi(argv[1]);
    int numProducers = atoi(argv[2]);
    int numConsumers = atoi(argv[3]);

    if (sleepTime < 1 || sleepTime > 100 || numProducers < 1 || numProducers > 5 || numConsumers < 1 || numConsumers > 5) {
        fprintf(stderr, "Invalid arguments\n");
        return 1;
    }

    // Initialize buffer
    for (int i = 0; i  < BUFFER_SIZE; i++) {
        buffer[i] = -1;
    }

    sem_init(&empty, 0, BUFFER_SIZE);
    sem_init(&full, 0, 0);
    pthread_mutex_init(&mutex, NULL);

    // Create producer and consumer threads
    pthread_t producers[numProducers], consumers[numConsumers];
    for (int i = 0; i < numProducers; i++) {
        int *id = malloc(sizeof(int));
        *id = i;
        pthread_create(&producers[i], NULL, producer, id);
    }

    for (int i = 0; i < numConsumers; i++) {
        int *id = malloc(sizeof(int));
        *id = i;
        pthread_create(&consumers[i], NULL, consumer, id);
    }

    // Main thread sleeps
    sleep(sleepTime);


    sem_destroy(&empty);
    sem_destroy(&full);
    pthread_mutex_destroy(&mutex);
    return 0;
}

// producer first waits and then producer and saves it inside using helper function
void *producer(void *param) {
    int threadId = *(int *)param;
    free(param);

    while (1) {
        sleep(rand() % 5);
        bufferItem item = rand() % 100;
        insertItem(item, threadId);
    }
}

// consumer first waits and removes from buffer using helper function
void *consumer(void *param) {
    int threadId = *(int *)param;
    free(param);

    while (1) {
        sleep(rand() % 5);
        removeItem(threadId);
    }
}

// helper function to insert and feed the buffer and adjust in
void insertItem(bufferItem item, int threadId) {
    sem_wait(&empty);
    pthread_mutex_lock(&mutex);

    buffer[in] = item;
    printf("Producer %d inserted item %d into buffer[%d]\n", threadId, item, in);
    in = (in + 1) % BUFFER_SIZE;


    pthread_mutex_unlock(&mutex);
    sem_post(&full);
}

// helper function to remove item and adjust buffer and out
void removeItem(int thread_id) {
    sem_wait(&full);
    pthread_mutex_lock(&mutex);

    bufferItem item = buffer[out];
    if (item != -1) {
        printf("Consumer %d consumed item %d from buffer[%d]\n", thread_id, item, out);
        buffer[out] = -1;  // Mark slot as empty
        out = (out + 1) % BUFFER_SIZE;
    }

    pthread_mutex_unlock(&mutex);
    sem_post(&empty);
}
