//
// Created by xjs on 2021/1/3.
//

#ifndef TINY_CTL_TCTL_UINT_H
#define TINY_CTL_TCTL_UINT_H
#include "tctl_class.h"

#define INHERIT_UINT \
struct {            \
    INHERIT_CLASS;  \
}

//由于对齐原因，所以直接使用long long长度的整型
typedef struct {
    union {
        INHERIT_UINT *_s;
        char _pad[sizeof(*(Object)NULL)];
    };
    unsigned long long val;
} *UInt;

void initUInt(void) __attribute__((constructor));
Form_t _UInt(void);
#define UINT UInt : _UInt()
#endif //TINY_CTL_TCTL_UINT_H
