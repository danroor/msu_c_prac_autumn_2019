#include <stdio.h>

int main(void)
{
    int n;
    scanf("%d", &n);
    	printf("%d\n", n);
    while ( (n = getchar()) != EOF ) {
        printf("%c", n);
    }
  
    printf("\n");
 
    return 0;
}
