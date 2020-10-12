#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <limits.h>

enum
{
    MAXSIZE = 5000, 
    POW = 1000000000ull, //10^9
    BASE = 10ull,
    NUMDIGITS = 9,
    GROUPBITS = 32ull
};

// 50000 / 9 = 5000 (approximately)
//unsigned long long numbers contain 
//20 decimal digits or less
//We are going to divide the number into
//several parts. Each part contains 
//9 decimal digits or less

int
main(void)
{
    unsigned int prec;
    scanf("%u", &prec);

    char c = getchar();

    while (isspace(c)) {
        c = getchar();
    }

    unsigned long long *a = calloc(MAXSIZE, sizeof(*a));
    int last = 0;

    short cnt = 1;
    a[last] = c - '0';

    c = getchar();
    while (c >= '0' && c <= '9') {
        if (cnt == NUMDIGITS) {
            last++;
            cnt = 0;
        }

        cnt++;
        a[last] *= BASE;
        a[last] += c - '0';
        c = getchar();
    }

    unsigned long long powlast = BASE;
    for (int i = 1; i < cnt; ++i) {
        powlast *= BASE;
    }
    
    unsigned int *res = 
            calloc(prec / GROUPBITS + (prec % GROUPBITS > 0), sizeof(*res));
    
    int ind = 0;

    unsigned long long carry = 0, buf;
    unsigned int prec4 = prec / GROUPBITS;

    for (unsigned int i = 0; i < prec4; i++) {
        buf = ((a[last] << GROUPBITS) + carry) / powlast;
        a[last] = ((a[last] << GROUPBITS) + carry) % powlast;
        carry = buf;

        for (int j = last - 1; j >= 0; --j) {
            buf = ((a[j] << GROUPBITS) + carry) / POW;
            a[j] = ((a[j] << GROUPBITS) + carry) % POW;
            carry = buf;
        }

        res[ind++] = carry;
        carry = 0;
    }

    prec4 = prec % GROUPBITS;

    if (!prec4) {
        ind--;
    }

    for (unsigned int i = 0; i < prec4; ++i) {

        buf = ((a[last] << 1ull) + carry) / powlast;
        a[last] = ((a[last] << 1ull) + carry) % powlast;
        carry = buf;

        for (int j = last - 1; j >= 0; --j) {
            buf = ((a[j] << 1ull) + carry) / POW;
            a[j] = ((a[j] << 1ull) + carry) % POW;
            carry = buf;
        }

        res[ind] <<= 1u;
        res[ind] += carry;
        carry = 0;
    }
    
    //get the bit following the first prec bits
    //and check whether there are more non-zero bits
    //after it
    buf = ((a[last] << 1ull) + carry) / powlast;
    a[last] = ((a[last] << 1ull) + carry) % powlast;
    carry = buf;
    
    cnt = a[last] == 0;

    for (int j = last - 1; j >= 0; --j) {
        buf = ((a[j] << 1ull) + carry) / POW;
        a[j] = ((a[j] << 1ull) + carry) % POW;
        carry = buf;

        cnt &= a[j] == 0;
    }

    unsigned int shift = prec % GROUPBITS;
    
    if (carry) {
        if (cnt) {
            if (res[ind] & 1) {
                if (shift) {
                    carry = res[ind] == (1u << shift) - 1u;
                } else {
                    carry = res[ind] == UINT_MAX;
                }
                res[ind]++;
                for (int i = ind - 1; i >= 0; --i) {
                    buf = res[i] + carry;
                    carry &= res[i] == UINT_MAX;
                    res[i] = buf;
                }

                if (carry) {
                    printf("1 ");
                }
            }
        } else {
            if (shift) {
                carry = res[ind] == (1u << shift) - 1u;
            } else {
                carry = res[ind] == UINT_MAX;
            }
            res[ind]++;
            for (int i = ind - 1; i >= 0; --i) {
                buf = res[i] + carry;
                carry &= res[i] == UINT_MAX;
                res[i] = buf;
            }

            if (carry) {
                printf("1 ");
            }
        }
    }  

    if (shift) {
        res[ind] <<= GROUPBITS - shift;
    }

    for (int i = 0; i <= ind; ++i) {
        printf("%08x ", res[i]);
    }
    printf("\n");
    
    free(a);
    free(res);
    return 0;
}
