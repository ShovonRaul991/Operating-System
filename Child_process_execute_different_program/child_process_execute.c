#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main(int argc, char *argv[]) {
    // Checking if executable files path are provided
    if (argc < 2) {
        printf("Usage: %s file1 file2 ..... filen\n", argv[0]);
        return 1;
    }

    // Looping through each executable file provided as command-line arguments
    for (int i = 1; i < argc; i++) {
        pid_t pid = fork();

        // Fork error handling
        if (pid < 0) {
            perror("fork error");
            return 1;
        }

        // Child process
        else if (pid == 0) {
            printf("Child process %d: Executing %s\n", getpid(), argv[i]);
            // Execute the executable file
            if (execlp(argv[i], argv[i], NULL) == -1) {
                perror("execlp error");
                return 1;
            }
        }

        // Parent process
        else {
            // Wait for the child process to terminate
            //so that child process can terminate after executing files serially
            wait(NULL);
        }
    }

    printf("All child processes have terminated.\n");

    return 0;
}
