#include <unistd.h>
#include <sys/wait.h>
#include <string.h>
#include <stdio.h>

enum { MAXSIGINT = 4 };

volatile int intcnt = 0, cur = 0;

void
handler(int sig)
{
    if (sig == SIGINT) {
        if (++intcnt == MAXSIGINT) {
            _exit(0);
        }
        printf("%d\n", cur);
        fflush(stdout);
    } else if (sig == SIGTERM) {
        _exit(0);
    }
}

int
prime(int x)
{
    int i = 2;
    while (i * i <= x) {
        if (x % i == 0) {
            return 0;
        }
        i++;
    }
    return 1;
}

int
main(int argc, char *argv[])
{
    struct sigaction sa;
    memset(&sa, 0, sizeof(sa));
    sa.sa_handler = handler;
    sa.sa_flags = SA_RESTART;

    sigemptyset(&sa.sa_mask);
    sigaddset(&sa.sa_mask, SIGINT);
    sigaddset(&sa.sa_mask, SIGTERM);
    sigaction(SIGINT, &sa, NULL);
    sigaction(SIGTERM, &sa, NULL);

    int low, high;
    scanf("%d %d", &low, &high);
    
    printf("%d\n", getpid());
    fflush(stdout);
    
    for (int i = low; i < high; ++i) {
        if (prime(i)) {
            cur = i;
        }
    }

    printf("-1\n");
    return 0;
}
