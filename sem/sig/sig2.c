#include <unistd.h>
#include <signal.h>
#include <stdio.h>

void alrm(int s)
{
    printf("Faster!\n");
    fflush(stdout);
    alarm(5);
}

int main(void)
{
    signal(SIGALRM, alrm);
    printf("Enter a number: ");
    int x;
    alarm(5);
    for (;;) if (scanf("%d", &x) == 1) break;

    printf("pasibki :3\n");
    return 0;
}
