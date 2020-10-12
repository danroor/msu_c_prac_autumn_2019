#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>

unsigned long long hash_function(unsigned long long);

int
main(int argc, char *argv[])
{
    unsigned long long h, count;
    sscanf(argv[2], "%llx", &h);
    sscanf(argv[3], "%llu", &count);

    int fd = open(argv[1], O_WRONLY | O_CREAT, 
            S_IWRITE | S_IREAD | S_IRGRP | S_IWGRP);
    if (fd == -1) {
        printf("Error: unable to open file\n");
        return 1;
    }

    if (count == 0) {
        close(fd);
        return 0;
    }

    long size = lseek(fd, 0, SEEK_END);
    for (int i = 0; i < count; ++i) {
        lseek(fd, size + (count - i - 1) * sizeof(h), SEEK_SET);
        write(fd, &h, sizeof(h));
        h = hash_function(h);
    }

    close(fd);
    return 0;
}
