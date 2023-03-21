#ifndef __BN_H_
#define __BN_H_

#include <linux/slab.h>

#define malloc(size) kmalloc(size, GFP_KERNEL)
#define realloc(p, size) krealloc(p, size, GFP_KERNEL)
#define calloc(cnt, size) kcalloc(cnt, size, GFP_KERNEL)
#define free(mem) kfree(mem)

typedef struct {
    uint64_t *data;
    size_t size;
    size_t cap;
} bn_t;

bn_t *bn_new(uint64_t v);
void bn_free(bn_t *tmp);
void bn_swap(bn_t *a, bn_t *b);
void bn_add(bn_t *dst, const bn_t *b);
void bn_set(bn_t *dst, const uint64_t v);
int bn_count(const bn_t *a);
void bn_mul(bn_t *dst, const bn_t *a, const bn_t *b);
void bn_assign(bn_t *dst, const bn_t *src);
void bn_lshift(bn_t *a);
#endif