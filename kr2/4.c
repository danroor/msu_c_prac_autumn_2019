#include <unistd.h>
#include <sys/wait.h>
#include <string.h>
#include <stdio.h>
int
launch(char *exe, char *argv[])
{ 
    printf("%s\n", argv[0]);
    pid_t pid = fork();
    if (pid == -1) {
        return 1;
    }

    if (!pid) {
        execvp(exe, argv);
        _exit(1);
        //the son process (not the launch function)
        //returns 1
    }
    
    int status;
    wait(&status);
    if (WIFEXITED(status)) {
        return WEXITSTATUS(status);
    }

    return 1;
}

int
main(int argc, char *argv[])
{
    printf("%d\n", argc);
    //printf("%s\n", argv[2]);
    for (int i = argc - 1; i > 1; --i) {
        printf("%d\n", i);
        //printf("%s\n", argv[i]);
        fflush(stdout);
        if (strcmp(argv[i], argv[1]) == 0) {
            printf("%s\n", argv[i + 4]);
            if (launch(argv[i + 1], argv + i)) {
                return 1;
            }
        }

        argv[i] = NULL;
    }

    return 0;
}
