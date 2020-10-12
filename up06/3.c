#include <string.h>

int
parse_rwx_permissions(const char *str)
{
    if (str == NULL) {
        return -1;
    }

    const char template[] = "rwxrwxrwx";

    int len = strlen(str);
    if (len != sizeof(template) - 1) {
        return -1;
    }

    int res = 0;
    for (int i = 0; i < len; ++i) {
        if (str[i] != '-' && str[i] != template[i]) {
            return -1;
        } else {
            res <<= 1;
            res |= str[i] == template[i];
        }
    }

    return res;
}
