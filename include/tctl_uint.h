//
// Created by xjs on 2021/1/3.
//

#ifndef TINY_CTL_TCTL_UINT_H
#define TINY_CTL_TCTL_UINT_H
#include "tctl_class.h"
#include "tctl_metaclass.h"

#define UINT_FUNC \
struct {          \
    CLASS_FUNC;   \
}

#define UINT_OBJ            \
struct {                    \
    unsigned long long val; \
}

typedef struct {
    METAOBJECT_HEAD(UINT_FUNC);
    UINT_OBJ;
} *UInt;

const void *_UInt(void);
#define UINT UInt : _UInt()
#endif //TINY_CTL_TCTL_UINT_H
