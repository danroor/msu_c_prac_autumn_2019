#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>

enum { RIGHTS = 0666 };

int flag = 0;

void h(int s) {
    if (s == SIGUSR1) flag++; 
}

int main(int argc, char *argv[])
{


    int *a = malloc(40);
   
    for (int i = 0; i < 10; ++i) a[i] = i;
    signal(SIGUSR1, h);
 
    pid_t pid = fork();

    if (pid) {
        a[0] = 1;
        kill(pid, SIGUSR1);
        wait(NULL);
    }
 
    if (!pid) for (;;) if (flag) break;

    for (int i = 0; i < 10; ++i) printf("%d ", a[i]);
    printf("\n%p\n", a);   
    
    free(a);

    return 0;


}
