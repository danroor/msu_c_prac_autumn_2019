#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>

typedef int semaphore;
semaphore s = 0;

void up(void)
{
    s++;
    
}

void down(void)
{
    //lock
    while (!s) {
        wait();
    }
    s--;
    //unlock
}

int main(void)
{
    
}
