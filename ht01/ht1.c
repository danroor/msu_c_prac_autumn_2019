#include <stdio.h>
#include <fcntl.h>
#include <stdlib.h>

char *
getline2(FILE *f)
{
    if (!f) {
        return NULL;
    }
   
    char *str = NULL;
    char *tmp = NULL;

    int size = 0, cur = 0;
    int c = getc(f);

    while (c != EOF) {
        if (cur >= size - 1) {
            size <<= 1;
            size += 2;
            tmp = realloc(str, size * sizeof(*str));
            if (!tmp) {
               
                if (str) {
                    free(str);
                }
                return NULL;
            }
            str = tmp;
        }
        str[cur++] = c;
        str[cur] = '\0';

        if (c == '\n') {
            return str;
        }
        c = getc(f);
    }

    return str; 
}
