//
// Created by xjs on 2020/12/20.
//

#ifndef TINY_CTL_TCTL_UNORDERED_MULTIMAP_H
#define TINY_CTL_TCTL_UNORDERED_MULTIMAP_H
#include "tctl_class.h"
#include "tctl_iterator.h"
#include "tctl_def.h"
#include "tctl_utility.h"

#define UNORDERED_MULTIMAP_FUNC       \
struct {                              \
    CLASS_FUNC;                       \
    Iterator (*begin)(void);          \
    Iterator (*end)(void);            \
    size_t (*size)(void);             \
    bool (*empty)(void);              \
    void (*erase)(Iterator iter);     \
    Iterator (*insert)(Pair x);       \
    size_t (*count)(FormWO_t x);      \
    Iterator (*find)(FormWO_t x);     \
    size_t (*bucket_count)(void);     \
    size_t (*max_bucket_count)(void); \
    void (*reserve)(size_t);          \
    void (*clear)(void);              \
    void (*swap)(Unordered_MultiMap); \
}

typedef struct _Unordered_MultiMap *Unordered_MultiMap;
struct _Unordered_MultiMap {
    METAOBJECT_HEAD(UNORDERED_MULTIMAP_FUNC);
};

void initUnordered_MultiMap(void) __attribute__((constructor));
Form_t _Unordered_MultiMap(void);
#define UNORDERED_MULTIMAP Unordered_MultiMap : _Unordered_MultiMap()
#endif //TINY_CTL_TCTL_UNORDERED_MULTIMAP_H
