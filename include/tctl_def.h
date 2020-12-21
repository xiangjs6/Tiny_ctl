//
// Created by xjs on 2020/9/4.
//

#ifndef TINY_CTL_TCTL_DEF_H
#define TINY_CTL_TCTL_DEF_H
#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>
#include "tctl_common.h"

typedef long long offset_t;
typedef char byte;

typedef int (*Compare)(const void *, const void *);
typedef size_t (*HashFunc)(const void *);
typedef void *(*ExtractKey)(const void *);

typedef void *(*BinaryOperation)(const void *first, const void *second, void *res);
typedef void (*UnaryOperation)(void *self);

#define pair(FT, ST) struct {FT first; ST second;} BYTE_ALIGNED
#endif //TINY_CTL_TCTL_DEF_H
