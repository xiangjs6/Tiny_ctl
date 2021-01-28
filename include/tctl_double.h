//
// Created by xjs on 2021/1/3.
//

#ifndef TINY_CTL_TCTL_DOUBLE_H
#define TINY_CTL_TCTL_DOUBLE_H
#include "tctl_class.h"
#include "tctl_metaclass.h"

#define DOUBLE_FUNC \
struct {            \
    CLASS_FUNC;     \
}

#define DOUBLE_OBJ \
struct {           \
    double val;    \
}

typedef struct {
    METAOBJECT_HEAD(DOUBLE_FUNC);
    DOUBLE_OBJ;
} *Double;

void initDouble(void) __attribute__((constructor));
double toDouble(FormWO_t t);
Form_t _Double(void);
#define DOUBLE Double : _Double()
#endif //TINY_CTL_TCTL_DOUBLE_H
