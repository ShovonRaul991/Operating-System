#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>

#define MAX_SIZE 10 // Maximum size for matrices
#define SHARED_MEMORY_KEY 1234 // Shared memory key

int main() {
    int m, n, r;

    // Read the dimensions of matrices A and B from the user
    printf("Enter the number of rows (m) for matrix A: ");
    scanf("%d", &m);
    printf("Enter the number of columns (n) for matrix A: ");
    scanf("%d", &n);
    printf("Enter the number of columns (r) for matrix B: ");
    scanf("%d", &r);

    if (n <= 0 || m <= 0 || r <= 0 || m > MAX_SIZE || n > MAX_SIZE || r > MAX_SIZE) {
        printf("Invalid matrix dimensions.\n");
        return 1;
    }

    int shmid = shmget(SHARED_MEMORY_KEY, sizeof(int) * m * r, 0666 | IPC_CREAT); // Create shared memory
    if (shmid == -1) {
        perror("shmget");
        exit(EXIT_FAILURE);
    }

    int *C = (int *)shmat(shmid, NULL, 0); // Attach shared memory
    if (C == (void *)-1) {
        perror("shmat");
        exit(EXIT_FAILURE);
    }

    int A[m][n], B[n][r]; // Matrices A, B
    // Read matrix A from the user
    printf("Enter elements of matrix A (%d x %d):\n", m, n);
    for (int i = 0; i < m; i++) {
        for (int j = 0; j < n; j++) {
            scanf("%d", &A[i][j]);
        }
    }

    // Read matrix B from the user
    printf("Enter elements of matrix B (%d x %d):\n", n, r);
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < r; j++) {
            scanf("%d", &B[i][j]);
        }
    }

    // Create m * r child processes to compute product matrix A x B
    int count = 0;
    for (int i = 0; i < m; i++) {
        for (int j = 0; j < r; j++) {
            pid_t pid = fork();

            if (pid < 0) {
                perror("fork error");
                return 1;
            } else if (pid == 0) { // Child process
                int sum = 0;
                for (int k = 0; k < n; k++) {
                    sum += A[i][k] * B[k][j];
                }
                C[i * r + j] = sum; // Store result in shared memory
                exit(EXIT_SUCCESS);
            }
        }
    }


    // Wait for all child processes to terminate
    for (int i = 0; i < m * r; i++) {
        wait(NULL);
    }

    // Print the resulting product matrix C
    printf("Product matrix A x B (%d x %d):\n", m, r);
    count = 0;
    for (int i = 0; i < m; i++) {
        for (int j = 0; j < r; j++) {
            printf("%d ", C[count++]);
        }
        printf("\n");
    }

    // Detach and release shared memory
    shmdt(C);
    shmctl(shmid, IPC_RMID, NULL);

    return 0;
}
