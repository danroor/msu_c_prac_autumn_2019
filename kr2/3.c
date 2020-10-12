#include <unistd.h>
#include <sys/wait.h>
#include <stdio.h>
#include <fcntl.h>

int
main(int argc, char *argv[])
{
    int sg[2]; //son-grandson
    int fg[2]; //father-grandson

    pipe(sg);
    pipe(fg);
    double x = 0;

    if (fork()) {
        //father
        close(fg[1]);
        close(sg[1]);

        wait(NULL);        
        if (read(fg[0], &x, sizeof(x)) > 0) {
            printf("%.10g\n", x);
        }
        close(sg[0]);
        close(fg[0]);
    } else {
        //son
        close(fg[0]);

        FILE *f = fopen(argv[1], "r");
        while (fscanf(f, "%lf", &x) == 1) {
            if (x >= 0) {
                write(sg[1], &x, sizeof(x));
            }
        }
        
        fclose(f);

        if (fork()) {
            close(sg[0]);
            close(sg[1]);
            close(fg[1]);
            wait(NULL);
            return 0;
        }
        //grandson
        close(sg[1]);     

        double sum = 0;
        int flag = 0;

        while (read(sg[0], &x, sizeof(x)) > 0) {
            sum += x;
            flag = 1;
        }
        
        if (flag) {
            write(fg[1], &sum, sizeof(sum));
        }

        close(sg[0]);
        close(fg[1]);
    }

    return 0;
}
