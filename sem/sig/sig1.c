#include <unistd.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>

void SigHandle(int sig)
{
    printf("Death\n");
    exit(0);
}

int main(void)
{
    int i = 0;
    signal(SIGINT, SigHandle);
    if (fork()) {
        while(i++ < 5) {
            printf("%d This is father\n", i);
            fflush(stdout);
        }
        wait(NULL);
    } else {
        while(i++ < 10) {
            printf("%d This is son\n", i);
            fflush(stdout);
        }
        kill(getppid(), SIGINT);
    }

    return 0;
}
