#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>

void
launch(char *exe, int in, int out)
{
    if (in != STDIN_FILENO) {
        if (dup2(in, STDIN_FILENO) == -1) {
            close(in);
            close(out);
            return;
        }
        if (close(in) == -1) {
            close(out);
            return;
        }
    }
    if (out != STDOUT_FILENO) {
        if (dup2(out, STDOUT_FILENO) == -1) {
            close(in);
            close(out);
            return;
        }
        if (close(out) == -1) {
            close(in);
            return;
        }
    }

    execlp(exe, exe, NULL);
    _exit(1);
}

void
killexit(int ret, pid_t *child, int n)
{
    for (int i = 0; i < n; ++i) {
        kill(child[i], SIGKILL);   
    }   
    free(child);
    _exit(ret);
}

int 
main(int argc, char *argv[])
{
    pid_t pid;
    if (argc == 1) {
        return 0;
    }
    if (argc == 2) {
        pid = fork();
        if (pid == -1) {
            return 1;
        }
        if (pid) {
            wait(NULL);
            return 0;
        } 
        //son
        launch(argv[1], STDIN_FILENO, STDOUT_FILENO);
        return 0;
    }

    int p[2][2];
    if (pipe(p[0]) == -1 || pipe(p[1]) == -1) {
        return 1;
    }
    
    pid = fork();
    if (pid == -1) {
        close(p[0][0]);
        close(p[0][1]);
        close(p[1][0]);
        close(p[1][1]);
        return 1;
    }
    if (!pid) {
        close(p[0][0]);
        close(p[1][0]);
        close(p[1][1]);
        launch(argv[1], STDIN_FILENO, p[0][1]);
        return 0;
    } 

    pid_t *child = calloc(argc - 1, sizeof(*child));
    child[0] = pid;
    
    for (int i = 2; i < argc - 1; ++i) {
        if ( (pid = fork()) == -1) {
            close(p[0][0]);
            close(p[0][1]);
            close(p[1][0]);
            close(p[1][1]);
            killexit(1, child, i - 1);
        }
        if (!pid) {
            free(child);
            close(p[i % 2][1]);
            close(p[!(i % 2)][0]);
            launch(argv[i], p[i % 2][0], p[!(i % 2)][1]);
        }
        child[i - 1] = pid;

        if (close(p[i % 2][0]) == -1) {
            close(p[i % 2][1]);
            close(p[!(i % 2)][0]);
            close(p[!(i % 2)][1]);
            killexit(1, child, i - 1);
        }

        if (close(p[i % 2][1]) == -1 || pipe(p[i % 2]) == -1) {
            close(p[!(i % 2)][0]);
            close(p[!(i % 2)][1]);
            killexit(1, child, i - 1);
        }
    }

    if (close(p[(argc - 1) % 2][1]) == -1) {
        close(p[!((argc - 1) % 2)][0]);
        close(p[!((argc - 1) % 2)][1]);
        killexit(1, child, argc - 2);
    }
    if (close(p[!((argc - 1) % 2)][0]) == -1) {
        close(p[!((argc - 1) % 2)][1]);
        killexit(1, child, argc - 2);
    }
    if (close(p[!((argc - 1) % 2)][1]) == -1) {
        killexit(1, child, argc - 2);
    }

    if ( (pid = fork()) == -1) {
        close(p[(argc - 1) % 2][0]);
        killexit(1, child, argc - 2);
    }
    
    child[argc - 2] = pid;

    if (!pid) {
        free(child);
        launch(argv[argc - 1], p[(argc - 1) % 2][0], STDOUT_FILENO);
    }

    if (close(p[(argc - 1) % 2][0]) == -1) {
        killexit(1, child, argc - 1);
    }
    
    free(child);
    for (int i = 1; i < argc; ++i) {
        wait(NULL);
    }

    return 0;
}
