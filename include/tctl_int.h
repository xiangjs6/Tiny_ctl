//
// Created by xjs on 2021/1/3.
//

#ifndef TINY_CTL_TCTL_INT_H
#define TINY_CTL_TCTL_INT_H

#include "tctl_class.h"
#include "tctl_metaclass.h"

#define INT_FUNC \
struct {         \
    CLASS_FUNC;  \
}

//由于对齐原因，所以直接使用long long长度的整型
#define INT_OBJ    \
struct {           \
    long long val; \
}

typedef struct {
    METAOBJECT_HEAD(INT_FUNC);
    INT_OBJ;
} *Int;

const void *_Int(void);
#define INT Int : _Int()
#endif //TINY_CTL_TCTL_INT_H
