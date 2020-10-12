#include <stdio.h>
#include <unistd.h>

enum
{
    MAXBUF = 16,
    BASE = 10
};

int
main(int argc, char *argv[])
{
    unsigned long long sum = 0, cur = 0;

    int sign = 1;
    int bufsize = MAXBUF;

    char buf[MAXBUF];
    char *s;

    while (( bufsize = read(STDIN_FILENO, buf, sizeof(buf)) ) > 0) {
        for (s = buf; s < buf + bufsize; ++s) {
            if (*s >= '0' && *s <= '9') {
                cur = cur * BASE + (*s - '0');
            } else if (*s == '+') {
                sign = 1;
            } else if (*s == '-') {
                sign = -1;
            } else {
                sum += cur * sign;
                cur = 0;
                sign = 1;
            }
        }
    }

    sum += cur * sign;

    printf("%lld\n", (long long) sum);
    
    return 0;
}
