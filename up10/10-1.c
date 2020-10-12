#include <unistd.h>
#include <sys/wait.h>
#include <stdio.h>
#include <time.h>

enum { MINYEAR = 1900 };

struct tm *
gettime(int p)
{
    time_t t;
    wait(NULL);
    read(p, &t, sizeof(t));
    return localtime(&t);
}

int 
main(void)
{
    int p[2];

    if (pipe(p) < 0) {
        return 1;
    } 

    struct tm *time1;

    if (fork()) {
        //father
        time1 = gettime(p[0]);
        printf("Y:%04d\n", time1->tm_year + MINYEAR);

    } else if (fork()) {
        //son
        time1 = gettime(p[0]);
        printf("M:%02d\n", time1->tm_mon + 1);

    } else if (fork()) {
        //grandson
        time1 = gettime(p[0]);
        printf("D:%02d\n", time1->tm_mday);

    } else {
        //grandgrandson
  
        time_t t = time(NULL);
        write(p[1], &t, sizeof(t));
        write(p[1], &t, sizeof(t));
        write(p[1], &t, sizeof(t));
    }

    close(p[0]);
    close(p[1]);

    return 0;
}
