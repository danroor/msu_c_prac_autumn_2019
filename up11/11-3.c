#include <unistd.h>
#include <sys/wait.h>
#include <dirent.h>
#include <stdio.h>
#include <string.h>

int
main(int argc, char *argv[])
{
    pid_t pid;
    FILE *f;
    int n, status, norm = 0;
    char cmd[PATH_MAX];
    sscanf(argv[1], "%d", &n);
    
    for (int i = 0; i < n; ++i) {
        if (i + 2 >= argc) {
            break;
        }
        pid = fork();
        if (pid < 0) {
            return 1;
        }
        if (!pid) {
            f = fopen(argv[i + 2], "r");
            if (f == NULL) {
                return 1;
            }
            
            fgets(cmd, PATH_MAX, f);
            fclose(f);
            cmd[strlen(cmd) - 1] = '\0'; //remove \n
            
            execlp(cmd, cmd, NULL);
            return 1;
        }
    }
    
    for (int i = 0; i < n; ++i) {
        if (wait(&status) != -1) {
            norm += WIFEXITED(status) && !WEXITSTATUS(status);
        }
    }
    
    for (int i = n + 2; i < argc; ++i) {
        pid = fork();
        if (pid < 0) {
            return 1;
        }
        if (!pid) {
            f = fopen(argv[i], "r");
            if (f == NULL) {
                return 1;
            }
            
            fgets(cmd, PATH_MAX, f);
            fclose(f);
            cmd[strlen(cmd) - 1] = '\0'; //remove \n
             
            execlp(cmd, cmd, NULL);
            return 1;
        }
        wait(&status);
        norm += WIFEXITED(status) && !WEXITSTATUS(status);
    }
    
    printf("%d\n", norm);

    return 0;
}
