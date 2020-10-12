#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <errno.h>
#include <limits.h>

enum
{
    SPACEONLY = 0x0bad1dea,
    EXTRASYM = 0xbedabeda,
    BASE = 10
};

char *
getline2(FILE *f);

int
main(void)
{
    int strnum = 1, num = 0, sign = 1;
    unsigned int sum = 0;
    long curnum = 0;

    char *start = NULL, *end = NULL;
    char *st = getline2(stdin);
    char *s;
    
    while (st != NULL) {
        s = st;
        while (*s) {
            if (isspace((unsigned char) *s)) {
                if (start != NULL) {
                    end = s - 1; 
                    if (*end == '+' || *end == '-') {
                        sum = EXTRASYM + strnum;
                        num = 1;
                        start = NULL;
                        break;
                    }
                    curnum = strtol(start, &end, BASE);
                    num++;
                    if (errno == ERANGE || curnum > INT_MAX || curnum < INT_MIN) {
                        curnum = sign * num;
                        errno = 0;
                    }
                    sum += curnum;
                    sign = 1;
                    start = NULL;
                }
            } else if (*s >= '0' && *s <= '9') {
                if (start == NULL) {
                    start = s;
                }
            } else if (*s == '+') {
                sign = 1;
                if (start == NULL) {
                    start = s;
                } else {
                    sum = EXTRASYM + strnum;
                    num = 1;
                    start = NULL;
                    break;
                }
            } else if (*s == '-') {
                sign = -1;
                if (start == NULL) {
                    start = s;
                } else {
                    sum = EXTRASYM + strnum;
                    num = 1;
                    start = NULL;
                    break;
                }
            } else {
                sum = EXTRASYM + strnum;
                num = 1;
                start = NULL;
                break;
            }
            s++;
        }

        if (start != NULL) {
            end = s - 1;
            if (*end == '+' || *end == '-') {
                sum = EXTRASYM + strnum;
                num = 1;
            } else {
                curnum = strtol(start, &end, BASE);
                num++;
                if (errno == ERANGE || curnum > INT_MAX || curnum < INT_MIN) {
                    curnum = sign * num;
                    errno = 0;
                }
                sum += curnum;
            }
        }

        if (num == 0) {
            printf("%d\n", SPACEONLY + strnum);
        } else {
            printf("%d\n", (int) sum);
        }

        sum = num = 0;
        sign = 1;
        start = NULL;
        strnum++;

        free(st);
        st = getline2(stdin);
    }
    
    return 0;
}
