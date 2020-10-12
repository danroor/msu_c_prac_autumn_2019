#include <unistd.h>
#include <stdio.h>
#include <sys/wait.h>

int
main(int argc, char *argv[])
{
    int max;
    sscanf(argv[1], "%d", &max);

    int cur = 0, lvl = 0;

    while (lvl < max) {
        cur <<= 1;
        cur++;
        lvl++;
        if (fork()) {
            wait(NULL);
            printf("%d\n", cur >> 1);
            fflush(stdout);
 
            cur++;
            if (fork()) {
                wait(NULL);
                return 0;
            }
        }
    }

    printf("%d\n", cur);

    return 0;
}
