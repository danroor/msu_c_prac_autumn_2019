#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>

int
main(void)
{
    if (fork()) {
        wait(NULL);
        printf("1\n");
    } else if (fork()) {
        wait(NULL);
        printf("2 ");
    } else {
        printf("3 ");
    }

    return 0;
}
