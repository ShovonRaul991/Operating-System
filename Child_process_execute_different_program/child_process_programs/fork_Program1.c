#include<stdio.h>
#include<unistd.h>

int main()
{
    printf("Hello world 1\n");
    fork();
    printf("Hello world 2\n");
    return 0;

}


//output will be 
// Hello world 1
// Hello world 2
// Hello world 2
