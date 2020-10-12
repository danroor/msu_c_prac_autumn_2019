#include <unistd.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <stdio.h>

int main(void)
{
    //dup2(STDIN_FILENO, STDIN_FILENO);
    close(STDIN_FILENO);
    printf("hello\n");
}
