#include <unistd.h>
#include <sys/wait.h>
#include <stdio.h>

int 
main(void)
{
    int p[2];
    pipe(p);

    if (fork()) {
        //father
        close(p[0]);

        int x;
        while (scanf("%d", &x) == 1) {
            write(p[1], &x, sizeof(x));
        }
        close(p[1]);
        wait(NULL);
        return 0;
    } 

    close(p[1]);

    if (fork()) {
        //son
        close(p[0]);
        wait(NULL);
    } else {
        //grandson
        long long sum = 0;
        int x;
        while (read(p[0], &x, sizeof(x)) == sizeof(x)) {
            sum += x;
        }

        printf("%lld\n", sum);
        close(p[0]);
        close(p[1]);
        return 0;
    }

    return 0;
}
