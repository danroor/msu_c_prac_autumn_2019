#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include <stdio.h>
#include <limits.h>

enum { BUFSIZE = 2 };

int
main(int argc, char *argv[])
{
    int fd = INT_MAX;
    printf("%d\n", fd+1);
}
