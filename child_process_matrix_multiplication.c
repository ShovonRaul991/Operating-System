#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

#define MAX_SIZE 10 // Maximum size for matrices

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

    int A[m][n], B[n][r], C[m][r]; // Matrices A, B, and the resulting product matrix C

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


    int status;
    // Array to store child process IDs
    pid_t child_pids[m][r]; 
    // Create m * r child processes to compute product matrix A x B
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
                printf("Child process %d: output = %d\n", getpid(), sum); // Print child process data
                exit(sum);
            }
            else{
                // Parent process
                child_pids[i][j] = pid; // Store child PID
            }
        }
    }

    // Wait for all child processes to terminate
    for (int i = 0; i < m; i++) {
        for (int j = 0; j < r; j++) {
            waitpid(child_pids[i][j], &status, 0); // Wait for specific child process

            if (WIFEXITED(status)) {
                // Child process terminated normally
                int child_exit_status = WEXITSTATUS(status); // Retrieve the exit status
                printf("Parent process: Child %d exited with value %d\n",child_pids[i][j], child_exit_status);

                // Store the result in the result matrix C
                C[i][j] = child_exit_status;
            } else {
                // Child process terminated abnormally
                printf("Parent process: Child process %d terminated abnormally\n",child_pids[i][j]);
            }
        }
    }


    // Print the resulting product matrix C
    printf("Product matrix A x B (%d x %d):\n", m, r);
    for (int i = 0; i < m; i++) {
        for (int j = 0; j < r; j++) {
            printf("%d ", C[i][j]);
        }
        printf("\n");
    }

    // Wait for all child processes to terminate
    for (int i = 0; i < m * r; i++) {
        wait(NULL);
    }

    return 0;
}
