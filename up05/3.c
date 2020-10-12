void
normalize_path(char *buf)
{
    char *rd   = buf;
    char *wrt  = buf;
    int flag = 0;

    while(*rd) {
        if (*rd == '/') {

            if (!flag) {
                *wrt = *rd;
                wrt++;
                flag = 1;
            }
        } else {
            *wrt = *rd;
            wrt++;
            flag = 0;
        }

        rd++;
    }
    
    *wrt = *rd;
    return;
}
