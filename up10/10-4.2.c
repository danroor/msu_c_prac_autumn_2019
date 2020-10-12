#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <sys/types.h>

int
launch(char *exe, int in, int out)
{
    pid_t pid = fork();
    if (pid == -1) {
        return 1;
    }

    if (!pid) {
        if (in != STDIN_FILENO) {
            if (dup2(in, STDIN_FILENO) == -1) {
                _exit(1);
            }
        }
        if (out != STDOUT_FILENO) {
            if (dup2(out, STDOUT_FILENO) == -1) {
                _exit(1);
            }
        }
        if (close(in) == -1 || close(out) == -1) {
            _exit(1);
        }

        execlp(exe, exe, NULL);
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
    pid_t pid;
    int status, ret = 0;

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
        return launch(argv[1], STDIN_FILENO, STDOUT_FILENO);
    }

    int p[2][2];
    pipe(p[0]);
    pipe(p[1]);
    
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
        close(p[0][1]);
        return 0;
    } 

    pid_t *child = calloc(argc - 1, sizeof(*child));
    child[0] = pid;
    
    for (int i = 2; i < argc - 1; ++i) {
        if ( (pid = fork()) == -1) {
            for (int j = 0; j < i - 1; ++j) {
                kill(child[j], SIGKILL);
                free(child);
                close(p[i % 2][0]);
                close(p[i % 2][1]);
                close(p[!(i % 2)][0]);
                close(p[!(i % 2)][1]);
                return 1;
            }
        }
        if (!pid) {
            free(child);
            close(p[i % 2][1]);
            close(p[!(i % 2)][0]);
            launch(argv[i], p[i % 2][0], p[!(i % 2)][1]);
            close(p[i % 2][0]);
            close(p[!(i % 2)][1]);
            return 0;
        }
        child[i - 1] = pid;
        close(p[i % 2][0]); 
        close(p[i % 2][1]);
        pipe(p[i % 2]);
    }

    close(p[(argc - 1) % 2][1]); 
    close(p[!((argc - 1) % 2)][0]);
    close(p[!((argc - 1) % 2)][1]);

    if ( (pid = fork()) == -1) {
        for (int j = 0; j < argc - 2; ++j) {
            kill(child[j], SIGKILL);
            free(child);
            close(p[i % 2][0]);
            close(p[i % 2][1]);
            close(p[!(i % 2)][0]);
            close(p[!(i % 2)][1]);
            return 1;
        }
    }
    if (!pid) {
        launch(argv[i], p[(argc - 1) % 2][0], STDOUT_FILENO);
        close(p[(argc - 1) % 2][0]);
        return 0;
    }
    
    for (int i = 1; i < argc; ++i) {
        wait(NULL);
    }

    return 0;
}
