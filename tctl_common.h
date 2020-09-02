//
// Created by xjs on 2020/9/1.
//

#ifndef TINY_CTL_TCTL_COMMON_H
#define TINY_CTL_TCTL_COMMON_H
#include <stddef.h>

#define MAX(x, y) (((x) > (y)) ? (x) : (y))
#define MIN(x, y) (((x) < (y)) ? (x) : (y))
#define BYTE_ALIGNED  __attribute__((packed, aligned(1)))

void *tmp_val(void *p, size_t n);
#define TEMP(v) ({typeof(v) __temp = v; (typeof(v)*)tmp_val(&__temp, sizeof(v));})
#endif //TINY_CTL_TCTL_COMMON_H
