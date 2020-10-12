#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <limits.h>

enum { BITS_IN_BYTE_POW = 3u }; // 2^3 = 8 bits in byte

int
main(int argc, char *argv[])
{
    unsigned short min = USHRT_MAX; //USHRT_MAX % 2 == 1
    unsigned short x;
    unsigned char buf[2];

    int f = open(argv[1], O_RDONLY);

    if (f > -1) {    
        while (read(f, &buf, sizeof(buf)) == sizeof(buf)) { 
            x = ( (unsigned short) buf[0] << 
                    ( (unsigned short) sizeof(buf[0]) << BITS_IN_BYTE_POW ) ) + buf[1];

            if ( (x & 1) == 0 && x < min) {
                min = x;
            }
        }
        close(f);
    }
    
    if (min != USHRT_MAX) {
        printf("%hu\n", min);
    }

    return 0;
}
