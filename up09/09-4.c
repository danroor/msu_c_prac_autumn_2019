#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <fcntl.h>

enum { ADD_STR_SIZE = 10 };

int
main(int argc, char *argv[])
{
    char *dir = getenv("XDG_RUNTIME_DIR");
    if (dir == NULL) {
        dir = getenv("TMPDIR");
    }

    char path[PATH_MAX];
    if (dir == NULL) {
        snprintf(path, PATH_MAX, "/tmp/script%d", getpid());
    } else {
        snprintf(path, PATH_MAX, "%s/script%d", dir, getpid());
    }

    int out = 0;
    int script = open(path, O_WRONLY | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR 
            | S_IXUSR | S_IRGRP);

    if (script == -1) {
        return 1;
    }
    dup2(STDOUT_FILENO, out);
    dup2(script, STDOUT_FILENO);
    close(script);
     
    printf("#!/usr/bin/env python3\nprint(%s", argv[1]);

    for (int i = 2; i < argc; ++i) {
        printf("*%s", argv[i]);
    }
    printf(")\n");
    fflush(stdout);

    dup2(out, STDOUT_FILENO);
    close(out);

    char cmd[2 * PATH_MAX + ADD_STR_SIZE];
    snprintf(cmd, 2 * PATH_MAX + ADD_STR_SIZE, "%s; rm -f %s", path, path);
    execlp("/bin/sh", "sh", "-c", cmd,  NULL);
    
    return 1;
}
