#include <stdio.h>
int main(void) {
    int x;
    scanf("%d", &x);
    x /= 0;
    printf("%d\n", x+1);
    return 0;
}
