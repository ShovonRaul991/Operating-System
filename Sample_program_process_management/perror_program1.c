#include <stdio.h>  
#include <stdlib.h>  
#include <errno.h>  
  
int main() {  
    int *ptr = malloc(-1 * sizeof(int));  
    if (ptr == NULL) {  
        perror("Error");  
    }  
    return 0;  
}  