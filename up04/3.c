#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>

struct Node
{
    int32_t key;
    int32_t left_idx;
    int32_t right_idx;
};

int //returns 1 if the node has no children, 0 otherwise
symm(int ind, int fd)
{
    struct Node node;
    memset(&node, 0, sizeof(node));
    
    if (read(fd, &node, sizeof(node)) != sizeof(node)) {
        return 1;
    }

    int flag = 0;
    
    if (node.right_idx) {
        if (ind + 1 != node.right_idx) {
            lseek(fd, node.right_idx * sizeof(node), SEEK_SET);
        }
        flag = symm(node.right_idx, fd);
    }
    printf("%d ", node.key);

    if (node.left_idx) {
        if ( !( (!node.right_idx && ind + 1 == node.left_idx) || 
                (ind + 1 == node.right_idx && ind + 2 == node.left_idx && flag) ) ) {
            
            lseek(fd, node.left_idx * sizeof(node), SEEK_SET);
        }
        symm(node.left_idx, fd);
    }

    return !node.left_idx && !node.right_idx;
}

int
main(int argc, char *argv[])
{
    int fd = open(argv[1], O_RDONLY);
    
    if (fd == -1) {
        printf("Error while opening file\n");
        return 1;
    }

    symm(0, fd);    
    printf("\n");

    close(fd);
    return 0;
}
