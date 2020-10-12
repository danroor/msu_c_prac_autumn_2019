#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include <stdio.h>
#include <limits.h>
#include <stdint.h>
#include <ctype.h>

enum { BUFSIZE = 2 };

int
main(int argc, char *argv[])
{
/*
    int fd = open("out.bin", O_WRONLY | O_CREAT, S_IREAD | S_IWRITE);
    char s[16] = "abcdabcdabcdabcd";
    //write(fd, s, sizeof(s));
    unsigned char l, m;
    unsigned short a = 1;
    
    l = a;
    m = a >> 8u;
    a = (l << 8u) + m;
    write(fd, &a, sizeof(a));
    

    a = 7;
    l = a;
    m = a >> 8u;
    a = (l << 8u) + m;
    write(fd, &a, sizeof(a));

    a = 1;
    l = a;
    m = a >> 8u;
    a = (l << 8u) + m;
    write(fd, &a, sizeof(a));

    a = 0;
    l = a;
    m = a >> 8u;
    a = (l << 8u) + m;
    write(fd, &a, sizeof(a));

    a = 1;
    l = a;
    m = a >> 8u;
    a = (l << 8u) + m;
    write(fd, &a, sizeof(a));

    a = USHRT_MAX - 1;
    l = a;
    m = a >> 8u;
    a = (l << 8u) + m;  
    write(fd, &a, sizeof(a));
    printf("%x\n", a);

    a = 1;
    l = a;
    m = a >> 8u;
    a = (l << 8u) + m;
    write(fd, &a, sizeof(a));

    a = 9;
    l = a;
    m = a >> 8u;
    a = (l << 8u) + m;
    write(fd, &a, sizeof(a));
    printf("%x\n", a);
/*
    uint16_t x = 1;

    if (*( (uint8_t *) &x)) printf("little\n"); else printf("big\n");
*/


    printf("%d\n", isspace('2'));

    //close(fd);
    return 0;
}
