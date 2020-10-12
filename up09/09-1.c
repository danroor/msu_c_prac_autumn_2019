#include <unistd.h>
#include <stdio.h>
#include <sys/wait.h>
#include <fcntl.h>

enum { ERROR = 42 };

int
main(int argc, char *argv[])
{
    if (!fork()) {
        int f1 = open(argv[2], O_RDONLY);
        int f2 = open(argv[3], O_WRONLY | O_CREAT | O_APPEND, S_IRUSR | S_IWUSR | 
                S_IRGRP | S_IWGRP);
        int f3 = open(argv[4], O_WRONLY | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR | 
                S_IRGRP | S_IWGRP);
        
        if (f1 == -1 || f2 == -1 || f3 == -1) {
            return ERROR;
        }
        
        if (dup2(f1, STDIN_FILENO) == -1) return ERROR;
        if (dup2(f2, STDOUT_FILENO) == -1) return ERROR;
        if (dup2(f3, STDERR_FILENO) == -1) return ERROR;
        
        if (close(f1) == -1) return ERROR;
        if (close(f2) == -1) return ERROR;
        if (close(f3) == -1) return ERROR;
    
        execlp(argv[1], argv[2], argv[3], argv[4], (char *) NULL);
        
        return ERROR;
    }
    
    int status;
    wait(&status);
    printf("%d\n", status);

    return 0;
}
