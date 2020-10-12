#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <fcntl.h>

enum { RIGHTS = 0666 };

int main(int argc, char *argv[])
{
    int conv[2];
    if (pipe(conv) < 0) {
        return 1;
    }
    int pid1 = fork();
    if (pid1 < 0) {
        return 1;
    } else if (pid1 == 0) {
        //syna_1
        if (dup2(conv[1], STDOUT_FILENO) < 0) {
            return 1;
        }
        if (close(conv[0]) < 0 || close(conv[1]) < 0) {
            return 1;
        }
        int stat1 = 0;
        int pid11 = fork();
        if (pid11 < 0) {
            return 1;
        } else if (pid11 == 0) {
            //vnuk1
            int fdin = open(argv[4], O_RDONLY);
            if (fdin < 0) {
                return 1;
            }
            if (dup2(fdin, STDIN_FILENO) < 0) {
                return 1;
            }
            if (close(fdin) < 0) {
                return 1;
            }
            ///execlp("/bin/sh", "sh", "-c", argv[1], NULL);
            execlp(argv[1], argv[1], NULL);
            _exit(1);
        } else {
            waitpid(pid11, &stat1, 0);
            if (WIFEXITED(stat1) && WEXITSTATUS(stat1) == 0) {
                int pid12 = fork();
                if (pid12 < 0) {
                    return 1;
                } else if (pid12 == 0) {
                    //vnuk2
                    ///execlp("/bin/sh", "sh", "-c", argv[2], NULL);
                    execlp(argv[2], argv[2], NULL);
                    _exit(1);
                } else {
                    waitpid(pid12, &stat1, 0);
                }
            }
            return 0;
        }
    } else {
        //oteccc
        int pid2 = fork();
        if (pid2 < 0) {
            return 1;
        } else if (pid2 == 0) {
            //syna2
            int fdout = open(argv[5], O_WRONLY | O_CREAT | O_APPEND, RIGHTS);
            if (fdout < 0) {
                return 1;
            }
            if (dup2(conv[0], STDIN_FILENO) < 0 || dup2(fdout, STDOUT_FILENO) < 0) {
                return 1;
            }
            if (close(conv[0]) < 0 || close(conv[1]) < 0 || close(fdout) < 0) {
                return 1;
            }
            ///execlp("/bin/sh","sh", "-c", argv[3], NULL);
            execlp(argv[3], argv[3], NULL);
            _exit(1);
        } else {
            //oteccc
            int st1 = 0, st2 = 0;
            waitpid(pid2, &st2, 0);
            waitpid(pid1, &st1, 0);
            if (close(conv[0]) < 0 || close(conv[1]) < 0) {
                return 1;
            }
            return 0;
        }
    }
    return 0;
}
