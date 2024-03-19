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
        printf("Press Enter to consume items...\n");
        getchar();

        sem_wait(semid, 0);

        if (queue->itemCount > 0) {
            int items_to_consume;
            printf("Enter number of items to consume (max 10): ");
            scanf("%d", &items_to_consume);

            for (int i = 0; i < items_to_consume; i++) {
                if (queue->itemCount == 0) {
                    break;
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
            }
        } else {
            printf("Queue is empty, consumer will not consume anything.\n");
            break;
        }

        sem_signal(semid, 0);
    }


    return 0;
}
