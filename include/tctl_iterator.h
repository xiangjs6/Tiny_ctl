//
// Created by xjs on 2020/9/1.
//

#ifndef TINY_CTL_TCTL_ITERATOR_H
#define TINY_CTL_TCTL_ITERATOR_H
#include "tctl_object.h"
#include "tctl_class.h"
#define INHERIT_ITERATORCLASS \
struct {                      \
    INHERIT_CLASS;            \
    void *(*derefer)(void);   \
}

typedef struct {
    union {
        INHERIT_ITERATORCLASS *_s;
        byte _pad[sizeof(*(Object)NULL)];
    };
    void *val;
} *Iterator;

typedef struct {
    union {
        INHERIT_ITERATORCLASS *_s;
        byte _pad[sizeof(*(Object)NULL)];
    };
    byte _o[0];
} *IteratorClass;
void initIteratorClass(void) __attribute__((constructor));

const void *_IteratorClass(void);

#define ITERATORCLASS IteratorClass : _IteratorClass()
#endif //TINY_CTL_TCTL_ITERATOR_H
