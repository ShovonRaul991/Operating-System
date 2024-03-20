#include <stdio.h>
#include <stdlib.h>
#include <unistd.h> // System call: getpid(), fork()
#include <sys/types.h> // System call: getpid(), fork()
#include <sys/ipc.h> // System call: ftok()
#include <sys/sem.h> // System calls: semget(), semop(), semctl()
#include <sys/shm.h> // System calls: shmget(), shmat(), shmdt(), shmctl(

#define MAX_ITEMS 10

// Structure of shared circular queue
typedef struct {
    int buffer[MAX_ITEMS];
    int head; // Index of the first element in the queue
    int tail; // Index of the last element in the queue
    int itemCount; // Number of items in the queue
} CircularQueue;

// Semaphore operations
static void sem_wait(int semid, int sem_num) {
    struct sembuf sops = {sem_num, -1, 0};
    semop(semid, &sops, 1);
}

static void sem_signal(int semid, int sem_num) {
    struct sembuf sops = {sem_num, +1, 0};
    semop(semid, &sops, 1);
}

int main() {
    key_t key = ftok(".", 'a'); // Unique key for semaphore and shared memory

    // Create semaphore
    int semid = semget(key, 2, IPC_CREAT | 0666);
    if (semid == -1) {
        perror("semget");
        exit(EXIT_FAILURE);
    }

    // Initialize semaphores
    semctl(semid, 0, SETVAL, 1); // mutex semaphore
    // semctl(semid, 1, SETVAL, 0); // empty semaphore

    // Create shared memory for circular queue
    int shmid = shmget(key, sizeof(CircularQueue), IPC_CREAT | 0666);
    if (shmid == -1) {
        perror("shmget");
        exit(EXIT_FAILURE);
    }

    CircularQueue *queue = (CircularQueue *)shmat(shmid, NULL, 0);
    if (queue == (void *)-1) {
        perror("shmat");
        exit(EXIT_FAILURE);
    }

    while (1) {
        printf("Press Enter to consume an item...\n");
        getchar(); // Wait for Enter key press

        sem_wait(semid, 0); // Wait for mutex

        if (queue->itemCount == 0) {
            printf("Queue is empty, consumer will not consume anything.\n");
            sem_signal(semid, 0); // Release mutex
            continue;
        }

        int item = queue->buffer[queue->head];

        // Check if the queue becomes empty after consuming
        if (queue->head == queue->tail) {
            queue->head = -1; // Reset head index
            queue->tail = -1; // Reset tail index
        } else {
            queue->head = (queue->head + 1) % MAX_ITEMS; // Update head index
        }

        queue->itemCount--;

        printf("Consumed: %d\n", item);

        sem_signal(semid, 0); // Release mutex
    }

    return 0;
}
