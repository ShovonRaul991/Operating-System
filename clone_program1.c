#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <sched.h>
#include <sys/wait.h>
#include <unistd.h>

#define STACK_SIZE 65536 // 64 KB stack size

// Function to be executed by the child process
static int child_func(void *arg) {
    printf("Child process: PID=%d\n", getpid());
    printf("Child process created\n");
    return 0;
}

int main() {
    printf("Parent process: PID=%d\n", getpid());
    // Allocate memory for child's stack
    char *stack = malloc(STACK_SIZE);
    if (stack == NULL) {
        perror("malloc");
        exit(EXIT_FAILURE);
    }

    // Clone the current process, sharing the same memory space (CLONE_VM)
    // and providing a private execution stack (CLONE_VM)
    pid_t child_pid = clone(child_func, stack + STACK_SIZE, CLONE_VM | CLONE_VFORK, NULL);
    
    if (child_pid == -1) {
        perror("clone");
        exit(EXIT_FAILURE);
    }

    // Wait for the child process to terminate
    if (waitpid(child_pid, NULL, 0) == -1) {
        perror("waitpid");
        exit(EXIT_FAILURE);
    }

    

    // Free the allocated stack memory
    free(stack);

    return 0;
}
