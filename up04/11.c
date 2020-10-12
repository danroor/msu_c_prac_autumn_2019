#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>

enum
{
    FORMAT1 = 100,
    FORMAT2 = 10,
    RECORD = 20,
    INTMIN = -0x7fffffff - 1
};

int
main(int argc, char *argv[])
{
    int price, flag = 0;
    int max1 = INTMIN, max2 = INTMIN;
    
    int f;
    int a[5];
    int ind = RECORD / sizeof(*a) - 1;
  
    for (int i = 1; i < argc; ++i) {
        printf("%s\n", argv[i]);
        f = open(argv[i], O_RDONLY);
    
        if (f > -1) {    
            while (read(f, a, sizeof(char) * RECORD) == RECORD) {    
                price = a[ind];

                if (price == INTMIN) {
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

    if (max1 != max2 && (max2 != INTMIN || flag)) {
        flag = 0;
        if (max2 == INTMIN) {
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
