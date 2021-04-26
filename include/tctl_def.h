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

typedef int (*Compare)(const void *, const void *);
typedef bool (*Equal)(const void *, const void *);
typedef bool (*Predicate)(const void *);
typedef void *(*Generator)(void);
typedef void (*UnaryFunc)(const void *);
typedef size_t (*HashFunc)(const void *);
typedef void *(*ExtractKey)(const void *); //提取出来的key，应为引用类型

typedef void *(*BinaryOperation)(const void *first, const void *second);
typedef void *(*UnaryOperation)(void *self);

#endif //TINY_CTL_TCTL_DEF_H
