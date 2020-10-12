#include <unistd.h>
#include <sys/wait.h>
#include <stdio.h>
#include <string.h>

enum { MAXSIG = 5 };

int cnt = 0;

void
handler(int sig)
{
    if (sig == SIGHUP) {
        if (cnt == MAXSIG) {
            _exit(0);
        }
        printf("%d\n", cnt++);
        fflush(stdout); 
    }
}

int
main(void)
{
    struct sigaction sa;
    memset(&sa, 0, sizeof(sa));
    sa.sa_handler = handler;
    sa.sa_flags = SA_RESTART;
    
    sigemptyset(&sa.sa_mask);
    sigaddset(&sa.sa_mask, SIGHUP);
    sigaction(SIGHUP, &sa, NULL);
    
    printf("%d\n", getpid());
    fflush(stdout);
    
    while (cnt < MAXSIG + 1) {
        pause();
    }

    return 0;
}
