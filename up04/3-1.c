int //returns 1 if the node has noc children 
symm(int ind, int fd)
{
    int32_t a[NODESIZE];

    if (read(fd, a, sizeof(a)) != sizeof(a)) {
        printf("\nError while reading file\n");
        exit(1);
    }

    int flag = 1;
    //printf("%d %d %d\n", a[0], a[1], a[2]);
    if (a[1]) {
        if (!(ind + 1 == a[1])) {
            lseek(fd, a[1] * sizeof(a), SEEK_SET);
        }
        flag &= symm(a[NODESIZE - 2], fd);
    }
    printf("%d ", a[0]);

    if (a[2]) {
        if (!(!a[1] && ind + 1 == a[2]) ) {
            lseek(fd, a[2] * sizeof(a), SEEK_SET);
        }
        symm(a[NODESIZE - 1], fd);
    }

    return !a[1] && !a[2];
}

/*
int //returns 1 if the node has no children, 0 otherwise
symm(int ind, int fd)
{
    int32_t a[NODESIZE];

    if (read(fd, a, sizeof(a)) != sizeof(a)) {
        printf("\nError while reading file\n");
        exit(1);
    }

    int flag = 0;
    
    if (a[1]) {
        if (ind + 1 != a[1]) {
            lseek(fd, a[1] * sizeof(a), SEEK_SET);
        }
        flag = symm(a[NODESIZE - 2], fd);
    }
    printf("%d ", a[0]);

    if (a[2]) {
        if ( !( (!a[1] && ind + 1 == a[2]) || 
                (ind + 1 == a[1] && ind + 2 == a[2] && flag) ) ) {
            
            lseek(fd, a[2] * sizeof(a), SEEK_SET);
        }
        symm(a[NODESIZE - 1], fd);
    }

    return !a[1] && !a[2];
} */
