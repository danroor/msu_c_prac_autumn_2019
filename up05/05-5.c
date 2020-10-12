#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

enum { MINYEAR = 1900 };

char *
readtime(FILE *f)
{
    if (!f) {
        return NULL;
    }
   
    char *str = NULL;
    char *tmp = NULL;

    int size = 0, cur = 0;
    int c = getc(f);

    while (c != EOF && isspace((unsigned char) c)) {
        c = getc(f);
    }

    //reading date (YMD)
    while (c != EOF && !isspace((unsigned char) c)) {
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
        str[cur] = ' ';

        c = getc(f);
    }

    cur++;
    while (c != EOF && isspace((unsigned char) c)) {
        c = getc(f);
    }

    //reading time (HMS)
    while (c != EOF && !isspace((unsigned char) c)) {
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
    char *str = readtime(f);
    if (str == NULL) {
        return NULL;
    }
    
    struct tm *time1 = calloc(1, sizeof(*time1));
    memset(time1, 0, sizeof(*time1));
    
    int year, month, day, hour, min, sec;
    sscanf(str, "%d/%d/%d %d:%d:%d", &year, &month, &day, &hour, &min, &sec);

    time1->tm_year = year - MINYEAR;
    time1->tm_mon = month - 1;
    time1->tm_mday = day;
    time1->tm_hour = hour;
    time1->tm_min = min;
    time1->tm_sec = sec;
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

    struct tm *time1 = gettime(f);
    if (time1 == NULL) {
        fclose(f);
        return 0;
    }

    time_t shift1 = mktime(time1);
    time_t shift2;

    free(time1);
    while ( (time1 = gettime(f)) != NULL ) {
        shift2 = mktime(time1);
        printf("%d\n", (int) difftime(shift2, shift1));
        shift1 = shift2;
        free(time1);
    }

    fclose(f);
    return 0;
}
