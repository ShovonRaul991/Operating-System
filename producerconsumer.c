#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

#define MAX_QUEUE_SIZE 10
#define MAX_THREADS 10

int circularQueue[MAX_QUEUE_SIZE];
int front = -1, rear = -1;
int current_size = 0; // Variable to store the current number of elements in the circular queue
int producerCount = 0, consumerCount = 0;

sem_t mutex, items, spaces;

void enqueue(int item);
int dequeue();
void *producer_thread(void *data);
void *consumer_thread(void *data);

int main() {
    // Semaphore initialization
    sem_init(&mutex, 0, 1);
    sem_init(&items, 0, 0);
    sem_init(&spaces, 0, MAX_QUEUE_SIZE);

    // Thread arrays for producers and consumers
    pthread_t producerThreads[MAX_THREADS], consumerThreads[MAX_THREADS];

    // Choice variable for user input
    
    int choice;

    while (1) {
        printf("\n1. Add Producer\n");
        printf("2. Add Consumer\n");
        printf("3. Remove Producer\n");
        printf("4. Remove Consumer\n");
        printf("5. Exit\n");
        printf("Enter your choice: \n");
        scanf("%d", &choice);
        

        switch (choice) {
            case 1:
                //printf("\t you choose to add producer\n");
                if (producerCount < MAX_THREADS) {
                    pthread_create(&producerThreads[producerCount++], NULL, producer_thread, NULL);
                    printf("Producer added.\n");
                } else {
                    printf("Cannot add more producer threads.\n");
                }
                break;

            case 2:
                //printf("\t you choose to add consumer\n");
                if (consumerCount < MAX_THREADS) {
                    pthread_create(&consumerThreads[consumerCount++], NULL, consumer_thread, NULL);
                    printf("Consumer added.\n");
                } else {
                    printf("Cannot add more consumer threads.\n");
                }
                break;

            case 3:
                //printf("\t you choose to remove producer\n");
                if (producerCount > 0) {
                    pthread_cancel(producerThreads[--producerCount]);
                    printf("Producer removed.\n");
                } else {
                    printf("No producer thread to remove.\n");
                }
                break;

            case 4:
                //printf("\t you choose to remove consumer\n");
                if (consumerCount > 0) {
                    pthread_cancel(consumerThreads[--consumerCount]);
                    printf("Consumer removed.\n");
                } else {
                    printf("No consumer thread to remove.\n");
                }
                break;

            case 5:
                printf("\t Operation aborted\n");
                sem_destroy(&mutex);
                sem_destroy(&items);
                sem_destroy(&spaces);

                return 0;

            default:
                printf("Invalid choice. Try again.\n");
        }
    }

    return 0;
}



void enqueue(int item) {
    
    if (current_size == MAX_QUEUE_SIZE) { // Check if queue is full based on current_size
        printf("Queue is full. Cannot add item %d.\n", item);
        sem_wait(&mutex);
        sem_wait(&spaces);
        
    } else {
        if (front == -1)
            front = 0;
        rear = (rear + 1) % MAX_QUEUE_SIZE;
        circularQueue[rear] = item;
        printf("Producer Count : %d \t Produced item %d.\n",producerCount, item);
        current_size++;
    }

    sem_post(&mutex);
    sem_post(&items);
}

int dequeue() {
    
    int item;
    if (current_size == 0) { // Check if queue is empty based on current_size
        printf("Queue is empty. Cannot consume.\n");
        sem_wait(&mutex);
        sem_wait(&items);
        
    } else {
        item = circularQueue[front];
        if (front == rear)
            front = rear = -1;
        else
            front = (front + 1) % MAX_QUEUE_SIZE;
        printf("Consumer Count : %d \t Consumed item %d.\n",consumerCount, item);
        current_size--;
    }

    sem_post(&mutex);
    sem_post(&spaces);

    return item;
}

void *producer_thread(void *data) {
    while (1) {
        int item = rand() % 100;
        enqueue(item);
        sleep(1);
    }
}

void *consumer_thread(void *data) {
    while (1) {
        dequeue();
        sleep(1);
    }
}
