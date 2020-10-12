#include <unistd.h>
#include <sys/wait.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include <fcntl.h>

enum { RIGHTS = S_IRUSR | S_IWUSR };

int
main(int argc, char *argv[])
{
    int x;
    while (scanf("%d", &x) > 0) 

    printf("%d\n", x);    
}
