#include <unistd.h>
#include <fcntl.h>

int main(void) {
    int fd = open("i.txt", O_WRONLY);
    int a = 1;
    for (int i = 0; i < 34000; ++i) {
        write(fd, &a, sizeof(a));
    }
    return 0;
}
