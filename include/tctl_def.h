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
typedef bool (*Equal)(FormWO_t, FormWO_t);
typedef void (*SwapFunc)(FormWO_t, FormWO_t);
typedef bool (*Predicate)(FormWO_t);
typedef void (*UnaryFunc)(FormWO_t);
typedef size_t (*HashFunc)(FormWO_t);
typedef FormWO_t (*ExtractKey)(FormWO_t); //提取出来的key，应为引用类型

typedef FormWO_t (*BinaryOperation)(FormWO_t first, FormWO_t second);
typedef void (*AssignOperation)(FormWO_t left, FormWO_t right);
typedef void (*UnaryOperation)(FormWO_t self);

#endif //TINY_CTL_TCTL_DEF_H
