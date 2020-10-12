#include <stdio.h>
#include <unistd.h>
#include <sys/stat.h>
#include <dirent.h>

int
main(int argc, char *argv[])
{
    struct stat f;
    unsigned long long sum = 0;
    DIR *pd = NULL;

    pd = opendir(argv[1]);
    if (pd == NULL) {
        printf("Error: Unable to open directory\n");
        return 1;
    }

    struct dirent *d = readdir(pd);
    char path[PATH_MAX];
        
    while (d != NULL) { 
        snprintf(path, PATH_MAX, "%s/%s", argv[1], d->d_name);
        if (stat(path, &f) == 0 && S_ISREG(f.st_mode) 
                && d->d_name[0] >= 'A' && d->d_name[0] <= 'Z' && f.st_uid == getuid()) {
                
            sum += f.st_size;
        }

        d = readdir(pd);
    }

    printf("%llu\n", sum);

    closedir(pd);
    return 0;
}
