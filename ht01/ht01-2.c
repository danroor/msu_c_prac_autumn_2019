#include <stdint.h>

int32_t 
satsum(int32_t v1, int32_t v2)
{
    int32_t s = 0;

    if (__builtin_sadd_overflow(v1, v2, &s)) {

        uint32_t us = ~0u >> 1u;
        if (v1 < 0) {
            us = ~us;
        }

        s = (int32_t) us;
    }

    return s;
}
