#include <sys/stat.h>

struct Task
{
    unsigned uid;
    int gid_count;
    unsigned *gids;
};

static int
rwx(const struct stat *stb, char category)
{
    mode_t mode = stb->st_mode;
    int r, w, x;

    switch (category) {
    case 'U':
        r = !!(mode & S_IRUSR);
        w = !!(mode & S_IWUSR);
        x = !!(mode & S_IXUSR);
        break;
    case 'G':
        r = !!(mode & S_IRGRP);
        w = !!(mode & S_IWGRP);
        x = !!(mode & S_IXGRP);
        break;
    default:
        r = !!(mode & S_IROTH);
        w = !!(mode & S_IWOTH);
        x = !!(mode & S_IXOTH);
        break;
    }

    return (r << 2) | (w << 1) | x;
}

static int
grp_access(const struct stat *stb, const struct Task *task)
{
    int cnt = task->gid_count;
    unsigned int gid = (unsigned int) stb->st_gid;

    for (int i = 0; i < cnt; ++i) {
        if (gid == task->gids[i]) {
            return 1;
        }
    }
 
    return 0;
}

int 
myaccess(const struct stat *stb, const struct Task *task, int access)
{
    if (task->uid == 0) {
        return 1;
    }

    if (task->uid == stb->st_uid) {
        return (rwx(stb, 'U') & access) == access;
    }

    if (grp_access(stb, task)) {
        return (rwx(stb, 'G') & access) == access;
    }

    return (rwx(stb, 'O') & access) == access;
}
