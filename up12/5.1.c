#include <unistd.h>
#include <sys/wait.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include <fcntl.h>

enum
{
    RIGHTS = S_IRUSR | S_IWUSR,
    NOFLAGS = 0,
    BUFSZ = 1000
};

struct message
{
    long type;
    int buf[BUFSZ];
} mes;

//extra-process (no input sequence for it)
void
readall(FILE *in, FILE *out, int num)
{
    int x;
    while (fscanf(in, "%d", &x) == 1) {
        fprintf(out, "%d\n", x);
    }
}

void
intersection(FILE *in1, FILE *in2, FILE *out, int)
{
    int x1, x2;
    while (fscanf(in1, "%d", &x1) == 1) {
        do {
            if (fscanf(in2, "%d", &x2) < 0) {
                return;
            }

        } while (x2 < x1);
        
        if (x2 == x1) {
            fprintf(out, "%d\n", x1);
        }
    }
}

int
main(int argc, char *argv[])
{
    int num = 0, sonnum;
    int files = argc - 1;
    pid_t pid;

    while (num < files) {
        sonnum = (num << 1) + 1;
        pid = fork();
        if (pid < 0) {
            return 1;
        }
        if (pid) {
            sonnum++;
            pid = fork();
            if (pid < 0) {
                return 1;
            }
        }
        num = sonnum;
    }

    //father (no input files)
    if (num == 0) {
        return 0;
    }

    //extra-process (no input sequence for it)
    return 1;
}
