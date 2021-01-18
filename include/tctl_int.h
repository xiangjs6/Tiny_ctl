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

//由于对齐原因，所以直接使用long long长度的整型
typedef struct {
    union {
        INHERIT_INT *_s;
        char _pad[sizeof(*(Object)NULL)];
    };
    long long val;
} *Int;

void initInt(void) __attribute__((constructor));
long long toInt(FormWO_t t);
Form_t _Int(void);
#define INT Int : _Int()
#endif //TINY_CTL_TCTL_INT_H
