//
// Created by xjs on 2020/9/1.
//

#ifndef TINY_CTL_TCTL_ITERATOR_H
#define TINY_CTL_TCTL_ITERATOR_H
#include "tctl_metaclass.h"
#include "tctl_class.h"
#define ITERATOR_FUNC                        \
struct {                                     \
    CLASS_FUNC;                              \
    void *(*derefer)(void);                  \
    Form_t (*type)(void);                    \
    long long (*dist)(struct _Iterator *it); \
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
}

typedef struct _Iterator{
    METAOBJECT_HEAD(ITERATOR_FUNC);
    ITERATOR_OBJ;
} *Iterator;

void initIterator(void) __attribute__((constructor));

Form_t _IteratorClass(void);
Form_t _Iterator(void);

//#define ITERATORCLASS IteratorClass : _IteratorClass()
#define ITERATOR Iterator : _Iterator()

long long distance(Iterator, Iterator);
#endif //TINY_CTL_TCTL_ITERATOR_H
