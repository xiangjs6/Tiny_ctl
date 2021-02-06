//
// Created by xjs on 2020/9/4.
//

#ifndef TINY_CTL_TCTL_DEF_H
#define TINY_CTL_TCTL_DEF_H
#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>
#include "tctl_common.h"
#include "tctl_metaclass.h"

typedef long long offset_t;
typedef char byte;

typedef int (*Compare)(FormWO_t, FormWO_t);
typedef size_t (*HashFunc)(const void *);
typedef void *(*ExtractKey)(const void *);

typedef void *(*BinaryOperation)(const void *first, const void *second, void *res);
typedef void (*UnaryOperation)(void *self);

#endif //TINY_CTL_TCTL_DEF_H
