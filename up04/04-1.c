#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <limits.h>

enum
{
    FORMAT1 = 100,
    FORMAT2 = 10,
    RECORD = 20,
    BITS_IN_BYTE = 8
};

int
main(int argc, char *argv[])
{
    int price, flag = 0;
    int max1 = INT_MIN, max2 = INT_MIN;
    
    int f;
    unsigned char a[RECORD];
  
    for (int i = 1; i < argc; ++i) {
        f = open(argv[i], O_RDONLY);
    
        if (f > -1) {    
            while (read(f, a, sizeof(a)) == RECORD) {    
                price = a[RECORD - 1] << BITS_IN_BYTE * 3;
                price += a[RECORD - 2] << (BITS_IN_BYTE << 1);
                price += a[RECORD - 3] << BITS_IN_BYTE;
                price += a[RECORD - 4];

                if (price == INT_MIN) {
                    flag = 1;
                } else if (price > max1) {
                    max2 = max1;
                    max1 = price;
                } else if (price > max2 && price != max1) {
                    max2 = price;
                }
            }

            close(f);
        }
    };

    if (max1 != max2 && (max2 != INT_MIN || flag)) {
        flag = 0;
        if (max2 == INT_MIN) {
            flag = 1;
            max2++;
        }
        if (max2 < 0) {
            printf("-");
            max2 = -max2;
        }
        printf("%d.", max2 / FORMAT1);
        
        max2 %= FORMAT1;
        if (max2 + flag < FORMAT2) {
            printf("0%d\n", max2 + flag);
        } else {
            printf("%d\n", max2 + flag);
        }
    }
    
    return 0;
}
