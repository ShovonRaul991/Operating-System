#include <stdio.h>
#include <stdlib.h>
#include <unistd.h> // System call: getpid(), fork()
#include <sys/types.h> // System call: getpid(), fork()
#include <sys/ipc.h> // System call: ftok()
#include <sys/sem.h> // System calls: semget(), semop(), semctl()
#include <sys/shm.h> // System calls: shmget(), shmat(), shmdt(), shmctl(


#define MAX_ITEMS 10

typedef struct {
    int buffer[MAX_ITEMS];
    int head;
    int tail;
    int itemCount;
} CircularQueue;

static void sem_wait(int semid, int sem_num) {
    struct sembuf sops = {sem_num, -1, 0};
    semop(semid, &sops, 1);
}

static void sem_signal(int semid, int sem_num) {
    struct sembuf sops = {sem_num, +1, 0};
    semop(semid, &sops, 1);
}

int main() {
    key_t key = ftok(".", 'a');

    int semid = semget(key, 2, IPC_CREAT | 0666);
    if (semid == -1) {
        perror("semget");
        exit(EXIT_FAILURE);
    }

    semctl(semid, 0, SETVAL, 1);

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
        sem_wait(semid, 0);

        if (queue->itemCount < MAX_ITEMS) {
            int items_to_produce;
            printf("Enter number of items to produce (max 10): ");
            scanf("%d", &items_to_produce);

            for (int i = 0; i < items_to_produce; i++) {
                if (queue->itemCount == MAX_ITEMS) {
                    break;
                }
                int item;
                printf("Enter item %d: ", i + 1);
                scanf("%d", &item);

                // Check if the queue is empty
                if (queue->itemCount == 0) {
                    queue->head = 0;
                    queue->tail = 0;
                } else {
                    queue->tail = (queue->tail + 1) % MAX_ITEMS; // Update tail index
                }

                queue->buffer[queue->tail] = item;
                queue->itemCount++;

                printf("Produced: %d\n", item);
            }
        } else {
            printf("Queue is full, producer will not produce anything.\n");
            break;
        }

        sem_signal(semid, 0);
    }

    return 0;
}
