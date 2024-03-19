#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>

int main()
{
    int pid,n;
    printf("Hello world 1\n");
    pid = fork();
    printf("Hello world 2\n");

    if (pid == 0)
    {
        printf("Fork success! for child\n");
        exit(2);
    }
    else if(pid == -1){
        printf("Fork failure\n");
    }
    else{
        printf("Fork success for parent\n");
    }
    wait(&n);
    printf("Exit returns %d\n",n);
    return 0;


}