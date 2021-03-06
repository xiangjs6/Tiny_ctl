//
// Created by xjs on 2020/11/19.
//

#ifndef TINY_CTL_TCTL_MULTISET_H
#define TINY_CTL_TCTL_MULTISET_H
#include "tctl_class.h"
#include "tctl_iterator.h"
#include "tctl_def.h"

#define MULTISET_FUNC                  \
struct {                               \
    CLASS_FUNC;                        \
    Iterator (*begin)(void);           \
    Iterator (*end)(void);             \
    size_t (*size)(void);              \
    bool (*empty)(void);               \
    void (*erase)(Iterator iter);      \
    Iterator (*insert)(const void *x); \
    size_t (*count)(const void *x);    \
    Iterator (*find)(const void *x);   \
    void (*clear)(void);               \
    void (*swap)(MultiSet);            \
}

typedef struct _MultiSet *MultiSet;
struct _MultiSet {
    METAOBJECT_HEAD(MULTISET_FUNC);
};

const void *_MultiSet(void);
#define MULTISET MultiSet : _MultiSet()
#endif //TINY_CTL_TCTL_MULTISET_H
