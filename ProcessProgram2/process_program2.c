#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <sys/ipc.h>
#include <sys/shm.h>

#define SHM_SIZE sizeof(unsigned long long)


//factorial function
unsigned long long find_factorial(int n) {
    unsigned long long fact = 1;
    if (n < 0) {
        printf("No factorial\n");
    } else {
        for (int m = 1; m <= n; ++m) {
            fact *= m;
        }
    }
    return fact;
}

//function to use the previous factorial values
unsigned long long find_factorial_by_history(unsigned long long prev, int n) {
    return prev * n;
}

int main(int argc, char *argv[]) {
    int num1, num2;
    if (argc != 3) {
        printf("Need two parameters\n");
        return 1;
    } else {
        num1 = atoi(argv[1]);
        num2 = atoi(argv[2]);
        if (num1 <= 0 || num2 <= 0 || num1 >= num2) {
            printf("Invalid range\n");
            return 1;
        }
    }

    //the above code is for parameter check

    key_t key = IPC_PRIVATE;
    int shmid;
    unsigned long long *shm_ptr;
    
    //Share memory creation error checking
    if ((shmid = shmget(key, SHM_SIZE, IPC_CREAT | 0666)) < 0) {
        perror("shmget");
        return 1;
    }

    //Share memory segment attachment error checking
    if ((shm_ptr = shmat(shmid, NULL, 0)) == (unsigned long long *)-1) {
        perror("shmat");
        return 1;
    }

    *shm_ptr = find_factorial(num1);
    printf("Child process %d: Factorial of %d is: %llu\n", getpid(), num1, *shm_ptr);

    for (int i = num1 + 1; i <= num2; i++) {
        pid_t pid = fork();

        if (pid < 0) {
            perror("fork");
            return 1;
        } else if (pid == 0) { // Child process
            unsigned long long result = find_factorial_by_history(*shm_ptr, i);
            *shm_ptr = result;
            printf("Child process %d: Factorial of %d is: %llu\n", getpid(), i, *shm_ptr);
            shmdt(shm_ptr);
            return 0;
        }
        else{
            pid_t child_p;
        
            child_p = waitpid(pid, NULL, 0); // Wait for the child process to terminate
            printf(" Child process id %d terminated\n",child_p);
        }
    }

    printf("Parent process: Total factorial from child processes: %llu\n", *shm_ptr);

    if (shmdt(shm_ptr) == -1) {
        perror("shmdt");
        return 1;
    }

    if (shmctl(shmid, IPC_RMID, NULL) == -1) {
        perror("shmctl");
        return 1;
    }

    return 0;
}
