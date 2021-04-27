//
// Created by xjs on 2020/9/1.
//

#ifndef TINY_CTL_TCTL_ITERATOR_H
#define TINY_CTL_TCTL_ITERATOR_H
#include "tctl_metaclass.h"
#include "tctl_class.h"
#define ITERATOR_FUNC                   \
struct {                                \
    CLASS_FUNC;                         \
    void *(*derefer)(void);             \
    long long (*dist)(Iterator it);     \
    Iterator (*reverse_iterator)(void); \
}

enum IterRank {
    ForwardIter,
    BidirectionalIter,
    RandomAccessIter,
    SequenceIter
};

#define ITERATOR_OBJ          \
struct {                      \
    const enum IterRank rank; \
    void const * const class; \
}

typedef struct _Iterator *Iterator;
struct _Iterator{
    METAOBJECT_HEAD(ITERATOR_FUNC);
    ITERATOR_OBJ;
};

const void *_IteratorClass(void);
const void *_Iterator(void);

//#define ITERATORCLASS IteratorClass : _IteratorClass()
#define ITERATOR Iterator : _Iterator()

long long distance(Iterator, Iterator);
void advance(Iterator it, long long n);
#endif //TINY_CTL_TCTL_ITERATOR_H
