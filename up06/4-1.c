#include <stdio.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <dirent.h>
#include <errno.h>

int
main(int argc, char *argv[])
{
    struct stat f;
    unsigned long long sum = 0;
    DIR *pd = NULL;

    for (int i = 1; i < argc; ++i) {
        pd = opendir(argv[i]);
        if (pd == NULL) {
            perror("Error: Unable to open directory\n");
            return 1;
        }

        struct dirent *d = readdir(pd);
        
        while (d != NULL) {
            stat(d->d_name, &f);
            if (errno != ENOENT && d->d_type == DT_REG && d->d_name[0] >= 'A' && 
                    d->d_name[0] <= 'Z' && f.st_uid == getuid()) {

                sum += f.st_size;
            }
            errno = 0;
            d = readdir(pd);
        }
    }

    printf("%llu\n", sum);
    closedir(pd);
    return 0;
}
