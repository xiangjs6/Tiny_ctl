//
// Created by xjs on 2021/1/3.
//

#ifndef TINY_CTL_TCTL_INT_H
#define TINY_CTL_TCTL_INT_H

#include "tctl_class.h"

#define INHERIT_INT \
struct {            \
    INHERIT_CLASS;  \
}

typedef struct {
    union {
        INHERIT_INT *_s;
        char _pad[sizeof(*(Object)NULL)];
    };
    int val;
} *Int;

void initInt(void) __attribute__((constructor));
const void *_Int(void);
//extern const void *_Int;
#define INT Int : _Int()
#endif //TINY_CTL_TCTL_INT_H
