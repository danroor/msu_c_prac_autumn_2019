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
    sigset_t mask;
    sigemptyset(&mask);
    sigaddset(&mask, SIGUSR1);
    sigprocmask(SIG_BLOCK, &mask, NULL);

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
    pid_t pid2, second;

    if (pid == -1) {
        close(p[0]);
        close(p[1]);
        return 1;
    }

    if (pid) {
        n = 2;
        if ( ( pid2 = fork() ) == -1) {
            close(p[0]);
            max = -1;
            write(p[1], &max, sizeof(max)); //pid < 0 for the first process (line 86)
            close(p[1]);
            wait(NULL);
            return 1;
        }
   
        if (pid2) {
            close(p[0]);
            second = wait(NULL); //first finished
            second = second == pid ? pid2 : pid; //second (still running)

            FILE *out = fdopen(p[1], "w");
            fprintf(out, "%d\n", max);
            fflush(out);
            close(p[1]);
            kill(second, SIGUSR1); 

            wait(NULL);
            printf("Done\n");
            return 0;
        }
    }

    sigemptyset(&mask);

    myturn = 0;

    FILE *in = fdopen(p[0], "r");
    FILE *out = fdopen(p[1], "w");

    if (n == 1) {
        fscanf(in, "%d", &pid);
        if (pid < 0) { //process 2 wasn't created
            close(p[0]);
            close(p[1]);
            return 1;
        }
    } else {
        fprintf(out, "%d\n", getpid());
        fflush(out);
        while (!myturn) {
            sigsuspend(&mask);
        } 
        myturn = 0;
        
        //process 2 knows the first process' PID (pid variable)
        fscanf(in, "%d", &cnt);
    }
 
    while (cnt < max) {
        printf("%d %d\n", n, cnt++);
        fprintf(out, "%d\n", cnt);
        fflush(stdout);
        fflush(out);

        kill(pid, SIGUSR1);
        while (!myturn) {
            sigsuspend(&mask);
        } 
        myturn = 0;
        
        fscanf(in, "%d", &cnt);
    }

    close(p[0]);
    close(p[1]);
    return 0;
}
