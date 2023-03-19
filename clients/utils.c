#include "utils.h"

char *bin_to_str(char *dst, const uint8_t *src, size_t size)
{
    int n = 1;
    dst[0] = 0;
    for (int i = size - 1; i >= 0; --i) {
        int last = src[i];
        for (int j = 0; j < n; j++) {
            last = dst[j] * 256 + last;
            dst[j] = last % 10;
            last /= 10;
        }
        for (; last; ++n) {
            dst[n] = last % 10;
            last /= 10;
        }
    }
    for (int i = 0, j = n - 1; i <= j; ++i, --j) {
        char c = dst[i];
        dst[i] = dst[j] + '0';
        dst[j] = c + '0';
    }
    dst[n] = 0;
    return dst;
}