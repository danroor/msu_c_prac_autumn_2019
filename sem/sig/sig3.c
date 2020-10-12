#include <unistd.h>
#include <sys/wait.h>
#include <signal.h>
#include <stdio.h>

volatile int flag;

void handle(int sig)
{
    if (sig == SIGUSR1) {
        printf("In process %d SIGUSR1 recieved\n", getpid());
        flag++;
        kill(getpid(), SIGUSR2);
    }
    if (sig == SIGUSR2) {
        printf("In process %d SIGUSR2 recieved\n", getpid());
        flag++;
        kill(getpid(), SIGUSR1);
    }
}

int
main(void)
{
    signal(SIGUSR1, handle);
    signal(SIGUSR2, handle);
    raise(SIGUSR1);
 
    return 0;
}
