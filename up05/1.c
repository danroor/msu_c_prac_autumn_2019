#include <stdio.h>
#include <inttypes.h>

int
main(void)
{
    unsigned int n, w;
    scanf("%o %o", &n, &w);
    
    uint32_t max = 1u;
    max <<= n - 1u;
  
    for (uint32_t i = 0; i < max; ++i) {
        printf("|%*" PRIo32 "|", w, i);
        printf("%*" PRIu32 "|", w, i);
        printf("%*" PRIu32 "|\n", w, i);  //positive number
    }

    for (uint32_t i = max; i < max << 1u; ++i) {
        printf("|%*" PRIo32 "|", w, i);
        printf("%*" PRIu32 "|", w, i);
        printf("%*" PRId32 "|\n", w, -(i & ~max) );  //negative number
    }

    return 0;
}
