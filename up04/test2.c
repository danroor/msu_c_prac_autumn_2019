#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <errno.h>
#include <stdint.h>

int main(void)
{
    int fd = open("input.bin", O_WRONLY);

    int a[39] = 
    {  7,  1, 2, 
/*1 */ 2,  6, 3,
/*2 */ 12, 5, 0, 
/*3 */ 5, 12, 4,
/*4 */ 6,  0, 0,
/*5 */ 8,  0,10,
/*6 */ 0,  0, 7,
/*7 */ 1,  0, 0,
/*8 */ 11, 9, 0,
/*9 */ 10, 0, 0,
/*10*/ 9,  0, 8,
/*11*/ 4,  0, 0,
/*12*/ 3,  0,11};

    write(fd, a, sizeof(a));
    close(fd);
    return 0;
}
