#include "bn.h"
void bn_size_inc(bn_t *a)
{
    if (++a->size > a->cap) {
        a->cap <<= 1;
        a->data = realloc(a->data, a->cap * sizeof(uint64_t));
    }
    a->data[a->size - 1] = 0;
}
bn_t *bn_new(uint64_t v)
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

void bn_add(bn_t *dst, bn_t *b)
{
    int ci = 0;
    for (int i = 0; i < b->size; ++i) {
        while (dst->size <= i)
            bn_size_inc(dst);
        int co = (dst->data[i] += b->data[i]) < b->data[i];
        ci = (dst->data[i] += ci) < ci || co;
    }
    for (int i = b->size; ci; ++i) {
        while (dst->size <= i)
            bn_size_inc(dst);
        ci = (dst->data[i] += ci) < ci;
    }
}
void bn_set(bn_t *dst, uint64_t v)
{
    dst->size = 1;
    dst->data[0] = v;
}
int bn_count(bn_t *a)
{
    uint8_t *p = (uint8_t *) a->data;
    int cnt = 0;
    for (int i = 0; i < a->size * 8; ++i) {
        if (p[i])
            cnt = i + 1;
    }
    return cnt;
}