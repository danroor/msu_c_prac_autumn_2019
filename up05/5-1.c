#include <time.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
//#include <stdlib.h>

enum
{
    MAXBUF = 1024,
    TIMEARRSZ = 6,
    MINYEAR = 1900,
    BASE = 10
};
/*
struct tm *
gettime(int fd)
{
    unsigned char c[MAXBUF];
    int timearr[TIMEARRSZ] = {0, 0, 0, 0, 0, 0};
    int ind = 0;

    int bufsize;
    while ( (bufsize = read(fd, c, MAXBUF)) > 0) {
        for (int i = 0; i < bufsize; ++i) {
            if (c[i] == '/' || c[i] == ':' || (isspace(c[i]) && ind == 2) ) {
                ind++;
            } else if (c[i] >= '0' && c[i] <= '9') {
                timearr[ind] *= BASE;
                timearr[ind] += c[i] - '0';
            }
        }
    }

    if (ind == 0) {
        return NULL;
    }

    struct tm *time1 = calloc(1, sizeof(*time1));

    time1->tm_year = timearr[0] - MINYEAR;
    time1->tm_mon = timearr[1] - 1;
    time1->tm_mday = timearr[2];
    time1->tm_hour = timearr[3];
    time1->tm_min = timearr[4];
    time1->tm_sec = timearr[5];

    time1->tm_wday = 0;
    time1->tm_yday = 0;
    time1->tm_isdst = -1;

    return time1;
}*/

char *
getline4(FILE *f)
{
    if (!f) {
        return NULL;
    }
   
    char *str = NULL;
    char *tmp = NULL;

    int size = 0, cur = 0;

    while (c != EOF && isspace(c)) {
        c = getc(f);
    }

    while (c != EOF && isspace(c)) {
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

        c = getc(f);
    }

    return str; 
}

struct tm *
gettime(FILE *f)
{
    unsigned char c[MAXBUF];
    int timearr[TIMEARRSZ] = {0, 0, 0, 0, 0, 0};
    int ind = 0;

    while ( (bufsize = read(fd, c, MAXBUF)) > 0) {
        for (int i = 0; i < bufsize; ++i) {
            if (c[i] == '/' || c[i] == ':' || (isspace(c[i]) && ind == 2) ) {
                ind++;
            } else if (c[i] >= '0' && c[i] <= '9') {
                timearr[ind] *= BASE;
                timearr[ind] += c[i] - '0';
            }
        }
    }

    if (ind == 0) {
        return NULL;
    }

    struct tm *time1 = calloc(1, sizeof(*time1));

    time1->tm_year = timearr[0] - MINYEAR;
    time1->tm_mon = timearr[1] - 1;
    time1->tm_mday = timearr[2];
    time1->tm_hour = timearr[3];
    time1->tm_min = timearr[4];
    time1->tm_sec = timearr[5];

    time1->tm_wday = 0;
    time1->tm_yday = 0;
    time1->tm_isdst = -1;

    return time1;
}

int
main(int argc, char *argv[])
{
    FILE *f = fopen(argv[1], "r");
    if (f == NULL) {
        printf("Error: Unable to open file\n");
        return 1;
    }

    struct tm *time1 = gettime(fd);
    
    time_t shift1 = mktime(time1);
    time_t shift2;

    while ( (time1 = gettime(f)) != NULL ) {
        shift2 = mktime(time1);
        printf("%d\n", (int) difftime(shift2, shift1));
        shift1 = shift2;
    }

    close(fd);
    return 0;
}
