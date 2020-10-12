#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <errno.h>

int
launch(char *exe, int in, int out)
{
    pid_t pid = fork();
    if (pid == -1) {
        return 1;
    }

    if (!pid) {
        dup2(in, STDIN_FILENO);
        close(in);
        dup2(out, STDOUT_FILENO);
        close(out);
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
    int p[2];
    pipe(p);

    int fd = open(argv[5], O_WRONLY | O_APPEND | O_CREAT | O_EXCL, S_IRUSR |
            S_IWUSR | S_IRGRP);
    if (fd == -1) {
        if (errno != EEXIST) {
            return 1;
        }
        errno = 0;
        fd = open(argv[5], O_WRONLY | O_APPEND);
    }

    pid_t pid = fork();
    if (pid == -1) {
        close(p[0]);
        close(p[1]);
        close(fd);
        return 1;
    } else if (!pid) {
        close(p[1]);
        if (launch(argv[2], p[0], fd)) {
            launch(argv[3], p[0], fd);
        }
 
        close(fd);       
        close(p[0]);  
        return 0;
    }

    close(p[0]);
    close(fd);

    fd = open(argv[4], O_RDONLY);
    launch(argv[1], fd, p[1]);  

    close(fd);
    close(p[1]);  

    wait(NULL);
    return 0;
}
