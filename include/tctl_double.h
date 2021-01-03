//
// Created by xjs on 2021/1/3.
//

#ifndef TINY_CTL_TCTL_DOUBLE_H
#define TINY_CTL_TCTL_DOUBLE_H
#include "tctl_class.h"

#define INHERIT_DOUBLE \
struct {            \
    INHERIT_CLASS;  \
}

typedef struct {
    union {
        INHERIT_DOUBLE *_s;
        char _pad[sizeof(*(Object)NULL)];
    };
    double val;
} *Double;

void initDouble(void) __attribute__((constructor));
const void *_Double(void);
#define DOUBLE Double : _Double()
#endif //TINY_CTL_TCTL_DOUBLE_H
