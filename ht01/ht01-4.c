#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>

enum
{
    DIGITS = 10,
    MAXBUF = 2048
};

char *
getline3(FILE *f)
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

        if (c == '\n') { //new line (Linux)
            str[cur - 1] = '\0';  //cut \n
            return str;
        }

        c = getc(f);

        if (str[cur - 1] == '\r' && c == '\n') { //new line (Windows)
            str[cur - 1] = '\0';  //cut \r\n
            return str;
        }
    }

    return str; 
}

int
main(void)
{
    char *name = getline3(stdin);
    
    int fd = open(name, O_RDONLY); 

    if (name != NULL) {
        free(name);
    }

    long long a[DIGITS] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

    if (fd > 0) {
        int bufsize;
        char *buf = calloc(MAXBUF, sizeof(*buf));

        while ((bufsize = read(fd, buf, MAXBUF * sizeof(*buf))) > 0) {

            for (int i = 0; i < bufsize; ++i) {
                if (buf[i] >= '0' && buf[i] <= '9') {
                    a[ buf[i] - '0']++;
                }
            }
        }
        free(buf);
        close(fd);
    }
    
    for (int i = 0; i < DIGITS; ++i) {
        printf("%d %lld\n", i, a[i]);
    }

    return 0;
}
