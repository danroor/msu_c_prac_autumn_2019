#include <unistd.h>
#include <stdio.h>
#include <string.h>

enum { BUFSZ = 50000 };

int main(void)
{
    int fd[2];
    pipe(fd);

    char buf[BUFSZ];
  
    memset(buf, 1, sizeof(buf));

    int r = write(fd[1], buf, sizeof(buf));
    perror("Pipe");
    printf("%d\n", r);  

    if (!fork()) {
        printf("Father is dead\n");
        return 0;
    }

    r = write(fd[1], buf, sizeof(buf));
    perror("Pipe");
    printf("%d\n", r);    

    return 0;
}
