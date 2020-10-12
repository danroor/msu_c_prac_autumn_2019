#include <stdio.h>
#include <signal.h>
#include <unistd.h>

volatile int handled = 0, sum = 0;

void
handler(int sig)
{
    ++sum;
}

int
main(void)
{
    int sig = 0;
    sigset_t mask;
    sigemptyset(&mask);
    sigaddset(&mask, SIGINT);
    sigprocmask(SIG_BLOCK, &mask, NULL);
    while (1) {
        if (sigwait(&mask, &sig) == 0 && sig == SIGINT) {
            puts("\nSignal handled"); 
            handled++;
        }
        if (handled > 5) return 0;
    }
}
