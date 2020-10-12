#include <unistd.h>
#include <sys/wait.h>
#include <string.h>
#include <stdio.h>

volatile int
mode = 0;

void
handler(int sig)
{
    if (sig == SIGINT) {
        mode = 0;
    } else if (sig == SIGQUIT) {
        mode = 1;
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
    sigaddset(&sa.sa_mask, SIGINT);
    sigaddset(&sa.sa_mask, SIGQUIT);
    sigaction(SIGINT, &sa, NULL);
    sigaction(SIGQUIT, &sa, NULL);
    
    printf("%d\n", getpid());
    fflush(stdout);
    
    int x;
    unsigned int res = 0;
    
    while (scanf("%d", &x) == 1) {
        if (mode) {
            res *= x;
        } else {
            res += x;
        }
        printf("%d\n", (int) res);
        fflush(stdout);
    }

    return 0;
}
