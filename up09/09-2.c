#include <unistd.h>
#include <sys/wait.h>

enum 
{
    RETSIG = 128,
    CANNOT_LAUNCH_SHELL = 127
};

int
mysys(const char *str)
{
    pid_t pid;

    if ( (pid = fork() ) == -1) {
        return -1;
    }

    if (!pid) {
        execlp("/bin/sh", "sh", "-c", str, NULL);
        _exit(CANNOT_LAUNCH_SHELL);
    }

    int status;
    waitpid(pid, &status, WUNTRACED);
    
    if (WIFEXITED(status)) {
        return WEXITSTATUS(status);
    }

    if (WIFSIGNALED(status)) {
        return RETSIG + WTERMSIG(status);
    }
    
    return 0;
}
