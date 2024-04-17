#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

#define QUEUE_SIZE 10
#define PRODUCER_COUNT 3
#define CONSUMER_COUNT 2

int queue[QUEUE_SIZE];
int front = -1, rear = -1;

sem_t mutex, full, empty;

void enQ(int item) {
    sem_wait(&empty);
    sem_wait(&mutex);

    if ((rear + 1) % QUEUE_SIZE == front) {
        printf("Queue is full, cannot add item %d\n", item);
    } else {
        if (front == -1) front = 0;
        rear = (rear + 1) % QUEUE_SIZE;
        queue[rear] = item;
        printf("Produced item %d\n", item);
    }

    sem_post(&mutex);
    sem_post(&full);
}

int deQ() {
    sem_wait(&full);
    sem_wait(&mutex);

    int item;
    if (front == -1) {
        printf("Queue is empty\n");
        return -1;
    } else {
        item = queue[front];
        printf("Consumed item %d\n", item);
        if (front == rear) {
            front = rear = -1;
        } else {
            front = (front + 1) % QUEUE_SIZE;
        }
    }

    sem_post(&mutex);
    sem_post(&empty);

    return item;
}

void *producer(void *param) {
    int id = *((int *)param);
    int producedItems = 0;

    while (1) {
        int item = rand() % 100; // Generate a random item
        enQ(item);
        producedItems++;

        if (producedItems >= 3) // Producer thread adds 3 items
            break;

        sleep(1); // Sleep to slow down output
    }

    pthread_exit(NULL);
}

void *consumer(void *param) {
    int id = *((int *)param);
    int consumedItems = 0;

    while (1) {
        int item = deQ();
        if (item != -1)
            consumedItems++;

        if (consumedItems >= 2) // Consumer thread consumes 2 items
            break;

        sleep(2); // Sleep to slow down output
    }

    pthread_exit(NULL);
}

int main() {
    pthread_t producerThreads[PRODUCER_COUNT], consumerThreads[CONSUMER_COUNT];
    int producerIds[PRODUCER_COUNT], consumerIds[CONSUMER_COUNT];

    sem_init(&mutex, 0, 1);
    sem_init(&full, 0, 0);
    sem_init(&empty, 0, QUEUE_SIZE);

    for (int i = 0; i < PRODUCER_COUNT; i++) {
        producerIds[i] = i;
        pthread_create(&producerThreads[i], NULL, producer, &producerIds[i]);
    }

    for (int i = 0; i < CONSUMER_COUNT; i++) {
        consumerIds[i] = i;
        pthread_create(&consumerThreads[i], NULL, consumer, &consumerIds[i]);
    }

    for (int i = 0; i < PRODUCER_COUNT; i++) {
        pthread_join(producerThreads[i], NULL);
    }

    for (int i = 0; i < CONSUMER_COUNT; i++) {
        pthread_join(consumerThreads[i], NULL);
    }

    sem_destroy(&mutex);
    sem_destroy(&full);
    sem_destroy(&empty);

    return 0;
}
