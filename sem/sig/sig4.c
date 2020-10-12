#include <unistd.h>
#include <sys/wait.h>
#include <signal.h>
#include <stdio.h>

volatile int i = 0;

void handler(int sig)
{
    if (sig == SIGUSR1) {
        printf("%d SIGUSR1 recieved\n", i);
    }
    if (sig == SIGUSR2) {
        printf("%d SIGUSR2 recieved\n", i);
    }
}

int
main(void)
{
    struct sigaction sa;
    sa.sa_handler = handler;
    sigemptyset(&sa.sa_mask);
    //sigaddset(&sa.sa_mask, SIGUSR1);
    //sigaddset(&sa.sa_mask, SIGUSR2);
    sigaction(SIGUSR1, &sa, NULL);
    sigaction(SIGUSR2, &sa, NULL);

    
    pid_t pid = getpid();
    while (i++ < 100) {
        if (i % 2) {
            kill(pid, SIGUSR1);
        } else {
            kill(pid, SIGUSR2);
        }
    }
 
    return 0;
}
