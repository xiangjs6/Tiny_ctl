//
// Created by xjs on 2020/9/1.
//

#ifndef TINY_CTL_TCTL_ITERATOR_H
#define TINY_CTL_TCTL_ITERATOR_H
#include "tctl_object.h"
#include "tctl_class.h"
#define INHERIT_ITERATOR \
struct {                      \
    INHERIT_CLASS;            \
    void *(*derefer)(void);   \
}

typedef struct {
    union {
        INHERIT_ITERATOR *_s;
        byte _pad[sizeof(*(Object)NULL)];
    };
} *Iterator;

typedef struct {
    union {
        INHERIT_ITERATOR *_s;
        byte _pad[sizeof(*(Object)NULL)];
    };
} *IteratorClass;
void initIterator(void) __attribute__((constructor));

const void *_IteratorClass(void);
const void *_Iterator(void);

#define ITERATORCLASS IteratorClass : _IteratorClass()
#define ITERATOR Iterator : _Iterator()
#endif //TINY_CTL_TCTL_ITERATOR_H
