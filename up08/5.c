#include <unistd.h>
#include <sys/wait.h>
#include <stdio.h>

int
main(void)
{
    pid_t pid;
    int status, x, flag = 0; //flag == 0 if the current process
                             //is the parent process
    
    while (scanf("%d", &x) == 1) {
        pid = fork();
        if (pid == -1) {
            printf("-1\n");
            return 1;
        }
        if (pid) {
            wait(&status);

            if (!WIFEXITED(status)) {
                return 1;
            }
            if (!WEXITSTATUS(status)) {
                printf("%d\n", x);
            }
 
            return WEXITSTATUS(status) & flag;
        }
        flag = 1;
    }

    return 0;
}
