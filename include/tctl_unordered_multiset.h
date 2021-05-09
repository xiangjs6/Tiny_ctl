//
// Created by xjs on 2020/12/20.
//

#ifndef TINY_CTL_TCTL_UNORDERED_MULTISET_H
#define TINY_CTL_TCTL_UNORDERED_MULTISET_H
#include "tctl_class.h"
#include "tctl_iterator.h"
#include "tctl_def.h"

#define UNORDERED_MULTISET_FUNC        \
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
    size_t (*bucket_count)(void);      \
    size_t (*max_bucket_count)(void);  \
    void (*reserve)(size_t);           \
    void (*clear)(void);               \
    void (*swap)(Unordered_MultiSet);  \
}

typedef struct _Unordered_MultiSet *Unordered_MultiSet;
struct _Unordered_MultiSet {
    METAOBJECT_HEAD(UNORDERED_MULTISET_FUNC);
};

const void *_Unordered_MultiSet(void);
#define UNORDERED_MULTISET Unordered_MultiSet : _Unordered_MultiSet()
#endif //TINY_CTL_TCTL_UNORDERED_MULTISET_H
