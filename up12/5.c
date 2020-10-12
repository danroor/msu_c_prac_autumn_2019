#include <unistd.h>
#include <sys/wait.h>
#include <stdio.h>
#include <fcntl.h>

void
readall(FILE *in, FILE *out)
{
    int x;
    while (fscanf(in, "%d", &x) == 1) {
        fprintf(out, "%d\n", x);
    }
}

void
intersection(FILE *in1, FILE *in2, FILE *out)
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
    int num = 0, left = 0, extrason = 0, extracur = 0;
    int files = argc - 1;
    if (files <= 1) {
        return 0;
    }
    FILE *in1, *in2, *out;
    pid_t pid, pid2;
    int p1[2], p2[2], father;
    dup2(STDOUT_FILENO, father);

    while (num < files) {
        left = (left << 1) + 1;
        num = (num << 1) + 1;
        pipe(p1);
        pid = fork();
        if (!pid) {
            //son1
            close(father);
            dup2(p1[1], father);
            close(p1[0]);
            close(p1[1]);
            extracur = num - left >= files;
            write(father, &extracur, sizeof(extracur));
            if (extracur) {
                close(father); 
                return 0;
            }
            if ((num << 1) >= files) {
                printf("%d %d\n", num, left);
                in1 = fopen(argv[num - left + 1], "r");
                out = fdopen(father, "w");
                readall(in1, out);
                fclose(in1);
                fclose(out);
                return 0;
            }
            //printf("sas\n");
            continue;
            //printf("sas\n");
        }
        //father
        close(p1[1]);
        read(p1[0], &extrason, sizeof(extrason));
        extracur += extrason;
        if (extrason) {
            close(p1[0]);
            p1[0] = -1;
        }

        num++;
        pipe(p2);
        pid2 = fork();
        if (!pid2) {
            //son2
            close(father);
            dup2(p2[1], father);
            close(p2[0]);
            close(p2[1]);
            close(p1[0]);
            close(p1[1]);
            extracur = num - left >= files;
            write(father, &extracur, sizeof(extracur));
            //printf("%d %d\n", num, left);
            if (extracur) {
                close(father); 
                return 0;
            }
            if ((num << 1) >= files) {
                //printf("%d %d\n", num, left);
                in1 = fopen(argv[num - left + 1], "r");
                out = fdopen(father, "w");
                readall(in1, out);
                fclose(in1);
                fclose(out);
                return 0;
            }
            continue;
        }
        //father
        close(p2[1]);
        read(p1[0], &extrason, sizeof(extrason));
        extracur += extrason;
        if (extrason) {
            close(p2[0]);
            p2[0] = -1;
        }
        out = fdopen(father, "w");
        switch (extracur) {
        case 1:
            if (p1[0] == -1) {
                in2 = fdopen(p2[0], "r");
                readall(in2, out);
                fclose(in2); 
                fclose(out);
                return 0;
            } else {
                in1 = fdopen(p1[0], "r");
                readall(in1, out);
                fclose(in1); 
                fclose(out);
                return 0;
            }
        case 2:
            extracur = 1;
            write(father, &extracur, sizeof(extracur));
            fclose(out);
            return 0;
        default:
            in1 = fdopen(p1[0], "r");
            in2 = fdopen(p2[0], "r");
            out = fdopen(father, "w");
            intersection(in1, in2, out);
            fclose(in1);
            fclose(in2); 
            fclose(out);
        }

        
        return 0;
    }

    return 0;
}
