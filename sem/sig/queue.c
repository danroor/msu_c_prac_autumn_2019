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
    struct sigaction act = {0};
    act.sa_handler = handler;
    act.sa_flags = SA_RESTART;
    sigaction(SIGINT, &act, NULL);
    while (1) {
        sleep(1);
        while (sum > handled) {
            puts("Signal handled\n"); 
            handled++;
        }
        if (sum > 5) return 0;
    }
}

//такая программа с использованием signal плохо работает на BSD-платформах, хорошо на system V
//использование sigaction вместо signal позволяет программе хорошо работать на обеих платформах
