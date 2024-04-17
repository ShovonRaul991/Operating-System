#include<stdio.h>
#include<unistd.h>

int main()
{
    int pid;
    printf("Hello world 1\n");
    pid = fork();
    printf("Hello world 2\n");

    if (pid == 0)
    {
        printf("Fork success! for child\n");
    }
    else if(pid == -1){
        printf("Fork failure\n");
    }
    else{
        printf("Fork success for parent\n");
    }
    printf("Hello world 3\n");
    return 0;


}


//output
// Hello world 1
// Hello world 2
// Fork success for parent
// Hello world 3
// Hello world 2
// Fork success! for child
// Hello world 3