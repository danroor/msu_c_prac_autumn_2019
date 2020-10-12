#include <unistd.h>
#include <sys/wait.h>
#include <fcntl.h>

//0 - successfully finished
//1 - otherwise
int
launch(char *exe, int in, int out)
{
    pid_t pid = fork();
    if (pid == -1) {
        return 1;
    }

    if (!pid) {
        if (in != STDIN_FILENO) {
            dup2(in, STDIN_FILENO);
            close(in);
        }
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
    
    pid_t pid = fork();
    if (pid == -1) {
        return 1;
    }

    if (!pid) {
        close(p[0]);

        int fd = open(argv[4], O_RDONLY);
        if (fd == -1) {
            close(p[1]);
            return 1;
        }

        if (launch(argv[1], fd, p[1])) {
            close(fd);
            close(p[1]);
            return 1;
        }
        launch(argv[2], STDIN_FILENO, p[1]);

        close(fd);
        close(p[1]); 
        return 0;
    }

    //the father process
    close(p[1]);

    int fd = open(argv[5], O_WRONLY | O_APPEND | O_CREAT, S_IRUSR | 
            S_IWUSR | S_IRGRP | S_IWGRP);

    if (fd == -1) {
        close(p[0]);
        wait(NULL);
        return 0;
    }
    launch(argv[3], p[0], fd);
  
    close(fd);
    close(p[0]);
    wait(NULL);
    return 0;
}
