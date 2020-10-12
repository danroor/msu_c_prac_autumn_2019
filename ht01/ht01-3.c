#include <stdio.h>

enum
{
    MASK = 0x80000000u,     //first bit indicator
    MANTLEN = 23u           //length of mantissa (bits)
};

int
float_fit(unsigned int x)
{
    while(x && (x & MASK) == 0) {
        x <<= 1u;
    }

    x <<= 1u + MANTLEN;

    return x == 0;
}

int
main(void)
{
    unsigned int n;

    while(scanf("%u", &n) == 1) {
        printf("%d\n", float_fit(n));
    }

    return 0;
}
