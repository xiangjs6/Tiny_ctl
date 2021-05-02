//
// Created by xjs on 2020/9/1.
//

#ifndef TINY_CTL_TCTL_COMMON_H
#define TINY_CTL_TCTL_COMMON_H
#include <stddef.h>
#include "tctl_metaclass.h"

#define MAX(x, y) (((x) > (y)) ? (x) : (y))
#define MIN(x, y) (((x) < (y)) ? (x) : (y))
#define BYTE_ALIGNED  __attribute__((packed, aligned(1)))

#define container_of(ptr, type, member) ((type *) ((char *)(ptr) - offsetof(type, member)))
#define autofree(func) __attribute__((cleanup(func)))

size_t hash_numeric(void *x);
size_t hash_str(void *x);
#endif //TINY_CTL_TCTL_COMMON_H
