#include <unistd.h>
#include <stdio.h>
#include <sys/wait.h>

int main(int argc, char *argv[])
{
    int max;
    sscanf(argv[1], "%d", &max);

    int p[2][2];
    pipe(p[0]);
    pipe(p[1]);

    pid_t child;
    int n = 1;

    if ( ( child = fork() ) == -1) {
        return 1;
    } else if (child) {

        n = 2;

        write(p[0][1], &child, sizeof(child));
        if ( ( child = fork() ) == -1) {
            return 1;
        }
   
        if (child) {
            wait(NULL);
            wait(NULL);
            printf("Done\n");
            return 0;
        }
    }

    if (dup2(p[n][0], STDIN_FILENO) == -1) {
        return 1;
    }
    if (dup2(p[n][1], STDOUT_FILENO) == -1) {
        return 1;
    }
 
    int cnt = 1;
    while (1) {

        if (n) {
            read(p[n][0], &cnt, sizeof(cnt));
        }

        if (cnt == max) {
            break;
        }

        printf("%d %d\n", n, cnt++);
        write(p[!n][1], &cnt, sizeof(cnt));

        if (!n) {
            read(p[n][0], &cnt, sizeof(cnt));
        }

        if (cnt == max) {
            break;
        }

    }

    close(p[0][0]);
    close(p[0][1]);
    close(p[1][0]);
    close(p[1][1]);

    return 0;
}
