#include <fcntl.h>
#include <unistd.h>
#include <limits.h>
#include <stdio.h>

int
main(int argc, char *argv[])
{ 
    int fd = open(argv[1], O_RDWR);
    if (fd == -1) {
        printf("Error: unable to open file\n");
        return 1;
    }

    int minpos = 0, i = 0;
    long long min = LLONG_MAX;
    long long x;

    int notempty = read(fd, &x, sizeof(x)) > 0;

    if (x < min) {
        min = x;
        minpos = i;
    }
    i++;
    while (read(fd, &x, sizeof(x)) > 0) {
        if (x < min) {
            min = x;
            minpos = i;
        }
        i++;
    }

    if (notempty && min != LLONG_MIN) {
        min = -min;
        lseek(fd, minpos * sizeof(min), SEEK_SET);
        write(fd, &min, sizeof(min));
    }
    
    close(fd);
    return 0;
}
