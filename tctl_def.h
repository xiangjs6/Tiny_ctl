//
// Created by xjs on 2020/9/4.
//

#ifndef TINY_CTL_TCTL_DEF_H
#define TINY_CTL_TCTL_DEF_H
#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>

typedef long long offset_t;
typedef char byte;

typedef byte (*Compare)(const void *, const void *);
#endif //TINY_CTL_TCTL_DEF_H
