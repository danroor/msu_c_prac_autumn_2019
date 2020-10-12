#include <stdio.h>
#include <sys/stat.h>

int
main(int argc, char *argv[])
{
    struct stat f;
    unsigned long long sum = 0;
    
    for (int i = 1; i < argc; ++i) {
        if (lstat(argv[i], &f) == 0 && (f.st_mode & S_IFMT) == S_IFREG && 
                f.st_nlink == 1) {
            sum += f.st_size;
        }
    }

    printf("%llu\n", sum);
    return 0;
}
