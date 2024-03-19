#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>

int main() {
        int status;
        pid_t pid=0;
        pid_t p1=0;
        printf("Hello World!\n"); /* printing "Hello =World!\n" just like that */

        p1 = fork();
        if (p1 == 0) {
                /* this part is executed by child process */
                int i;

                /* the following part is just to keep the child executing something
                    so that it is live for some period and does not terminate */

                //till i less than 7 it will wait for user input
                for (i=0; i<7; i++) {
                        printf("%d\n", i++);
                        getchar(); /* child process waits for user input, remains alive and does not terminate */
                }
                exit(i); /* the child process terminates with status 12, note that 12 is just a number,
                                 this status (12) is communicated to the parent process (in some form) and the process
                                 may use it (12) through wait() if it wishes to do so.
                              */

        }

       /* this part will be executed only  by the parent  process */
        pid = wait(&status);  //dividing by 256 will remove the bits inserted by os by default
        printf("pid = %d status = %d!\n, Status more than 7 so exited...", pid, status/256); /* Look the value of status that the  parent process prints! */
}