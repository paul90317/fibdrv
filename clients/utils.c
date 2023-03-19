#include "utils.h"

char *bin_to_str(char *dst, const uint8_t *src, size_t size)
{
    static const int max = 1000000;
    int *tmp = calloc(size, sizeof(int)); /* 10e6 each element */
    int n = 1;
    for (int i = 0; i < size; ++i) {
        int last = src[i];
        for (int j = 0; j < n; j++) {
            tmp[j] = tmp[j] * 256 + last;
            last = tmp[j] / max;
            tmp[j] %= max;
        }
        for (; last; ++n) {
            tmp[n] = last % max;
            last /= max;
        }
    }
    bool leading_zero = true;
    int len = 0;
    for (int i = n - 1; i >= 0; --i) {
        for (int k = max / 10; k; k /= 10) {
            int this = (tmp[i] / k) % 10;
            if (this)
                leading_zero = false;
            if (!leading_zero)
                dst[len++] = this + '0';
        }
    }
    if (len == 0)
        dst[len++] = '0';
    dst[len] = 0;
    return dst;
}