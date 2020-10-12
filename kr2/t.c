#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>

int main(void) {
    int n = 2;
    int fd = open("t.txt", O_WRONLY | O_TRUNC);
    write(fd, &n, sizeof(n));
    return 0;
}
