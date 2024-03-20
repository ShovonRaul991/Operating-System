#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>

int main() {
    pid_t p;
    int i;
    for (i = 0; i < 6; i++) {
        p = fork();
        if (p == 0) {
            printf("Child process %d!\n", i + 1);
            return 0;
        }
        else if (p > 0) {
            waitpid(p, NULL, 0); // Wait for the child process to terminate
            printf("Parent process! ");
        }
        else {
            perror("fork");
            return 1;
        }
    }
    return 0;
}
