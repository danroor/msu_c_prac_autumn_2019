#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include <stdio.h>

enum { BUFSIZE = 2 };

int
main(int argc, char *argv[])
{
    unsigned short x;
    int fd = open(argv[1], O_WRONLY | O_CREAT | O_TRUNC, S_IWRITE | S_IREAD);

    unsigned char least, most;
    while (scanf("%hu", &x) == 1) {
        least = x;
        most = x >> ((unsigned short) sizeof(x) << 2u);

        x = least << ((unsigned short) sizeof(x) << 2u);
        x += most;
        write(fd, &x, BUFSIZE);
    }

    close(fd);
    return 0;
}
