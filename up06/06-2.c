#include <stdio.h>
#include <time.h>
#include <string.h>

enum
{
    MINYEAR = 1900,
    SEC_IN_DAY = 86400, //60^2 * 24
    DAYS_IN_MONTH = 30,
    MONTHS_IN_YEAR = 12,
    BIRTHYEAR = 1925,
    BIRTHMONTH = 10,
    BIRTHDAY = 7
};

int
main(void)
{
    struct tm date;
    memset(&date, 0, sizeof(date));

    date.tm_year = BIRTHYEAR - MINYEAR;
    date.tm_mon = BIRTHMONTH - 1;
    date.tm_mday = BIRTHDAY;
    date.tm_isdst = -1;

    time_t birth = mktime(&date);
    
    int year, month, day;
    long long dif;

    while (scanf("%d %d %d", &year, &month, &day) == 3) {
        date.tm_year = year - MINYEAR;
        date.tm_mon = month - 1;
        date.tm_mday = day;

        dif = (long long) difftime(mktime(&date), birth);
        
        day = !!(dif % SEC_IN_DAY);
        dif /= SEC_IN_DAY;
        dif += day;
        
        day = dif % DAYS_IN_MONTH;
        dif /= DAYS_IN_MONTH;
        month = dif % MONTHS_IN_YEAR;
        year = dif / MONTHS_IN_YEAR;

        printf("%d %d %d\n", year + 1, month + 1, day + 1);
    }
    
    return 0;
}
