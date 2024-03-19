#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <math.h>

// Function to evaluate a term of the polynomial
double evaluate_term(double coefficient, double exponent, double x) {
    return coefficient * pow(x, exponent);
}

int main(int argc, char *argv[]) {
    if (argc < 3) {
        printf("Usage: %s <x> <a_n> <a_n-1> ... <a_1> <a_0>\n", argv[0]);
        return 1;
    }

    double x = atof(argv[1]);
    int degree = argc - 3;
    double *coefficients = (double *)malloc((degree + 1) * sizeof(double));

    if (coefficients == NULL) {
        perror("malloc");
        return 1;
    }

    // Parse coefficients from command line arguments
    for (int i = 0; i <= degree; i++) {
        coefficients[i] = atof(argv[i + 2]);
    }

    // The pipefd array holds two file descriptors: 
    // pipefd[0] for reading from the pipe and pipefd[1] for writing to the pipe.
    int pipefd[2];
    if (pipe(pipefd) == -1) {
        perror("pipe");
        return 1;
    }

    // Create n child processes
    for (int i = 0; i <= degree; i++) {
        pid_t pid = fork();

        if (pid < 0) {
            perror("fork");
            return 1;
        } else if (pid == 0) { // Child process
            double result = evaluate_term(coefficients[i], degree - i, x);
            printf("Child process %d: Result = %lf\n", getpid(), result); // Print child process data
            close(pipefd[0]); // Close unused read end of pipe
            write(pipefd[1], &result, sizeof(double)); // Write result to pipe
            close(pipefd[1]); // Close write end of pipe
            exit(0);
        }
    }

    close(pipefd[1]); // Close write end of pipe in parent

    double total_result = 0.0;
    double child_result;

    // Read results from child processes and sum them up
    for (int i = 0; i <= degree; i++) {
        read(pipefd[0], &child_result, sizeof(double)); // Read result from pipe
        total_result += child_result;
    }

    close(pipefd[0]); // Close read end of pipe in parent

    printf("Total result = %lf\n", total_result);

    // Wait for all child processes to terminate as total result is collected
    for (int i = 0; i <= degree; i++) {
        wait(NULL);
    }

    free(coefficients);
    return 0;
}
