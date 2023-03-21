#include "bn.h"

static inline void bn_size_inc(bn_t *a)
{
    if (++a->size > a->cap) {
        a->cap <<= 1;
        a->data = realloc(a->data, a->cap * sizeof(uint64_t));
    }
    a->data[a->size - 1] = 0;
}
bn_t *bn_new(const uint64_t v)
{
    bn_t *tmp = malloc(sizeof(bn_t));
    tmp->cap = 1;
    tmp->size = 1;
    tmp->data = malloc(sizeof(uint64_t));
    tmp->data[0] = v;
    return tmp;
}
void bn_free(bn_t *tmp)
{
    free(tmp->data);
    free(tmp);
}
void bn_swap(bn_t *a, bn_t *b)
{
    bn_t c = *a;
    *a = *b;
    *b = c;
}

void bn_add(bn_t *dst, const bn_t *b)
{
    int ci = 0;
    for (int i = 0; i < b->size; ++i) {
        if (dst->size <= i)
            bn_size_inc(dst);
        int co = (dst->data[i] += b->data[i]) < b->data[i];
        ci = (dst->data[i] += ci) < ci || co;
    }
    for (int i = b->size; ci; ++i) {
        if (dst->size <= i)
            bn_size_inc(dst);
        ci = (dst->data[i] += ci) < ci;
    }
}
static inline void bn_add_offset(bn_t *dst,
                                 const uint64_t *buf,
                                 const int offset)
{
    int ci = 0, i;
    for (i = 0; i < 2 && buf[i]; ++i) {
        if (dst->size <= i + offset)
            bn_size_inc(dst);
        int co = (dst->data[i + offset] += buf[i]) < buf[i];
        ci = (dst->data[i + offset] += ci) < ci || co;
    }
    for (; ci; ++i) {
        if (dst->size <= i + offset)
            bn_size_inc(dst);
        ci = (dst->data[i + offset] += ci) < ci;
    }
}
void bn_set(bn_t *dst, const uint64_t v)
{
    dst->size = 1;
    dst->data[0] = v;
}
int bn_count(const bn_t *a)
{
    uint8_t *p = (uint8_t *) a->data;
    int cnt = 0;
    for (int i = 0; i < a->size * 8; ++i) {
        if (p[i])
            cnt = i + 1;
    }
    return cnt;
}
void bn_mul(bn_t *dst, const bn_t *a, const bn_t *b)
{
    bn_set(dst, 0);
    uint64_t buf[2];
    for (int i = 0; i < a->size; ++i) {
        for (int j = 0; j < b->size; ++j) {
            __asm__("mulq %3"
                    : "=a"(buf[0]), "=d"(buf[1])
                    : "%0"(a->data[i]), "rm"(b->data[j]));
            bn_add_offset(dst, buf, i + j);
        }
    }
}

void bn_assign(bn_t *dst, const bn_t *src)
{
    dst->size = 0;
    for (int i = 0; i < src->size; ++i) {
        bn_size_inc(dst);
        dst->data[i] = src->data[i];
    }
}

void bn_lshift(bn_t *a)
{
    int ci = 0;
    for (int i = 0; i < a->size; ++i) {
        int co = a->data[i] >> 63;
        a->data[i] = a->data[i] << 1 | ci;
        ci = co;
    }
    if (ci) {
        bn_size_inc(a);
        a->data[a->size - 1] = 1;
    }
}