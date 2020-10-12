#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>

int
main(void)
{
    int n;
    scanf("%d", &n);
  
    printf("1");
    fflush(stdout);
    for (int i = 2; i <= n; ++i) {
        if (fork()) {
            wait(NULL);
            exit(0);
        } else {
            printf(" %d", i);
            fflush(stdout);
        }
    }
    printf("\n");

    return 0;
}
