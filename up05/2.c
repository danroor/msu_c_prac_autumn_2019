#include <stdio.h>

enum
{
    STRSZ = 10,
    BITS = 9
};

int
main(int argc, char *argv[])
{
    unsigned int r;
    const char *template = "rwxrwxrwx";
    char res[STRSZ];
    res[STRSZ - 1] = '\0';
    

    for (int j = 1; j < argc; ++j) {
        sscanf(argv[j], "%o", &r);
        
        for (int i = 0; i < BITS; ++i) {
            if ( (r >> (BITS - i - 1)) & 1) {
                res[i] = template[i];
            } else {
                res[i] = '-';
            }
        }

        printf("%s\n", res);
    }

    return 0;
}
