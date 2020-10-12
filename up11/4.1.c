#include <unistd.h>
#include <stdio.h>
#include <sys/wait.h>

volatile int myturn = 0;

void
handler(int sig)
{
    if (sig == SIGUSR1) {
        myturn++;
    }
}

int main(int argc, char *argv[])
{
    struct sigaction sa;
    sa.sa_handler = handler;
    sa.sa_flags = SA_RESTART;
    sigemptyset(&sa.sa_mask);
    sigaction(SIGUSR1, &sa, NULL);

    int max;
    sscanf(argv[1], "%d", &max);

    if (max <= 1) {
        printf("Done\n");
        return 0;
    }

    int p[2];
    pipe(p);

    int n = 1, cnt = 1;
    pid_t pid = fork();
    pid_t pid2;

    if (pid == -1) {
        close(p[0]);
        close(p[1]);
        return 1;
    }

    if (pid) {
        n = 2;
        if ( ( pid2 = fork() ) == -1) {
            close(p[0]);
            close(p[1]);
            kill(pid, SIGKILL);
            wait(NULL);
            return 1;
        }
   
        if (pid2) {
            close(p[0]);
            close(p[1]);

            if (wait(NULL) == pid) {
                kill(pid2, SIGKILL);
            } else {
                kill(pid, SIGKILL);
            }

            wait(NULL);
            printf("Done\n");
            return 0;
        }
    }

    FILE *in = fdopen(p[0], "r");
    FILE *out = fdopen(p[1], "w");

    if (n == 1) {
        fscanf(in, "%d", &pid);
        fprintf(out, "%d\n", getpid());
    } else {
        fprintf(out, "%d\n", getpid());
        fflush(out);
        while (!myturn) {
            pause();
        } 
        myturn = 0;
        
        fscanf(in, "%d", &pid);
        fscanf(in, "%d", &cnt);
    }
 
    while (cnt < max) {
        printf("%d %d\n", n, cnt++);
        fprintf(out, "%d\n", cnt);
        fflush(stdout);
        fflush(out);

        kill(pid, SIGUSR1);
        while (!myturn) {
            pause();
        } 
        myturn = 0;
        
        fscanf(in, "%d", &cnt);
    }

    close(p[0]);
    close(p[1]);
    return 0;
}
