#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <errno.h>
#include <stdint.h>

int main(void)
{
    int fd = open("input.bin", O_WRONLY);

    int a[12] = 
    {  3, 2, 0, 
       0, 0, 0, 
       1, 1, 3, 
       2, 0, 0};

    write(fd, a, sizeof(a));
    close(fd);
    return 0;
}
