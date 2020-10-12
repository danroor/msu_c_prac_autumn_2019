#include <sys/wait.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <sys/mman.h>
#include <stdlib.h>
#include <unistd.h>

int main(void)
{
    int *mem = mmap(NULL, sizeof(int), PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANON, -1, 0);
  
    mem[0] = 123;
    if (!fork()) {
        printf("%d\n", mem[0]);
        mem[0] += 100;
    } else {
        wait(NULL);
        printf("%d\n", mem[0]);
    }
  
    
 
    return 0;
}
