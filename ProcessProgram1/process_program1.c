#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>
#include <sys/wait.h>

//function to execute factorial of a given input , 
// it will take the command line argument which is by default greater than 0
unsigned long long find_factorial(int n){
    int m;
    unsigned long long fact = 1;
    if(n<0)
    {
        printf("No factorial");
    }
    else{
        for(m=1; m <= n; ++m){
            fact *= m;
        }
    }
    return fact;
}

int main(int argc, char *argv[]){

    if (argc != 2) {
        printf("Numeric command line argument required!!\n");
        return 1; // Exit with an error status
    }


    __pid_t p; //to take the process id
    int i;

    //converting string input to integer
    int num = atoi(argv[1]);

    

    if(num != 0){    //if string or other datatype is entered then num will be 0 after converting those datatype to integer
        for(i=0;i<num;i++)
        {
            p = fork();
            if (p == 0) {  //while successful child process creation fork will return 0 in child process execution
            
            printf("%d Child process !\n", i + 1);
            printf("The factorial of %d is: %llu\n",i+1,find_factorial(i+1));
            
            return 0;
            }
            else if (p > 0) {      //for parent process fork will return process id
            pid_t child_p;
        
            child_p = waitpid(p, NULL, 0); // Wait for the child process to terminate
            printf("Child process id %d terminated\n",child_p);
            // printf("Parent process! ");
            }
            else {
                perror("fork");
                return 1;
            }
        }
    }
    else{
        printf("Enter a proper input\n");
    }

    return 0;
}