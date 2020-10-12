#include <unistd.h>
#include <stdio.h>
#include <sys/wait.h>

int main(int argc, char *argv[])
{
    int max;
    sscanf(argv[1], "%d", &max);

    if (max <= 1) {
        printf("Done\n");
        return 0;
    }

    int fs[2]; //first -> second
    int sf[2]; //second -> first
    pipe(fs);
    pipe(sf);

    int n = 1;
    pid_t child = fork();

    if (child == -1) {
        return 1;
    }

    if (child) {
        n = 2;
        if ( ( child = fork() ) == -1) {
            wait(NULL);
            return 1;
        }
   
        if (child) {
            close(fs[0]);
            close(fs[1]);
            close(sf[0]);
            close(sf[1]);
            wait(NULL);
            wait(NULL);
            printf("Done\n");
            return 0;
        }
    }

    FILE *in = n == 1 ? fdopen(sf[0], "r") : fdopen(fs[0], "r");
    FILE *out = n == 1 ? fdopen(fs[1], "w") : fdopen(sf[1], "w");

    close(fs[n - 1]);
    close(sf[n % 2]);
 
    int cnt = 1;
    while (1) {
        if (n == 2 && (fscanf(in, "%d", &cnt) == EOF || cnt == max) ) {
            break;
        }

        printf("%d %d\n", n, cnt++);
        fflush(stdout); 
        fprintf(out, "%d\n", cnt);
        fflush(out); 

        if (n == 1 && (fscanf(in, "%d", &cnt) == EOF || cnt == max) ) {
            break;
        }
    }

    close(fs[n % 2]);
    close(sf[n - 1]);
    return 0;
}
